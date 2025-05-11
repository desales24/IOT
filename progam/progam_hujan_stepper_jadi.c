#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzer = 7;
const int motor_dc = 6;
const int DIR_PIN  = 4;
const int STEP_PIN = 5;
const int LED_siang = 13; // LED siang
const int LED_malam = 10; // LED malam
const int LED_hujan = 8;  // LED hujan

const int sensorCahaya = A0;
const int sensorHujan  = A1;

const int langkah = 400;
const int delayStep = 800;

bool sudahMasuk = false;
bool hujanSebelumnya = false;

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(sensorCahaya, INPUT);
  pinMode(sensorHujan, INPUT);
  pinMode(motor_dc, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LED_siang, OUTPUT);
  pinMode(LED_malam, OUTPUT);
  pinMode(LED_hujan, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jemuran Otomatis");
  delay(2000);
}

void loop() {
  int nilaiCahaya = analogRead(sensorCahaya);
  int nilaiHujan  = analogRead(sensorHujan);

  int cahaya = nilaiCahaya < 500 ? 1 : 0;
  int hujan = nilaiHujan > 500 ? 1 : 0;

  // LED indikator
  digitalWrite(LED_siang, cahaya == 1 ? HIGH : LOW);
  digitalWrite(LED_malam, cahaya == 0 ? HIGH : LOW);
  digitalWrite(LED_hujan, hujan == 1 ? HIGH : LOW);

  // Tampilkan sensor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cahaya: ");
  lcd.print(cahaya == 1 ? "Ada  " : "Tidak");

  lcd.setCursor(0, 1);
  lcd.print("Hujan : ");
  lcd.print(hujan == 1 ? "Ada  " : "Tidak");
  delay(2000);

  // Tampilkan status cuaca
  lcd.clear();
  lcd.setCursor(0, 0);
  if (cahaya == 1 && hujan == 0) {
    lcd.print("Siang Cerah");
  } else if (cahaya == 0 && hujan == 0) {
    lcd.print("Malam Cerah");
  } else if (cahaya == 1 && hujan == 1) {
    lcd.print("Siang Hujan");
  } else if (cahaya == 0 && hujan == 1) {
    lcd.print("Malam Hujan");
  }
  delay(2000);

  // Jemuran masuk saat hujan mulai
  if (hujan == 1 && !sudahMasuk && !hujanSebelumnya) {
    digitalWrite(DIR_PIN, LOW); // Arah masuk
    digitalWrite(motor_dc, HIGH); // Kipas nyala

    // Aktifkan buzzer selama 12 detik (bunyi putus-putus)
    unsigned long startTime = millis();
    unsigned long durasi = 12000;
    unsigned long lastToggle = 0;
    bool buzzerOn = false;

    int stepCount = 0;

    while (millis() - startTime < durasi) {
      // Stepper jalan
      if (stepCount < langkah) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayStep);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayStep);
        stepCount++;
      }

      // Buzzer berdenyut tiap 200ms
      if (millis() - lastToggle >= 200) {
        lastToggle = millis();
        buzzerOn = !buzzerOn;
        if (buzzerOn) {
          tone(buzzer, 1000); // Nyala
        } else {
          noTone(buzzer);     // Mati
        }
      }
    }

    noTone(buzzer); // Matikan buzzer setelah 12 detik
    sudahMasuk = true;
    hujanSebelumnya = true;
  }

  // Jemuran keluar saat hujan berhenti
  if (hujan == 0 && sudahMasuk && hujanSebelumnya) {
    digitalWrite(motor_dc, LOW); // Kipas mati
    digitalWrite(DIR_PIN, HIGH); // Arah keluar
    for (int i = 0; i < langkah; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayStep);
    }
    sudahMasuk = false;
    hujanSebelumnya = false;
    delay(2000);
  }

  delay(300); // jeda stabilisasi
}
