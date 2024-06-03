#include <Arduino.h>
#include <Preferences.h>

#include "Button.h"
#include "Motor.h"
#include "Timekeeper.h"
#include "BrakeLight.h"
#include "FootPedal.h"
#include "FlashLight.h"
#include "ThreePosSwitch.h"
#include "BatteryMonitor.h"
#include "VirtualBrakePedal.h"

#define CALIBRATE_OFF        0
#define CALIBRATE_PEDAL_UP   1
#define CALIBRATE_PEDAL_DOWN 2
#define CALIBRATE_DONE       3
#define CALIBRATE_START      CALIBRATE_PEDAL_UP

const int PIN_MOTOR_A_ENA = 14;
const int PIN_MOTOR_A_DIR1 = 12;
const int PIN_MOTOR_A_DIR2 = 13;

const int PIN_MOTOR_B_ENA = 5;
const int PIN_MOTOR_B_DIR1 = 26;
const int PIN_MOTOR_B_DIR2 = 27;

const int PIN_THROTTLE = 15;
const int PIN_BATT_SENSE = 4;
const int PIN_REVERSE_SW = 16;
const int PIN_FORWARD_SW = 17;
const int PIN_CALIBRATE_BTN = 36;

const int PIN_STATUS_LED = 2;
const int PIN_TAIL_LIGHTS = 22;
const int PIN_BRAKE_LIGHT = 19;
const int PIN_REVERSE_LIGHTS = 18;
const int PIN_HEADLIGHTS = 21;

const int PIN_POWER_RELAY = 23;

const int BRAKE_TIME_BEFORE_REVERSE = 2000;
const double MIN_BRAKE_LIGHT_DELTA = -0.4;

Preferences prefs;

BrakeLight *tailLights, *brakeLight, *reverseLights;
FlashLight *statusLight, *headLights, *powerRelay;
Motor *motorA, *motorB;
FootPedal *gasPedal;
VirtualBrakePedal *brakePedal;
BatteryMonitor *battery;

Button *calibrateBtn;
int calibrateMode = CALIBRATE_OFF;

ThreePosSwitch *reverseSwitch;
bool isReversing = false;
bool reverseInverted = false;

bool isInitializing = true;
bool deadBatteryShutdownNeeded = false;
bool deadBatteryShutdownActivated = false;

void onCalibrateBtnPressed();
void onCalibrateBtnLongPressed();
void onReverseSwitchChanged(ThreePosSwitch *sender, int newPosition);

Timekeeper *timekp;

void setup() {
  Serial.begin(115200);
  Serial.println("# PowerWheels Controller v3.0");

  prefs.begin("car_controller", false);
  //prefs.clear();
  
  reverseInverted = prefs.getBool("reverse_inv", false);
  Serial.printf("# Reverse inverted? %s\n", reverseInverted ? "YES" : "NO");

  //brakeTimeBeforeReverse = prefs.getLong("brake_time_before_rev", 2000);

  motorA = new Motor(MCPWM_UNIT_0, PIN_MOTOR_A_DIR1, PIN_MOTOR_A_DIR2, PIN_MOTOR_A_ENA, false);
  motorB = new Motor(MCPWM_UNIT_1, PIN_MOTOR_B_DIR1, PIN_MOTOR_B_DIR2, PIN_MOTOR_A_ENA, false);

  gasPedal = new FootPedal(PIN_THROTTLE);
  gasPedal->setUpValue(prefs.getDouble("throttle_min", 0.0));
  gasPedal->setDownValue(prefs.getDouble("throttle_max", 2500.0));
  gasPedal->setUpDeadZone(prefs.getDouble("thr_dz_min", 0.05));
  gasPedal->setDownDeadZone(prefs.getDouble("thr_dz_max", 0.95));
  gasPedal->setMaxUpSlope(prefs.getDouble("max_thr_delta", 1.0));
  gasPedal->overridePedalValue(0.0);

  brakePedal = new VirtualBrakePedal();

  battery = new BatteryMonitor(PIN_BATT_SENSE);

  Serial.printf("# Throttle Range: %d (up) to %d (down)\n", gasPedal->getUpValue(), gasPedal->getDownValue());
  
  statusLight = new FlashLight(PIN_STATUS_LED);
  statusLight->setOff();

  reverseSwitch = new ThreePosSwitch(PIN_REVERSE_SW, PIN_FORWARD_SW, INPUT_PULLUP, LOW);
  reverseSwitch->setDebounceDelay(50);
  reverseSwitch->setOnChangedCallback(onReverseSwitchChanged);
  isReversing = (reverseSwitch->getPosition() == (reverseInverted ? SWITCH_POS_B : SWITCH_POS_A));

  calibrateBtn = new Button(PIN_CALIBRATE_BTN, INPUT_PULLUP, LOW);
  calibrateBtn->setDebounceDelay(100);
  calibrateBtn->setOnPressedCallback(onCalibrateBtnPressed);
  calibrateBtn->setOnLongPressedCallback(onCalibrateBtnLongPressed, 3000);

  tailLights = new BrakeLight(PIN_TAIL_LIGHTS, false, 200, 0);
  tailLights->setLevels(0xff, 0x2f);
  brakeLight = new BrakeLight(PIN_BRAKE_LIGHT, false, 200, 0);
  brakeLight->setLevels(0xff, 0x00);
  reverseLights = new BrakeLight(PIN_REVERSE_LIGHTS, false, 50, 0);
  reverseLights->setLevels(0xff, 0x00);

  headLights = new FlashLight(PIN_HEADLIGHTS);
  headLights->setOff();
  statusLight = new FlashLight(PIN_STATUS_LED);
  statusLight->setOff();

  powerRelay = new FlashLight(PIN_POWER_RELAY);
  powerRelay->setOff();

  timekp = new Timekeeper();

  Serial.println("# Initialization complete.");
}

