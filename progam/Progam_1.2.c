#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorCahaya = A0;
int sensorHujan = A1;

int ledSiang = 10;
int ledMalam = 9;
int ledHujan = 8;
int buzzer = 6;

bool statusHujanSebelumnya = false;  // status sebelumnya

void setup() {
  Serial.begin(9600);
  lcd.begin();
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
  int nilaiCahaya = analogRead(sensorCahaya);
  int nilaiHujan = analogRead(sensorHujan);

  int cahaya = nilaiCahaya < 500 ? 1 : 0;     // 1 = siang
  int hujan  = nilaiHujan  < 500 ? 1 : 0;     // 1 = hujan

  Serial.print("Cahaya: ");
  Serial.print(nilaiCahaya);
  Serial.print(" | Hujan: ");
  Serial.println(nilaiHujan);      

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cahaya: ");
  lcd.print(cahaya == 1 ? "Ada  " : "Tidak");

  lcd.setCursor(0, 1);
  lcd.print("Hujan : ");
  lcd.print(hujan == 1 ? "Ada  " : "Tidak");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);

  if (cahaya == 1 && hujan == 0) {
    lcd.print("Siang Cerah");
    digitalWrite(ledSiang, HIGH);
    digitalWrite(ledMalam, LOW);
    digitalWrite(ledHujan, LOW);
    noTone(buzzer);
  } else if (cahaya == 0 && hujan == 0) {
    lcd.print("Malam Cerah");
    digitalWrite(ledSiang, LOW);
    digitalWrite(ledMalam, HIGH);
    digitalWrite(ledHujan, LOW);
    noTone(buzzer);
  } else if (hujan == 1) {
    // Hanya bunyi dengan pola berdenyut saat hujan
    if (!statusHujanSebelumnya) {
      bunyikanBuzzerBerdetak();  // Panggil fungsi untuk bunyi berdenyut
      statusHujanSebelumnya = true;
    }

    lcd.print(cahaya == 1 ? "Siang Hujan" : "Malam Hujan");
    digitalWrite(ledSiang, cahaya);
    digitalWrite(ledMalam, !cahaya);
    digitalWrite(ledHujan, HIGH);

    // Hanya bunyi dengan pola berdenyut saat hujan
  }

  // Reset status jika hujan berhenti
  if (hujan == 0) {
    statusHujanSebelumnya = false;
  }

  delay(2000);
}

// Fungsi untuk bunyikan buzzer dengan pola berdenyut (nyala-mati)
void bunyikanBuzzerBerdetak() {
  for (int i = 0; i < 6; i++) {  // Denyut 6 kali
    tone(buzzer, 1000);  // Bunyi 1000Hz
    delay(500);          // 500ms bunyi
    noTone(buzzer);      // Matikan
    delay(500);          // 500ms mati
  }
}
