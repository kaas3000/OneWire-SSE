#define OWHIGH LOW
#define OWLOW HIGH

#define OW_ACTION_SKIP 0
#define OW_ACTION_HIGH 1
#define OW_ACTION_LOW 2

int pinIn = A0;
int pinOut = A1;
int pulsesDetected = 0;
unsigned long pulseStart = 0;
unsigned long previousTime = 0;
boolean previousValue = false;
boolean pulseValue = OWLOW;

int currentValue = 0;
int currentAction = OW_ACTION_SKIP;
unsigned long currentTime = 0;

// int leds[] = {
//   2, 5, 4, 3
// };

int leds[] = {
  2, 3, 4, 5
};

void setup() {
  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);

  for(int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], OWLOW);
  }

  Serial.begin(115200);
}

void loop() {
  updateVariables();

  detectAndProcessSyncPulse();

  detectAndProcessNewPulse();

  // If a millisecond has passed
  if (currentTime == previousTime + 1) {
    unsigned long currentMillisecond = currentTime - pulseStart;

    if (currentAction != OW_ACTION_SKIP) {
      boolean sendValue = (currentAction == OW_ACTION_HIGH) ? OWHIGH : OWLOW;

      if (currentMillisecond == 1) {
        digitalWrite(pinOut, OWLOW);
      }

      if (currentMillisecond == 3) {
        digitalWrite(pinOut, sendValue);
      }

      if (currentMillisecond == 9) {
        digitalWrite(pinOut, OWHIGH);
      }
    }



    if (currentMillisecond == 4) {
      pulseValue = currentValue;
    }

    if (currentMillisecond == 9) {
      Serial.print(pulsesDetected);
      Serial.print(" - ");
      Serial.print(currentTime - pulseStart);
      Serial.print(" - ");

      Serial.println(pulseValue);

      digitalWrite(leds[pulsesDetected - 1], !pulseValue);
    }
  }

  previousValue = currentValue;
  previousTime = currentTime;
}

void updateVariables() {
  currentValue = readCurrentValue();
  currentTime = millis();
}

/**
 * Detect the sync pulse and setup a new pulse train
 */
void detectAndProcessSyncPulse() {
  if (currentValue < previousValue && (currentTime - pulseStart) >= 15) {
    Serial.println("\nNEW PULSETRAIN");

    pulsesDetected = 0;
    pulseStart = currentTime;
  }
}

/**
 * Detect a new pulse by recognizing a falling edge,
 * increment the amount of pulses in the train and
 * reset some variables
 */
void detectAndProcessNewPulse() {
  if (currentValue < previousValue) {

    pulsesDetected++;

    pulseStart = currentTime;
    pulseValue = false;
  }
}

boolean readCurrentValue() {
  return !digitalRead(pinIn);
}

boolean getAction() {
  int action = OW_ACTION_LOW;

  if (pulsesDetected % 2) {
    action = OW_ACTION_HIGH;
  }

  return action;
}

void sendSyncPulse() {
  digitalWrite(pinOut, OWHIGH);
  delay(20);
}

void resetLeds() {
  for(int i = 0; i < 4; i++) {
    digitalWrite(leds[i], OWLOW);
  }
}
