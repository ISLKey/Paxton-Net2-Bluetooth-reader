/**
 * Configuration Example
 * 
 * This file shows how to configure the BLE to Wiegand Access Control system.
 * Copy the relevant sections to the main .ino file and modify as needed.
 */

// ==================== WIEGAND CONFIGURATION ====================

// GPIO Pins for Wiegand Output
// Connect these pins to your Paxton Net2 Plus controller
#define WIEGAND_D0_PIN 25  // Data0 pin
#define WIEGAND_D1_PIN 26  // Data1 pin

// Wiegand Format Selection
// Options: 26 or 34
#define WIEGAND_FORMAT 26

// Facility Code (for 26-bit format)
// This is a static value that will be used for all credentials
// Range: 0-255 (8 bits)
#define FACILITY_CODE 123

// ==================== BLE SCANNER CONFIGURATION ====================

// BLE Scan Duration (in seconds)
// How long each scan cycle lasts
#define BLE_SCAN_TIME 5

// BLE Scan Interval (in milliseconds)
// Lower values = more frequent scanning = higher power consumption
#define BLE_SCAN_INTERVAL 100

// ==================== SECURITY & DEBOUNCE ====================

// Device Cooldown Period (in milliseconds)
// Prevents multiple rapid transmissions for the same device
// Recommended: 5000 (5 seconds)
#define DEVICE_COOLDOWN_MS 5000

// ==================== DEBUG SETTINGS ====================

// Enable/Disable Debug Mode
// When enabled, detailed logs will be printed to Serial Monitor
// Set to false for production use
#define DEBUG_MODE true

// ==================== AUTHORIZED DEVICES ====================

// List of Authorized BLE Device MAC Addresses
// Add the MAC addresses of devices you want to grant access to
// Format: "aa:bb:cc:dd:ee:ff" (lowercase or uppercase)
//
// How to find MAC addresses:
// - Android: Settings > About Phone > Status > Bluetooth Address
// - iOS: Not directly visible; use a BLE scanner app or check device advertising
// - BLE Beacons: Usually printed on the device or in documentation
//
const char* authorizedDevices[] = {
  "aa:bb:cc:dd:ee:ff",  // Example device 1 - Replace with actual MAC
  "11:22:33:44:55:66",  // Example device 2 - Replace with actual MAC
  "a4:c1:38:12:34:56",  // Example device 3
  // Add more devices below:
  
};

// ==================== NOTES ====================

/**
 * WIEGAND 26-BIT FORMAT:
 * - Total: 26 bits
 * - Structure: [Even Parity][8-bit Facility Code][16-bit Card Number][Odd Parity]
 * - Facility Code: Static value (0-255)
 * - Card Number: Derived from last 2 bytes of MAC address (0-65535)
 * 
 * WIEGAND 34-BIT FORMAT:
 * - Total: 34 bits
 * - Structure: [Even Parity][16-bit Facility Code][16-bit Card Number][Odd Parity]
 * - Facility Code: Derived from bytes 3-4 of MAC address (0-65535)
 * - Card Number: Derived from last 2 bytes of MAC address (0-65535)
 * 
 * MAC ADDRESS MAPPING:
 * - MAC format: AA:BB:CC:DD:EE:FF
 * - 26-bit: Uses EE:FF as card number
 * - 34-bit: Uses CC:DD as facility code, EE:FF as card number
 * 
 * PAXTON NET2 PLUS CONFIGURATION:
 * 1. In Net2 software, configure the reader as "Wiegand reader"
 * 2. Set the format to match your WIEGAND_FORMAT setting (26 or 34 bit)
 * 3. If using 26-bit, configure the facility code to match FACILITY_CODE
 * 4. Add users with card numbers matching the MAC-derived values
 * 
 * EXAMPLE:
 * MAC Address: A4:C1:38:AB:CD:EF
 * - 26-bit: Facility Code = 123 (from config), Card Number = 52719 (0xCDEF)
 * - 34-bit: Facility Code = 14507 (0x38AB), Card Number = 52719 (0xCDEF)
 */

