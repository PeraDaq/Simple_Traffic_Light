#include <Arduino.h> // Imported Arduino library for pinMode, digitalWrite, delay, attachInterrupt functions

// Pin definitions for LEDs, Push Button and Buzzer
constexpr int amb_led = 3;
constexpr int grn_led = 4;
constexpr int red_led = 5;

constexpr int push_button = 2;
constexpr int buzzer = 6;

// Telemetry and Debugging Constants run by the preprocessor to enable/disable telemetry and debug logs, and set telemetry heartbeat interval
#define ENABLE_TELEMETRY 1
#define ENABLE_DEBUG_LOGS 0
#define TELEMETRY_HEARTBEAT_MS 250

// Debugging Macros
#if ENABLE_DEBUG_LOGS
  // Macros for debug printing, which can be easily disabled by setting ENABLE_DEBUG_LOGS to 0
  #define DBG_print(x) Serial.print(x)
  #define DBG_println(x) Serial.println(x)
#else
  // If debug logs are disabled, define empty macros to avoid compilation errors
  #define DBG_print(x) do {} while (0)
  #define DBG_println(x) do {} while (0)
#endif

// Volatile variables used in the ISR and main loop to track button presses and stop sequence state
volatile bool buttonEdgePending = false;
unsigned long lastHeartbeatMs = 0;

// Function to emit telemetry data, which can be enabled or disabled using the ENABLE_TELEMETRY constant
void emitMetric(const char* name, long value) {
// Emit telemetry data if enabled, otherwise do nothing (using (void) to avoid unused variable warnings)
#if ENABLE_TELEMETRY
  Serial.print(name);
  Serial.print(':');
  Serial.println(value);
#else
  (void)name;
  (void)value;
#endif
}

// Function to read the state of a pin and emit it as telemetry data, which can be enabled or disabled using the ENABLE_TELEMETRY constant
void emitPinState(const char* name, uint8_t pin) {
  emitMetric(name, digitalRead(pin));
}

// Variables to track time and state of the stop sequence
unsigned long startTime = 0;
unsigned long stopTime = 0;
volatile bool stopSequenceRunning = false;

volatile bool stopRequested = false;

// declared here to avoid implicit declaration warnings, and to ensure they are recognized as ISR functions by the compiler
void runStopSequence();
void stopTraffic();

