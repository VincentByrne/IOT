// Define the pin connected to the reed switch
const int reedSwitchPin = 11;

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Configure the reed switch pin as input with internal pull-up resistor
  pinMode(reedSwitchPin, INPUT_PULLUP);
  
  // Optional: Print a startup message
  Serial.println("Reed Switch Test Initialized");
}

void loop() {
  // Read the state of the reed switch
  int switchState = digitalRead(reedSwitchPin);
  
  // Determine and print the switch state
  if (switchState == LOW) {
    // Reed switch is closed (magnet is near)
    Serial.println("Reed Switch: CLOSED");
  } else {
    // Reed switch is open (no magnet)
    Serial.println("Reed Switch: OPEN");
  }
  
  // Wait for half a second before the next read
  delay(500);
}
