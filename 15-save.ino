#define MEMORY_KEY  0xaa55

struct SAVE {
  int key;
  int acc0max;
  int acc0min;
  int acc1max;
  int acc1min;
  int speed_coast;
  int speed_acc;
  
  int key2;
  float pid_kp;
  float pid_ki;
  float pid_kd;
  float pid_dt;
};

SAVE save;

#define PID_KP 1
#define PID_KI 0.5
#define PID_KD 0.7
#define PID_DT 500

void Load() {
  EEPROM.get(0, save);
  if (save.key != MEMORY_KEY) {
    memset(&save, 0, sizeof(SAVE));  
  }
  if (save.key2 != MEMORY_KEY) {
    save.pid_kp = PID_KP;
    save.pid_ki = PID_KI;
    save.pid_kd = PID_KD;
    save.pid_dt = PID_DT;

  }
}

void Save() {
  save.key = MEMORY_KEY;
  save.key2 = MEMORY_KEY;
  EEPROM.put(0, save);
}
