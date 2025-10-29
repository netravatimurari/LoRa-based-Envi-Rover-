#include <HardwareSerial.h>
#include "DHT.h"
// ---- LoRa ----
HardwareSerial LoRa(1); // UART1
// ---- Motor driver pins ----
#define DIR1 14
#define PWM1 27
#define DIR2 12
#define PWM2 26
int speedVal = 200;
// ---- DHT22 Sensor ----
#define DHTPIN 32
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// ---- LDR Sensor ----
#define LDR_A0 33
#define LDR_D0 25
// ---- MQ-2 Gas Sensor ----
#define MQ2_A0 34
#define MQ2_D0 35
// ---- Ultrasonic Sensor ----
#define TRIG_PIN 4
#define ECHO_PIN 2
long duration;
float distance;
// ---- Telemetry ----
unsigned long lastTelemetry = 0;
const long telemetryInterval = 5000; // 5 sec
// ---- Active Command ----
char activeCommand = 'x'; // No movement initially
void setup() {
 Serial.begin(115200);
 LoRa.begin(115200, SERIAL_8N1, 16, 17); // RX=16, TX=17
 Serial.println("■ LoRa Rover Ready");
 // ---- Init LoRa module ----
 LoRa.println("AT+RESET"); delay(1000);
 LoRa.println("AT+ADDRESS=1"); delay(200); // Rover = 1
 LoRa.println("AT+NETWORKID=5"); delay(200);
 LoRa.println("AT+PARAMETER=12,7,1,4"); delay(200);
 LoRa.println("AT+BAND=868000000"); delay(200);
 // ---- Motor pins ----
 pinMode(DIR1, OUTPUT);
 pinMode(DIR2, OUTPUT);
 ledcAttach(PWM1, 5000, 8);
 ledcAttach(PWM2, 5000, 8);
 stopMotors();
 // ---- Sensors ----
 dht.begin();
 pinMode(LDR_A0, INPUT);
 pinMode(MQ2_A0, INPUT);
 pinMode(TRIG_PIN, OUTPUT);
 pinMode(ECHO_PIN, INPUT);
 randomSeed(analogRead(0)); // For random left/right turn
}
// ---- Motor control ----
void forward() { digitalWrite(DIR1, HIGH); digitalWrite(DIR2, HIGH); ledcWrite(PWM1, speedVal); ledcWrite(PWM2, speedVal); }
void backward() { digitalWrite(DIR1, LOW); digitalWrite(DIR2, LOW); ledcWrite(PWM1, speedVal); ledcWrite(PWM2, speedVal); }
void left() { digitalWrite(DIR1, LOW); digitalWrite(DIR2, HIGH); ledcWrite(PWM1, speedVal); ledcWrite(PWM2, speedVal); }

void right() { digitalWrite(DIR1, HIGH); digitalWrite(DIR2, LOW); ledcWrite(PWM1, speedVal); ledcWrite(PWM2, speedVal); }
void stopMotors(){ ledcWrite(PWM1, 0); ledcWrite(PWM2, 0); }
// ---- Ultrasonic ----
float readDistanceCM() {
 digitalWrite(TRIG_PIN, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG_PIN, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_PIN, LOW);
 duration = pulseIn(ECHO_PIN, HIGH, 25000);
 if (duration == 0) return -1;
 return duration / 58.2;
}
// ---- Execute movement command ----
void executeCommand(char cmd) {
 switch (cmd) {
 case 's': case 'S': forward(); Serial.println("➡ Forward"); break;
 case 'w': case 'W': backward(); Serial.println("■ Backward"); break;
 case 'a': case 'A': left(); Serial.println("■ Left"); break;
 case 'd': case 'D': right(); Serial.println("■ Right"); break;
 case 'x': case 'X': stopMotors(); Serial.println("■ Stop"); break;
 }
}
void loop() {
 // ---- Receive control commands ----
 if (LoRa.available()) {
 String incoming = LoRa.readStringUntil('\n');
 if (incoming.startsWith("+RCV=")) {
 int firstComma = incoming.indexOf(',');
 int secondComma = incoming.indexOf(',', firstComma + 1);
 int thirdComma = incoming.indexOf(',', secondComma + 1);
 if (secondComma != -1 && thirdComma != -1) {
 String payload = incoming.substring(secondComma + 1, thirdComma);
 payload.trim();
 if (payload.length() > 0) {
 char cmd = payload.charAt(0);
 activeCommand = cmd; // Save the command
 executeCommand(cmd);
 }
 }
 }
 }
 // ---- Obstacle check (only when moving) ----
 if (activeCommand != 'x' && activeCommand != 'X') {
 float dist = readDistanceCM();
 if (dist > 0 && dist < 20) { // Obstacle detected
 Serial.println("■ Obstacle detected! Stopping...");
 stopMotors();
 delay(500);
 // Turn randomly left or right
 if (random(0, 2) == 0) { left(); Serial.println("■ Avoid Left"); }
 else { right(); Serial.println("■ Avoid Right"); }
 delay(700); // Adjust to control turn time
 executeCommand(activeCommand); // Resume previous command
 }
 }
 // ---- Send sensor telemetry ----
 unsigned long now = millis();
 if (now - lastTelemetry > telemetryInterval) {
 lastTelemetry = now;
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 int ldrRaw = analogRead(LDR_A0);
 float ldrPercent = map(ldrRaw, 4095, 0, 0, 100); // Inverted: high light → low value

int mq2Raw = analogRead(MQ2_A0);
 float dist = readDistanceCM();
 if (!isnan(h) && !isnan(t)) {
 Serial.printf("■ T:%.1f°C H:%.1f%% L:%.0f%% G:%d D:%.1fcm\n",
 t, h, ldrPercent, mq2Raw, dist);
 String msg = "T:" + String(t, 1) + "C H:" + String(h, 1) + "%" +
 " L:" + String(ldrPercent, 0) + "%" +
 " G:" + String(mq2Raw) +
 " D:" + String(dist, 1) + "cm";
 String atCmd = "AT+SEND=2," + String(msg.length()) + "," + msg;
 LoRa.println(atCmd);
 Serial.println("■ Sent: " + msg);
 }
 }
}
