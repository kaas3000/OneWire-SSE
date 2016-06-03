#define OWHIGH LOW
#define OWLOW HIGH

// #define OW_ACTION_READ 0
// #define OW_ACTION_WRITE 1

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

  Serial.begin(9600);

  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);
}

void loop() {
  int currentValue = readCurrentValue();
  unsigned long currentTime = millis();

  if (currentValue != previousValue) {
    boolean syncPulse = false;

    // rising or falling edge detected
    if (currentTime - pulseStart >= 15 && currentValue == OWLOW) {
      syncPulse = true;
      Serial.println("Sync pulse detected :)");
      Serial.println(pulsesDetected);

      pulsesDetected = 0;
      pulseStart = millis();
    }

    if (currentValue == OWHIGH && !syncPulse) {
      pulsesDetected++;
      pulseStart = millis();
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
