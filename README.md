# Rover: MIE444 Mechatronics Principles Project

## Project Overview
This repository contains the code developed as part of the **MIE444: Mechatronics Principles** course. The project involved designing and building a rover capable of navigating a maze, identifying a loading zone, picking up a load, and delivering it to a designated drop-off point. 

---

## Features
- **Custom Superstructure Design**: Designed in **SolidWorks**, including mounts and a gripper mechanism. Components were either 3D-printed or laser-cut.
- **Gamified Control**: Users can control the rover via a smartphone app paired with a **HC-05 Bluetooth module**.
- **Sensor Visualization**: Ultrasonic sensor data is transmitted via a **HM-10 Bluetooth module** and displayed on a laptop.
- **Dual Arduino Setup**: An **UNO** handles user interaction, while a **Mega** integrates sensor data and drives the rover.
- **PID Motor Control**: Ensures precise movement using encoders on the DC motors.
- **Orientation Assistance**: A compass provides directional feedback within the maze.

---

## Technologies Used
- **SolidWorks**: For mechanical design.
- **C++**: Arduino programming for localization, obstacle avoidance, and block pickup.
- **Python**: Visualization of sensor data on an external laptop.

---

## Future Improvements
- **Autonomous Navigation**: Implement histogram localization.
- **Advanced Sensors**: Upgrade to time-of-flight sensors for better accuracy.
- **Power Optimization**: Consolidate power supplies to reduce complexity and cost.

---

## Video Highlights
Check out the highlights of our project: https://www.youtube.com/watch?v=i4Io34JAZaQ

---
