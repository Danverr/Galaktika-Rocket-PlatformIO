#include <sensors/Gps_TinyGps.h>



bool Gps_TinyGps::begin() {
    return Serial6;
}



void Gps_TinyGps::updateData() {
    coords[0] = tinyGps.location.lat();
    coords[1] = tinyGps.location.lng();
    coords[2] = tinyGps.altitude.meters();
    speed = tinyGps.speed.mps();
}



void Gps_TinyGps::encodeSerial(int b) {
    tinyGps.encode(b);
}