const int buzzerPin = 8;    // Buzzer connected to digital pin 8
const int buzzerDelay = 10;  // Delay between frequencies
void setup() {
  pinMode(buzzerPin, OUTPUT); // Buzzer pin as an output
}

void loop() { 
  for (int freq = 500; freq <= 2000; freq += 5) {
    tone(buzzerPin, freq);
    delay(buzzerDelay);
  }
  for (int freq = 2000; freq >= 500; freq -= 5) {
    tone(buzzerPin, freq);
    delay(buzzerDelay);
  }
}
