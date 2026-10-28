
  /*
  ============================================================
   SMART BLIND STICK
  ============================================================
  Two ultrasonic sensors:
   - FRONT sensor (waist/chest height) -> detects obstacles ahead
   - DOWN sensor  (pointed at the ground near the tip) -> detects
     potholes / open drains / missing steps (sudden drop in ground)

  Alerts:
   - Obstacle ahead  -> buzzer beeps FASTER the closer the object is
                         (tone = 1000 Hz), LED blinks in sync
   - Hole / drop      -> buzzer gives a rapid, higher-pitched alert
                         (tone = 2000 Hz), LED blinks fast
   - Push button       -> mutes/unmutes the buzzer (LED still works
                         as a silent visual alert when muted)

  Hardware:
   - Arduino Uno
   - HC-SR04 Ultrasonic Sensor x2
   - Active Buzzer
   - LED + 220 ohm resistor
   - Push button (with internal pull-up, no external resistor needed)
  ============================================================
*/

// ---------- Pin definitions ----------
const int FRONT_TRIG = 9;
const int FRONT_ECHO  = 10;

const int DOWN_TRIG  = 11;
const int DOWN_ECHO   = 12;

const int BUZZER_PIN  = 8;
const int LED_PIN     = 13;
const int BUTTON_PIN  = 2;   // active LOW (INPUT_PULLUP)

// ---------- Thresholds ----------
const int OBSTACLE_DISTANCE_CM = 100;  // warn if something is closer than this
const int MIN_SAFE_DISTANCE_CM = 10;   // fastest beep rate distance
const int GROUND_NORMAL_CM     = 15;   // normal tip-to-ground distance
const int HOLE_THRESHOLD_CM    = 25;   // if down sensor reads MORE than this,
                                        // it means the ground has dropped away
                                        // (pothole / missing step)

// ---------- State ----------
bool muted = false;
bool lastButtonState = HIGH;

unsigned long lastBeepTime = 0;
bool buzzerToneOn = false;

unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 60; // ms between sensor polls

long frontDistance = 999;
long downDistance   = 0;

void setup() {
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  pinMode(DOWN_TRIG, OUTPUT);
  pinMode(DOWN_ECHO, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Smart Blind Stick - Initialized");
}

// Returns distance in cm, or 999 if no echo received (out of range)
long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // 25ms timeout (~4m range)
  if (duration == 0) {
    return 999; // no object detected within range
  }
  long distance = duration * 0.0343 / 2; // speed of sound = 343 m/s
  return distance;
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading == LOW && lastButtonState == HIGH) {
    muted = !muted;
    Serial.println(muted ? "Buzzer MUTED" : "Buzzer UNMUTED");
    noTone(BUZZER_PIN);
    delay(50); // simple debounce
  }
  lastButtonState = reading;
}

void soundAlert(int freq, unsigned long beepInterval) {
  unsigned long now = millis();
  if (now - lastBeepTime >= beepInterval) {
    lastBeepTime = now;
    buzzerToneOn = !buzzerToneOn;

    if (buzzerToneOn) {
      if (!muted) tone(BUZZER_PIN, freq);
      digitalWrite(LED_PIN, HIGH);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void loop() {
  handleButton();

  unsigned long now = millis();
  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    frontDistance = readDistanceCM(FRONT_TRIG, FRONT_ECHO);
    downDistance   = readDistanceCM(DOWN_TRIG, DOWN_ECHO);

    Serial.print("Front: ");
    Serial.print(frontDistance);
    Serial.print(" cm | Down: ");
    Serial.print(downDistance);
    Serial.println(" cm");
  }

  bool holeDetected     = (downDistance > HOLE_THRESHOLD_CM);
  bool obstacleDetected = (frontDistance < OBSTACLE_DISTANCE_CM);

  if (holeDetected) {
    // Highest priority alert: ground drop-off ahead
    soundAlert(2000, 80); // fast, high-pitched beeping
  }
  else if (obstacleDetected) {
    // Beep rate scales with how close the obstacle is
    long clamped = constrain(frontDistance, MIN_SAFE_DISTANCE_CM, OBSTACLE_DISTANCE_CM);
    unsigned long interval = map(clamped, MIN_SAFE_DISTANCE_CM, OBSTACLE_DISTANCE_CM, 80, 500);
    soundAlert(1000, interval);
  }
  else {
    // All clear
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }
}
