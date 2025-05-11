#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ganti alamat jika perlu

const int motor_dc = 6;

const int DIR_PIN  = 4;
const int STEP_PIN = 5;

// Inisialisasi sensor
const int sensorCahaya = A0;
const int sensorHujan  = A1;

const int langkah = 400;
const int delayStep = 800;

bool sudahMasuk = false;  // Status jemuran: masuk/tidak
bool hujanSebelumnya = false;  // Status hujan sebelumnya

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(sensorCahaya, INPUT);
  pinMode(sensorHujan, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jemuran Otomatis");
  delay(2000); // Tampilkan pesan awal selama 2 detik
}

void loop() {
  int nilaiCahaya = analogRead(sensorCahaya);
  int nilaiHujan  = analogRead(sensorHujan);

  int cahaya = nilaiCahaya < 500 ? 1 : 0;
  int hujan = nilaiHujan > 500 ? 1 : 0;

  // Tampilkan kondisi jika hujan
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

  delay(2000); // Delay 3 detik untuk stabilisasi tampilan

  // Gerakkan jemuran jika hujan mulai
  if (hujan == 1 && !sudahMasuk && !hujanSebelumnya) {
    digitalWrite(DIR_PIN, LOW); // Arah masuk
    for (int i = 0; i < langkah; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayStep);
    }
    sudahMasuk = true;
    hujanSebelumnya = true;
    delay(2000); // Delay setelah jemuran masuk
  }

  // Jika hujan berhenti, keluarkan jemuran
  if (hujan == 0 && sudahMasuk && hujanSebelumnya) {
    digitalWrite(DIR_PIN, HIGH); // Arah keluar
    for (int i = 0; i < langkah; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayStep);
    }
    sudahMasuk = false;
    hujanSebelumnya = false;
    delay(2000); // Delay setelah jemuran keluar
  }

  delay(300); // jeda stabilisasi
}
