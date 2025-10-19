# ESP32 BLE Access Control System - Documentation Index

**Version**: 2.0  
**Last Updated**: October 19, 2025

---

## 📚 Complete Documentation Package

This project includes comprehensive documentation to help you install, configure, and deploy your ESP32 BLE Access Control System. Choose the guide that best fits your needs and experience level.

---

## 🚀 Getting Started

### For First-Time Users
**Start here if you're new to ESP32 or access control systems.**

1. **[INSTALLATION_GUIDE.md](ble_wiegand_access_control/INSTALLATION_GUIDE.md)** - Complete step-by-step installation guide
   - Software installation (Arduino IDE, drivers, Python)
   - Hardware assembly with detailed wiring instructions
   - Firmware configuration walkthrough
   - Upload process with troubleshooting
   - Testing and verification procedures
   - Access controller setup
   - **Estimated time**: 45-60 minutes

### For Experienced Users
**Quick setup for those familiar with Arduino and ESP32.**

2. **[QUICK_START.md](ble_wiegand_access_control/QUICK_START.md)** - Fast track installation
   - Essential steps only
   - Quick reference tables
   - Configuration examples
   - Common troubleshooting
   - **Estimated time**: 15-20 minutes

---

## 📖 Reference Documentation

### Hardware Setup

3. **[WIRING_DIAGRAM.md](ble_wiegand_access_control/WIRING_DIAGRAM.md)** - Visual wiring diagrams
   - ESP32 pinout diagrams
   - Paxton Net2 Plus wiring
   - Generic access panel connections
   - Power supply options
   - Wire specifications
   - Safety notes and common mistakes

### Project Overview

4. **[README.md](ble_wiegand_access_control/README.md)** - Project overview and features
   - System architecture
   - Feature list
   - Format specifications (Wiegand 26, 34, EM4100 40)
   - Usage instructions
   - Troubleshooting guide

### Technical Details

5. **[PROJECT_SUMMARY_v2.md](PROJECT_SUMMARY_v2.md)** - Technical documentation
   - Protocol specifications
   - MAC address mapping strategies
   - Security considerations
   - Compatibility information
   - Future enhancements
   - References and sources

---

## 🛠️ Tools and Utilities

### Credential Converter

6. **[tools/mac_to_credential.py](ble_wiegand_access_control/tools/mac_to_credential.py)** - MAC to credential converter
   - Converts MAC addresses to Wiegand/EM4100 credentials
   - Supports batch processing
   - CSV export for access control software
   - Command-line interface

**Usage Examples:**
```bash
# Single MAC conversion
python mac_to_credential.py -m "A4:C1:38:AB:CD:EF"

# Batch conversion with CSV output
python mac_to_credential.py -i mac_list.txt -o credentials.csv

# Specific format only
python mac_to_credential.py -i mac_list.txt -o em4100.csv -f 40
```

---

## 📁 Firmware Files

### Main Firmware

7. **[ble_wiegand_access_control.ino](ble_wiegand_access_control/ble_wiegand_access_control.ino)** - ESP32 firmware
   - BLE scanning and device authentication
   - Multi-format credential generation
   - Wiegand/EM4100 signal transmission
   - Debug mode and logging

### Library

8. **[wiegandOutput.h](ble_wiegand_access_control/wiegandOutput.h)** - Wiegand/EM4100 protocol library
   - Precise timing control
   - Parity calculation
   - Support for 26-bit, 34-bit, and 40-bit formats

### Configuration Reference

9. **[config_example.h](ble_wiegand_access_control/config_example.h)** - Configuration examples
   - Detailed parameter explanations
   - Example configurations
   - Best practices

---

## 📊 Quick Reference

### Output Format Selection

| Format | Use Case | Facility Code | Card Number | Total IDs |
|--------|----------|---------------|-------------|-----------|
| **Wiegand 26-bit** | Standard systems | 8-bit (0-255) | 16-bit (0-65535) | 16.7 million |
| **Wiegand 34-bit** | Large installations | 16-bit (0-65535) | 16-bit (0-65535) | 4.2 billion |
| **EM4100 40-bit** | EM4100 readers | 8-bit version | 32-bit ID | 1.1 trillion |

### Configuration Parameters

```cpp
// Essential Configuration
#define OUTPUT_FORMAT 26           // 26, 34, or 40
#define FACILITY_CODE 123          // For Wiegand 26-bit
#define WIEGAND_D0_PIN 25          // GPIO for Data0
#define WIEGAND_D1_PIN 26          // GPIO for Data1

// Authorized Devices
const char* authorizedDevices[] = {
  "AA:BB:CC:DD:EE:FF",  // Device 1
  "11:22:33:44:55:66",  // Device 2
};
```

### Wiring Quick Reference

```
ESP32          Access Panel
GPIO 25  ───►  D0 (Data0)
GPIO 26  ───►  D1 (Data1)
GND      ───►  GND (0V)
```

---

## 🎯 Recommended Reading Order

### For Complete Beginners

1. **README.md** - Understand what the system does
2. **WIRING_DIAGRAM.md** - See how to connect hardware
3. **INSTALLATION_GUIDE.md** - Follow step-by-step setup
4. **Test and verify** - Get it working
5. **PROJECT_SUMMARY_v2.md** - Learn technical details (optional)

