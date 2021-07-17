#include "sensors/Altimeter_Bmp280.h"

bool Altimeter_Bmp280::begin() {
    if (bmp.begin() == false) {
        return false;
    }

    zeroHeight = bmp.readAltitude();
    return true;
}

void Altimeter_Bmp280::updateData() {
    altitude = bmp.readAltitude() - zeroHeight;
}