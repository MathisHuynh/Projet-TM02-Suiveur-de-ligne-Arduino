#include "Config.h"

const int sensorPins[5] = {A0, A1, A2, A3, A6};
int sensorWeight[5] = {6, 4, 0, -4, -6};
int sensorValue[5];
int sensorArray[5];
int lsensorArray[5];

const int motorG = 11;
const int avantG = 3;
const int arriereG = 2;

const int motorD = 10;
const int avantD = 5;
const int arriereD = 4;

const int thresholdN = 500;

double error;
int active;

int P, D, I, lp, correc;
const float Kp = 2;
const float Kd = 20;
const float Ki = 0;

int vg, vd;
int v = 255;
int vmax = 255;
int vmin = -255;

int onLine = 1;
int count = 0;

LiquidCrystal_I2C lcd(0x27, 20, 2);