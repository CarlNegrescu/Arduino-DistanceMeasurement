///
/// @file DistanceSensor.h
///
/// @brief DistanceSensor class implementation
///
/// @author Carl Negrescu
/// @date May 29, 2025
///

#include "DistanceSensor.h"
#include "DebugUtils.h"

namespace CNEGR
{
  /// @brief Constructor.
  DistanceSensor::DistanceSensor(uint32_t       minTriggerPulseDurationUs,  ///< The minimum trigger pulse duration in microseconds
                                 uint32_t       minDistanceMm,              ///< The minimum distance the sensor can detect in millimeters
                                 uint32_t       maxDistanceMm,              ///< The maximum distance the sensor can detect in millimeters
                                 SignalPolarity triggerPolarity,            ///< The trigger signal polarity
                                 SignalPolarity echoPolarity                ///< The echo signal polarity
                                )
    :_initDone(false),
    _triggerPin(NOT_A_PIN),
    _echoPin(NOT_A_PIN),
    _minTriggerPulseDurationUs(minTriggerPulseDurationUs),
    _minDistanceMm(minDistanceMm),
    _maxDistanceMm(maxDistanceMm),
    _triggerPolarity(triggerPolarity),
    _echoPolarity(echoPolarity)
  {
    _name[0] = '\0';
  }

  /// @brief Default destructor.
  DistanceSensor::~DistanceSensor()
  {
    Deinit();
  }

  /// @brief Initialization function.
  ///
  /// @param configuration      The configuration data.
  ///
  /// @retval RESULT_OK         The device was successfully configured.
  /// @retval RESULT_BUSY       The device was already configured.
  ///                           Deinit() must be called before calling Init() again.
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
  ///                           of error state.
  /// @retval RESULT_BAD_PARAM  A parameter value is invalid
  ///
  Result DistanceSensor::Init(const Config& configuration)
  {
    if (IsInitialized())
    {
      // Already initialized
      return RESULT_BUSY;
    }

    if (configuration.name == NULL)
    {
      // Name is invalid
      return RESULT_BAD_PARAM;
    }

    // Copy the name
    strncpy((char *)(&_name[0]), configuration.name, sizeof(_name)-1);

    _triggerPin = configuration.triggerPin;
    _echoPin = configuration.echoPin;

    // Configure the trigger pin as an output.
    pinMode(_triggerPin, OUTPUT);

    // Configure the echo pin as an intput.
    pinMode(_echoPin, INPUT);

    // Set the init done flag
    _initDone = true;
    return RESULT_OK;
  }

  /// @brief Get whether the sensor device was initialized
  ///
  /// @return boolean true if it is initialized
  /// and available for use
  bool DistanceSensor::IsInitialized() const
  {
    return _initDone;
  }

  /// @brief Deinitialization function for the device.
  ///
  void DistanceSensor::Deinit()
  {
    // Clear the name
    _name[0] = '\0';

    // Set all pins to inputs which basically puts them in a
    // high-impedence state (low power consumption)
    pinMode(_echoPin, INPUT);
    pinMode(_triggerPin, INPUT);

    // And reset the init done flag
    _initDone = false;
  }

  /// @brief Measures the distance.
  ///
  /// @param distance           Contains the measured distance in millimeters if the measurement was successful.
  ///
  /// @retval RESULT_OK         The distance was successfully measured.
  /// @retval RESULT_NOT_READY  The distance measurement failed because the sensor was not initialized (Init() wasn't called)
  /// @retval RESULT_TIMEOUT    The distance measurement failed because there was a timeout while waiting for the echo signal.
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
  ///                           of error state.
  Result DistanceSensor::MeasureDistance(uint32_t& distance)
  {
    const uint32_t ambientTemperature = 20 * 10;
    return MeasureDistance(ambientTemperature, distance);
  }

  /// @brief Measures the distance and adjusts the result for the ambient temperature.
  ///
  /// @param ambientTemperature The ambient temperature in deci-degrees celsius
  /// @param distance           Contains the measured distance in millimeters if the measurement was successful.
  ///
  /// @retval RESULT_OK         The distance was successfully measured.
  /// @retval RESULT_NOT_READY  The distance measurement failed because the sensor was not initialized (Init() wasn't called)
  /// @retval RESULT_TIMEOUT    The distance measurement failed because there was a timeout while waiting for the echo signal.
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
  ///                           of error state.
  Result DistanceSensor::MeasureDistance(uint32_t ambientTemperature, uint32_t& distance)
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    // Trigger the distance measurement
    TriggerMeasurement();

    // Read the distance from the sensor
    distance = 0;
    Result result = ReadDistance(ambientTemperature, distance);

