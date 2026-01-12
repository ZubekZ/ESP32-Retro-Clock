# 🕹️ Smart Retro Clock (ESP32)

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue) ![Platform](https://img.shields.io/badge/Platform-ESP32-red) ![Status](https://img.shields.io/badge/Status-Prototype-yellow)

## 📖 Project Overview
The **Smart Retro Clock** is an embedded system project developed at IFPR (Federal Institute of Paraná). It is designed to bridge the gap between retro aesthetics and modern IoT functionality.

Built on the **ESP32** microcontroller, this device acts as a desktop companion that displays time, system status, and reactive animations on a **2.4" TFT Display**. Unlike standard IoT clocks that rely solely on Wi-Fi/NTP, this project explores **UART Serial Communication** to synchronize data directly with a host PC, ensuring low latency and reduced power consumption.

## ✨ Key Features
* **🔌 Hybrid Synchronization:** Designed to fetch time and data via Serial (USB) communication.
* **🎨 High-Performance Rendering:** Utilizes **SPI protocol** to drive the TFT Display (ST7789/ILI9341 drivers) targeting 30+ FPS.
* **🤖 Reactive "Moods":** A "Tamagotchi-like" personality system concept, where the clock changes expressions (Happy, Tired, Irritated) based on system uptime.
* **⚡ Optimized Engineering:** Designed for low power consumption (< 180 mA target) and fast boot time (< 5 seconds).

## 🛠️ Hardware & Tech Stack
* **Microcontroller:** ESP32 DevKit V1 (32-bit dual-core, 240MHz).
* **Display:** 2.4" TFT LCD (SPI Interface).
* **Protocols:**
    * **SPI:** High-speed display communication (MOSI, CLK, CS, DC, RST).
    * **UART:** Bidirectional JSON/String communication with the PC.
    * **GPIO:** Interrupt-based button control.

## 🔌 Pinout Configuration (Default)
| Component | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **TFT MOSI** | GPIO 23 | Data |
| **TFT CLK** | GPIO 18 | Clock |
| **TFT CS** | GPIO 5 | Chip Select |
| **TFT DC** | GPIO 2 | Data/Command |
| **TFT RST** | GPIO 4 | Reset |
| **Button** | GPIO 15 | Input (Pull-up) |

## ⚙️ Engineering Requirements
This project was architected to meet specific non-functional requirements:
1.  **Latency:** Serial communication response < 100ms.
2.  **State Management:** Finite State Machine (FSM) architecture to handle transitions (Clock -> Menu -> Sleep).
3.  **Visual Style:** 8-bit Pixel Art aesthetic.

## 👨‍💻 Author
**João Pedro da Veiga Zubek**
*Computer Science Student at IFPR*  
