// #define USE_ONLY_BMP_WHEN_WAITING
#define USE_BMP388
// #define USE_BMP280
// #define DONT_USE_BMP
// #define DONT_USE_THRUSTER_STABILIZATION
//#define USE_LOOP_DELAY
#define LOG_DELTA 0
#define WARMING_TIME 1500
#define CALIBRATION_TIME 3000
#define THRUSTER_WORKING_TIME 2500
#define USE_THRUSTER
#define AXIS_X_MIN 40
#define AXIS_X_MAX 120
#define AXIS_X_CENTER 91
#define AXIS_Z_MIN 51
#define AXIS_Z_MAX 117
#define AXIS_Z_CENTER 88
#define AXIS_ROTATION 0.0

// #define TURN_ADDITIONAL_LED
#define ADDITIONAL_LED_PIN 6

#define WAIT_TAKEOFF_TIME 30000

#define X_ERROR_COEFFICIENT (-1)
#define Z_ERROR_COEFFICIENT (1)

#define MAX_ACCEPTABLE_LANDING_HEIGHT_DELTA 0.45

#define PARACHUTE_PIN 22

#define THRUSTER_PIN 20
#define PID_P_COEFFICIENT 22.0 // 10.0
#define PID_I_COEFFICIENT 0.0
#define PID_D_COEFFICIENT 7.2  // 6.4

#define ROCKET_MASS 0.760

#define CRITICAL_LANDING_VELOCITY 8.0
#define LANDING_ENGINE_BURNING_TIME 1.8
#define AVERAGE_LANDING_ENGINE_FORCE (50.0 / LANDING_ENGINE_BURNING_TIME)
#define G_CONSTANT 9.80665
#define DROP_HEIGHT 180
#define FREE_FALL_MAX_ACC 250
#define ENGINE_WARMING_TIME 0.2

#define ENABLE_BUZZER false