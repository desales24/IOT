#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C dengan alamat 0x27 dan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisiasi pin sensor
int sensorCahaya = A0;
int sensorHujan = A1;

// Inisiasi pin output
int ledSiang = 10;
int ledMalam = 9;
int ledHujan = 8;
int buzzer = 6;

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.print("Jemuran Otomatis");
  delay(2000);
  lcd.clear();

  // Set pin input/output
  pinMode(sensorCahaya, INPUT);
  pinMode(sensorHujan, INPUT);
  pinMode(ledSiang, OUTPUT);
  pinMode(ledMalam, OUTPUT);
  pinMode(ledHujan, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Baca sensor
  int nilaiCahaya = analogRead(sensorCahaya);
  int nilaiHujan = analogRead(sensorHujan);

  int cahaya = nilaiCahaya < 500 ? 1 : 0;     // 1 = ada cahaya (siang)
  int hujan = nilaiHujan < 500 ? 1 : 0;       // 1 = hujan terdeteksi

  // Debugging di Serial Monitor
  Serial.print("Sensor Cahaya: ");
  Serial.print(nilaiCahaya);
  Serial.print(" | Sensor Hujan: ");
  Serial.println(nilaiHujan);

  // Tampilkan status sensor di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cahaya: ");
  lcd.print(cahaya == 1 ? "Ada  " : "Tidak");

  lcd.setCursor(0, 1);
  lcd.print("Hujan : ");
  lcd.print(hujan == 1 ? "Ada  " : "Tidak");
  delay(2000);

  // Tampilkan status cuaca & kontrol LED/Buzzer
  lcd.clear();
  lcd.setCursor(0, 0);

  if (cahaya == 1 && hujan == 0) {
    lcd.print("Siang Cerah");
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
    digitalWrite(ledHujan, LOW);
    digitalWrite(buzzer, LOW);
  } else if (cahaya == 0 && hujan == 0) {
    lcd.print("Malam Cerah");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    digitalWrite(ledHujan, LOW);
    digitalWrite(buzzer, LOW);
  } else if (cahaya == 1 && hujan == 1) {
    lcd.print("Siang Hujan");
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
    digitalWrite(ledHujan, HIGH);
    digitalWrite(buzzer, HIGH);
  } else if (cahaya == 0 && hujan == 1) {
    lcd.print("Malam Hujan");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    digitalWrite(ledHujan, HIGH);
    digitalWrite(buzzer, HIGH);
  }

  delay(2000);
}
