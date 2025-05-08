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

// Variabel status
int cahaya = 0;
int hujan = 0;
int statusHujan = 0;

// Buzzer control
bool buzzerAktif = false;
unsigned long waktuMulaiBuzzer = 0;
int nadaIndex = 0;
unsigned long waktuNadaTerakhir = 0;
int nadaList[] = {200, 250, 300, 350, 400, 450}; // total 12 detik (2 detik tiap nada)

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
}

void loop() {
  // Baca sensor
  cahaya = analogRead(sensorCahaya) < 500 ? 1 : 0;
  statusHujan = analogRead(sensorHujan) > 500 ? 1 : 0;

  // Jika hujan baru mulai
  if (statusHujan == 1 && hujan == 0) {
    mulaiBuzzer();
  }

  hujan = statusHujan; // update status

  // Update buzzer
  updateBuzzer();

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
    digitalWrite(ledHujan, LOW);
  } else if (cahaya == 0 && hujan == 0) {
    lcd.print("Malam Cerah");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    digitalWrite(ledHujan, LOW);
  } else if (cahaya == 1 && hujan == 1) {
    lcd.print("Siang Hujan");
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
    digitalWrite(ledHujan, HIGH);
  } else if (cahaya == 0 && hujan == 1) {
    lcd.print("Malam Hujan");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    digitalWrite(ledHujan, HIGH);
  }

  delay(2000); // jeda antar tampilan
}

// Memulai buzzer nada bertingkat selama 12 detik
void mulaiBuzzer() {
  buzzerAktif = true;
  waktuMulaiBuzzer = millis();
  waktuNadaTerakhir = millis();
  nadaIndex = 0;
  tone(buzzer, nadaList[nadaIndex]);
}

// Update buzzer setiap 2 detik
void updateBuzzer() {
  if (!buzzerAktif) return;

  unsigned long sekarang = millis();

  // Ganti ke nada berikutnya setiap 2 detik
  if (sekarang - waktuNadaTerakhir >= 2000) {
    nadaIndex++;
    waktuNadaTerakhir = sekarang;

    if (nadaIndex < 6) {
      tone(buzzer, nadaList[nadaIndex]);
    }
  }

  // Stop setelah 12 detik
  if (sekarang - waktuMulaiBuzzer >= 12000) {
    noTone(buzzer);
    buzzerAktif = false;
  }
}
