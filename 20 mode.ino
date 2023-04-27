
#define PIN_BRAKE 3 // yellow wire

enum {
  MODE_OFF,
  MODE_ON,
  MODE_CALIBRATE,
  MODE_CRUISE,
  MODE_LIMITER  
};

int mode = MODE_OFF;


void InitMode() {
    pinMode(PIN_BRAKE, INPUT_PULLUP);
    if (save.key != MEMORY_KEY) StartCalibrate();
}


void Mode() {
  switch (mode) {
    case MODE_OFF:
      ModeOff();
      return;  
    case MODE_ON:
      ModeOn();
      return;  
    case MODE_CALIBRATE:
      ModeCalibrate();
      return;  
    case MODE_CRUISE:
//      ModeManual();
      ModeCruise();
      return;  
    case MODE_LIMITER:
      ModeLimiter();
      return;  
  }
}

void ModeOff() {
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_ON;
      return;
    case KEY_ON_OFF_EXLONG:
      if (speed == 0) {
        StartCalibrate();
      }
      return;
  }
}

void ModeOn() {
  calc_acc100 = acc100;
  SetAcc100(acc100);

  int key = GetKey();
  if (key == KEY_ON_OFF) {
      mode = MODE_OFF;
      return;
  }

  if (!digitalRead(PIN_BRAKE)) {
    switch (key) {
      case KEY_COAST_SET:
        StartCruise();        
        return;
      case KEY_ACC_RES:
//        StartCruise();
        return;
      case KEY_CANCEL_LONG:
        mode = MODE_LIMITER;
        return;
      case KEY_COAST_SET_LONG:
//        StartCruise(save.speed_coast);
        return;
      case KEY_ACC_RES_LONG:
//        StartCruise(save.speed_acc);
        return;
    }
  }
}


void ModeCruise() {
  Cruise();

  if (digitalRead(PIN_BRAKE)) {
    mode = MODE_ON;
    return;
  }
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_OFF;
      return;
    case KEY_CANCEL:
      mode = MODE_ON;
      return;
    case KEY_COAST_SET:
//      speed_tgt -= 1;
      return;
    case KEY_ACC_RES:
//      speed_tgt += 1;
      return;
    case KEY_COAST_SET_LONG:
//      if (save.speed_coast) speed_tgt = save.speed_coast;
      return;
    case KEY_ACC_RES_LONG:
//      if (save.speed_acc) speed_tgt = save.speed_acc;
      return;
    case KEY_COAST_SET_EXLONG:
//      save.speed_coast = speed_tgt;
//      Save();
      return;
    case KEY_ACC_RES_EXLONG:
//      save.speed_acc = speed_tgt;
//      Save();
      return;    
  }
}

void ModeLimiter() {
  calc_acc100 = acc100;
  SetAcc100(acc100);

  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_OFF;
      return;
    case KEY_CANCEL:
      mode = MODE_ON;
      return;
  }
}

/*
void ModeManual() {
  SetAcc100(acc100);

  if (digitalRead(PIN_BRAKE)) {
      mode = MODE_ON;
      return;
  }

  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_OFF;
      acc100 = 0;
      return;
    case KEY_CANCEL:
      mode = MODE_ON;
      acc100 = 0;
      return;
    case KEY_COAST_SET:
      if (acc100 > 0) acc100--;     
      return;
    case KEY_ACC_RES:
      if (acc100 < 100) acc100++;
      mode = MODE_CRUISE;
      return;
  }
}
*/

void ModeCalibrate() {
  Calibrate();

  if (digitalRead(PIN_BRAKE)) {
      mode = MODE_OFF;
      return;
  }

  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF_EXLONG:
      mode = MODE_ON;
      Save();
      return;
    case KEY_ON_OFF:
    case KEY_CANCEL:
      mode = MODE_OFF;
      Load();
      return;
  }
}

