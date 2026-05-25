# 💧 HydroSense — Smart Hydration Bottle

A real-time IoT water quality monitoring system built with Arduino Uno R4 WiFi and a React dashboard. Sensors inside the bottle measure TDS and temperature, send data to ThingSpeak every 10 seconds, and reflect live readings on a web dashboard.

---

## 🌐 Live Demo

**[https://smartbottle-kappa.vercel.app/](https://smartbottle-kappa.vercel.app/)**

---

## 🔧 Hardware Components

| Component | Purpose | Pin |
|---|---|---|
| Arduino Uno R4 WiFi | Main controller + WiFi | — |
| TDS Sensor | Measures water purity (ppm) | A0 |
| DS18B20 Temperature Sensor | Measures water temperature (°C) | D2 |
| OLED Display (SSD1306 128×64) | Shows live readings locally | I2C (SDA/SCL) |
| 4.7kΩ Resistor | Pull-up for DS18B20 | Between D2 and 3.3V |
| Breadboard + Jumper Wires | Connections | — |
| USB Cable | Power + code upload | — |

---

## 🧠 How It Works

```
TDS Sensor ──┐
             ├──► Arduino Uno R4 WiFi ──► ThingSpeak ──► HydroSense Dashboard
DS18B20  ────┘         │
                       ▼
                  OLED Display
                 (local preview)
```

1. Arduino reads TDS and temperature every 10 seconds
2. Displays values on the OLED screen instantly
3. Sends a JSON payload to ThingSpeak over WiFi
4. Dashboard fetches latest data from ThingSpeak every 10 seconds
5. All cards, charts and alerts update automatically

---

## 📡 Data Flow

Arduino sends this data to ThingSpeak:

```json
{
  "field1": "421.3",
  "field2": "28.4",
  "field3": "512"
}
```

| Field | Data | Unit |
|---|---|---|
| field1 | TDS value | ppm |
| field2 | Temperature | °C |
| field3 | Raw analog value | 0–1023 |

---

## 💻 Software — Dashboard

Built with React + Tailwind CSS + Recharts.

### Pages

| Page | What it shows |
|---|---|
| Dashboard | Live TDS, temperature, hydration tracker, charts, alerts |
| History | Last 50 readings in a table, newest first |
| Settings | Daily water goal, reminder interval, user name |
| About | Project info, components, team |

### TDS Water Quality Scale

| TDS Range | Status | Color |
|---|---|---|
| 0 – 300 ppm | Pure / Excellent | 🟢 Green |
| 301 – 600 ppm | Good / Acceptable | 🟡 Yellow |
| 601 – 900 ppm | Moderate — Filter Soon | 🟠 Orange |
| 900+ ppm | Poor — Do Not Drink | 🔴 Red |

### Temperature Scale

| Range | Status |
|---|---|
| Below 10°C | Too Cold |
| 10°C – 30°C | Ideal |
| 30°C – 40°C | Warm |
| Above 40°C | Too Hot |

---

## 🚀 Getting Started

### Step 1 — Arduino Setup

Install these libraries in Arduino IDE:

```
OneWire
DallasTemperature
Adafruit GFX Library
Adafruit SSD1306
WiFiS3 (built-in for Uno R4 WiFi)
```

Open the Arduino code and fill in your credentials at the top:

```cpp
const char* SSID       = "YOUR_WIFI_NAME";
const char* PASSWORD   = "YOUR_WIFI_PASSWORD";
const char* TS_API_KEY = "YOUR_THINGSPEAK_WRITE_KEY";
```

Upload the code to your Arduino Uno R4 WiFi.  
Open Serial Monitor at **9600 baud** to confirm connection:

```
Connecting to WiFi.....
WiFi connected: 192.168.1.12
ThingSpeak response: 1
Sent → TDS:421.3 Temp:28.4 Raw:512
```

---

### Step 2 — ThingSpeak Setup

1. Go to [thingspeak.com](https://thingspeak.com) and sign up (free)
2. Create a new channel with these fields:
   - Field 1 → TDS (ppm)
   - Field 2 → Temperature (C)
   - Field 3 → Raw TDS
3. Copy your **Channel ID** from the URL
4. Go to **API Keys** tab → copy the **Write API Key**
5. Paste Write API Key into Arduino code
6. Copy **Read API Key** for the dashboard

---

### Step 3 — Dashboard Setup

In the dashboard source code, find and fill in:

```js
const CHANNEL_ID   = "YOUR_CHANNEL_ID";
const READ_API_KEY = "YOUR_READ_API_KEY";
```

Then run the project:

```bash
npm install
npm run dev
```

Open [http://localhost:3000](http://localhost:3000)

---

## 📁 Project Structure

```
hydrosense/
│
├── arduino/
│   └── hydrosense.ino        # Main Arduino sketch
│
├── dashboard/
│   ├── src/
│   │   ├── pages/
│   │   │   ├── Dashboard.jsx
│   │   │   ├── History.jsx
│   │   │   ├── Settings.jsx
│   │   │   └── About.jsx
│   │   ├── components/
│   │   │   ├── Navbar.jsx
│   │   │   ├── StatCard.jsx
│   │   │   ├── AlertStrip.jsx
│   │   │   └── Charts.jsx
│   │   └── App.jsx
│   ├── package.json
│   └── README.md
│
└── README.md                 # This file
```

---

## ⚙️ Configuration Reference

| Variable | File | Description |
|---|---|---|
| `SSID` | Arduino | Your WiFi network name |
| `PASSWORD` | Arduino | Your WiFi password |
| `TS_API_KEY` | Arduino | ThingSpeak Write API Key |
| `CHANNEL_ID` | Dashboard | ThingSpeak Channel ID |
| `READ_API_KEY` | Dashboard | ThingSpeak Read API Key |
| `SEND_EVERY` | Arduino | Seconds between each upload (default: 10) |

---

## 🔄 Real-Time Sync Details

- Arduino sends data to ThingSpeak every **10 seconds**
- Dashboard polls ThingSpeak every **10 seconds**
- Every fetch uses `cache: "no-store"` + `?t=Date.now()` to prevent stale data
- Dashboard stores last **50 readings** in a local array
- Stat cards always use `readings[0]` — the most recent entry
- Charts use the last **20 readings**
- History page shows all **50 readings**

---

## 🛠️ Troubleshooting

| Problem | Cause | Fix |
|---|---|---|
| Serial Monitor shows `....` forever | Wrong WiFi credentials | Double check SSID and PASSWORD |
| ThingSpeak response is `0` | Sending faster than 15s | Arduino already handles this — wait |
| Dashboard shows "Waiting for data" | No readings yet | Wait for first Arduino upload |
| Dashboard shows OFFLINE badge | ThingSpeak fetch failed | Check internet connection |
| Readings one step behind | Browser cache | Already fixed with `no-store` + timestamp |
| Temperature shows -127°C | DS18B20 wiring issue | Check 4.7kΩ pull-up resistor |

---

## 📊 ThingSpeak API Endpoints Used

```
Latest reading:
GET https://api.thingspeak.com/channels/{ID}/feeds/last.json
    ?api_key={KEY}&results=1&t={timestamp}

Chart history:
GET https://api.thingspeak.com/channels/{ID}/feeds.json
    ?api_key={KEY}&results=20&t={timestamp}

Full history (50 entries):
GET https://api.thingspeak.com/channels/{ID}/feeds.json
    ?api_key={KEY}&results=50&t={timestamp}
```

---

## 🎓 Project Info

| Detail | Info |
|---|---|
| Project Name | HydroSense — Smart Hydration Bottle |
| Category | IoT + Web Dashboard |
| Hardware | Arduino Uno R4 WiFi |
| Cloud | ThingSpeak (free tier) |
| Frontend | React + Tailwind CSS + Recharts |

---

## 🔭 Future Scope

### 🏗️ Hardware Improvements

**PLA 3D Printed Bottle Body**
Replace the current plastic bottle with a custom-designed bottle body printed using PLA (Polylactic Acid) filament. PLA is food-safe, biodegradable, and can be printed in any shape — allowing a perfectly fitted enclosure for all electronics and sensors with no exposed wires.

**pH Sensor Integration**
Add a pH sensor alongside the TDS sensor to detect water acidity or alkalinity. Ideal drinking water is between pH 6.5 and 8.5. This would make the water quality analysis far more complete and accurate.

**Turbidity Sensor**
Add a turbidity sensor to measure how clear or cloudy the water is. Cloudy water can indicate bacteria or suspended particles even when TDS is low — adding a safety layer the current system cannot detect.

**Flow Sensor**
Attach a small water flow sensor at the bottle mouth to automatically detect every sip. This removes the need for the manual "+1 Glass" button and makes hydration tracking fully automatic.

**Rechargeable Battery + Wireless Charging**
Replace the USB-powered setup with a built-in 3.7V LiPo battery and TP4056 charging module. Add wireless (Qi) charging support so the bottle charges by simply placing it on a pad — no cables needed.

**UV-C LED Water Purification**
Embed a small UV-C LED inside the bottle that activates on demand to kill bacteria and viruses in the water. Controlled directly from the dashboard or with a physical button on the bottle.

**Soft Touch Capacitive Buttons**
Replace physical push buttons with capacitive touch sensors on the outer surface of the bottle for a sleek, waterproof, seamless look.

---

### 📱 Software Improvements

**Mobile App (React Native)**
Build a dedicated iOS and Android app using React Native so users get push notifications, hydration reminders, and live sensor readings directly on their phone — even when the browser is closed.

**AI-Powered Water Quality Prediction**
Train a simple machine learning model on historical TDS and temperature readings to predict when water quality will drop or when a filter replacement is needed — before it actually happens.

**Voice Assistant Integration**
Connect HydroSense with Google Assistant or Alexa so users can ask "Hey Google, is my water safe to drink?" and get an instant spoken answer based on live sensor data.

**Multi-Bottle Support**
Allow multiple bottles to be registered under one account — useful for families or shared office spaces — each with their own independent live readings and history.

**Weekly Health Report via Email**
Auto-generate and email a weekly PDF report showing average TDS, average temperature, total water consumed, and hydration streaks — similar to a Fitbit weekly summary.

**Offline Mode with Local Storage Sync**
Store sensor readings locally on the device when internet is unavailable and sync them to ThingSpeak automatically once the connection is restored — no data loss during outages.

---

### 🌍 Scalability and Real-World Use

**Water Source Mapping**
Allow users to tag their water source (tap, borewell, RO filtered, bottled) and build a community map of water quality across cities — crowdsourced water quality data at scale.

**Integration with Health Platforms**
Sync daily water intake data with Apple Health, Google Fit, or Fitbit so HydroSense becomes part of the user's complete health picture alongside sleep, steps, and heart rate.

**School and Hospital Deployment**
Scale the system to monitor water quality in multiple taps or dispensers across a school or hospital building — each sensor reporting to a central admin dashboard with zone-wise alerts.

**Filter Life Estimator**
Based on cumulative TDS exposure over time, calculate and display estimated remaining filter life. Alert the user when the filter needs replacement before water quality actually degrades.


## 📄 License

This project is built for educational purposes.  
Free to use, modify, and improve.
