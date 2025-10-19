# ESP32 BLE to Wiegand/EM4100 Access Control System - Project Summary

**Author**: Manus AI  
**Date**: October 19, 2025  
**Version**: 2.0 (with EM4100 support)  
**Project Type**: Embedded Systems / Access Control

---

## Executive Summary

This project delivers a complete, production-ready solution for implementing Bluetooth Low Energy (BLE) based access control using an ESP32 microcontroller and standard access control systems. The system enables keyless entry by detecting authorized BLE devices (smartphones, beacons, etc.) and transmitting their credentials in **Wiegand 26-bit**, **Wiegand 34-bit**, or **EM4100 40-bit** format to access control panels.

The solution includes firmware for the ESP32, a Python-based configuration tool for credential mapping, and comprehensive documentation for deployment and integration with various access control systems including Paxton Net2 Plus.

---

## What's New in Version 2.0

### EM4100 40-bit Output Support
- Added native support for EM4100 40-bit format (Clock & Data output)
- Compatible with EM4100 readers that use Data0/Data1 interface
- Uses 8-bit version field and 32-bit card ID from MAC address
- Selectable output format: Wiegand 26, Wiegand 34, or EM4100 40

### Enhanced Credential Converter
- Updated Python tool now supports all three formats
- Single command generates credentials for all formats
- CSV export includes Wiegand 26, Wiegand 34, and EM4100 credentials
- Easier setup for mixed-format environments

---

## Technical Specifications

### Supported Output Formats

#### Wiegand 26-bit Format
The standard 26-bit Wiegand format consists of 26 bits structured as follows:
- 1 leading parity bit (even parity over the first 12 data bits)
- 8 bits for the facility code (0-255)
- 16 bits for the card number (0-65,535)
- 1 trailing parity bit (odd parity over the last 12 data bits)

**Timing Requirements:**
- **Pulse Width**: 50 µs (within the 20-100 µs specification)
- **Pulse Interval**: 1000 µs (1 ms, within the 200 µs - 20 ms specification)

**MAC Address Mapping:**
- Facility Code: Static value configured in firmware (e.g., 123)
- Card Number: Last 2 bytes (16 bits) of MAC address
- Example: MAC `A4:C1:38:AB:CD:EF` → FC: 123, CN: 52719 (0xCDEF)

#### Wiegand 34-bit Format
The 34-bit format extends the credential space:
- 1 leading parity bit (even parity)
- 16 bits for the facility code (0-65,535)
- 16 bits for the card number (0-65,535)
- 1 trailing parity bit (odd parity)

**MAC Address Mapping:**
- Facility Code: Bytes 3-4 of MAC address
- Card Number: Last 2 bytes of MAC address
- Example: MAC `A4:C1:38:AB:CD:EF` → FC: 14507 (0x38AB), CN: 52719 (0xCDEF)

#### EM4100 40-bit Format
The EM4100 format uses 40 bits without parity:
- 8 bits for version/customer ID (0-255)
- 32 bits for unique card ID (0-4,294,967,295)

**Electrical Signaling:**
- Same as Wiegand: Data0 and Data1 lines
- Idle state: Both lines at +5V HIGH
- Logic 0: Data0 pulled LOW for pulse duration
- Logic 1: Data1 pulled LOW for pulse duration
- Pulse width: 50 µs (compatible with 35-500 µs spec)
- Pulse interval: 1000 µs

**MAC Address Mapping:**
- Version: First byte of MAC address
- Card ID: Bytes 2-5 (32 bits) of MAC address
- Example: MAC `A4:C1:38:AB:CD:EF` → Ver: 164 (0xA4), ID: 3241716685 (0xC138ABCD)

### Protocol Comparison

| Feature | Wiegand 26 | Wiegand 34 | EM4100 40 |
|---------|-----------|-----------|-----------|
| Total Bits | 26 | 34 | 40 |
| Data Bits | 24 | 32 | 40 |
| Parity Bits | 2 (even + odd) | 2 (even + odd) | 0 (none) |
| Field 1 | 8-bit FC (0-255) | 16-bit FC (0-65535) | 8-bit Ver (0-255) |
| Field 2 | 16-bit CN (0-65535) | 16-bit CN (0-65535) | 32-bit ID (0-4.2B) |
| Unique IDs | 16.7 million | 4.2 billion | 1.1 trillion |
| Use Case | Standard access | Large systems | EM4100 readers |

---

## System Components

### 1. ESP32 Firmware (`ble_wiegand_access_control.ino`)

The main firmware file implements the complete BLE-to-access-control bridge functionality. It includes:

- **BLE Scanner Module**: Continuously scans for BLE advertising packets using the ESP32's built-in BLE libraries.
- **Device Authentication**: Compares detected MAC addresses against a configurable whitelist of authorized devices.
- **Tokenization Engine**: Converts MAC addresses to credentials using format-specific mapping strategies.
- **Multi-Format Signal Generator**: Generates precise electrical pulses for Wiegand or EM4100 protocols.
- **Debounce Mechanism**: Implements a configurable cooldown period (default: 5 seconds) to prevent multiple rapid transmissions.
- **Debug Mode**: Provides detailed serial output for troubleshooting and verification.

