///
/// @file MockTrafficLight.h
///
/// @brief MockTrafficLight class definition
///
/// @author Carl Negrescu
/// @date May 21, 2025
///
#pragma once

#if !defined(_MOCKTRAFFICLIGHT_H_)
#define _MOCKTRAFFICLIGHT_H_

#include "ITrafficLight.h"
#include "CommonDefines.h"

namespace CNEGR
{
  #define MAX_DEVICE_NAME_LENGTH MAX_COMPONENT_NAME_LENGTH

  /// @brief MockTrafficLight class definition
  ///
  class MockTrafficLight: public ITrafficLight
  {
  public:
    /// @brief Constructor.
    MockTrafficLight();

    /// @brief Destructor.
    virtual ~MockTrafficLight();

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
    bool            _initDone;                        ///< A flag to indicate whether the sensor was initialized
    char            _name[MAX_DEVICE_NAME_LENGTH];    ///< A symbolic name for this sensor
    LightState      _redLightState;                   ///< The state of the Red light
    LightState      _yellowLightState;                ///< The state of the Yellow light
    LightState      _greenLightState;                 ///< The state of the Green light
  };
}

#endif // _MOCKTRAFFICLIGHT_H_
