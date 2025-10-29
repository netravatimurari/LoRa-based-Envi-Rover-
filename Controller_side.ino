#include <WiFi.h>
const char* ssid = "RoverControl"; // AP name
const char* password = "12345678"; // AP password
WiFiServer server(80);
WiFiClient client;
// Globals to store the LAST known sensor values
float currentHumidity = 0.0;
float currentTemperature = 0.0;
int currentGasADC = 0;
int currentLightADC = 0; // LDR ADC value
float currentGasVoltage = 0.0; 
// --- PRINT CONTROL GLOBALS --- // ADDED
unsigned long lastPrintTime = 0;
const long PRINT_INTERVAL = 1000; // Print telemetry no faster than every 1000 milliseconds (1 second)
// Function to determine air quality status based on MQ2 ADC value (0-4095)
String getGasStatus(int adcValue) {
 if (adcValue <= 1500) {
 return "GOOD";
 } else if (adcValue <= 2500) {
 return "MODERATE";
 } else {
 return "HAZARDOUS";
 }
}
// Function to determine light status based on LDR ADC value (0-4095)
String getLightStatus(int adcValue) {
 // Assuming LDR module outputs HIGH (4095) in dark and LOW (0) in bright light
 if (adcValue >= 3500) {
 return "DARK";
 } else if (adcValue >= 1000) {
 return "NORMAL";
 } else {
 return "BRIGHT";
 }
}
// Function to print the complete telemetry line
void printTelemetry() {
 String gasStatus = getGasStatus(currentGasADC);
 String lightStatus = getLightStatus(currentLightADC);
 
 Serial.print("Temp: ");
 Serial.print(currentTemperature, 1); 
 Serial.print("°C | Humid: ");
 Serial.print(currentHumidity, 1); 
 Serial.print("% | Gas: ");
 Serial.print(currentGasADC);
 Serial.print(" (");
 Serial.print(gasStatus);
 Serial.print(") | Light: "); 
 Serial.print(currentLightADC);
 Serial.print(" (");
 Serial.print(lightStatus);
 Serial.println(")");
}
void setup() {
 Serial.begin(115200);
 // Start Wi-Fi Access Point
 WiFi.softAP(ssid, password);
 IPAddress IP = WiFi.softAPIP();
 Serial.print("Controller AP IP: ");
 Serial.println(IP);
 server.begin();
Serial.println("Waiting for Rover to connect...");
}
void loop() {
 // Accept Rover connection
 if (!client || !client.connected()) {
 client = server.available();
 if (client) {
 Serial.println("\n--- Rover connected! ---");
 }
 }
 // Send commands typed in Serial Monitor (W, S, A, D, X)
 if (Serial.available() && client.connected()) {
 char c = Serial.read();
 client.write(c);
 Serial.print("Sent Command: ");
 Serial.println(c);
 }
 // ------------------------------------------------------------------
 // Receive and update sensor data
 // ------------------------------------------------------------------
 if (client.connected() && client.available()) {
 String incoming = client.readStringUntil('\n');
 incoming.trim(); 
 bool dataUpdated = false;
 // 1. Check for MQ2 data (Format: MQ2:value,voltage)
 if (incoming.startsWith("MQ2:")) {
 String data = incoming.substring(4); 
 int commaIndex = data.indexOf(',');
 
 if (commaIndex != -1) {
 currentGasADC = data.substring(0, commaIndex).toInt();
 currentGasVoltage = data.substring(commaIndex + 1).toFloat();
 dataUpdated = true;
 }
 }
 // 2. Check for DHT data (Format: DHT:H,T)
 else if (incoming.startsWith("DHT:")) {
 String data = incoming.substring(4); 
 int commaIndex = data.indexOf(',');
 
 if (commaIndex != -1) {
 currentHumidity = data.substring(0, commaIndex).toFloat();
 currentTemperature = data.substring(commaIndex + 1).toFloat();
 dataUpdated = true;
 }
 }
 
 // 3. Check for LDR data (Format: LDR:value)
 else if (incoming.startsWith("LDR:")) {
 String data = incoming.substring(4);
 currentLightADC = data.toInt();
 dataUpdated = true;
 }
 
 // ------------------------------------------------------------------
 // Print Control Logic (The requested delay mechanism) // MODIFIED
 // ------------------------------------------------------------------
 // Only print if:
 // 1. New data was received AND
 // 2. The minimum print interval has passed
 if (dataUpdated && (millis() - lastPrintTime >= PRINT_INTERVAL)) {
 printTelemetry();
 lastPrintTime = millis(); // Reset the timer
 }
 }
}