// Setup function to initialize Serial communication, configure pin modes, and attach the interrupt service routine to the push button pin
void setup() {
  // Initialize Serial Communication for debugging
  Serial.begin(9600);

  // Setting up Arudino IOs
  DBG_println("Setting up Arduino Green, Amber, Red LED Pins (3, 4, 5)...");
  pinMode(grn_led, OUTPUT);
  pinMode(amb_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Setting up Arduino Push Button "Input Pullup" pin
  DBG_println("Setting up Arduino Push Button \"Input Pullup\" pin 2...");
  pinMode(push_button, INPUT_PULLUP);

  // Attaching Interrupt Service Routine to Arduino Push Button Pin,and running ISR "stopTraffic" on falling edge
  DBG_println("Attaching ISR to Push Button pin, monitoring button status continuously!");
  attachInterrupt(digitalPinToInterrupt(push_button), stopTraffic, FALLING);

  // baseline snapshot of all telemetry metrics and pin states at the start of the program
  emitMetric("state", 0);
  emitMetric("stop_requested", 0);
  emitMetric("stop_running", 0);
  emitPinState("led_green", grn_led);
  emitPinState("led_amber", amb_led);
  emitPinState("led_red", red_led);
  emitPinState("led_builtin", LED_BUILTIN);
  emitPinState("buzzer_on", buzzer);
}

void loop() {
  // Ignore button presses while stop sequence is running
  if (stopSequenceRunning) {
    return;
  }
  // Traffic on
  DBG_println("Green Traffic Light On!");

  // NORMAL STATE (set once)
  digitalWrite(amb_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(grn_led, HIGH);

  // Turning Built in LED Green to indicate On Operation
  digitalWrite(LED_BUILTIN, LOW);

  // WAIT HERE until button is pressed
  while (!stopRequested) {
    DBG_println("Doing nothing .. :)..");
    unsigned long now = millis();
    // Emit button edge metric if a button edge is pending, which is set in the ISR when the button is pressed
    if (buttonEdgePending) {
      emitMetric("btn_edge", 1);
      buttonEdgePending = false;
    }
    // Emit a heartbeat metric at regular intervals to indicate that the system is alive and waiting for a button press, which can be useful for telemetry and debugging purposes
    if (now - lastHeartbeatMs >= TELEMETRY_HEARTBEAT_MS) {
      emitMetric("wait_ms", now);
      lastHeartbeatMs = now;
    }
  }

  // STOP REQUESTED → transition delay
  emitMetric("state", 1);
  delay(3000);
  stopRequested = false;
  emitMetric("stop_requested", 0);

  // Button Pressed! Wait for 3 seconds before running stop sequence
  DBG_println("Button Pressed! Waiting for 3 seconds before running stop sequence...");
  delay(3000);

  // STOP REQUESTED → run stop sequence
  // Exiting while loop, toggling ISR flag to flase and running "Stop" sequence
  DBG_println("Button pressed! ISR Flag toggled to True!");
  DBG_println("ISR Flag toggled to False! Runing Stop Sequence...");
  stopRequested = false;
  runStopSequence();

  // After stop sequence, loop() restarts → normal state again
}

void runStopSequence() {
  // start recording time for stop sequence
  stopSequenceRunning = true;
  startTime = millis();
  emitMetric("state", 2);
  emitMetric("stop_running", 1);
  emitMetric("phase", 10);

  //Stop Requested!
  // Turning Built in LED Red to indicate Stop Operation
  digitalWrite(LED_BUILTIN, HIGH);
  DBG_println("Green Off");

  // Turn green off
  digitalWrite(grn_led, LOW);
  delay(500);

  emitMetric("phase", 20);

  // Flashing Amber light
  DBG_println("Flashing Amber On.....");
  for (int i = 0; i < 5; i++) {
    digitalWrite(amb_led, HIGH);
    delay(500);
    digitalWrite(amb_led, LOW);
    delay(500);
  }
  DBG_println("Flashing Amber OFF.....");

  // Red
  DBG_println("Red On");
  digitalWrite(red_led, HIGH);
  emitMetric("phase", 30);

  // Buzzer On
  DBG_println("Buzzer On");
  emitMetric("phase", 40);
  for (int i = 0; i < 20; i++) {
    tone(buzzer, 2000); // 2000 Hz tone
    delay(100);
    noTone(buzzer); // Stop the tone
    delay(100);
  }

  // Buzzer Off
  DBG_println("Buzzer Off");
  noTone(buzzer); // Stop the tone

  // Flashing Amber light
  DBG_println("Flashing Amber On");
  emitMetric("phase", 50);
  for (int i = 0; i < 3; i++) {
    digitalWrite(amb_led, HIGH);
    delay(500);
    digitalWrite(amb_led, LOW);
    delay(500);
  }
  DBG_println("Flashing Amber Off.....");
  DBG_println("Red Off.....");

  // recording time for stop sequence
  stopTime = millis();
  emitMetric("seq_total_ms", stopTime - startTime);
  emitMetric("phase", 60);
  emitMetric("stop_running", 0);
  emitMetric("state", 0);
  stopSequenceRunning = false;// asserting stop sequence is done

  // Print total time taken for stop sequence
  DBG_print("Total time taken for stop sequence: ");
  DBG_print((stopTime - startTime) / 1000);
  DBG_println(" seconds");

  stopSequenceRunning = false;// asserting stop sequence is done

  // Back to green happens automatically when loop restarts
}

void stopTraffic() {
  if (!stopSequenceRunning) {
    stopRequested = true; // Set the flag to indicate that a stop has been requested
    buttonEdgePending = true; // Set the flag to indicate that a button edge is pending
  }
}
