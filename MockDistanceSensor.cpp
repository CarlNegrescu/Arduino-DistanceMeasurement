///
/// @file MockDistanceSensor.h
///
/// @brief MockDistanceSensor class implementation
///
/// @author Carl Negrescu
/// @date May 25, 2025
///

#include "MockDistanceSensor.h"

namespace CNEGR
{
  const uint32_t minTriggerPulseDurationUs  = 10;                         ///< The minimum trigger pulse duration in microseconds
  const uint32_t minDistanceMm              = 20;                         ///< The minimum distance the sensor can detect in millimeters
  const uint32_t maxDistanceMm              = 4000;                       ///< The maximum distance the sensor can detect in millimeters
  const uint32_t speedOfSound               = 340;                        ///< The speed of sound in meters per second

  /// @brief Constructor.
  MockDistanceSensor::MockDistanceSensor()
    :_initDone(false),
    _minTriggerPulseDurationUs(minTriggerPulseDurationUs),
    _minDistanceMm(minDistanceMm),
    _maxDistanceMm(maxDistanceMm)
  {
    _name[0] = '\0';
  }

  /// @brief Default destructor.
  MockDistanceSensor::~MockDistanceSensor()
  {
    Deinit();
  }

  /// @brief Initialization function.
  ///
  /// @param configuration      The configuration data.
  ///
  /// @retval RESULT_OK         The device was successfully configured.
  /// @retval RESULT_BUSY       The  device was already configured.
  ///                           Deinit() must be called before calling Init() again.
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
  ///                           of error state.
  /// @retval RESULT_BAD_PARAM  A parameter value is invalid
  ///
  Result MockDistanceSensor::Init(const Config& configuration)
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

    // Seed the random number generator
    randomSeed(analogRead(0));

    // Copy the name
    strncpy((char *)(&_name[0]), configuration.name, sizeof(_name)-1);

    // Set the init done flag
    _initDone = true;
    return RESULT_OK;
  }

  /// @brief Get whether the sensor device was initialized
  ///
  /// @return boolean true if it is initialized
  /// and available for use
  bool MockDistanceSensor::IsInitialized() const
  {
    return _initDone;
  }

  /// @brief Deinitialization function for the device.
  ///
  void MockDistanceSensor::Deinit()
  {
    // Clear the name
    _name[0] = '\0';

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
  Result MockDistanceSensor::MeasureDistance(uint32_t& distance)
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
  Result MockDistanceSensor::MeasureDistance(uint32_t ambientTemperature, uint32_t& distance)
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    // Simulate triggering the measurement
    TriggerMeasurement();

    // Simulate reading the distance from the sensor
    distance = 0;
    ReadDistance(ambientTemperature, distance);

    return RESULT_OK;
  }

  void MockDistanceSensor::TriggerMeasurement()
  {
    // Simulate the trigger pulse
    // First a short startup delay
    delayMicroseconds(_minTriggerPulseDurationUs/5);
    // Next the actual trigger pulse delay
    delayMicroseconds(_minTriggerPulseDurationUs);
    // And finaly a short delay
    delayMicroseconds(_minTriggerPulseDurationUs/5);
  }

  /// @brief Gets the speed of sound at the specified ambient temperature
  ///
  /// @param ambientTemperature The ambient temperature in deci-degrees celsius
  ///
  float SpeedOfSound(float ambientTemperature)
  {
    return 331.4f + (0.6f * (float)ambientTemperature / 10.0f);
  }

  static uint32_t Distance2Time(uint32_t ambientTemperature, uint32_t distanceMm)
  {
    // The time in seconds is calculated as:
    //    timeInSeconds = distance * 2 / speedOfSound;

    // Calculate the distance in millimeters
    uint32_t timeUs = ((float)distanceMm/1000.0f / (SpeedOfSound(ambientTemperature) / 2)) * 1000.0f * 1000.0f;
    return timeUs;
  }

  void MockDistanceSensor::ReadDistance(uint32_t ambientTemperature, uint32_t& distance)
  {
    // Generate a random value in the [_minDistanceMm, _maxDistanceMm] interval
    distance = random(_minDistanceMm, _maxDistanceMm);

    // Simulate waiting for the measurement
    uint32_t timeUs = Distance2Time(ambientTemperature, distance);

    if (timeUs < 1000)
      delayMicroseconds(timeUs);
    else
      delay((timeUs/1000 + 1));
  }
}
