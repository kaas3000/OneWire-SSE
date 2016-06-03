#define OWHIGH LOW
#define OWLOW HIGH

#define OW_ACTION_READ 0
#define OW_ACTION_WRITE 1
#define OW_ACTION_READWRITE 2

int pinIn;
int pinOut;
int pulsesDetected;
unsigned long pulseStart;
boolean previousValue;

void setup() {
  pinIn = A0;
  pinOut = A1;
  previousValue = OWHIGH;
  pulseStart = millis();
  pulsesDetected = 0;
  actionId = 0;
  int actions[][] = {};

  Serial.begin(9600);

  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);
}

void loop() {
  int currentValue = readCurrentValue();
  unsigned long currentTime = millis();
  int currentAction = getAction();

  if (currentValue != previousValue) {
    boolean syncPulse = false;

    // rising or falling edge detected
    if (currentTime - pulseStart >= 15 && currentValue == OWLOW) {
      syncPulse = true;
      // Serial.println("Sync pulse detected :)");
      // Serial.println(pulsesDetected);

      pulsesDetected = 0;
      pulseStart = millis();
    }

    if (currentValue == OWHIGH && !syncPulse) {
      pulsesDetected++;
      pulseStart = millis();
    }
  } else {
    /*
     * ELSE
     */
    if ((pulsesDetected == 1 && currentAction == OW_ACTION_READWRITE) || (pulsesDetected == 1 && currentAction == OW_ACTION_WRITE)) {
      sendSyncPulse();

      pulseStart = millis();
    } else if (currentAction == OW_ACTION_WRITE)
  }

  previousValue = currentValue;
}

boolean readCurrentValue() {
  return !digitalRead(pinIn);
}

boolean getAction() {
  int action = OW_ACTION_READWRITE;

  return action;
}

void sendSyncPulse() {
  digitalWrite(pinOut, OWHIGH);
  delay(20);
}
