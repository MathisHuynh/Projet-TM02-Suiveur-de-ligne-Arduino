#include "Sensor.h"
#include "Config.h"

bool noir(int capteur) {
    return capteur < thresholdN;
}

void readLine() {
    onLine = 0;
    for (int i = 0; i < 5; i++) {
        sensorValue[i] = constrain(analogRead(sensorPins[i]), 0, 1000);
        sensorArray[i] = noir(sensorValue[i]);
        if (sensorArray[i]) {
            onLine = 1;
            for (int j = 0; j < 5; j++) {
                lsensorArray[j] = noir(constrain(analogRead(sensorPins[j]), 0, 1000));
            }
        }
    }
    if (!onLine) {
        if (correc > 0 && correc <= 120) correc++;
        else if (correc < 0 && correc >= -120) correc--;
    }
}