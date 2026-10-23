// Pin Definitions
const int SOUND_PIN  = A0;  // KY-037 Analog output
const int RED_LED    = 8;   // High noise indicator
const int YELLOW_LED = 9;   // Medium noise indicator
const int GREEN_LED  = 10;  // Safe noise indicator
const int BUZZER     = 7;   // Active buzzer

// Noise Thresholds (Adjust based on sensor calibration)
const int LOW_THRESHOLD  = 290;
const int HIGH_THRESHOLD = 380;

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  int noiseLevel = analogRead(SOUND_PIN);
  String level; // "low" / "medium" / "high" - matches the dashboard's expected values

  if (noiseLevel < LOW_THRESHOLD) {
    // Normal level: Green ON
    level = "low";
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }
  else if (noiseLevel >= LOW_THRESHOLD && noiseLevel < HIGH_THRESHOLD) {
    // Warning level: Yellow ON
    level = "medium";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }
  else {
    // Alert level: Red ON + Buzzer ON
    level = "high";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }

  // Send as compact JSON so the Python dashboard can parse it directly:
  // {"value":305,"level":"medium"}
  // (Plain "Noise Level: 305" text isn't valid JSON and isn't a bare
  // number either, so the dashboard's parser silently drops those lines.)
  Serial.print("{\"value\":");
  Serial.print(noiseLevel);
  Serial.print(",\"level\":\"");
  Serial.print(level);
  Serial.println("\"}");

  delay(100); // Small delay for reading stability
}
