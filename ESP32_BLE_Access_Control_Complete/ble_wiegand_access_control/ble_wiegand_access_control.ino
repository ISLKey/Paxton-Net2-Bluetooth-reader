/**
 * BLE to Wiegand Access Control System
 * 
 * This firmware enables an ESP32 to act as a Bluetooth-to-Wiegand bridge
 * for access control systems. It scans for authorized BLE devices and
 * sends their credentials via Wiegand protocol to a Paxton Net2 Plus controller.
 * 
 * Hardware:
 * - ESP32 Development Board
 * - Paxton Net2 Plus Controller
 * 
 * Connections:
 * - GPIO 25 -> Paxton D0 (Data0)
 * - GPIO 26 -> Paxton D1 (Data1)
 * - GND -> Paxton Ground
 * 
 * Author: Manus AI
 * Date: October 2025
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "wiegandOutput.h"

// ==================== CONFIGURATION ====================

// Wiegand GPIO Pins
#define WIEGAND_D0_PIN 25
#define WIEGAND_D1_PIN 26

// Output Format Selection
// Options: 26 (Wiegand 26-bit), 34 (Wiegand 34-bit), 40 (EM4100)
#define OUTPUT_FORMAT 26  // Change to 26, 34, or 40

// Facility Code (for 26-bit format)
#define FACILITY_CODE 123

// BLE Scan Settings
#define BLE_SCAN_TIME 5        // Scan duration in seconds
#define BLE_SCAN_INTERVAL 100  // Scan interval in ms

// Debounce/Cooldown Settings
#define DEVICE_COOLDOWN_MS 5000  // 5 seconds cooldown per device

// Debug Mode
#define DEBUG_MODE true

// Authorized BLE Device MAC Addresses
// Add MAC addresses in the format "aa:bb:cc:dd:ee:ff"
const char* authorizedDevices[] = {
  "aa:bb:cc:dd:ee:ff",  // Example device 1
  "11:22:33:44:55:66",  // Example device 2
  // Add more devices here
};
const int numAuthorizedDevices = sizeof(authorizedDevices) / sizeof(authorizedDevices[0]);

// ==================== GLOBAL OBJECTS ====================

BLEScan* pBLEScan;
WiegandOut wiegandOut(WIEGAND_D0_PIN, WIEGAND_D1_PIN, DEBUG_MODE);

// Cooldown tracking
struct DeviceCooldown {
  String macAddress;
  unsigned long lastSeen;
};

DeviceCooldown deviceCooldowns[10];  // Track up to 10 devices
int cooldownIndex = 0;

// ==================== HELPER FUNCTIONS ====================

/**
 * Convert MAC address string to lowercase for comparison
 */
String normalizeMacAddress(String mac) {
  mac.toLowerCase();
  return mac;
}

/**
 * Check if a device is in the authorized list
 */
bool isDeviceAuthorized(String macAddress) {
  String normalizedMac = normalizeMacAddress(macAddress);
  
  for (int i = 0; i < numAuthorizedDevices; i++) {
    if (normalizedMac == normalizeMacAddress(String(authorizedDevices[i]))) {
      return true;
    }
  }
  return false;
}

/**
 * Check if a device is in cooldown period
 */
bool isDeviceInCooldown(String macAddress) {
  unsigned long currentTime = millis();
  
  for (int i = 0; i < 10; i++) {
    if (deviceCooldowns[i].macAddress == macAddress) {
      if ((currentTime - deviceCooldowns[i].lastSeen) < DEVICE_COOLDOWN_MS) {
        return true;
      }
    }
  }
  return false;
}

/**
 * Update device cooldown timestamp
 */
void updateDeviceCooldown(String macAddress) {
  unsigned long currentTime = millis();
  
  // Check if device already exists in cooldown list
  for (int i = 0; i < 10; i++) {
    if (deviceCooldowns[i].macAddress == macAddress) {
      deviceCooldowns[i].lastSeen = currentTime;
      return;
    }
  }
  
  // Add new device to cooldown list
  deviceCooldowns[cooldownIndex].macAddress = macAddress;
  deviceCooldowns[cooldownIndex].lastSeen = currentTime;
  cooldownIndex = (cooldownIndex + 1) % 10;
}

/**
 * Extract card number from MAC address
 * Uses the last 2 bytes (16 bits) of the MAC address
 */
uint32_t macToCardNumber(String macAddress) {
  // Remove colons from MAC address
  String cleanMac = macAddress;
  cleanMac.replace(":", "");
  
  // Get last 4 hex characters (2 bytes = 16 bits)
  String lastBytes = cleanMac.substring(cleanMac.length() - 4);
  
  // Convert hex string to integer
  uint32_t cardNumber = strtoul(lastBytes.c_str(), NULL, 16);
  
  if (DEBUG_MODE) {
    Serial.print("MAC: ");
    Serial.print(macAddress);
    Serial.print(" -> Card Number: ");
    Serial.println(cardNumber);
  }
  
  return cardNumber;
}

/**
 * Extract facility code from MAC address (for 34-bit format)
 * Uses bytes 3-4 of the MAC address
 */
uint32_t macToFacilityCode34(String macAddress) {
  // Remove colons from MAC address
  String cleanMac = macAddress;
  cleanMac.replace(":", "");
  
  // Get bytes 3-4 (characters 4-7)
  String middleBytes = cleanMac.substring(4, 8);
  
  // Convert hex string to integer
  uint32_t facilityCode = strtoul(middleBytes.c_str(), NULL, 16);
  
  if (DEBUG_MODE) {
    Serial.print("MAC: ");
    Serial.print(macAddress);
    Serial.print(" -> Facility Code (34-bit): ");
    Serial.println(facilityCode);
  }
  
  return facilityCode;
}

