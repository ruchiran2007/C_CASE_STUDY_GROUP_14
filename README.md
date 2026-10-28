# 👩‍🦯 Smart Blind Stick — AI + Raspberry Pi Enhanced Design
> **Multi-Sensor Fusion Architecture for Enhanced Obstacle & Ground Hazard Detection**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Hardware](https://img.shields.io/badge/Hardware-Raspberry_Pi_|_ESP32-blue.svg)]()
[![AI Engine](https://img.shields.io/badge/Model-YOLO_Object_Detection-green.svg)]()

## 📌 Project Overview
Visually impaired individuals face significant safety challenges navigating complex environments due to the limitations of traditional canes—such as overhead hazard blindness, lack of ground drop-off detection, and zero semantic awareness. 

This project presents an **academic prototype** of an AI-enhanced Smart Blind Stick powered by a **decoupled dual-processing architecture**:
- **Raspberry Pi + Pi Camera**: High-level visual AI (YOLO) to identify **"WHAT"** the hazard is (pedestrians, vehicles, ground drop-offs, stairs).
- **ESP32 / Arduino**: Embedded safety controller handling dual ultrasonic sensors to measure **"HOW CLOSE"** hazards are and triggering real-time haptic/audible warnings.

## ✨ Key Features
- 🧠 **Edge Visual AI**: Real-time object classification and semantic hazard context.
- 📡 **Multi-Sensor Fusion**: Combines ultrasonic distance echo with computer vision inference.
- ⚡ **Dual-Processor Split**: Separates heavy vision processing from low-latency real-time actuator outputs.
- 🛑 **Ground Hazard Detection**: Detects stairs, potholes, and open drains before physical contact.
- 🛡️ **Fail-Safe Redundancy**: Automatic fallback to ultrasonic-only proximity mode during camera or lighting faults.
- 🧪 **Rigorously Validated**: Evaluated across a 195-case prototype test matrix.

## 👥 Authors / Team
- **Lade Ruchiran** — Project Lead & Researcher
- **T. Shashank** — Hardware & Embedded Systems Developer
- **V. Karthik** — AI & Computer Vision Specialist
- **M.V.V. Nageswara Rao** — Sensor Fusion & Firmware Engineer
