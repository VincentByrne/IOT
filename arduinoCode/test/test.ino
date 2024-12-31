// Blink LED connected to Analog Pin A1 (Digital Pin 15)

const int ledPin = A1; // You can also use 15 instead of A1

void setup() {
  pinMode(ledPin, OUTPUT); // Set A1 as an OUTPUT
}

void loop() {
  digitalWrite(ledPin, HIGH); // Turn LED on
  delay(1000);                // Wait for 1 second
  digitalWrite(ledPin, LOW);  // Turn LED off
  delay(1000);                // Wait for 1 second
}
