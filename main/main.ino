#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"
#include "Motor.h"
#include "Sensor.h"
#include "PID.h"
#include "Display.h"

byte carreBlanc[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

void setup() {
    pinMode(avantG, OUTPUT);
    pinMode(arriereG, OUTPUT);
    pinMode(avantD, OUTPUT);
    pinMode(arriereD, OUTPUT);

    pinMode(motorD, OUTPUT);
    pinMode(motorG, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.createChar(0, carreBlanc);

    for (int i = 0; i < 5; i++) {
        pinMode(sensorPins[i], INPUT);
    }

    for (int i = 0; i < 5; i++) {
        sensorValue[i] = constrain(analogRead(sensorPins[i]), 0, 1000);
        lsensorArray[i] = noir(sensorValue[i]);
    }
}

void loop() {
    readLine();

    if (onLine == 1) {
        linefollow();
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (lsensorArray[0] || lsensorArray[4]) {
        digitalWrite(LED_BUILTIN, LOW);
        if (error > 0) {
            ctrl_motor(-130, v);
        } else {
            ctrl_motor(v, -130);
        }
    }
}