unsigned long lastStatusUpdate = 0;
bool lastBrakeLightsOn = false;
void loop() {
  if(!timekp->loop(500)) {
    return;
  }

  battery->loop(timekp);

  calibrateBtn->loop();
  reverseSwitch->loop();
  gasPedal->loop(timekp);
  brakePedal->loop(timekp);

  tailLights->loop();
  brakeLight->loop();
  reverseLights->loop();
  statusLight->loop();
  headLights->loop();
  powerRelay->loop();

  bool isStatusUpdateFrame = false;
  if((millis() - lastStatusUpdate) >= 500) {
    isStatusUpdateFrame = true;
    lastStatusUpdate = millis();
  }

  if(isStatusUpdateFrame) {
    if(false) {
      Serial.print("Battery: ");
      Serial.print(battery->getBatteryVoltage());
      Serial.print(" V (");
      Serial.print(battery->getBatteryPercent());
      Serial.print("%, raw: ");
      Serial.print(battery->getRawReading());
      Serial.println(" mV)");
    }

    if(true) {
      Serial.print("Gas Pedal: ");
      Serial.print(gasPedal->getPosition());
      Serial.print(" (raw: ");
      Serial.print(gasPedal->getRawPosition());
      Serial.print("; other: ");
      Serial.print(gasPedal->getClampedPosition());
      Serial.println(")");
    }
    if(true) {
      Serial.print("Brake Pedal: ");
      Serial.print(brakePedal->getPosition());
      Serial.print(" (raw: ");
      Serial.print(brakePedal->getRawPosition());
      Serial.print("; other: ");
      Serial.print(brakePedal->getClampedPosition());
      Serial.println(")");
    }
  }

  if(calibrateMode == CALIBRATE_PEDAL_UP) {
    gasPedal->calibrate(FOOTPEDAL_CALIBRATE_UP);
    return;
  }
  else if(calibrateMode == CALIBRATE_PEDAL_DOWN) {
    gasPedal->calibrate(FOOTPEDAL_CALIBRATE_DOWN);
    return;
  }
  else if(calibrateMode == CALIBRATE_DONE) {
    if(reverseSwitch->isPosA()) {
      Serial.println("Reverse set to NORMAL");
      reverseInverted = false;
    }
    else if(reverseSwitch->isPosB()) {
      Serial.println("Reverse set to INVERTED");
      reverseInverted = true;
    }
    prefs.putBool("reverse_inv", reverseInverted);

    prefs.putDouble("throttle_min", gasPedal->getUpValue());
    prefs.putDouble("throttle_max", gasPedal->getDownValue());

    Serial.print("Calibration: pedal up: ");
    Serial.print(gasPedal->getUpValue());
    Serial.print("; pedal down: ");
    Serial.print(gasPedal->getDownValue());
    Serial.print("; reverse: ");
    Serial.println(reverseInverted ? "inverted" : "normal");

    statusLight->setBlink(5, 100, 100, 0, FLASHLIGHT_END_OFF, 1);
    calibrateMode = CALIBRATE_OFF;
    return;
  }

  if(isInitializing && battery->isStable()) {
    isInitializing = false;
    Serial.println("Battery reading stabilized");
    if(!battery->isBatteryDead()) {
      Serial.println("Battery not dead; turning on power relay...");
      powerRelay->setOn();
      headLights->setOn();
    }
  }

  bool batteryOverride = false;
  if(isInitializing) batteryOverride = true;
  if(!isInitializing && battery->isBatteryDead()) {
    batteryOverride = true;

    if(!deadBatteryShutdownNeeded) {
      Serial.println("Dead battery shutdown needed!");
      deadBatteryShutdownNeeded = true;
      statusLight->setBlink(500, 500, FLASHLIGHT_END_HOLD, 0);
    }
  }

  bool firstDBSFrame = false;
  if(deadBatteryShutdownNeeded && !deadBatteryShutdownActivated && gasPedal->getPosition() < 0.001) {
    deadBatteryShutdownActivated = true;
    Serial.println("Dead Battery Shutdown Activated");
    firstDBSFrame = true;
  }

  if(deadBatteryShutdownActivated && firstDBSFrame) {
    powerRelay->setOff();
    headLights->setOff();
    tailLights->setLevels(0, 0);
    tailLights->deactivate();
    brakeLight->deactivate();
    reverseLights->deactivate();
    motorA->setPower(0.0f);
    motorB->setPower(0.0f);
  }

  if(deadBatteryShutdownActivated) {
    return;
  }

  bool isBraking = false;
  
  //if(gasPedal->getSmoothedSlope() <= MIN_BRAKE_LIGHT_DELTA || gasPedal->getPosition() < 0.001) {
  if(gasPedal->getPosition() < 0.001) {
    isBraking = true;
    brakePedal->activate();
  }
  else {
    brakePedal->deactivate();
  }


  bool reversingOverride = false;
  bool reverseSwitchActive = (digitalRead(PIN_REVERSE_SW) == (reverseInverted ? HIGH : LOW));
  if(reverseSwitchActive != isReversing) {
    reversingOverride = true;
    if(gasPedal->getPosition() < 0.001) {
      isReversing = reverseSwitchActive;
    }
  }

  if(batteryOverride || reversingOverride) {
    gasPedal->overridePedalValue(0.0);
    isBraking = true;
  } else {
    gasPedal->cancelOverride();
  }

  if(isBraking != lastBrakeLightsOn) {
    lastBrakeLightsOn = isBraking;
    Serial.print("=== Setting brake lights ");
    Serial.println(isBraking ? "ON" : "OFF");
  }
  
  // set lights
  if(isBraking) {
    tailLights->activate();
    brakeLight->activate();
  }
  if(isReversing) {
    reverseLights->activate();
  }

  // set motor direction
  if(isBraking) {
    motorA->setDriveMode(MOTOR_DRIVE_BRAKE);
    motorB->setDriveMode(MOTOR_DRIVE_BRAKE);
  }
  else {
    if(isReversing) {
      motorA->setDriveMode(MOTOR_DRIVE_REVERSE);
      motorB->setDriveMode(MOTOR_DRIVE_REVERSE);
    }
    else {
      motorA->setDriveMode(MOTOR_DRIVE_FORWARD);
      motorB->setDriveMode(MOTOR_DRIVE_FORWARD);
    }
  }

  // set motor power
  float motorPower = (float)(gasPedal->getPosition() * 99.0);
  if(isReversing) motorPower *= 0.85f;
  if(isBraking) motorPower = (float)(brakePedal->getPosition() * 99.0);
  if(motorPower > 99.0f) motorPower = 99.0f;
  if(motorPower < 0.0f) motorPower = 0.0f;
  if(motorPower > 0.1f) {
    motorPower = 25.0f + (0.75f * motorPower);
  }
  if(isStatusUpdateFrame) {
    Serial.print("Motor power: ");
    Serial.print(motorPower);
    if(isBraking) {
      Serial.print(" (braking)");
    }
    Serial.println();
  }
  motorA->setPower(motorPower);
  motorB->setPower(motorPower);
  
}

void onCalibrateBtnPressed() {
  Serial.println("===== Calibrate button pressed");
  if(calibrateMode == CALIBRATE_OFF) return;

  calibrateBtn->cancelLongPress();
  calibrateMode++;
  Serial.print("Beginning calibration mode ");
  Serial.println(calibrateMode);
  statusLight->setBlink(calibrateMode, 250, 250, 1000, FLASHLIGHT_END_OFF, 0);
}

void onCalibrateBtnLongPressed() {
  Serial.println("===== Calibrate button long-pressed");
  if(calibrateMode != CALIBRATE_OFF) return;

  calibrateMode = CALIBRATE_START;

  Serial.println("Beginning calibration mode 1");

  motorA->setPower(0.0f);
  motorB->setPower(0.0f);

  statusLight->setBlink(calibrateMode, 250, 250, 1000, FLASHLIGHT_END_OFF, 0);
}

void onReverseSwitchChanged(ThreePosSwitch *sender, int newPosition) {
  Serial.print("Reverse switch changed: ");
  Serial.println(newPosition);
}