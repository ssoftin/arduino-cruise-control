
#define PERIOD 500

unsigned long prev_time;

float tgt_speed;
float prev_speed;


void StartCruise() {
  if (speed) {
    mode = MODE_CRUISE;
    tgt_speed = speed;
    prev_speed = speed;
    calc_acc100 = acc100;
    prev_time = millis();
  }
}

void Cruise() {
  unsigned long cur_time = millis();
  if (cur_time - prev_time < PERIOD) return;
  prev_time = cur_time;

  float diff = speed - prev_speed;
  prev_speed = speed;
  
  if (speed + diff < tgt_speed - 0.5) {
    if (diff <= 0) calc_acc100 += 1;
  }
  if (speed + diff > tgt_speed + 0.5) {
    if (diff >= 0) calc_acc100 -= 1;
  }




  //calc_acc100 += tgt_speed - speed;

  // if (speed < tgt_speed) {
  //   calc_acc100 += 100 - 100 * speed / tgt_speed;
  // } 
  // if (speed > tgt_speed) {
  //   calc_acc100 -= 2 * (100 * speed / tgt_speed - 100);
  // }

  if (calc_acc100 > 30) calc_acc100 = 30;
  if (calc_acc100 < 0)  calc_acc100 = 0;

  SetAcc100(calc_acc100);
}