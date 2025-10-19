# Wiring Diagrams - ESP32 BLE Access Control

This document provides visual wiring diagrams for connecting your ESP32 to various access control systems.

---

## Basic Wiring Diagram

### Standard Connection

```
┌─────────────────────────────┐
│      ESP32 DevKit Board     │
│                             │
│  ┌───────────────────────┐  │
│  │                       │  │
│  │      ESP32 Chip       │  │
│  │                       │  │
│  └───────────────────────┘  │
│                             │
│  [GPIO 25] ●────────────────┼──── Green Wire ──►  D0 (Data0)
│                             │
│  [GPIO 26] ●────────────────┼──── White Wire ──►  D1 (Data1)
│                             │
│  [GND]     ●────────────────┼──── Black Wire ──►  GND (0V)
│                             │
│  [USB]     ◄────USB Cable───┼──── To Computer/Power
│                             │
└─────────────────────────────┘
                                    ┌──────────────────────┐
                                    │  Access Control Panel│
                                    │                      │
                                    │  ┌────────────────┐  │
                                    │  │ Reader Input   │  │
                                    │  │                │  │
                                    │  │ D0  ●          │  │
                                    │  │ D1  ●          │  │
                                    │  │ 12V ●          │  │
                                    │  │ GND ●          │  │
                                    │  │                │  │
                                    │  └────────────────┘  │
                                    │                      │
                                    └──────────────────────┘
```

---

## ESP32 DevKit Pinout

### Top View of ESP32-DevKitC

```
                         ┌─────────────────┐
                         │   Micro USB     │
                         └─────────────────┘
                                 ▼
    ┌────────────────────────────────────────────────┐
    │  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  │
    │ 3V3 EN VP VN 34 35 32 33 25 26 27 14 12 GND 13│
    │                        ▲  ▲     ▲              │
    │                        │  │     └── GND        │
    │                        │  └────── GPIO 26 (D1) │
    │                        └───────── GPIO 25 (D0) │
    │                                                │
    │              ESP32-DevKitC                     │
    │                                                │
    │ 23 22 TX RX 21 GND 19 18 5 17 16 4 0 2 15 8 7 │
    │  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  ●  │
    └────────────────────────────────────────────────┘
```

**Connections:**
- **GPIO 25** (9th pin from left on top row) → D0
- **GPIO 26** (10th pin from left on top row) → D1
- **GND** (14th pin from left on top row) → GND

---

## Paxton Net2 Plus Wiring

### Paxton Net2 Plus Terminal Block

```
┌─────────────────────────────────────────┐
│         Paxton Net2 Plus Board          │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  Reader 1 Input Terminals         │  │
│  │                                   │  │
│  │  [D0]  [D1]  [12V]  [0V]          │  │
│  │   ●     ●     ●      ●            │  │
│  │   │     │            │            │  │
│  │   │     │            │            │  │
│  └───┼─────┼────────────┼────────────┘  │
│      │     │            │                │
└──────┼─────┼────────────┼────────────────┘
       │     │            │
       │     │            │
    Green  White       Black
       │     │            │
       │     │            │
       ▼     ▼            ▼
    ┌──────────────────────────┐
    │       ESP32 Board        │
    │                          │
    │  GPIO25  GPIO26   GND    │
    │    ●       ●       ●     │
    └──────────────────────────┘
```

**Important Notes:**
- **DO NOT connect** to the 12V terminal
- Only use D0, D1, and 0V (GND)
- ESP32 is powered separately via USB

---

## Generic Access Panel Wiring

### Standard Wiegand Reader Input

Most access panels have similar terminal layouts:

```
Access Panel Terminal Block:
┌─────────────────────────────┐
│  Reader Input               │
│                             │
│  Label:  D0  D1  +12V  GND  │
│  Color:  🟢  ⚪  🔴   ⚫   │
│  Pin:    ●   ●   ●    ●     │
└──────────┼───┼────────┼─────┘
           │   │        │
           │   │        └──── Black wire to ESP32 GND
           │   └──────────── White wire to ESP32 GPIO26
           └──────────────── Green wire to ESP32 GPIO25
```

**Wire Color Convention:**
- 🟢 **Green** = D0 (Data0)
- ⚪ **White** = D1 (Data1)
- ⚫ **Black** = GND (Ground/0V)
- 🔴 **Red** = +12V (DO NOT CONNECT to ESP32)

---

## Power Options

### Option 1: USB Power (Testing/Development)

```
┌──────────┐
│ Computer │
│    or    │
│ USB PSU  │
└────┬─────┘
     │ USB Cable
     ▼
┌─────────────┐
│    ESP32    │
│             │
│  USB Port   │
└─────────────┘
```

**Pros:**
- Easy for testing
- Can monitor Serial output
- No additional wiring

**Cons:**
- Not suitable for permanent installation
- Computer must be on

### Option 2: External 5V Power Supply

```
┌──────────────┐
│ 5V DC Power  │
│   Adapter    │
│  (1A min)    │
└──────┬───────┘
       │
       ├──── Red (+5V) ──► VIN or 5V pin on ESP32
       │
       └──── Black (GND) ──► GND pin on ESP32
```

**Pros:**
- Permanent installation
- Reliable power
- No computer needed

**Cons:**
- Requires additional power supply
- Must be regulated 5V

### Option 3: Power from Access Panel (Advanced)

