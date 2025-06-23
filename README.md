# 🌡️ ESP32 Temp‑Humidity & Step Tracker with MQTT

A Wokwi-simulated ESP32 project that reads DHT22 (temp + humidity) and MPU6050 gyroscope data, displays it on two I2C LCDs, and publishes updates to an MQTT broker.

---

## ⚙️ Features

- **DHT22** reads temperature & humidity.
- **MPU6050** gyro rotation detects “steps” (increments when rotation changes >90°).
- Dual **I2C LCDs**:
  - LCD1 (0x27): shows current temperature & humidity.
  - LCD2 (0x28): shows WiFi initialization and status.
- **WiFi** connects to an access point (default: Wokwi‑GUEST).
- **MQTT** using `PubSubClient` to publish:
  - `/Temperature`
  - `/Humidity`
  - `/Steps`
- Publishes updates every 500 ms when connected.

---

## 🛠️ Hardware & Libraries

**ESP32** simulation on Wokwi with:
- `DHTesp`, `LiquidCrystal_I2C`, `WiFi`, `PubSubClient`
- `Adafruit_MPU6050`, `Adafruit_Sensor`

Wiring & library names match Wokwi defaults. :contentReference[oaicite:3]{index=3}

---

## 🧩 Code Overview

- `setup()`:
  - Initialize serial, sensors, LCDs, Wi-Fi & MQTT.
  - Confirm sensor availability.
- `loop()`:
  - Reconnect MQTT if needed.
  - Read DHT22 and MPU6050 gyro.
  - Calculate steps when rotationX changes by >90°.
  - Update LCD and send MQTT messages every 500ms.
- `reconnect()` ensures MQTT reconnection. :contentReference[oaicite:4]{index=4}

---

## 📥 How to Run (Wokwi)

1. Open [this Wokwi link](https://wokwi.com/projects/387707585216871425).
2. Click **Fork** to edit and run your own version.
3. Modify `ssid`, `password`, or `mqttServer` as needed.

---

## 🚀 Future Improvements

- Use MPU6050 accelerometer for true step counting.
- Add timestamped MQTT topics.
- Store history or log to file/SD.
- Add alerts (e.g. high humidity).
- Build a dashboard or integrate with Home Assistant.

---

## 📄 License

MIT License
