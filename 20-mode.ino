
#define PIN_BRAKE 3 // yellow wire

enum {
  MODE_OFF,
  MODE_ON,
  MODE_CALIBRATE,
  MODE_CRUISE
};

enum {
  SUBMODE_NO,
  SUBMODE_SETTINGS
};

int mode = MODE_OFF;
int sub_mode = SUBMODE_NO;

void InitMode() {
    pinMode(PIN_BRAKE, INPUT_PULLUP);
    pinMode(PIN_TYPE, INPUT_PULLUP);    
    if (save.key != MEMORY_KEY) StartCalibrate();
}

void Mode() {
  switch (sub_mode) {
    case SUBMODE_SETTINGS:
      SubModeSettings();
      return;
    default:
      break;
  }

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
      ModeCruise();
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
  int key = GetKey();
  if (key == KEY_ON_OFF) {
      mode = MODE_OFF;
      return;
  }

  if (!digitalRead(PIN_BRAKE)) {
    switch (key) {
      case KEY_COAST_SET:
        StartCruise(speed);        
        return;
      case KEY_ACC_RES:
        StartCruise(tgt_speed);
        return;
      case KEY_COAST_SET_LONG:
        StartCruise(save.speed_coast);
        return;
      case KEY_ACC_RES_LONG:
        StartCruise(save.speed_acc);
        return;
      case KEY_CANCEL_LONG:
        cursor = 0;
        sub_mode = SUBMODE_SETTINGS;
        return;      
    }
  }
}


void ModeCruise() {
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
    case KEY_CANCEL_LONG:
      cursor = 0;
      sub_mode = SUBMODE_SETTINGS;
      return;
    case KEY_COAST_SET:
      if (tgt_speed > MIN_SPEED) {
        tgt_speed -= Unit();
        Blink(1);
      }
      return;
    case KEY_ACC_RES:
      tgt_speed += Unit();
      Blink(1);
      return;
    case KEY_COAST_SET_LONG:
      if (save.speed_coast) {
        tgt_speed = save.speed_coast;
        Blink(2);
      }
      return;
    case KEY_ACC_RES_LONG:
      if (save.speed_acc) {
        tgt_speed = save.speed_acc;
        Blink(2);
      }
      return;
    case KEY_COAST_SET_EXLONG:
      save.speed_coast = tgt_speed;
      Save();
      Blink(4);
      return;
    case KEY_ACC_RES_EXLONG:
      save.speed_acc = tgt_speed;
      Save();
      Blink(4);
      return;    
  }
}

void SubModeSettings() {
  if (digitalRead(PIN_BRAKE)) {
    sub_mode = SUBMODE_NO;
    return;
  }
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
    case KEY_CANCEL:
      sub_mode = SUBMODE_NO;
      return;
    case KEY_CANCEL_LONG:
      cursor = -1;
      sub_mode = SUBMODE_NO;
      Save();
      return;
    case KEY_COAST_SET:
      *(menu[cursor].data) -= menu[cursor].step;
      return;
    case KEY_ACC_RES:
      *(menu[cursor].data) += menu[cursor].step;
      return;
    case KEY_COAST_SET_LONG:
      cursor ++;
      cursor %= MENU_SIZE;
      return;
    case KEY_ACC_RES_LONG:
      cursor --;
      cursor %= MENU_SIZE;
      return;
  }
}

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

