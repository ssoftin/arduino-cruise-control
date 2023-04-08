#define MEMORY_KEY  0xaa55

struct SAVE {
  int key;
  int acc0max;
  int acc0min;
  int acc1max;
  int acc1min;
  int speed_coast;
  int speed_acc;
};

SAVE save;

void Load() {
  EEPROM.get(0, save);
  if (save.key != MEMORY_KEY) {
    memset(&save, 0, sizeof(SAVE));  
  }
}

void Save() {
  save.key = MEMORY_KEY;
  EEPROM.put(0, save);
}