    return result;
  }

  /// @brief Gets the speed of sound at the specified ambient temperature
  ///
  /// @param ambientTemperature The ambient temperature in deci-degrees celsius
  ///
  float SpeedOfSound(uint32_t ambientTemperature)
  {
    return 331.4f + (0.6f * (float)ambientTemperature / 10.0f);
  }

  uint32_t DistanceSensor::Time2Distance(uint32_t ambientTemperature, uint32_t timeUs)
  {
    // The distance in meters is calculated as:
    //   distance = timeInSeconds / speedOfSound / 2;

    // Calculate the distance in millimeters
    uint32_t distanceMm = ((float)timeUs/1000.0f/1000.0f * (SpeedOfSound(ambientTemperature) / 2)) * 1000.0f;
    return distanceMm;
  }

  void DistanceSensor::SetTriggerPintState(bool active)
  {
    uint8_t state = 0;

    if (active)
      state = (_triggerPolarity == SignalPolarity::ActiveHigh ? HIGH : LOW);
    else
      state = (_triggerPolarity == SignalPolarity::ActiveHigh ? LOW : HIGH);

    digitalWrite(_triggerPin, state);
  }

  void DistanceSensor::TriggerMeasurement()
  {
    // First make sure that we start with trigger pin not being active
    SetTriggerPintState(false);
    // Wait a small amount of time to allow the sensor to see the pin state change and re-sync
    delayMicroseconds(_minTriggerPulseDurationUs/5);

    // Activate the trigger pin
    SetTriggerPintState(true);
    // Need to keep the trigger pin active for the _minTriggerPulseDurationUs
    delayMicroseconds(_minTriggerPulseDurationUs);

    // Deactivate the trigger pin
    SetTriggerPintState(false);
    // Followed by a short delay
    delayMicroseconds(_minTriggerPulseDurationUs/5);
  }

  static uint32_t Distance2Time(uint32_t ambientTemperature, uint32_t distanceMm)
  {
    // The time in seconds is calculated as:
    //    timeInSeconds = distance * 2 / speedOfSound;

    // Calculate the distance in millimeters
    uint32_t timeUs = ((float)distanceMm/1000.0f / (SpeedOfSound(ambientTemperature) / 2)) * 1000.0f * 1000.0f;
    return timeUs;
  }

  bool DistanceSensor::GetEchoPinState()
  {
    bool rawPinState = (digitalRead(_echoPin) != 0 ? true : false);

    return ((_echoPolarity == SignalPolarity::ActiveHigh) ? rawPinState : !rawPinState);
  }

  Result DistanceSensor::ReadDistance(uint32_t ambientTemperature, uint32_t& distance)
  {
    // Calculate the maximum wait duration for the echo pulse
    uint32_t maxWaitDurationUs = Distance2Time(ambientTemperature, _maxDistanceMm);
    Logger::Debug(F("maxWaitDurationUs is %u us"), maxWaitDurationUs);

    bool echoPinState = false;
    uint32_t startTime = micros();
    uint32_t endTime   = startTime + maxWaitDurationUs;
    uint32_t time = startTime;

    Logger::Debug(F("startTime is %u us"), startTime);
    Logger::Debug(F("endTime is %u us"), endTime);

    // Wait for the raising edge of the echo pulse
    while ((echoPinState == false) && (time < endTime))
    {
      echoPinState = GetEchoPinState();
      time = micros();
    };

    // Did not see the raising edge of the echo pulse
    // Possible reasons:
    //  (1) The sensor is not working correctly, or
    //  (2) The sensor is not connected to the board or
    //  (3) There are no objects in the detection range
    //
    // Since we can't differentiate between these reasons
    // we simply return a timeout error
    if (echoPinState == false)
    {
      Logger::Debug(F("Timeout waiting for the echo pulse raising edge!"));
      return RESULT_TIMEOUT;
    }

    // The echo pulse started so record the time when this happen
    uint32_t echoPulseStartTimeUs = micros();

    Logger::Debug(F("echoPulseStartTimeUs is %d us"), echoPulseStartTimeUs);

    // Adjust the endTime and wait for the falling edge of the echo pulse
    endTime   = micros() + maxWaitDurationUs;

    // Wait for the raising edge of the echo pulse
    while ((echoPinState == true) && (time < endTime))
    {
      echoPinState = GetEchoPinState();
      time = micros();
    };

    // Did not see the falling edge of the echo pulse
    // Possible reasons:
    //  (1) The sensor is not working correctly, or
    //  (2) The sensor is not connected to the board or
    //  (3) There are no objects in the detection range
    //
    // Since we can't differentiate between these reasons
    // we simply return a timeout error
    if (echoPinState == true)
    {
      Logger::Debug(F("Timeout waiting for the echo pulse falling edge!"));
      return RESULT_TIMEOUT;
    }

    // The echo pulse finished so now record its duration
    uint32_t echoPulseDurationUs = time - echoPulseStartTimeUs;

    Logger::Debug(F("echoPulseDurationUs is %u us"), echoPulseDurationUs);

    distance = Time2Distance(ambientTemperature, echoPulseDurationUs);
    return RESULT_OK;
  }
}
