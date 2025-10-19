# Quick Start Guide - ESP32 BLE Access Control

**For experienced users who just want the essentials.**

---

## 1. Hardware Setup (5 minutes)

```
ESP32          Access Panel
GPIO 25  ───►  D0 (Data0)
GPIO 26  ───►  D1 (Data1)
GND      ───►  GND (0V)
```

Power ESP32 via USB or 5V supply.

---

## 2. Software Setup (10 minutes)

### Install Arduino IDE + ESP32 Support

```bash
# Add to Arduino IDE → Preferences → Additional Board Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Then: Tools → Board → Boards Manager → Search "esp32" → Install
```

### Configure Firmware

Open `ble_wiegand_access_control.ino` and edit:

```cpp
// 1. Set output format (26, 34, or 40)
#define OUTPUT_FORMAT 26

// 2. Set facility code (for Wiegand 26-bit)
#define FACILITY_CODE 123

// 3. Add your BLE device MAC addresses
const char* authorizedDevices[] = {
  "A4:C1:38:AB:CD:EF",  // Your device
  "12:34:56:78:9A:BC",  // Another device
};
```

### Upload

1. **Tools → Board** → ESP32 Dev Module
2. **Tools → Port** → Select COM port
3. **Click Upload** (→ button)
4. Hold BOOT button if "Connecting..." hangs

---

## 3. Generate Credentials (2 minutes)

```bash
cd tools/
python3 mac_to_credential.py -i mac_list.txt -o credentials.csv
```

Import `credentials.csv` into your access control software.

---

## 4. Test

1. Open **Tools → Serial Monitor** (115200 baud)
2. Bring BLE device near ESP32
3. Should see: `✓ Authorized device detected`
4. Door unlocks!

---

## Format Selection

| Format | When to Use |
|--------|-------------|
| **26** | Standard Wiegand (most common) |
| **34** | Extended Wiegand (large systems) |
| **40** | EM4100 readers with Clock & Data |

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No port found | Install CP210x or CH340 drivers |
| Upload timeout | Hold BOOT button during upload |
| Not detected | Check MAC address format (use colons) |
| Panel no response | Verify wiring, check format matches panel |

---

## Configuration Reference

```cpp
// GPIO Pins
#define WIEGAND_D0_PIN 25
#define WIEGAND_D1_PIN 26

// Output Format: 26, 34, or 40
#define OUTPUT_FORMAT 26

// Facility Code (0-255, for 26-bit only)
#define FACILITY_CODE 123

// Cooldown (milliseconds)
#define DEVICE_COOLDOWN_MS 5000

// Debug Mode
#define DEBUG_MODE true  // Set false for production
```

---

## MAC to Credential Mapping

### Wiegand 26-bit
- Facility Code: Static (configured)
- Card Number: Last 2 bytes of MAC

### Wiegand 34-bit
- Facility Code: Bytes 3-4 of MAC
- Card Number: Last 2 bytes of MAC

### EM4100 40-bit
- Version: First byte of MAC
- Card ID: Bytes 2-5 of MAC

---

**Example:**

MAC: `A4:C1:38:AB:CD:EF`

| Format | Output |
|--------|--------|
| W26 | FC: 123, CN: 52719 |
| W34 | FC: 14507, CN: 52719 |
| EM4100 | Ver: 164, ID: 3241716685 |

---

**Done!** See `INSTALLATION_GUIDE.md` for detailed instructions.

