///
/// @file MockDistanceSensor.h
///
/// @brief MockDistanceSensor class definition
///
/// @author Carl Negrescu
/// @date May 25, 2025

#pragma once

#if !defined(_MOCKDISTANCESENSOR_H_)
#define _MOCKDISTANCESENSOR_H_

#include "IDistanceSensor.h"
#include "CommonDefines.h"

namespace CNEGR
{
  #define MAX_SENSOR_NAME_LENGTH MAX_COMPONENT_NAME_LENGTH

  /// @brief MockDistanceSensor class definition
  ///
  class MockDistanceSensor: public IDistanceSensor
  {
  public:
    /// @brief Constructor.
    MockDistanceSensor();

    /// @brief Destructor.
    virtual ~MockDistanceSensor();

  public:
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
    virtual Result Init(const Config& configuration);

    /// @brief Get whether the sensor device was initialized
    ///
    /// @return boolean true if it is initialized
    /// and available for use
    virtual bool IsInitialized() const;

    /// @brief Deinitialization function for the device.
    ///
    virtual void Deinit();

    /// @brief Measures the distance.
    ///
    /// @param distance           Contains the measured distance in millimeters if the measurement was successful.
    ///
    /// @retval RESULT_OK         The distance was successfully measured.
    /// @retval RESULT_NOT_READY  The distance measurement failed because the sensor was not initialized (Init() wasn't called)
    /// @retval RESULT_TIMEOUT    The distance measurement failed because there was a timeout while waiting for the echo signal.
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
    ///                           of error state.
    virtual Result MeasureDistance(uint32_t& distance);

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
    virtual Result MeasureDistance(uint32_t ambientTemperature, uint32_t& distance);

  private:
    void TriggerMeasurement();
    void ReadDistance(uint32_t ambientTemperature, uint32_t& distance);

  private:
    bool            _initDone;                        ///< A flag to indicate whether the sensor was initialized
    char            _name[MAX_SENSOR_NAME_LENGTH];    ///< A symbolic name for this sensor
    uint32_t        _minTriggerPulseDurationUs;       ///< The minimum trigger pulse duration in microseconds
    uint32_t        _minDistanceMm;                   ///< The minimum distance the sensor can detect in millimeters
    uint32_t        _maxDistanceMm;                   ///< The maximum distance the sensor can detect in millimeters
  };
}
#endif // _MOCKDISTANCESENSOR_H_