### For Experienced Users

1. **QUICK_START.md** - Fast setup
2. **WIRING_DIAGRAM.md** - Verify connections
3. **README.md** - Reference as needed
4. **PROJECT_SUMMARY_v2.md** - Deep dive into protocols (if interested)

### For System Integrators

1. **PROJECT_SUMMARY_v2.md** - Technical specifications
2. **README.md** - Feature overview
3. **INSTALLATION_GUIDE.md** - Deployment procedures
4. **WIRING_DIAGRAM.md** - Installation standards

---

## 🔧 Troubleshooting Resources

### Common Issues and Solutions

| Issue | Where to Look |
|-------|---------------|
| Upload fails | INSTALLATION_GUIDE.md → Part 4 → Troubleshooting |
| No BLE detection | INSTALLATION_GUIDE.md → Part 5 → Troubleshooting |
| Panel doesn't respond | WIRING_DIAGRAM.md → Testing Connections |
| Wrong credentials | QUICK_START.md → MAC to Credential Mapping |
| General problems | README.md → Troubleshooting section |

---

## 📦 What's Included in the Package

```
ble_wiegand_access_control/
├── ble_wiegand_access_control.ino    # Main firmware
├── wiegandOutput.h                    # Protocol library
├── config_example.h                   # Configuration reference
├── README.md                          # Project overview
├── INSTALLATION_GUIDE.md              # Complete setup guide
├── QUICK_START.md                     # Fast setup guide
├── WIRING_DIAGRAM.md                  # Visual wiring diagrams
└── tools/
    ├── mac_to_credential.py           # Credential converter
    ├── example_mac_list.txt           # Example MAC list
    └── all_credentials.csv            # Example output
```

---

## 🆘 Getting Help

### Before Asking for Help

1. Check the **Troubleshooting** section in INSTALLATION_GUIDE.md
2. Review the **Serial Monitor** output for error messages
3. Verify your **wiring** against WIRING_DIAGRAM.md
4. Confirm your **configuration** matches your hardware
5. Test with **debug mode enabled** (`DEBUG_MODE true`)

### What to Include When Reporting Issues

- ESP32 board model
- Arduino IDE version
- Output format being used (26, 34, or 40)
- Serial Monitor output (copy/paste)
- Wiring setup description
- Access panel model
- What you've already tried

---

## 📝 Version History

### Version 2.0 (October 2025)
- ✅ Added EM4100 40-bit format support
- ✅ Enhanced credential converter with multi-format output
- ✅ Complete documentation package
- ✅ Detailed installation guide
- ✅ Visual wiring diagrams
- ✅ Quick start guide

### Version 1.0 (October 2025)
- Initial release
- Wiegand 26-bit and 34-bit support
- Basic BLE scanning
- MAC to credential conversion

---

## 🎓 Learning Resources

### Understanding the Protocols

- **Wiegand Protocol**: See PROJECT_SUMMARY_v2.md → References
- **EM4100 Format**: See PROJECT_SUMMARY_v2.md → EM4100 Specifications
- **BLE Basics**: Arduino ESP32 BLE documentation

### ESP32 Resources

- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/)
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

### Access Control Systems

- Paxton Net2 documentation
- Wiegand protocol specifications
- EM4100 datasheet

---

## ✅ Pre-Installation Checklist

Before you begin, make sure you have:

- [ ] ESP32 development board
- [ ] Micro-USB cable
- [ ] 3 jumper wires
- [ ] Computer with Arduino IDE installed
- [ ] Python 3 installed (for credential converter)
- [ ] Access to your access control panel
- [ ] MAC addresses of your BLE devices
- [ ] 30-60 minutes of uninterrupted time

---

## 🎉 Quick Success Path

**Follow this path for fastest results:**

1. **Hardware**: Connect 3 wires (WIRING_DIAGRAM.md)
2. **Software**: Install Arduino IDE + ESP32 support (INSTALLATION_GUIDE.md Part 1)
3. **Configure**: Edit 3 lines in firmware (QUICK_START.md)
4. **Upload**: Flash to ESP32 (INSTALLATION_GUIDE.md Part 4)
5. **Test**: Open Serial Monitor and test (INSTALLATION_GUIDE.md Part 5)
6. **Deploy**: Add credentials to panel (INSTALLATION_GUIDE.md Part 6)

**Total time**: ~30 minutes for experienced users, ~60 minutes for beginners

---

## 📞 Support

This is an open-source project. For support:

1. Review the documentation thoroughly
2. Check troubleshooting sections
3. Verify your setup against the guides
4. Test with debug mode enabled

---

**Ready to get started?** 

👉 **New users**: Open [INSTALLATION_GUIDE.md](ble_wiegand_access_control/INSTALLATION_GUIDE.md)  
👉 **Experienced users**: Open [QUICK_START.md](ble_wiegand_access_control/QUICK_START.md)  
👉 **Need wiring help**: Open [WIRING_DIAGRAM.md](ble_wiegand_access_control/WIRING_DIAGRAM.md)

---

**Good luck with your installation!** 🚀

