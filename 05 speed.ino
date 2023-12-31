#define INTERRUPT_SPEED 0
#define PIN_SPEED 2        // matches interrupt 0 - white wire
#define PULSEBUFSIZE 256

// 40hz = 30km/h
// millis 1000 = 1sec
// 40hz: d = 25
// 750 / d = km/h

unsigned long pulsebuf[PULSEBUFSIZE];
volatile int bufhead = 0;

void InitSpeed() {
  pinMode(PIN_SPEED, INPUT_PULLUP);
  attachInterrupt(INTERRUPT_SPEED, InterruptSpeed, FALLING);
  memset(pulsebuf, 0, sizeof(unsigned long) * PULSEBUFSIZE);  
}

void InterruptSpeed() {
  AddPulse();
}

void AddPulse() {
  bufhead++;
  if (bufhead >= PULSEBUFSIZE) bufhead = 0;
  pulsebuf[bufhead] = millis();
}

unsigned long buf(int i) {  
  while (i < 0) i += PULSEBUFSIZE;
  while (i >= PULSEBUFSIZE) i -= PULSEBUFSIZE;
  return pulsebuf[i];
}

float GetSpeedInterval(unsigned long l) {
  unsigned long time = millis();
  int head = bufhead;
  int i = 0;
  for (; i < PULSEBUFSIZE; i++) if (time - buf(head-i) > l) break;
  if (i <= 1) return 0;
  i--;
  unsigned long d = buf(head) - buf(head-i);
  if (d == 0) return 0;
  return 750.0 * i / d;
}
