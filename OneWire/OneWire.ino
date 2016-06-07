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
int pulseValue;

int leds[] = {
  2, 5, 4, 3
};

void setup() {
  pinIn = A0;
  pinOut = A1;
  previousValue = OWHIGH;
  pulseStart = millis();
  pulsesDetected = 0;
  // actionId = 0;
  // int actions[][] = {};
  pulseValue = -1;

  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);

  for(int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], OWLOW);
  }

  Serial.begin(115200);
}

void loop() {
  int currentValue = readCurrentValue();
  unsigned long currentTime = millis();

  if ((currentTime - pulseStart >= 15)) {
    Serial.println("NEW PULSETRAIN");

    pulsesDetected = 0;
    pulseStart = currentTime;
    // pulseValue = -1;
    // resetLeds();
  }

  if ((currentValue < previousValue)) {
  Serial.println(currentTime - pulseStart);

  Serial.println(pulseValue);

  digitalWrite(leds[pulsesDetected], !pulseValue);

    pulsesDetected++;

    pulseStart = currentTime;
    pulseValue = -1;
  }




  if (currentTime == pulseStart + 4 && pulseValue == -1) {
    pulseValue = currentValue;
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

void resetLeds() {
  for(int i = 0; i < 4; i++) {
    digitalWrite(leds[i], OWLOW);
  }
}
