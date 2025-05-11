// Inisiasi pin lcd 16*2 (12c)

const int DIR_PIN  = 4;
const int STEP_PIN = 5;

const int sensorCahaya = A0;
const int sensorHujan  = A1;

const int langkah = 400;       // Sesuaikan dengan motor & microstepping
const int delayStep = 800;     // Delay antar langkah (us)

bool kondisiSebelumnya = false; // true = terang & tidak hujan

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(sensorCahaya, INPUT);
  pinMode(sensorHujan, INPUT);
}

void loop() {
  int cahaya = analogRead(sensorCahaya) < 500 ? 1 : 0;       // 1 = terang
  int statusHujan = analogRead(sensorHujan) > 500 ? 1 : 0;   // 1 = hujan

  bool sekarangTerangKering = (cahaya == 1 && statusHujan == 0);

  if (sekarangTerangKering && !kondisiSebelumnya) {
    // Terang & tidak hujan → putar kanan
    digitalWrite(DIR_PIN, HIGH);
    for (int i = 0; i < langkah; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayStep);
    }
    kondisiSebelumnya = true;
  }

  if (!sekarangTerangKering && kondisiSebelumnya) {
    // Gelap atau hujan → putar kiri
    digitalWrite(DIR_PIN, LOW);
    for (int i = 0; i < langkah; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayStep);
    }
    kondisiSebelumnya = false;
  }

  delay(100); // Delay singkat untuk stabilisasi
}
