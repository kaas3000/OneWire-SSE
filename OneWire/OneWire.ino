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

boolean roleSender = false;

int currentValue = 0;
int currentAction = OW_ACTION_SKIP;
unsigned long currentTime = 0;

// int leds[] = {
//   2, 5, 4, 3
// };

int leds[] = {
  2, 3, 4, 5
};

int switches[] = {
  6, 7, 8, 12
};

void setup() {
  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], OWLOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(switches[i], INPUT);
  }

  Serial.begin(115200);
}

void loop() {
  updateVariables();

  if (roleSender) {
    sendSyncPulseIfNeccesary();
    startNewPulseIfNeccesary();
  } else {
    detectAndProcessSyncPulse();
    detectAndProcessNewPulse();
  }


  // If a millisecond has passed
  if (currentTime == previousTime + 1) {
    // Serial.println(previousTime);
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

// Serial.println(currentTime - previousTime);

  previousValue = currentValue;
  previousTime = currentTime;
}

void updateVariables() {
  currentValue = readCurrentValue();
  currentTime = millis();
  currentAction = getAction();
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
    processNewPulse();
  }
}

void startNewPulseIfNeccesary() {
  if (currentTime - pulseStart > 9) {
    // Serial.println("new pulse");
    processNewPulse();
  }
}

/**
 * Check if the syncpulse should be sended and send it
 */
void sendSyncPulseIfNeccesary() {
  if (pulsesDetected >= 4 && currentTime - pulseStart > 9) {
    sendSyncPulse();

        pulsesDetected = 0;
        pulseStart = currentTime;
  }
}

boolean readCurrentValue() {
  return !digitalRead(pinIn);
}

boolean getAction() {
  int action = OW_ACTION_LOW;

  if (digitalRead(switches[pulsesDetected])) {
    action = OW_ACTION_HIGH;
  }

  return action;
}

void sendSyncPulse() {
  digitalWrite(pinOut, OWHIGH);
  delay(20);
}

/**
 * Set all variables correctly for a new pulse
 */
void processNewPulse() {
  pulsesDetected++;

  pulseStart = currentTime;
  pulseValue = false;
}

void resetLeds() {
  for(int i = 0; i < 4; i++) {
    digitalWrite(leds[i], OWLOW);
  }
}
