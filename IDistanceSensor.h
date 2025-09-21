///
/// @file IDistanceSensor.h
///
/// @brief IDistanceSensor interface definition
///
/// @author Carl Negrescu
/// @date May 21, 2025
///
#pragma once

#if !defined(_IDISTANCESENSOR_H_)
#define _IDISTANCESENSOR_H_

#include <Arduino.h>
#include "Result.h"

namespace CNEGR
{
  /// @brief IDistanceSensor interface definition
  ///
  class IDistanceSensor
  {
  public:
    virtual ~IDistanceSensor() {}

  public:
    struct Config
    {
      const char* name;                       ///< A symbolic name for the sensor
      uint8_t     triggerPin;                 ///< The trigger GPIO pin number (output)
      uint8_t     echoPin;                    ///< The echo GPIO pin number (input)
    };

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
    virtual Result Init(const Config& configuration) = 0;

    /// @brief Get whether the sensor device was initialized
    ///
    /// @return boolean true if it is initialized
    /// and available for use
    virtual bool IsInitialized() const = 0;

    /// @brief Deinitialization function for the device.
    ///
    virtual void Deinit() = 0;

    /// @brief Measures the distance.
    ///
    /// @param distance           Contains the measured distance in millimeters if the measurement was successful.
    ///
    /// @retval RESULT_OK         The distance was successfully measured.
    /// @retval RESULT_NOT_READY  The distance measurement failed because the sensor was not initialized (Init() wasn't called)
    /// @retval RESULT_TIMEOUT    The distance measurement failed because there was a timeout while waiting for the echo signal.
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind
    ///                           of error state.
    virtual Result MeasureDistance(uint32_t& distance) = 0;

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
    virtual Result MeasureDistance(uint32_t ambientTemperature, uint32_t& distance) = 0;
  };
}

#endif // _IDISTANCESENSOR_H_
