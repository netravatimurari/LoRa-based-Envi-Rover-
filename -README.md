# LoRa-based-Envi-Rover-

![Image](https://github.com/user-attachments/assets/ed85a1c0-3f86-4fff-8d42-30003dea8493)


An environment monitoring rover is remotely operated robotic system built using Lora module, sensors and microcontroller for real-time data collection via C++ programming.

It integrates ESP32 with various sensors (temperature, humidity, illumination, and gas detection) and a LoRa communication module to transmit data over long distances. Users can access the data via Blynk on mobile or PC.

---

## 💻 Project Overview


The Envi Rover is LoRa based, long range, remotely operated robotic system designed to monitor environmental parameters such as temperature, humidity, darkness, gas leakage and air quality of disaster zones, agricultural, industrial environments and smart cities. It uses LoRa communication to transmit data wirelessly over long distances without the usage of internet.

---

## 🧭 System Flowchart
Below is the system flow for data transmission and control between the Rover and Controller.

<img width="1224" height="816" alt="Image" src="https://github.com/user-attachments/assets/9110eba2-df36-4a99-846d-80a49bcfa5e8" />

---

## ⚙️ Features
- Remote Control: Move the rover using W, A, S, D keys; X to stop.
- Real-Time Data Monitoring: Temperature, humidity, gas levels, and illumination are continuously monitored and transmitted.
- Long-Range Communication: LoRa module enables data transmission over large distances.
- Obstacle Detection: Ultrasonic sensor with servo rotation detects and avoids obstacles automatically.
- Blynk Integration: View live environmental data from anywhere using the Blynk dashboard.
- Deployment Ready: Suitable for agriculture, disaster zones, industrial environments, and scientific research.

---

## 🧩 Components Used
- ESP32 (Microcontroller)  
- LoRa SX1278 module  
- DHT22 sensor (Temperature & Humidity)  
- MQ135 sensor (Air Quality)
- LDR sensor 
- Motor driver (Cytron MDD10A)  
- DC Motors and Chassis  

---

## 🔧 Working Principle
The sensors collect environmental data, which is processed by the microcontroller and transmitted through the LoRa module to a remote receiver. The controller side displays the collected data in real-time.



## ✔️ How it Works
- The rover continuously collects data from sensors and transmits it via LoRa to the controller.
- Users can send movement commands (W, A, S, D) to the rover, and X stops it.
- Ultrasonic sensor with servo rotates automatically to detect obstacles and avoid collisions.
- Blynk integration allows users to view real-time data on mobile or PC dashboards.
- Data can be accessed anytime, enabling remote monitoring even in inaccessible locations.

---

## 🌐 Blynk Dashboard (Web)
Blynk web dashboard showing real-time sensor data visualization

![Image](https://github.com/user-attachments/assets/fd29ba76-5182-47a6-b681-08b8915a4d61)

---

## 💻 Code
All source codes are available in the repository (Rover side and Controller side).

---

## 🧑‍💻 Authors
Developed by
Netravati Murari,
[teammate 1] Narendra Giriyappanavar
[teammate 2] Harish Patil
[teammate 3] Murali Vhanmani

---

## 📫 Contact
If you have any queries, feel free to reach out at:  
📧 netrajoy128@gmail.com
📧 hbpatil275@gmail.com
📧 muralivijayvhanmani@gmail.com
📧 naren.rg09@gmail.com

---

## 🔗links
https://www.espressif.com/en/products/socs/esp32
https://www.semtech.com/products/wireless-rf/lora-transceivers
https://blynk.io/
