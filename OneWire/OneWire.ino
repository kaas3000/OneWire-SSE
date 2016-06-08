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
boolean asdf = false;

// int leds[] = {
//   2, 5, 4, 3
// };

int leds[] = {
  2, 3, 4, 5
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



  if (currentValue < previousValue) {
    /*
     * Syncpulse detected, new pulse train
     */
    if (currentTime - pulseStart >= 15) {
      // Serial.print(currentTime - pulseStart);
      // Serial.print(pulseValue);
      Serial.println(" NEW PULSETRAIN");

      pulsesDetected = 0;
      pulseStart = currentTime;
      // pulseValue = -1;
      // resetLeds();
    }

    pulsesDetected++;

    pulseStart = currentTime;
    pulseValue = -1;
    asdf = false;
  }




  if (currentTime == pulseStart + 4 && pulseValue == -1) {
    pulseValue = currentValue;
  }

  if (currentTime == pulseStart + 9 && asdf == false) {
      Serial.print(pulsesDetected);
      Serial.print(" - ");
      Serial.print(currentTime - pulseStart);
      Serial.print(" - ");

      Serial.println(pulseValue);

      digitalWrite(leds[pulsesDetected - 1], !pulseValue);

      asdf = true;
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
