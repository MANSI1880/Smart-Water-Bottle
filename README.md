# HydroSense | Smart Hydration Bottle Dashboard

HydroSense is a modern, responsive web application designed to monitor water quality (TDS), temperature, and daily hydration goals in real-time. It connects to a ThingSpeak-enabled smart water bottle to provide live analytics and historical data.

## Features

- **Live Monitoring**: Real-time data polling from ThingSpeak sensors.
- **Hydration Tracking**: Log your daily water intake with a persistent glass counter.
- **Visual Analytics**: Interactive line charts for TDS and Temperature history using Recharts.
- **Dark/Light Mode**: Full theme support with persistent user preference.
- **Responsive Design**: optimized for both desktop and mobile devices with a dedicated mobile navigation bar.
- **Offline Reliability**: Graceful handling of API failures with last-known data persistence and status badges.

## Tech Stack

- **Framework**: React 19 + Vite
- **Styling**: Tailwind CSS v4
- **Charts**: Recharts
- **Icons**: Lucide React
- **Routing**: React Router 7

## Getting Started

### Prerequisites

- Node.js (v18 or higher)
- npm or yarn

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/MANSI1880/Smart-Water-Bottle.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Smart-Water-Bottle
   ```
3. Install dependencies:
   ```bash
   npm install
   ```

### Configuration

Open `src/hooks/useThingSpeak.js` and update the following constants with your ThingSpeak credentials:

```javascript
const CHANNEL_ID = 'YOUR_CHANNEL_ID';
const READ_API_KEY = 'YOUR_READ_API_KEY';
```

### Running Locally

```bash
npm run dev
```
The app will be available at `http://localhost:5173`.

## Hardware Components Used

- Arduino Uno R4 WiFi
- TDS Sensor (Analog)
- DS18B20 Temperature Sensor
- OLED Display

## License

This project is open-source and available under the MIT License.