/**
 * Send credential to access controller (Wiegand or EM4100)
 */
void sendCredential(String macAddress) {
  uint32_t cardNumber = macToCardNumber(macAddress);
  
  if (OUTPUT_FORMAT == 26) {
    // 26-bit format with static facility code
    if (DEBUG_MODE) {
      Serial.println("=================================");
      Serial.println("Sending 26-bit Wiegand credential");
      Serial.print("Facility Code: ");
      Serial.println(FACILITY_CODE);
      Serial.print("Card Number: ");
      Serial.println(cardNumber);
      Serial.println("=================================");
    }
    
    // Combine facility code and card number into single value
    // For WiegandOut library: facilityCode (8 bits) + cardNumber (16 bits)
    uint32_t wiegandValue = (FACILITY_CODE << 16) | cardNumber;
    wiegandOut.send(wiegandValue, 26, true);
    
  } else if (OUTPUT_FORMAT == 34) {
    // 34-bit format using MAC address for both facility and card
    uint32_t facilityCode = macToFacilityCode34(macAddress);
    
    if (DEBUG_MODE) {
      Serial.println("=================================");
      Serial.println("Sending 34-bit Wiegand credential");
      Serial.print("Facility Code: ");
      Serial.println(facilityCode);
      Serial.print("Card Number: ");
      Serial.println(cardNumber);
      Serial.println("=================================");
    }
    
    // Combine facility code and card number
    uint32_t wiegandValue = (facilityCode << 16) | cardNumber;
    wiegandOut.send(wiegandValue, 34, true);
    
  } else if (OUTPUT_FORMAT == 40) {
    // 40-bit EM4100 format
    // Use first byte of MAC as version, last 4 bytes as card ID
    String cleanMac = macAddress;
    cleanMac.replace(":", "");
    
    // Extract version (first byte)
    String versionStr = cleanMac.substring(0, 2);
    uint8_t version = strtoul(versionStr.c_str(), NULL, 16);
    
    // Extract card ID (last 4 bytes = 32 bits)
    String cardIdStr = cleanMac.substring(4, 12);
    uint32_t cardId = strtoul(cardIdStr.c_str(), NULL, 16);
    
    if (DEBUG_MODE) {
      Serial.println("=================================");
      Serial.println("Sending 40-bit EM4100 credential");
      Serial.print("Version: 0x");
      Serial.print(version, HEX);
      Serial.print(" (");
      Serial.print(version);
      Serial.println(")");
      Serial.print("Card ID: 0x");
      Serial.print(cardId, HEX);
      Serial.print(" (");
      Serial.print(cardId);
      Serial.println(")");
      Serial.println("=================================");
    }
    
    wiegandOut.sendEM4100(version, cardId);
  }
}

// ==================== BLE CALLBACK CLASS ====================

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String macAddress = String(advertisedDevice.getAddress().toString().c_str());
    
    if (DEBUG_MODE) {
      Serial.print("BLE Device found: ");
      Serial.print(macAddress);
      Serial.print(" | RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
    
    // Check if device is authorized
    if (isDeviceAuthorized(macAddress)) {
      Serial.print("✓ Authorized device detected: ");
      Serial.println(macAddress);
      
      // Check cooldown
      if (isDeviceInCooldown(macAddress)) {
        if (DEBUG_MODE) {
          Serial.println("  -> Device in cooldown, skipping...");
        }
        return;
      }
      
      // Send credential (Wiegand or EM4100)
      sendCredential(macAddress);
      
      // Update cooldown
      updateDeviceCooldown(macAddress);
      
    } else {
      if (DEBUG_MODE) {
        Serial.println("  -> Not authorized");
      }
    }
  }
};

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=================================");
  Serial.println("BLE to Wiegand Access Control");
  Serial.println("=================================");
  Serial.print("Output Format: ");
  if (OUTPUT_FORMAT == 40) {
    Serial.println("EM4100 40-bit");
  } else {
    Serial.print("Wiegand ");
    Serial.print(OUTPUT_FORMAT);
    Serial.println("-bit");
  }
  Serial.print("Facility Code: ");
  Serial.println(FACILITY_CODE);
  Serial.print("D0 Pin: GPIO ");
  Serial.println(WIEGAND_D0_PIN);
  Serial.print("D1 Pin: GPIO ");
  Serial.println(WIEGAND_D1_PIN);
  Serial.print("Authorized Devices: ");
  Serial.println(numAuthorizedDevices);
  Serial.println("=================================\n");
  
  // Initialize BLE
  Serial.println("Initializing BLE...");
  BLEDevice::init("");
  
  // Create BLE Scanner
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(BLE_SCAN_INTERVAL);
  pBLEScan->setWindow(99);  // Less or equal setInterval value
  
  Serial.println("BLE Scanner initialized");
  Serial.println("System ready. Scanning for devices...\n");
}

// ==================== MAIN LOOP ====================

void loop() {
  // Start BLE scan
  BLEScanResults foundDevices = pBLEScan->start(BLE_SCAN_TIME, false);
  
  if (DEBUG_MODE) {
    Serial.print("Scan complete. Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("---");
  }
  
  // Clear scan results to free memory
  pBLEScan->clearResults();
  
  // Small delay before next scan
  delay(1000);
}

