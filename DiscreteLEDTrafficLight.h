///
/// @file DiscreteLEDTrafficLight.h
///
/// @brief DiscreteLEDTrafficLight class definition
///
/// @author Carl Negrescu
/// @date May 23, 2025
///
#pragma once

#if !defined(_DISCRETELEDTRAFFICLIGHT_H_)
#define _DISCRETELEDTRAFFICLIGHT_H_

#include "ITrafficLight.h"

namespace CNEGR
{
  #define MAX_DEVICE_NAME_LENGTH MAX_COMPONENT_NAME_LENGTH

  /// @brief DiscreteLEDTrafficLight class definition
  ///
  class DiscreteLEDTrafficLight: public ITrafficLight
  {
  public:
    /// @brief Constructor.
    DiscreteLEDTrafficLight();

    /// @brief Destructor.
    virtual ~DiscreteLEDTrafficLight();

  public:
    /// @brief Initialization function.
    ///
    /// @param configuration      The configuration data.
    ///
    /// @retval RESULT_OK         The device was successfully configured.
    /// @retval RESULT_BUSY       The  device was already configured. Deinit() must be called before calling Init() again.
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    /// @retval RESULT_BAD_PARAM  A parameter value is invalid
    ///
    virtual Result Init(const Config& configuration);

    /// @brief Get whether the device was initialized
    ///
    /// @return boolean true if it is initialized
    /// and available for use
    virtual bool IsInitialized() const;

    /// @brief Deinitialization function for the device.
    ///
    virtual void Deinit();

    /// @brief Turns On the specified light. Only one light can be turned on at a time. If a light is already on when this method
    /// is invoked it will be turned off before the new light is turned on.
    ///
    /// @param whichLight         Defines which light should be turned on.
    ///
    /// @retval RESULT_OK         The light was successfully turned on.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    virtual Result TurnOn(LightSelector whichLight);

    /// @brief Turns Off the specified light.
    ///
    /// @param whichLight         Defines which light should be turned off.
    ///
    /// @retval RESULT_OK         The light was successfully turned off.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    virtual Result TurnOff(LightSelector whichLight);

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
    virtual Result SetState(LightSelector whichLight, LightState state);

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
    virtual Result GetState(LightSelector whichLight, LightState& state);

    /// @brief Turns off all the lights.
    ///
    /// @retval RESULT_OK         The light state was successfully set.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    ///
    virtual Result SetAllLightsOff();

    /// @brief Performs a test of the lights.
    ///
    /// @retval RESULT_OK         The test completed successfully.
    /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
    /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
    ///
    virtual Result PerformLightsTest();

  private:
    /// @brief Sets the state of the specified pin.
    ///
    /// @param pin      The pin number
    /// @param active   The pin state
    ///
    ///
    void SetPintState(uint8_t pin, bool active);

    /// @brief Gets the state of the specified pin.
    ///
    /// @param pin      The pin number
    ///
    /// @retval Returns the pin state
    ///
    bool GetPintState(uint8_t pin);

  private:
    bool            _initDone;                      ///< A flag to indicate whether the sensor was initialized
    char            _name[MAX_DEVICE_NAME_LENGTH];  ///< A symbolic name for this sensor
    uint8_t         _redLightPin;                   ///< The GPIO pin number (output) to control the Red light
    uint8_t         _yellowLightPin;                ///< The GPIO pin number (output) to control the Yellow light
    uint8_t         _greenLightPin;                 ///< The GPIO pin number (output) to control the Green light
    SignalPolarity  _pinsPolarity;                  ///< The polarity for the GPIO pins
  };
}

#endif // _DISCRETELEDTRAFFICLIGHT_H_

