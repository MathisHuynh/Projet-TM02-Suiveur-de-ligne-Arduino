#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern const int sensorPins[5];
extern int sensorWeight[5];
extern int sensorValue[5];
extern int sensorArray[5];
extern int lsensorArray[5];

extern const int motorG;
extern const int avantG;
extern const int arriereG;

extern const int motorD;
extern const int avantD;
extern const int arriereD;

extern const int thresholdN;

extern double error;
extern int active;

extern int P, D, I, lp, correc;
extern const float Kp;
extern const float Kd;
extern const float Ki;

extern int vg, vd;
extern int v;
extern int vmax;
extern int vmin;

extern int onLine;
extern int count;

extern LiquidCrystal_I2C lcd;

#endif