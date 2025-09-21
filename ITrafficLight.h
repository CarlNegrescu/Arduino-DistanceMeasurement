///
/// @file ITrafficLight.h
///
/// @brief ITrafficLight interface definition
///
/// @author Carl Negrescu
/// @date May 23, 2025

#pragma once

#if !defined(_ITRAFFICLIGHT_H_)
#define _ITRAFFICLIGHT_H_

#include <Arduino.h>
#include "Result.h"
#include "CommonDefines.h"

namespace CNEGR
{
  /// @brief ITrafficLight interface definition
  ///
  class ITrafficLight
  {
  public:
    virtual ~ITrafficLight() {}

  public:
    struct Config
    {
      const char*     name;                       ///< A symbolic name for the traffic light component
      uint8_t         redLightPin;                ///< The GPIO pin number (output) to control the Red light
      uint8_t         yellowLightPin;             ///< The GPIO pin number (output) to control the Yellow light
      uint8_t         greenLightPin;              ///< The GPIO pin number (output) to control the Green light
      SignalPolarity  pinsPolarity;               ///< The polarity for the GPIO pins
    };

    enum LightSelector
    {
      RedLight,
      YellowLight,
      GreenLight
    };

    enum LightState
    {
      On,
      Off
    };

    /// @brief Initialization function.
    ///
    /// @param configuration      The configuration data.
    ///
    /// @retval RESULT_OK         The device was successfully configured.
    /// @retval RESULT_BUSY       The  device was already configured. Deinit() must be called before calling Init() again.
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    /// @retval RESULT_BAD_PARAM  A parameter value is invalid
    ///
    virtual Result Init(const Config& configuration) = 0;

    /// @brief Get whether the device was initialized
    ///
    /// @return boolean true if it is initialized
    /// and available for use
    virtual bool IsInitialized() const = 0;

    /// @brief Deinitialization function for the device.
    ///
    virtual void Deinit() = 0;

    /// @brief Turns On the specified light. Only one light can be turned on at a time. If a light is already on when this method
    /// is invoked it will be turned off before the new light is turned on.
    ///
    /// @param whichLight         Defines which light should be turned on.
    ///
    /// @retval RESULT_OK         The light was successfully turned on.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    virtual Result TurnOn(LightSelector whichLight) = 0;

    /// @brief Turns Off the specified light.
    ///
    /// @param whichLight         Defines which light should be turned off.
    ///
    /// @retval RESULT_OK         The light was successfully turned off.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    virtual Result TurnOff(LightSelector whichLight) = 0;

    /// @brief Sets the state for the specified light.
    ///
    /// @param whichLight         Defines which light should be turned ON or OFF.
    /// @param state              The light state
    ///
    /// @retval RESULT_OK         The light state was successfully set.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    /// @retval RESULT_BAD_PARAM  A parameter value is invalid
    ///
    virtual Result SetState(LightSelector whichLight, LightState state) = 0;

    /// @brief Gets the state for the specified light.
    ///
    /// @param whichLight         Defines which light should be turned ON or OFF.
    /// @param state              The light state
    ///
    /// @retval RESULT_OK         The light state was retrieved successfully.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    /// @retval RESULT_BAD_PARAM  A parameter value is invalid
    ///
    virtual Result GetState(LightSelector whichLight, LightState& state) = 0;

    /// @brief Turns off all the lights.
    ///
    /// @retval RESULT_OK         The light state was successfully set.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    ///
    virtual Result SetAllLightsOff() = 0;

    /// @brief Performs a test of the lights.
    ///
    /// @retval RESULT_OK         The test completed successfully.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    ///
    virtual Result PerformLightsTest() = 0;
  };
}

#endif // _ITRAFFICLIGHT_H_
