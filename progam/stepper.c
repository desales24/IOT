// Pin ke A4988
const int DIR_PIN  = 4;
const int STEP_PIN = 5;

// Inisialisasi sensor
int sensorCahaya = A0;
int sensorHujan = A1;


// Jumlah langkah untuk satu putaran
const int langkah = 400;  // Ganti sesuai kebutuhan atau microstepping
const int delayStep = 800; // Dalam microseconds

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
}

void loop() {
  // Putar ke kanan
  digitalWrite(DIR_PIN, HIGH);
  for (int i = 0; i < langkah; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(delayStep);
  }
  
  delay(1000); // Jeda 1 detik

  // Putar ke kiri
  digitalWrite(DIR_PIN, LOW);
  for (int i = 0; i < langkah; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(delayStep);
  }

  delay(1000); // Jeda 1 detik
}
