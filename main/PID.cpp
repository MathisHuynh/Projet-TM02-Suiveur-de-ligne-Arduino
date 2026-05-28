#include "PID.h"
#include "Config.h"
#include "Motor.h"

void linefollow() {
    error = 0;
    active = 0;
    for (int i = 0; i < 5; i++) {
        error += sensorWeight[i] * sensorArray[i] * sensorValue[i];
        active += sensorArray[i];
    }
    error = error / active;

    P = error;
    I = I + error;
    D = error - lp;
    lp = P;

    correc = (Kp * P) + (Ki * I) + (Kd * D);

    calc_turn();
}