**Key Configuration Parameters:**
```cpp
#define WIEGAND_D0_PIN 25          // GPIO pin for Data0
#define WIEGAND_D1_PIN 26          // GPIO pin for Data1
#define OUTPUT_FORMAT 26           // 26, 34, or 40
#define FACILITY_CODE 123          // For 26-bit Wiegand
#define DEVICE_COOLDOWN_MS 5000    // Cooldown period
#define DEBUG_MODE true            // Enable debug output

const char* authorizedDevices[] = {
  "AA:BB:CC:DD:EE:FF",  // Add your device MACs here
};
```

### 2. Wiegand/EM4100 Output Library (`wiegandOutput.h`)

A custom implementation that handles:
- Precise timing control for pulse width and intervals
- Automatic parity bit calculation for Wiegand formats (even and odd)
- Support for Wiegand 26-bit, 34-bit, and EM4100 40-bit formats
- Format-specific encoding (with/without parity)
- Debug output for verification

**Key Methods:**
```cpp
// Send Wiegand 26-bit
wiegandOut.send26(facilityCode, cardNumber);

// Send Wiegand 34-bit
wiegandOut.send34(facilityCode, cardNumber);

// Send EM4100 40-bit
wiegandOut.sendEM4100(version, cardId);
```

### 3. MAC to Credential Converter Tool (`tools/mac_to_credential.py`)

A Python 3 command-line utility that converts BLE device MAC addresses to their corresponding credentials for all supported formats.

**Features:**
- Single MAC conversion or batch processing from file
- Support for Wiegand 26, Wiegand 34, and EM4100 40 formats
- CSV export for easy import into access control software
- Multiple MAC address format support (colon, dash, dot separators)
- Simultaneous output of all formats for comparison

**Example Usage:**
```bash
# Convert a single MAC address to all formats
python mac_to_credential.py -m "A4:C1:38:AB:CD:EF"

# Batch convert and generate CSV with all formats
python mac_to_credential.py -i mac_list.txt -o credentials.csv

# Generate only EM4100 credentials
python mac_to_credential.py -i mac_list.txt -o em4100_only.csv -f 40
```

**Example Output:**
```
MAC Address: A4:C1:38:AB:CD:EF
  W26: FC=123, CN=52719
  W34: FC=14507, CN=52719
  EM4100: Ver=164, ID=3241716685
```

### 4. Documentation

- **README.md**: Complete user guide with setup instructions, wiring diagrams, and troubleshooting
- **config_example.h**: Detailed configuration reference with examples and notes
- **PROJECT_SUMMARY_v2.md**: This technical overview document

---

## Hardware Setup

### Required Components

| Component               | Specification                     |
| ----------------------- | --------------------------------- |
| ESP32 Development Board | Any standard ESP32 board          |
| Power Supply            | 5V via USB or 3.3V regulated      |
| Jumper Wires            | 3 wires (D0, D1, Ground)          |

### Wiring Configuration

| ESP32 Pin | Access Panel Terminal | Signal Type |
| --------- | --------------------- | ----------- |
| GPIO 25   | D0 (Data0)            | Data 0 line |
| GPIO 26   | D1 (Data1)            | Data 1 line |
| GND       | 0V (Ground)           | Common Ground |

The GPIO pins are configurable in the firmware to accommodate different ESP32 board layouts or user preferences.

---

## Deployment Workflow

### Phase 1: Hardware Assembly
1. Connect the ESP32 to the access control panel using the wiring diagram
2. Ensure a stable power supply for the ESP32
3. Verify all connections are secure

### Phase 2: Firmware Configuration
1. Install the Arduino IDE and ESP32 board support
2. Open the `ble_wiegand_access_control.ino` file
3. Configure the authorized device MAC addresses
4. Set the desired output format (26, 34, or 40)
5. Set the facility code (for 26-bit Wiegand)
6. Upload the firmware to the ESP32

### Phase 3: Credential Mapping
1. Use the `mac_to_credential.py` tool to convert MAC addresses to credentials
2. Generate a CSV file with all authorized devices
3. Note the credentials for each device in your chosen format

### Phase 4: Controller Configuration
1. Open your access control software
2. Configure the reader input to match your chosen format
3. Add users with the credentials from the CSV
4. Test access with each authorized device

### Phase 5: Testing and Validation
1. Monitor the ESP32 serial output during testing
2. Verify that authorized devices trigger access grants
3. Confirm that unauthorized devices are ignored
4. Test the cooldown mechanism to prevent duplicate reads

---

## Format Selection Guide

