

struct Menu {
  float* data;
  float step;
  char sym;  
};

#define MENU_SIZE 4
Menu menu[MENU_SIZE];
unsigned int cursor = 255;

void InitMenu() {
  menu[0].sym = 'p';
  menu[0].data = &save.pid_kp;
  menu[0].step = 0.1;

  menu[1].sym = 'i';
  menu[1].data = &save.pid_ki;
  menu[1].step = 0.1;

  menu[2].sym = 'd';
  menu[2].data = &save.pid_kd;
  menu[2].step = 0.1;

  menu[3].sym = 't';
  menu[3].data = &save.pid_dt;
  menu[3].step = 50;
}
