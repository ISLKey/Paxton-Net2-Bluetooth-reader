# ESP32 BLE Access Control System - Complete Installation Guide

**Version**: 2.0  
**Author**: Manus AI  
**Date**: October 2025

---

## Table of Contents

1. [What You'll Need](#what-youll-need)
2. [Part 1: Software Installation](#part-1-software-installation)
3. [Part 2: Hardware Assembly](#part-2-hardware-assembly)
4. [Part 3: Firmware Configuration](#part-3-firmware-configuration)
5. [Part 4: Uploading Firmware to ESP32](#part-4-uploading-firmware-to-esp32)
6. [Part 5: Testing and Verification](#part-5-testing-and-verification)
7. [Part 6: Access Controller Setup](#part-6-access-controller-setup)
8. [Part 7: Final Testing](#part-7-final-testing)
9. [Troubleshooting](#troubleshooting)

---

## What You'll Need

### Hardware Components

| Item | Description | Where to Buy | Approx. Cost |
|------|-------------|--------------|--------------|
| ESP32 Development Board | Any ESP32 board (ESP32-DevKitC, NodeMCU-32S, etc.) | Amazon, AliExpress, SparkFun | $5-15 |
| Micro-USB Cable | For programming and power | Amazon, local electronics store | $3-5 |
| Jumper Wires (3x) | Male-to-male or male-to-female | Amazon, electronics store | $5 (pack) |
| Access Control Panel | Your existing system (e.g., Paxton Net2 Plus) | Already installed | - |

### Software Requirements

| Software | Version | Purpose |
|----------|---------|---------|
| Arduino IDE | 1.8.19 or later | Programming the ESP32 |
| Python | 3.7 or later | Running credential converter tool |
| USB Driver | CP210x or CH340 | ESP32 communication (may be needed) |

### Optional Tools

- Multimeter (for testing connections)
- Soldering iron (if you need to solder headers to ESP32)
- Screwdriver (for access panel terminals)

---

## Part 1: Software Installation

### Step 1.1: Install Arduino IDE

**For Windows:**

1. Go to https://www.arduino.cc/en/software
2. Click **"Windows Win 10 and newer, 64 bits"**
3. Download the installer (`.exe` file)
4. Run the installer and follow the prompts
5. Accept all default settings
6. Click **Install** and wait for completion
7. Launch Arduino IDE

**For macOS:**

1. Go to https://www.arduino.cc/en/software
2. Click **"macOS Intel, 10.14: "Mojave" or newer, 64 bits"**
3. Download the `.dmg` file
4. Open the downloaded file
5. Drag Arduino IDE to Applications folder
6. Launch Arduino IDE from Applications

**For Linux:**

1. Go to https://www.arduino.cc/en/software
2. Download the Linux 64-bit version
3. Extract the archive:
   ```bash
   tar -xf arduino-*.tar.xz
   ```
4. Run the install script:
   ```bash
   cd arduino-*/
   sudo ./install.sh
   ```
5. Launch Arduino IDE

### Step 1.2: Install ESP32 Board Support

1. **Open Arduino IDE**

2. **Go to File → Preferences** (or Arduino → Preferences on macOS)

3. **Find "Additional Board Manager URLs"** field

4. **Paste this URL:**
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

5. **Click OK**

6. **Go to Tools → Board → Boards Manager**

7. **Search for "esp32"**

8. **Find "esp32 by Espressif Systems"**

9. **Click Install** (this may take several minutes)

10. **Wait for "INSTALLED" to appear**

11. **Close Boards Manager**

### Step 1.3: Install USB Drivers (if needed)

**Check if drivers are needed:**

1. Connect your ESP32 to your computer via USB
2. Go to **Tools → Port** in Arduino IDE
3. If you see a COM port (Windows) or `/dev/cu.usbserial` (macOS) or `/dev/ttyUSB0` (Linux), you're good!
4. If not, you need to install drivers

**For CP210x chip (most common):**

1. Go to https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Download the driver for your OS
3. Install and restart your computer
4. Reconnect ESP32

**For CH340 chip:**

1. Go to https://sparks.gogo.co.nz/ch340.html
2. Download the driver for your OS
3. Install and restart your computer
4. Reconnect ESP32

### Step 1.4: Install Python (for credential converter)

**For Windows:**

1. Go to https://www.python.org/downloads/
2. Download Python 3.x (latest version)
3. **IMPORTANT**: Check "Add Python to PATH" during installation
4. Click Install Now
5. Verify installation:
   ```cmd
   python --version
   ```

**For macOS:**

1. Python 3 is usually pre-installed
2. Verify:
   ```bash
   python3 --version
   ```
3. If not installed, download from https://www.python.org/downloads/

**For Linux:**

```bash
sudo apt update
sudo apt install python3 python3-pip
python3 --version
```

---

## Part 2: Hardware Assembly

### Step 2.1: Identify ESP32 Pins

Look at your ESP32 board and locate these pins:

- **GPIO 25** (labeled as D25, GPIO25, or just 25)
- **GPIO 26** (labeled as D26, GPIO26, or just 26)
- **GND** (Ground - usually multiple GND pins available)

**Common ESP32 Board Pinouts:**

```
ESP32-DevKitC Layout (example):
                    ┌─────────────┐
                    │   USB Port  │
                    └─────────────┘
    3V3  ──┤1                 38├── GND
    EN   ──┤2                 37├── GPIO23
    VP   ──┤3                 36├── GPIO22
    VN   ──┤4                 35├── GPIO1
    GPIO34──┤5                 34├── GPIO3
    GPIO35──┤6                 33├── GPIO21
    GPIO32──┤7                 32├── GND
    GPIO33──┤8                 31├── GPIO19
    GPIO25──┤9  ← DATA0        30├── GPIO18
    GPIO26──┤10 ← DATA1        29├── GPIO5
    GPIO27──┤11                28├── GPIO17
    GPIO14──┤12                27├── GPIO16
    GPIO12──┤13                26├── GPIO4
    GND   ──┤14 ← GROUND       25├── GPIO0
    ...
```

### Step 2.2: Identify Access Panel Terminals

Locate the reader input terminals on your access control panel. They are usually labeled:

- **D0** or **Data0** or **Green wire**
- **D1** or **Data1** or **White wire**
- **GND** or **0V** or **COM** or **Black wire**

**For Paxton Net2 Plus:**
- Look for the green terminal block on the board
- Terminals are labeled: D0, D1, 12V, 0V
- You'll use: **D0**, **D1**, and **0V (GND)**

### Step 2.3: Connect Wires

**IMPORTANT: Disconnect power from the access panel before wiring!**

Use 3 jumper wires to make these connections:

| ESP32 Pin | Wire Color (suggested) | Access Panel Terminal |
|-----------|------------------------|----------------------|
| GPIO 25   | Green                  | D0 (Data0)           |
| GPIO 26   | White                  | D1 (Data1)           |
| GND       | Black                  | GND (0V)             |

**Connection Steps:**

1. **Turn OFF power** to your access control panel
2. Take a green jumper wire
3. Connect one end to **GPIO 25** on ESP32
4. Connect other end to **D0** terminal on access panel
5. Take a white jumper wire
6. Connect one end to **GPIO 26** on ESP32
7. Connect other end to **D1** terminal on access panel
8. Take a black jumper wire
9. Connect one end to **GND** on ESP32
10. Connect other end to **GND (0V)** terminal on access panel

**Visual Diagram:**

```
ESP32                          Access Control Panel
┌──────────┐                   ┌────────────────┐
│          │                   │                │
│  GPIO25  ├───Green Wire──────┤ D0             │
│          │                   │                │
│  GPIO26  ├───White Wire──────┤ D1             │
│          │                   │                │
│   GND    ├───Black Wire──────┤ GND (0V)       │
│          │                   │                │
│   USB    │◄──USB Cable───────┤ (to computer)  │
└──────────┘                   └────────────────┘
```

### Step 2.4: Power the ESP32

**Option 1: USB Power (for testing)**
- Connect ESP32 to your computer via USB cable
- ESP32 will be powered and programmable

**Option 2: External Power (for permanent installation)**
- Use a 5V USB power adapter
- Or connect to VIN pin (if your panel provides 5V)
- **Do NOT exceed 5V on VIN or 3.3V on 3V3 pin!**

---

## Part 3: Firmware Configuration

### Step 3.1: Download Project Files

1. Extract the `ble_wiegand_em4100_access_control.zip` file
2. Navigate to the extracted folder
3. You should see:
   ```
   ble_wiegand_access_control/
   ├── ble_wiegand_access_control.ino
   ├── wiegandOutput.h
   ├── config_example.h
   ├── tools/
   └── README.md
   ```

### Step 3.2: Open Firmware in Arduino IDE

1. **Open Arduino IDE**
2. **Go to File → Open**
3. **Navigate to** the `ble_wiegand_access_control` folder
4. **Select** `ble_wiegand_access_control.ino`
5. **Click Open**

You should now see the firmware code with two tabs:
- `ble_wiegand_access_control` (main file)
- `wiegandOutput.h` (library)

### Step 3.3: Find Your BLE Device MAC Address

You need the MAC address of the Bluetooth device(s) you want to use for access.

**For Android Phone:**

1. Open **Settings**
2. Go to **About Phone** → **Status**
3. Look for **Bluetooth Address**
4. Write it down (e.g., `A4:C1:38:AB:CD:EF`)

**For iPhone:**

1. iPhones don't show Bluetooth MAC directly
2. **Option A**: Use a BLE scanner app on another device to see your iPhone's MAC
3. **Option B**: Use the Serial Monitor after uploading firmware to see detected devices

**For BLE Beacon:**

1. Check the beacon's documentation or packaging
2. MAC address is usually printed on the device
3. Or use a BLE scanner app to detect it

**BLE Scanner Apps:**
- **Android**: "nRF Connect" (free, by Nordic Semiconductor)
- **iOS**: "LightBlue" (free)
- **Windows**: "Bluetooth LE Explorer" (Microsoft Store)

### Step 3.4: Configure Authorized Devices

In the Arduino IDE, find this section (around line 45):

```cpp
// Authorized BLE Device MAC Addresses
// Add MAC addresses in the format "aa:bb:cc:dd:ee:ff"
const char* authorizedDevices[] = {
  "aa:bb:cc:dd:ee:ff",  // Example device 1
  "11:22:33:44:55:66",  // Example device 2
  // Add more devices here
};
```

**Replace the example MAC addresses with your actual devices:**

```cpp
const char* authorizedDevices[] = {
  "A4:C1:38:AB:CD:EF",  // John's Phone
  "12:34:56:78:9A:BC",  // Jane's Beacon
  "AA:BB:CC:DD:EE:FF",  // Office Tablet
};
```

**Important Notes:**
- Use **uppercase or lowercase** (both work)
- Use **colons** between bytes (`:`)
- Add a **comma** after each line except the last
- Add **comments** (`//`) to identify each device

### Step 3.5: Configure Output Format

Find this line (around line 35):

```cpp
#define OUTPUT_FORMAT 26  // Change to 26, 34, or 40
```

**Choose your format:**

- **`26`** = Wiegand 26-bit (most common, works with most systems)
- **`34`** = Wiegand 34-bit (extended format for larger systems)
- **`40`** = EM4100 40-bit (for EM4100 readers with Clock & Data output)

**Example:**
```cpp
#define OUTPUT_FORMAT 26  // Using standard Wiegand 26-bit
```

### Step 3.6: Configure Facility Code (Wiegand 26-bit only)

If you chose **Wiegand 26-bit**, set your facility code (around line 38):

```cpp
#define FACILITY_CODE 123
```

**Change `123` to your facility code** (0-255). If you don't know your facility code:
- Check your existing access cards
- Ask your access control administrator
- Use `1` as a default for testing

**Example:**
```cpp
#define FACILITY_CODE 1  // Using facility code 1
```

### Step 3.7: Configure GPIO Pins (optional)

If you want to use different pins than GPIO 25 and 26, change these lines (around line 30):

```cpp
#define WIEGAND_D0_PIN 25
#define WIEGAND_D1_PIN 26
```

**Example (using GPIO 32 and 33):**
```cpp
#define WIEGAND_D0_PIN 32
#define WIEGAND_D1_PIN 33
```

### Step 3.8: Configure Debug Mode (optional)

For initial setup, **leave debug mode ON**:

```cpp
#define DEBUG_MODE true
```

After everything works, you can turn it off for production:

```cpp
#define DEBUG_MODE false
```

### Step 3.9: Save Your Configuration

1. **Go to File → Save** (or press Ctrl+S / Cmd+S)
2. Your configuration is now saved!

---

## Part 4: Uploading Firmware to ESP32

### Step 4.1: Select Board Type

1. **Go to Tools → Board**
2. **Expand "ESP32 Arduino"**
3. **Select your board model:**
   - **ESP32 Dev Module** (most common, works for most boards)
   - **ESP32-DevKitC** (if you have this specific model)
   - **NodeMCU-32S** (if you have NodeMCU ESP32)
   - If unsure, choose **"ESP32 Dev Module"**

### Step 4.2: Select Upload Speed

1. **Go to Tools → Upload Speed**
2. **Select "115200"** (recommended for reliability)
3. If upload fails later, try "921600" for faster upload

### Step 4.3: Select Port

1. **Connect ESP32 to computer via USB**
2. **Wait 5-10 seconds** for drivers to load
3. **Go to Tools → Port**
4. **Select the port:**
   - **Windows**: `COM3`, `COM4`, etc.
   - **macOS**: `/dev/cu.usbserial-XXXX` or `/dev/cu.SLAB_USBtoUART`
   - **Linux**: `/dev/ttyUSB0` or `/dev/ttyACM0`

**If no port appears:**
- Check USB cable (try a different one)
- Install USB drivers (see Step 1.3)
- Try a different USB port on your computer

### Step 4.4: Upload Firmware

1. **Click the Upload button** (→ arrow icon) in Arduino IDE
2. **Wait for "Connecting..."** message
3. **You may see:**
   ```
   Connecting........_____.....
   ```
4. **If it says "Connecting..." for more than 10 seconds:**
   - **Hold the BOOT button** on ESP32
   - **Keep holding** until you see "Writing at 0x00001000..."
   - **Release the button**

5. **Watch the progress:**
   ```
   Sketch uses 234567 bytes (17%) of program storage space.
   Writing at 0x00001000... (5%)
   Writing at 0x00002000... (10%)
   ...
   Writing at 0x00010000... (100%)
   ```

6. **Wait for "Hard resetting via RTS pin..."**

7. **Success message:**
   ```
   Leaving...
   Hard resetting via RTS pin...
   Done uploading.
   ```

**If upload fails:**
- See [Troubleshooting](#troubleshooting) section below

---

## Part 5: Testing and Verification

### Step 5.1: Open Serial Monitor

1. **Go to Tools → Serial Monitor** (or press Ctrl+Shift+M / Cmd+Shift+M)
2. **Set baud rate to 115200** (bottom-right dropdown)
3. **Press the RESET button** on ESP32 (or unplug and replug USB)

### Step 5.2: Verify Startup Messages

You should see output like this:

```
=================================
BLE to Wiegand Access Control
=================================
Output Format: Wiegand 26-bit
Facility Code: 123
D0 Pin: GPIO 25
D1 Pin: GPIO 26
Authorized Devices: 3
=================================

Initializing BLE...
BLE Scanner initialized
System ready. Scanning for devices...

Scan complete. Devices found: 5
---
BLE Device found: a4:c1:38:ab:cd:ef | RSSI: -45
  -> Not authorized
BLE Device found: 12:34:56:78:9a:bc | RSSI: -62
✓ Authorized device detected: 12:34:56:78:9a:bc
=================================
Sending 26-bit Wiegand credential
Facility Code: 123
Card Number: 39612
=================================
Sending 26-bit Wiegand:
  Data: 0x7B9ABC
  Even Parity: 0
  Odd Parity: 1
  Binary: 001111011100110101011110001
  Transmission complete
---
```

### Step 5.3: Test with Your BLE Device

1. **Make sure Bluetooth is ON** on your phone/beacon
2. **Bring your device close** to the ESP32 (within 1-2 meters)
3. **Watch the Serial Monitor**
4. **You should see:**
   - "BLE Device found: [your MAC]"
   - "✓ Authorized device detected"
   - "Sending [format] credential"
   - Binary data output

### Step 5.4: Verify Cooldown

1. **Keep your device near ESP32**
2. **You should see:**
   ```
   ✓ Authorized device detected: 12:34:56:78:9a:bc
     -> Device in cooldown, skipping...
   ```
3. **This prevents multiple rapid reads**
4. **Wait 5 seconds** and it will read again

### Step 5.5: Test Unauthorized Device

1. **Bring a different BLE device** near ESP32 (one NOT in your authorized list)
2. **You should see:**
   ```
   BLE Device found: ff:ee:dd:cc:bb:aa | RSSI: -55
     -> Not authorized
   ```
3. **No credential is sent**

---

## Part 6: Access Controller Setup

### Step 6.1: Generate Credentials

1. **Open a terminal/command prompt**
2. **Navigate to the tools folder:**
   ```bash
   cd path/to/ble_wiegand_access_control/tools
   ```

3. **Create a text file** with your MAC addresses:
   ```bash
   # On Windows (Notepad)
   notepad my_devices.txt
   
   # On macOS/Linux
   nano my_devices.txt
   ```

4. **Add your MAC addresses** (one per line):
   ```
   A4:C1:38:AB:CD:EF
   12:34:56:78:9A:BC
   AA:BB:CC:DD:EE:FF
   ```

5. **Save and close** the file

6. **Run the converter:**
   ```bash
   python3 mac_to_credential.py -i my_devices.txt -o credentials.csv
   ```
   (On Windows, use `python` instead of `python3`)

7. **Open credentials.csv** in Excel or text editor

### Step 6.2: Configure Paxton Net2 Plus

**For Wiegand 26-bit:**

1. **Open Paxton Net2 software**
2. **Go to System → Hardware**
3. **Double-click your door/reader**
4. **Set Reader Type** to "Wiegand reader"
5. **Set Format** to "Wiegand 26 bit"
6. **Click OK**

7. **Go to Users**
8. **Click Add User**
9. **Enter user details** (name, etc.)
10. **Go to Tokens tab**
11. **Click Add**
12. **Select "Wiegand 26 bit"**
13. **Enter Facility Code** from CSV (e.g., 123)
14. **Enter Card Number** from CSV (e.g., 52719)
15. **Click OK**
16. **Repeat for each user**

**For Wiegand 34-bit:**

1. Follow steps 1-6 above, but select **"Wiegand 34 bit"**
2. When adding tokens, use the **W34 Facility Code** and **W34 Card Number** from CSV

**For EM4100 40-bit:**

1. Set Reader Type to **"EM4100"** or **"125kHz RFID"**
2. When adding tokens, enter the **Card ID** from CSV
3. Some systems may require the Version field as well

### Step 6.3: Configure Generic Access Controllers

**For controllers with web interface:**

1. Log into controller web interface
2. Go to Reader/Input settings
3. Set input type to "Wiegand" or "EM4100"
4. Set format to match your ESP32 configuration
5. Go to Users/Cards section
6. Add users with credentials from CSV file

**For controllers with keypad:**

1. Enter programming mode (consult manual)
2. Add user codes
3. Enter facility code and card number from CSV
4. Exit programming mode

---

## Part 7: Final Testing

### Step 7.1: Power Everything On

1. **Power ON** your access control panel
2. **Power ON** the ESP32 (via USB or external power)
3. **Wait 10 seconds** for both to initialize

### Step 7.2: Test Access

1. **Approach the door** with your authorized BLE device
2. **Bluetooth must be ON** on your device
3. **Get within 1-2 meters** of the ESP32
4. **You should:**
   - See activity in Serial Monitor (if connected)
   - Hear a beep from access panel (if configured)
   - See LED indication on panel (if configured)
   - **Door unlocks!**

### Step 7.3: Test Multiple Users

1. **Test each authorized device**
2. **Verify each one grants access**
3. **Check Serial Monitor** to see which device was detected

### Step 7.4: Test Unauthorized Access

1. **Approach with a device NOT in the authorized list**
2. **Door should remain locked**
3. **No beep or LED indication**

### Step 7.5: Test Range

1. **Start far from ESP32** (5+ meters)
2. **Walk slowly toward it**
3. **Note the distance** when access is granted
4. **Typical range**: 2-10 meters (depends on environment)

---

## Troubleshooting

### Upload Issues

**Problem: "Port not found" or "No port available"**

**Solution:**
1. Install USB drivers (see Step 1.3)
2. Try different USB cable
3. Try different USB port on computer
4. Check if ESP32 LED lights up when connected

**Problem: "Connecting........" timeout**

**Solution:**
1. Hold BOOT button during upload
2. Try lower upload speed (115200)
3. Press RESET button, then upload immediately
4. Check if correct board is selected

**Problem: "Sketch too big"**

**Solution:**
1. Go to Tools → Partition Scheme
2. Select "Huge APP (3MB No OTA/1MB SPIFFS)"
3. Try uploading again

### Runtime Issues

**Problem: No BLE devices detected**

**Solution:**
1. Check if Bluetooth is ON on your device
2. Some phones need a BLE app running to advertise
3. Move device closer to ESP32
4. Check Serial Monitor for "BLE Scanner initialized"

**Problem: Device detected but not authorized**

**Solution:**
1. Check MAC address in firmware matches exactly
2. MAC addresses are case-insensitive but format must match
3. Use colons (`:`) not dashes (`-`)
4. Remove any spaces

**Problem: Access panel doesn't respond**

**Solution:**
1. Check wiring connections (D0, D1, GND)
2. Verify panel is configured for Wiegand/EM4100 input
3. Check output format matches panel configuration
4. Use multimeter to verify signals on D0/D1 lines
5. Check facility code matches panel configuration

**Problem: Multiple rapid reads**

**Solution:**
1. Increase `DEVICE_COOLDOWN_MS` value
2. Default is 5000 (5 seconds)
3. Try 10000 (10 seconds)

**Problem: ESP32 keeps resetting**

**Solution:**
1. Check power supply (needs stable 5V, 500mA+)
2. Try different power source
3. Check for short circuits in wiring
4. Remove and reconnect all wires

### Debugging Tips

**Enable verbose output:**

1. Set `DEBUG_MODE` to `true`
2. Upload firmware
3. Open Serial Monitor
4. Watch detailed output

**Check signal with multimeter:**

1. Set multimeter to DC voltage
2. Measure between D0 and GND (should be ~5V idle)
3. Measure between D1 and GND (should be ~5V idle)
4. When device detected, you should see brief voltage drops

**Test with known working card:**

1. If you have existing RFID cards that work
2. Note their facility code and card number
3. Temporarily add those values to firmware
4. Test if panel responds

---

## Next Steps

### For Production Use

1. **Disable debug mode:**
   ```cpp
   #define DEBUG_MODE false
   ```

2. **Use external power supply** (not USB from computer)

3. **Secure the ESP32** in a protective enclosure

4. **Use proper wire management** (cable ties, conduit)

5. **Document your configuration** (save MAC addresses, facility codes)

6. **Create backup** of firmware file

### Advanced Configuration

1. **Adjust scan interval** (line ~42):
   ```cpp
   #define BLE_SCAN_TIME 5  // Increase for better range
   ```

2. **Adjust cooldown** (line ~45):
   ```cpp
   #define DEVICE_COOLDOWN_MS 5000  // Increase to prevent rapid reads
   ```

3. **Change GPIO pins** if needed (line ~30-31)

4. **Add more authorized devices** (up to hundreds supported)

---

## Support

If you encounter issues not covered in this guide:

1. Check the Serial Monitor output for error messages
2. Verify all connections are secure
3. Ensure firmware configuration matches your hardware
4. Consult your access panel documentation
5. Review the PROJECT_SUMMARY.md for technical details

---

**Congratulations!** You've successfully installed and configured your ESP32 BLE Access Control System!

