#include <Arduino.h>

constexpr int amb_led = 3;
constexpr int grn_led = 4;
constexpr int red_led = 5;

constexpr int push_button = 2;

constexpr int buzzer = 6;

unsigned long startTime = 0;
unsigned long stopTime = 0;
volatile bool stopSequenceRunning = false;

volatile bool stopRequested = false;

void setup() {
  Serial.begin(9600);
  // Setting up Arudino IOs
  Serial.println("Setting up Arduino Green, Amber, Red LED Pins (3, 4, 5)...");
  pinMode(grn_led, OUTPUT);
  pinMode(amb_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Setting up Arduino Push Button "Input Pullup" pin
  Serial.println("Setting up Arduino Push Button \"Input Pullup\" pin 2...");
  pinMode(push_button, INPUT_PULLUP);
  // Attaching Interrupt Service Routine to Arduino Push Button Pin,and running ISR "stopTraffic" on falling edge 
  Serial.println("Attaching ISR to Push Button pin, monitoring button status continuously!");
  attachInterrupt(digitalPinToInterrupt(push_button), stopTraffic, FALLING);
}

void loop() {
  // Ignore button presses while stop sequence is running 
  if (stopSequenceRunning) {
    return; 
  }
  // Traffic on
  Serial.println("Green Traffic Light On!"); 
  // NORMAL STATE (set once)
  digitalWrite(amb_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(grn_led, HIGH);
  
  // Turning Built in LED Green to indicate On Operation
  digitalWrite(LED_BUILTIN, LOW);

  // WAIT HERE until button is pressed
  while (!stopRequested) {
    // do nothing, just wait
    Serial.println("Doing nothing .. :)..");
  }

  // Button Pressed! Wait for 3 seconds before running stop sequence
  Serial.println("Button Pressed! Waiting for 3 seconds before running stop sequence...");
  delay(3000);

  // STOP REQUESTED → run stop sequence
  // Exiting while loop, toggling ISR flag to flase and running "Stop" sequence  
  Serial.println("Button pressed! ISR Flag toggled to True!");
  Serial.println("ISR Flag toggled to False! Runing Stop Sequence...");
  stopRequested = false;
  runStopSequence();

  // After stop sequence, loop() restarts → normal state again
}

void runStopSequence() {
  // start recording time for stop sequence
  //
  stopSequenceRunning = true;
  startTime = millis();
  //Stop Requested!
  // Turning Built in LED Red to indicate Stop Operation
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Green Off");
  // Turn green off
  digitalWrite(grn_led, LOW);
  delay(500);

  // Flashing Amber light
  Serial.println("Flashing Amber On.....");
  for (int i = 0; i < 5; i++) {
    digitalWrite(amb_led, HIGH);
    delay(500);
    digitalWrite(amb_led, LOW);
    delay(500);
  }
  Serial.println("Flashing Amber OFF.....");

  // Red
  Serial.println("Red On");
  digitalWrite(red_led, HIGH);

  // Buzzer On
  Serial.println("Buzzer On");
  for (int i = 0; i < 20; i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  
  // Buzzer Off
  Serial.println("Buzzer Off");
  digitalWrite(buzzer, LOW);

  // Flashing Amber light
  Serial.println("Flashing Amber On");
  for (int i = 0; i < 3; i++){
    digitalWrite(amb_led, HIGH);
    delay(500);
    digitalWrite(amb_led, LOW);
    delay(500);
  }
  Serial.println("Flashing Amber Off.....");
  Serial.println("Red Off.....");

  // recording time for stop sequence
  stopTime = millis();
  stopSequenceRunning = false;// asserting stop sequence is done
  // Print total time taken for stop sequence
  Serial.print("Total time taken for stop sequence: ");
  Serial.print((stopTime - startTime) / 1000);
  Serial.println(" seconds");
  stopSequenceRunning = false;// asserting stop sequence is done
  // Back to green happens automatically when loop restarts
}

void stopTraffic() {
  if (!stopSequenceRunning) {
    stopRequested = true; // Set the flag to indicate that a stop has been requested
  
  // Button Pressed!
  stopRequested = true;
  }
}
