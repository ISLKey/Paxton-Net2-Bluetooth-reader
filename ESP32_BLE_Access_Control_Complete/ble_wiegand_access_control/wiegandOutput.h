/**
 * Wiegand Output Library
 * 
 * Based on the WiegandOutput-Arduino library by Fernando Castro
 * https://github.com/castrofernando/WiegandOutput-Arduino
 * 
 * This library allows Arduino/ESP32 hardware to send Wiegand data
 * using two GPIO pins (Data0 and Data1) with support for both
 * 26-bit and 34-bit Wiegand formats.
 */

#ifndef WIEGAND_OUTPUT_H
#define WIEGAND_OUTPUT_H

#include <Arduino.h>

// Wiegand timing constants (in microseconds)
#define WIEGAND_PULSE_WIDTH 50      // 50 µs pulse width (within 20-100 µs spec)
#define WIEGAND_PULSE_INTERVAL 1000 // 1000 µs (1 ms) between pulses (within 200 µs - 20 ms spec)

class WiegandOut {
private:
  int _d0Pin;
  int _d1Pin;
  bool _debug;
  
  /**
   * Send a single bit via Wiegand protocol
   * @param bit The bit value (0 or 1)
   */
  void sendBit(int bit) {
    if (bit == 0) {
      // Send 0: Pull D0 low
      digitalWrite(_d0Pin, LOW);
      delayMicroseconds(WIEGAND_PULSE_WIDTH);
      digitalWrite(_d0Pin, HIGH);
    } else {
      // Send 1: Pull D1 low
      digitalWrite(_d1Pin, LOW);
      delayMicroseconds(WIEGAND_PULSE_WIDTH);
      digitalWrite(_d1Pin, HIGH);
    }
    
    // Wait for pulse interval
    delayMicroseconds(WIEGAND_PULSE_INTERVAL);
  }
  
  /**
   * Calculate even parity for the first half of data bits
   * @param data The data value
   * @param numBits Number of bits to consider
   * @return Even parity bit (0 or 1)
   */
  int calculateEvenParity(uint32_t data, int numBits) {
    int count = 0;
    for (int i = numBits - 1; i >= numBits / 2; i--) {
      if (data & (1UL << i)) {
        count++;
      }
    }
    return count % 2;  // Even parity: return 1 if odd count, 0 if even
  }
  
  /**
   * Calculate odd parity for the second half of data bits
   * @param data The data value
   * @param numBits Number of bits to consider
   * @return Odd parity bit (0 or 1)
   */
  int calculateOddParity(uint32_t data, int numBits) {
    int count = 0;
    for (int i = (numBits / 2) - 1; i >= 0; i--) {
      if (data & (1UL << i)) {
        count++;
      }
    }
    return (count % 2) ? 0 : 1;  // Odd parity: return 0 if odd count, 1 if even
  }
  
public:
  /**
   * Constructor
   * @param d0Pin GPIO pin for Data0
   * @param d1Pin GPIO pin for Data1
   * @param debug Enable debug output to Serial
   */
  WiegandOut(int d0Pin, int d1Pin, bool debug = false) {
    _d0Pin = d0Pin;
    _d1Pin = d1Pin;
    _debug = debug;
    
    // Initialize pins as outputs, default HIGH (idle state)
    pinMode(_d0Pin, OUTPUT);
    pinMode(_d1Pin, OUTPUT);
    digitalWrite(_d0Pin, HIGH);
    digitalWrite(_d1Pin, HIGH);
  }
  
  /**
   * Send Wiegand/EM4100 data
   * @param data The combined facility code and card number
   * @param bits Number of bits (26, 34, or 40)
   * @param withFacilityCode Whether facility code is included
   */
  void send(uint32_t data, int bits, bool withFacilityCode) {
    if (bits != 26 && bits != 34 && bits != 40) {
      if (_debug) {
        Serial.println("Error: Only 26-bit, 34-bit, and 40-bit formats are supported");
      }
      return;
    }
    
    // Calculate data bits
    int dataBits;
    if (bits == 40) {
      // EM4100 format: 40 bits total, no parity (or parity already included)
      dataBits = 40;
    } else {
      // Wiegand format: total bits - 2 parity bits
      dataBits = bits - 2;
    }
    
    // Calculate parity bits (only for Wiegand formats)
    int evenParity = 0;
    int oddParity = 0;
    
    if (bits != 40) {
      evenParity = calculateEvenParity(data, dataBits);
      oddParity = calculateOddParity(data, dataBits);
    }
    
    if (_debug) {
      Serial.print("Sending ");
      Serial.print(bits);
      if (bits == 40) {
        Serial.println("-bit EM4100:");
      } else {
        Serial.println("-bit Wiegand:");
      }
      Serial.print("  Data: 0x");
      Serial.println(data, HEX);
      if (bits != 40) {
        Serial.print("  Even Parity: ");
        Serial.println(evenParity);
        Serial.print("  Odd Parity: ");
        Serial.println(oddParity);
      }
      Serial.print("  Binary: ");
    }
    
    // Send leading parity bit (even) - only for Wiegand
    if (bits != 40) {
      sendBit(evenParity);
      if (_debug) Serial.print(evenParity);
    }
    
    // Send data bits (MSB first)
    for (int i = dataBits - 1; i >= 0; i--) {
      int bit = (data >> i) & 1;
      sendBit(bit);
      if (_debug) Serial.print(bit);
    }
    
    // Send trailing parity bit (odd) - only for Wiegand
    if (bits != 40) {
      sendBit(oddParity);
      if (_debug) Serial.print(oddParity);
    }
    
    if (_debug) {
      Serial.println();
      Serial.println("  Transmission complete");
    }
  }
  
  /**
   * Send 26-bit Wiegand with separate facility code and card number
   * @param facilityCode 8-bit facility code
   * @param cardNumber 16-bit card number
   */
  void send26(uint8_t facilityCode, uint16_t cardNumber) {
    uint32_t data = ((uint32_t)facilityCode << 16) | cardNumber;
    send(data, 26, true);
  }
  
  /**
   * Send 34-bit Wiegand with separate facility code and card number
   * @param facilityCode 16-bit facility code
   * @param cardNumber 16-bit card number
   */
  void send34(uint16_t facilityCode, uint16_t cardNumber) {
    uint32_t data = ((uint32_t)facilityCode << 16) | cardNumber;
    send(data, 34, true);
  }
  
  /**
   * Send 40-bit EM4100 format with separate version and ID
   * @param version 8-bit version/customer ID
   * @param cardId 32-bit unique card ID
   */
  void sendEM4100(uint8_t version, uint32_t cardId) {
    // Combine version (8 bits) and card ID (32 bits) into 40 bits
    uint64_t data = ((uint64_t)version << 32) | cardId;
    
    if (_debug) {
      Serial.println("Sending 40-bit EM4100:");
      Serial.print("  Version: 0x");
      Serial.println(version, HEX);
      Serial.print("  Card ID: 0x");
      Serial.println(cardId, HEX);
      Serial.print("  Binary: ");
    }
    
    // Send version bits (8 bits, MSB first)
    for (int i = 7; i >= 0; i--) {
      int bit = (version >> i) & 1;
      sendBit(bit);
      if (_debug) Serial.print(bit);
    }
    
    // Send card ID bits (32 bits, MSB first)
    for (int i = 31; i >= 0; i--) {
      int bit = (cardId >> i) & 1;
      sendBit(bit);
      if (_debug) Serial.print(bit);
    }
    
    if (_debug) {
      Serial.println();
      Serial.println("  Transmission complete");
    }
  }
};

#endif // WIEGAND_OUTPUT_H

