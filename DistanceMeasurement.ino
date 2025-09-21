#include <Arduino.h>
#define __ASSERT_USE_STDERR
#include <assert.h>

#include "DebugUtils.h"
#include "StateMachine.h"

#include "MockDistanceSensor.h"
#include "MockTrafficLight.h"
#include "HCSR04.h"
#include "DiscreteLEDTrafficLight.h"

const uint8_t triggerPin      = 3;
const uint8_t echoPin         = 2;

const uint8_t redLightPin     = 4;
const uint8_t yellowLightPin  = 5;
const uint8_t greenLightPin   = 6;

const uint32_t maxDistanceThresholdMm        = 3000;
const uint32_t farThresholdMm                = 1500;
const uint32_t nearThresholdMm               = 250;
const uint32_t movingDistanceThresholdMm     = 50;
const uint32_t movingTimeThresholdMs         = 100;
const uint32_t holdingTimeThresholdMs        = 2000;
const uint32_t waitTimeBetweenMeasurementsMs = 100;

CNEGR::IDistanceSensor *distanceSensor;
CNEGR::ITrafficLight   *trafficLight;
CNEGR::StateMachine    *stateMachine;

/// @brief The main app setup function
///
void setup()
{
  // Initialize serial communication at 19200 bits per second.
  Serial.begin(19200);

  Logger::SetLogLevel(Logger::Level::INFO);

  // Create the distance sensor object
  distanceSensor = new CNEGR::HCSR04();
  //distanceSensor = new CNEGR::MockDistanceSensor();
  // Assert if the the distanceSensor object can't be created
  assert(distanceSensor != nullptr);

  // Setup de distance sensor
  CNEGR::IDistanceSensor::Config distanceSensorConfig;

  distanceSensorConfig.name       = "DistanceSensor1";
  distanceSensorConfig.triggerPin = triggerPin;
  distanceSensorConfig.echoPin    = echoPin;

  Result result = distanceSensor->Init(distanceSensorConfig);
  if (result != RESULT_OK)
  {
    // Sensor Init() failed, assert as we can't continue the execution
    assert(result == RESULT_OK);
  }

  // Create the traffic light object
  //trafficLight   = new CNEGR::MockTrafficLight();
  trafficLight   = new CNEGR::DiscreteLEDTrafficLight();
  // Assert if the the trafficLight object can't be created
  assert(trafficLight != nullptr);

  // Setup the traffic light component
  CNEGR::ITrafficLight::Config trafficLightConfig;

  trafficLightConfig.name           = "TrafficLight1";
  trafficLightConfig.redLightPin    = redLightPin;
  trafficLightConfig.yellowLightPin = yellowLightPin;
  trafficLightConfig.greenLightPin  = greenLightPin;
  trafficLightConfig.pinsPolarity   = CNEGR::SignalPolarity::ActiveHigh;

  result = trafficLight->Init(trafficLightConfig);
  if (result != RESULT_OK)
  {
    // TrafficLight Init() failed, assert as we can't continue the execution
    assert(result == RESULT_OK);
  }

  // Create the state machine object
  stateMachine = new CNEGR::StateMachine();
  // Assert if the the stateMachine object can't be created
  assert(stateMachine != nullptr);

  // Setup the state machine component
  CNEGR::StateMachine::Config stateMachineConfig;
  stateMachineConfig.distanceSensor                     = distanceSensor;
  stateMachineConfig.trafficLight                       = trafficLight;
  stateMachineConfig.maxDistanceThresholdMm             = maxDistanceThresholdMm;
  stateMachineConfig.farThresholdMm                     = farThresholdMm;
  stateMachineConfig.nearThresholdMm                    = nearThresholdMm;
  stateMachineConfig.movingDistanceDetectionThresholdMm = movingDistanceThresholdMm;
  stateMachineConfig.movingTimeThresholdMs              = movingTimeThresholdMs;
  stateMachineConfig.holdingTimeThresholdMs             = holdingTimeThresholdMs;

  stateMachine->Init(stateMachineConfig);

  // Everything is now setup up abd ready to go
}

/// @brief The main app loop
///
void loop()
{
  stateMachine->Update();

  delay(waitTimeBetweenMeasurementsMs);
}
