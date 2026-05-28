#include "Motor.h"
#include "Config.h"

void motor(int motorNum, int pwr) {
    analogWrite(motorNum, 255);
    if (pwr < 0) {
        if (motorNum == motorG) {
            digitalWrite(avantG, LOW);
            analogWrite(arriereG, abs(pwr));
        } else {
            digitalWrite(avantD, LOW);
            analogWrite(arriereD, abs(pwr));
        }
    } else {
        if (motorNum == motorG) {
            analogWrite(avantG, pwr);
            digitalWrite(arriereG, LOW);
        } else {
            analogWrite(avantD, pwr);
            digitalWrite(arriereD, LOW);
        }
    }
}

void ctrl_motor(int G, int D) {
    motor(motorG, G);
    motor(motorD, D);
}

void calc_turn() {
    vd = v + correc;
    vg = v - correc;
    if (vd > vmax) vd = vmax;
    if (vg > vmax) vg = vmax;
    if (vd < vmin) vd = vmin;
    if (vg < vmin) vg = vmin;
    ctrl_motor(vg, vd);
}