### When to Use Wiegand 26-bit
- **Most common format**, widely supported
- **Simple installations** with single facility
- **Legacy systems** that only support 26-bit
- **Up to 65,535 users** per facility code

### When to Use Wiegand 34-bit
- **Larger installations** requiring more unique IDs
- **Multiple facilities** using MAC-derived facility codes
- **Modern systems** that support extended formats
- **Up to 4.2 billion unique combinations**

### When to Use EM4100 40-bit
- **EM4100 readers** with Clock & Data output
- **Maximum unique IDs** (over 1 trillion combinations)
- **Systems expecting EM4100 format** specifically
- **No parity overhead** (all 40 bits are data)

---

## Compatibility

### Tested Access Control Systems
- **Paxton Net2 Plus**: Supports all three formats (Wiegand 26, 34, and EM4100)
- **Generic Wiegand Controllers**: Support Wiegand 26 and 34
- **EM4100 Compatible Readers**: Support EM4100 40-bit format

### Reader Compatibility
The ESP32 acts as a **credential source**, not a reader. It connects to the controller's reader input terminals (D0, D1, GND) and emulates the output of:
- Wiegand proximity card readers
- EM4100 RFID card readers
- Any reader using Data0/Data1 protocol

---

## Security Considerations

### Authentication
The system uses a whitelist approach where only devices with pre-configured MAC addresses are authorized. This provides a basic level of security, but it's important to note:

- **MAC addresses are not encrypted** in BLE advertising packets and can be observed by anyone with a BLE scanner
- **MAC spoofing is possible** with specialized hardware or software
- This system is suitable for **low to medium security applications** where convenience is prioritized

### Recommendations for Enhanced Security
For higher security requirements, consider:
- Implementing BLE pairing and encryption
- Using BLE beacons with rotating identifiers
- Combining this system with additional authentication factors (PIN, biometric, etc.)
- Implementing time-based access restrictions in the controller software
- Regular auditing of access logs
- Using random MAC addresses with a secure registration process

---

## Advantages and Limitations

### Advantages
- **Cost-effective**: Uses inexpensive, readily available hardware
- **Easy to deploy**: Simple wiring and configuration
- **Flexible**: Works with any BLE device with a static MAC address
- **Universal compatibility**: Supports three major access control formats
- **Scalable**: Can support thousands of users
- **Open source**: Fully customizable firmware and tools
- **No proprietary cards needed**: Use existing smartphones or cheap BLE beacons

### Limitations
- **BLE range**: Typically 10-30 meters, affected by obstacles
- **MAC address visibility**: Not suitable for high-security applications
- **Power requirements**: ESP32 requires continuous power
- **One-way communication**: Cannot write data back to BLE devices
- **Device compatibility**: Requires BLE devices with consistent advertising
- **Format selection**: Must choose one format, cannot switch dynamically

---

## Future Enhancements

Potential improvements for future versions:
1. **Dynamic format selection** based on controller detection
2. **Web-based configuration interface** for easier device management
3. **MQTT integration** for remote monitoring and logging
4. **Support for encrypted BLE connections** for enhanced security
5. **Multi-reader support** using a single ESP32
6. **RSSI-based proximity detection** for more precise access control
7. **Integration with cloud-based access management platforms**
8. **Support for additional formats** (HID, iClass, etc.)

---

## References

1. Safe and Sound Security. (2024). "Everything You Need To Know About The Wiegand Protocol?" Retrieved from https://getsafeandsound.com/2018/09/wiegand-protocol/

2. Desai, J. (2020). "Wiegand Interface." Medium. Retrieved from https://medium.com/@jaydesai36/wiegand-interface-517a24107ede

3. Castro, F. (2023). "Wiegand Output Library for Arduino." PlatformIO Registry. Retrieved from https://registry.platformio.org/libraries/castro.fernando/Wiegand%20Output

4. Paxton Access Ltd. (2019). "Net2 Plus System Specifications." Retrieved from https://www.paxton-access.com/wp-content/uploads/2019/03/DS1038-US.pdf

5. Priority 1 Design. (2007). "EM4100 Protocol Description." Retrieved from https://www.priority1design.com.au/em4100_protocol.html

6. ACCX Products. (2013). "EM4100 Reader Pinouts." Retrieved from https://www.accxproducts.com/wiki/index.php?title=EM4100_Reader_pinouts

---

## License and Attribution

This project incorporates concepts and code from the following open-source projects:
- **WiegandOutput-Arduino** by Fernando Castro (MIT License)
- **ESP32 Arduino Core** by Espressif Systems
- **ESP-RFID** by Crypter (GPL-3.0 License)

The firmware and tools developed for this project are provided as-is for educational and commercial use. Please ensure compliance with local regulations regarding access control systems and data privacy.

---

## Support and Contact

For questions, issues, or contributions, please refer to the project documentation or contact the development team.

**Project Status**: Production Ready  
**Last Updated**: October 19, 2025  
**Version**: 2.0.0

