#include <LiquidCrystal.h>

// Inisialisasi LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Inisiasi sensor dan pin
int sensorCahaya = A0;
int sensorHujan = A1;

int ledSiang = 10;
int ledMalam = 9;
int ledHujan = 8;
int buzzer = 6;

// Status sensor
int cahaya = 0;
int hujan = 0;
int statusHujan = 0;

// Motor L298N
#define ENA 13
#define IN1 A2
#define IN2 A3
#define INB 7
#define IN3 A4
#define IN4 A5

// Buzzer
bool buzzerAktif = false;
unsigned long waktuMulaiBuzzer = 0;
int nadaIndex = 0;
unsigned long waktuNadaTerakhir = 0;
int nadaList[] = {200, 250, 300, 350, 400, 450};

// Motor 1
int putaranMotor1 = 0;
bool motor1Berputar = false;
bool motor1ArahMaju = true;
unsigned long waktuPutarMotor1 = 0;
unsigned long waktuAwalMotor1 = 0;
unsigned long waktuMaxBerputarMotor1 = 10000; // 10 detik maksimal

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Jemuran Otomatis");
  delay(2000);
  lcd.clear();

  pinMode(sensorCahaya, INPUT);
  pinMode(sensorHujan, INPUT);
  pinMode(ledSiang, OUTPUT);
  pinMode(ledMalam, OUTPUT);
  pinMode(ledHujan, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(INB, OUTPUT);
}

void loop() {
  // Baca sensor
  cahaya = analogRead(sensorCahaya) < 500 ? 1 : 0;
  statusHujan = analogRead(sensorHujan) > 500 ? 1 : 0;

  // Deteksi hujan mulai
  if (statusHujan == 1 && hujan == 0) {
    mulaiBuzzer();
    motor1Berputar = true;
    motor1ArahMaju = true;
    putaranMotor1 = 0;
    waktuPutarMotor1 = millis();
    waktuAwalMotor1 = millis();
    motor1Maju();
  }

  // Deteksi hujan berhenti
  if (statusHujan == 0 && hujan == 1) {
    motor1Berputar = true;
    motor1ArahMaju = false;
    putaranMotor1 = 0;
    waktuPutarMotor1 = millis();
    waktuAwalMotor1 = millis();
    motor1Mundur();
  }

  hujan = statusHujan;

  // Update buzzer
  updateBuzzer();

  // Update motor 1
  if (motor1Berputar) {
    if (millis() - waktuPutarMotor1 >= 1000) {
      putaranMotor1++;
      waktuPutarMotor1 = millis();
    }

    // Hentikan motor jika terlalu lama berputar atau sudah 10 putaran
    if ((millis() - waktuAwalMotor1 >= waktuMaxBerputarMotor1) || (putaranMotor1 >= 10)) {
      motor1Stop();
      motor1Berputar = false;
    }
  }

  // Update motor 2 (nyala hanya saat hujan)
  if (statusHujan == 1) {
    motor2Maju();
    digitalWrite(ledHujan, HIGH);
  } else {
    motor2Stop();
    digitalWrite(ledHujan, LOW);
  }

  // Tampilkan nilai sensor
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
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
  } else if (cahaya == 0 && hujan == 0) {
    lcd.print("Malam Cerah");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
  } else if (cahaya == 1 && hujan == 1) {
    lcd.print("Siang Hujan");
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
    delay(5000);
    digitalWrite(ledHujan, HIGH);
  } else if (cahaya == 0 && hujan == 1) {
    lcd.print("Malam Hujan");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    delay(5000);
    digitalWrite(ledHujan, HIGH);
  }

  delay(2000);
}

// ----------------------------
// Fungsi buzzer
// ----------------------------
void mulaiBuzzer() {
  buzzerAktif = true;
  waktuMulaiBuzzer = millis();
  waktuNadaTerakhir = millis();
  nadaIndex = 0;
  tone(buzzer, nadaList[nadaIndex]);
}

void updateBuzzer() {
  if (!buzzerAktif) return;

  unsigned long sekarang = millis();
  if (sekarang - waktuNadaTerakhir >= 2000) {
    nadaIndex++;
    waktuNadaTerakhir = sekarang;

    if (nadaIndex < 6) {
      tone(buzzer, nadaList[nadaIndex]);
    }
  }

  if (sekarang - waktuMulaiBuzzer >= 12000) {
    noTone(buzzer);
    buzzerAktif = false;
  }
}

// ----------------------------
// Fungsi motor 1
// ----------------------------
void motor1Maju() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);
}

void motor1Mundur() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);
}

void motor1Stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

// ----------------------------
// Fungsi motor 2
// ----------------------------
void motor2Maju() {
  delay(5000); // Tunggu 5 detik
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(INB, HIGH);
}

void motor2Stop() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(INB, LOW);
}
