#define OWHIGH LOW
#define OWLOW HIGH

// #define OW_ACTION_READ 0
// #define OW_ACTION_WRITE 1

int pinIn;
int pinOut;
int pulsesDetected;
unsigned long pulseStart;
boolean previousValue;

int leds[] = {
  2, 3, 4, 5
};
int switches[] = {
  6, 7, 8, 12
};

void setup() {
  pinIn = A0;
  pinOut = A1;
  previousValue = OWHIGH;
  pulseStart = millis();
  pulsesDetected = 0;

  Serial.begin(9600);

  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], OWLOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(switches[i], INPUT);
  }
}

void loop() {
  int currentValue = readCurrentValue();
  unsigned long currentTime = millis();

  if (currentValue != previousValue) {
    boolean syncPulse = false;

    // rising or falling edge detected
    if (currentTime - pulseStart >= 15 && currentValue == OWHIGH) {
      syncPulse = true;
      Serial.println("Sync pulse detected :)");
      Serial.println(pulsesDetected);

      pulsesDetected = 0;
      pulseStart = millis();
    }


    if (currentValue == OWLOW && !syncPulse) {
      pulsesDetected++;
      pulseStart = millis();

      if(pulsesDetected >= 1 && pulsesDetected <= 4){
        boolean switchValue = digitalRead(switches[(pulsesDetected % 4)]);
        pinMode(pinOut, OUTPUT);
        // digitalWrite(pinOut,OWLOW);
        delay(2);
        digitalWrite(pinOut,!switchValue);
        delay(6);

        pinMode(pinOut, INPUT);
        // digitalWrite(pinOut,OWHIGH);
        delay(1);

      }



      if(pulsesDetected == 5){
        delay(4);
        digitalWrite(leds[0],readCurrentValue());
      }
      if(pulsesDetected == 6){
        delay(4);
        digitalWrite(leds[1],readCurrentValue());
      }
      if(pulsesDetected == 7){
        delay(4);
        digitalWrite(leds[2],readCurrentValue());
      }
      if(pulsesDetected == 8){
        delay(4);
        digitalWrite(leds[3],readCurrentValue());
        // Serial.println("nummer 96");
        // Serial.println(readCurrentValue());
      }

    }
  } else {
  }

  previousValue = currentValue;
}

boolean readCurrentValue() {
  return !digitalRead(pinIn);
}

boolean getAction() {
}