**⚠️ WARNING: Only if your panel provides regulated 5V output!**

```
Access Panel                    ESP32
┌──────────────┐              ┌──────────┐
│              │              │          │
│  +5V  ●──────┼──Red Wire────┤ VIN      │
│              │              │          │
│  GND  ●──────┼──Black Wire──┤ GND      │
│              │              │          │
└──────────────┘              └──────────┘
```

**Check panel specifications first!**
- Most panels output 12V (too high for ESP32)
- Some have auxiliary 5V output
- Use voltage regulator if only 12V available

---

## Complete Installation Diagram

### Full System with All Components

```
                    ┌─────────────────┐
                    │   Power Source  │
                    │   (5V USB PSU)  │
                    └────────┬────────┘
                             │
                             ▼
    ┌────────────────────────────────────────┐
    │          ESP32 Development Board       │
    │                                        │
    │   GPIO25 ●────────┐                    │
    │                   │                    │
    │   GPIO26 ●────────┼────┐               │
    │                   │    │               │
    │   GND    ●────────┼────┼───┐           │
    │                   │    │   │           │
    └───────────────────┼────┼───┼───────────┘
                        │    │   │
                     Green White Black
                        │    │   │
                        ▼    ▼   ▼
    ┌────────────────────────────────────────┐
    │      Access Control Panel              │
    │                                        │
    │  ┌──────────────────────────────┐      │
    │  │  Reader Input                │      │
    │  │  D0  D1  +12V  GND           │      │
    │  │  ●   ●    ●     ●            │      │
    │  └──────────────────────────────┘      │
    │                                        │
    │  ┌──────────────────────────────┐      │
    │  │  Door Lock Output            │      │
    │  │  COM  NO  NC                 │      │
    │  │  ●    ●   ●                  │      │
    │  └────┬─────┬───────────────────┘      │
    └───────┼─────┼────────────────────────────┘
            │     │
            ▼     ▼
    ┌────────────────┐
    │   Door Lock    │
    │   Mechanism    │
    └────────────────┘

         ┌──────────────┐
         │ BLE Device   │
         │ (Phone/      │
         │  Beacon)     │
         └──────────────┘
                │
                │ Bluetooth
                │ Signal
                ▼
         (Detected by ESP32)
```

---

## Wire Specifications

### Recommended Wire Gauge

| Distance | Wire Gauge | Wire Type |
|----------|-----------|-----------|
| < 3 meters | 22-24 AWG | Jumper wires |
| 3-10 meters | 20-22 AWG | Stranded copper |
| 10-50 meters | 18-20 AWG | Alarm cable |
| > 50 meters | 16-18 AWG | Shielded cable |

### Wire Types

**For short distances (< 3m):**
- Standard jumper wires (male-to-male or male-to-female)
- Breadboard wires
- Dupont connectors

**For permanent installation:**
- 22 AWG stranded alarm cable
- CAT5/CAT6 ethernet cable (use 3 pairs)
- Shielded cable for long runs or noisy environments

---

## Connection Checklist

Before powering on, verify:

- [ ] GPIO 25 connected to D0
- [ ] GPIO 26 connected to D1
- [ ] ESP32 GND connected to Panel GND
- [ ] **NO connection** to 12V terminal
- [ ] All connections are secure
- [ ] No bare wire exposed (short circuit risk)
- [ ] ESP32 has proper 5V power supply
- [ ] Access panel is powered off during wiring

---

## Testing Connections

### Visual Inspection

1. Check all three wires are connected
2. Verify correct GPIO pins (25, 26, GND)
3. Ensure no loose connections
4. Look for any damaged wires

### Multimeter Testing

**Before powering on:**

1. Set multimeter to continuity mode (beep)
2. Test D0 wire: Touch one probe to GPIO25, other to D0 terminal
3. Should beep (connection confirmed)
4. Test D1 wire: Touch one probe to GPIO26, other to D1 terminal
5. Should beep (connection confirmed)
6. Test GND wire: Touch one probe to ESP32 GND, other to panel GND
7. Should beep (connection confirmed)

**After powering on:**

1. Set multimeter to DC voltage (20V range)
2. Measure D0 to GND: Should read ~5V (idle state)
3. Measure D1 to GND: Should read ~5V (idle state)
4. When device detected, voltage should briefly drop to ~0V

---

## Safety Notes

⚠️ **Important Safety Information:**

1. **Never connect ESP32 to 12V** - It will be destroyed instantly
2. **Always disconnect panel power** before wiring
3. **Use proper wire gauge** for the distance
4. **Avoid running wires** near high-voltage AC lines
5. **Secure all connections** to prevent shorts
6. **Use proper enclosure** for permanent installation
7. **Follow local electrical codes** for access control wiring

---

## Common Wiring Mistakes

❌ **Wrong:**
- Connecting 12V to ESP32 (will destroy board)
- Swapping D0 and D1 (won't work)
- Missing GND connection (unreliable operation)
- Using thin wire for long runs (signal degradation)

✅ **Correct:**
- Only connect D0, D1, and GND
- Power ESP32 separately with 5V
- Use proper wire gauge for distance
- Secure all connections

---

For more information, see:
- `INSTALLATION_GUIDE.md` - Complete setup instructions
- `QUICK_START.md` - Fast setup for experienced users
- `README.md` - Project overview and features

