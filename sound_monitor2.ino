/*
  Machine Noise Monitor - Arduino Uno
  ------------------------------------
  Reads a KY-037 / KY-038 sound sensor on A0 and sends structured JSON
  lines over Serial (USB) that the Python dashboard can parse directly:

      {"value":345,"level":"high"}

  It also drives the same three LEDs and the buzzer locally, so the
  physical hardware and the on-screen dashboard always agree.

  *** IMPORTANT WIRING NOTE ***
  Pins D0 and D1 on an Arduino Uno are the hardware Serial RX/TX lines
  used for USB communication with your PC. If the buzzer stays wired to
  D1, every buzzer pulse fights with (and corrupts) the Serial data the
  dashboard depends on. Move the buzzer's signal wire from D1 to D7
  (or any other free digital pin) before using this sketch.
*/

const int SOUND_PIN   = A0;
const int LED_GREEN   = 10;
const int LED_YELLOW  = 9;
const int LED_RED     = 8;
const int BUZZER_PIN  = 7;   // moved from D1 - see wiring note above

// Keep these in sync with the dashboard's default settings (they can
// also be overridden live from the dashboard's Settings panel, which
// only affects the PC-side coloring/logic - update these too if you
// want the physical LEDs/buzzer to match new thresholds).
const int THRESHOLD_LOW    = 400;  // value <  THRESHOLD_LOW    => "low"
const int THRESHOLD_MEDIUM = 500;  // value <  THRESHOLD_MEDIUM => "medium", else "high"

const unsigned long SEND_INTERVAL_MS = 100; // ~10 readings/second
unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

String levelFor(int value) {
  if (value < THRESHOLD_LOW) return "low";
  if (value < THRESHOLD_MEDIUM) return "medium";
  return "high";
}

void applyOutputs(const String &level) {
  digitalWrite(LED_GREEN,  level == "low");
  digitalWrite(LED_YELLOW, level == "medium");
  digitalWrite(LED_RED,    level == "high");
  digitalWrite(BUZZER_PIN, level == "high");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;

    int value = analogRead(SOUND_PIN);
    String level = levelFor(value);
    applyOutputs(level);

    // Compact JSON, one object per line - easy and fast to parse in Python.
    Serial.print("{\"value\":");
    Serial.print(value);
    Serial.print(",\"level\":\"");
    Serial.print(level);
    Serial.println("\"}");
  }
}
