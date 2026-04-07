# ♻️ CleanWise

Smart Waste Management System using Embedded Systems and IoT.

---

## 🚀 About

CleanWise is a smart dumpster system designed to improve waste management by monitoring fill levels, tracking user recycling activity, and optimizing collection processes.

The system combines embedded hardware with a remote application to create a connected and efficient recycling ecosystem.

---

## ⚙️ Features

- 📦 Waste level detection (full / available)  
- ⚖️ Weight measurement of deposited waste  
- 🟢🔴 LED status indication  
- 📲 User interaction via app (ID + points system)  
- ☁️ Real-time data sent to remote server  
- 🔔 Notifications when dumpsters are full  

---

## 🏗️ Architecture

### 🔧 Hardware
- Raspberry Pi 4  
- Infrared sensors  
- Load Cell + HX711  
- Reed switch (lid detection)  
- LED indicator  

### 💻 Software
- C++  
- Linux (Buildroot)  
- PThreads  
- MQTT (Mosquitto)  
- MySQL  
- Qt (GUI application)  

---

## 📄 Documentation

Full report available here:  
👉 `docs/Group11_Implementation_Report.pdf`
