///
/// @file MockTrafficLight.h
///
/// @brief MockTrafficLight class implementation
///
/// @author Carl Negrescu
/// @date May 21, 2025
///
#include "MockTrafficLight.h"

namespace CNEGR
{
  /// @brief Constructor.
  MockTrafficLight::MockTrafficLight()
    :_initDone(false),
    _redLightState(LightState::Off),
    _yellowLightState(LightState::Off),
    _greenLightState(LightState::Off)
  {
    _name[0] = '\0';
  }

  /// @brief Destructor.
  MockTrafficLight::~MockTrafficLight()
  {
    Deinit();
  }

  /// @brief Initialization function.
  ///
  /// @param configuration      The configuration data.
  ///
  /// @retval RESULT_OK         The device was successfully configured.
  /// @retval RESULT_BUSY       The  device was already configured. Deinit() must be called before calling Init() again.
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  /// @retval RESULT_BAD_PARAM  A parameter value is invalid
  ///
  Result MockTrafficLight::Init(const Config& configuration)
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

    // Make sure that all the lights are off
    SetAllLightsOff();

    // Set the init done flag
    _initDone = true;
    return RESULT_OK;
  }

  /// @brief Get whether the device was initialized
  ///
  /// @return boolean true if it is initialized
  /// and available for use
  bool MockTrafficLight::IsInitialized() const
  {
    return _initDone;
  }

  /// @brief Deinitialization function for the device.
  ///
  void MockTrafficLight::Deinit()
  {
    // Turn off all the lights
    SetAllLightsOff();

    // Clear the name
    _name[0] = '\0';

    // And reset the init done flag
    _initDone = false;
  }

  /// @brief Turns On the specified light. Only one light can be turned on at a time. If a light is already on when this method
  /// is invoked it will be turned off before the new light is turned on.
  ///
  /// @param whichLight         Defines which light should be turned on.
  ///
  /// @retval RESULT_OK         The light was successfully turned on.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  Result MockTrafficLight::TurnOn(LightSelector whichLight)
  {
    return SetState(whichLight, LightState::On);
  }

  /// @brief Turns Off the specified light.
  ///
  /// @param whichLight         Defines which light should be turned off.
  ///
  /// @retval RESULT_OK         The light was successfully turned off.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  Result MockTrafficLight::TurnOff(LightSelector whichLight)
  {
    return SetState(whichLight, LightState::Off);
  }

  /// @brief Sets the state for the specified light.
  ///
  /// @param whichLight         Defines which light should be turned on/off.
  /// @param state              The light state,
  ///
  /// @retval RESULT_OK         The light state was successfully set.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  /// @retval RESULT_BAD_PARAM  A parameter value is invalid
  ///
  Result MockTrafficLight::SetState(LightSelector whichLight, LightState state)
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    Result result = RESULT_OK;

    if ((state != LightState::On) ||
        (state != LightState::On))
      return RESULT_BAD_PARAM;

    switch(whichLight)
    {
      case LightSelector::RedLight:
        _redLightState    = state;
        _yellowLightState = LightState::Off;
        _greenLightState  = LightState::Off;
        break;

      case LightSelector::YellowLight:
        _redLightState    = LightState::Off;
        _yellowLightState = state;
        _greenLightState  = LightState::Off;
        break;

      case LightSelector::GreenLight:
        _redLightState    = LightState::Off;
        _yellowLightState = LightState::Off;
        _greenLightState  = state;
        break;

      default:
        result = RESULT_BAD_PARAM;
        break;
    }

    return result;
  }

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
  Result MockTrafficLight::GetState(LightSelector whichLight, LightState& state)
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    Result result = RESULT_OK;

    switch(whichLight)
    {
      case LightSelector::RedLight:
        state = _redLightState;
        break;

      case LightSelector::YellowLight:
        state = _yellowLightState;
        break;

      case LightSelector::GreenLight:
        state = _greenLightState;
        break;

      default:
        result = RESULT_BAD_PARAM;
        break;
    }

    return result;
  }

  /// @brief Turns off all the lights.
  ///
  /// @retval RESULT_OK         The light state was successfully set.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called)
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  ///
  Result MockTrafficLight::SetAllLightsOff()
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    _redLightState    = LightState::Off;
    _yellowLightState = LightState::Off;
    _greenLightState  = LightState::Off;

    return RESULT_OK;
  }

  /// @brief Performs a test of the lights.
  ///
  /// @retval RESULT_OK         The test completed successfully.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  ///
  Result MockTrafficLight::PerformLightsTest()
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    Result result = TurnOn(LightSelector::RedLight);

    if(result == RESULT_OK)
    {
      delay(500);
      result = TurnOn(LightSelector::YellowLight);
    }

    if(result == RESULT_OK)
    {
      delay(500);
      result = TurnOn(LightSelector::GreenLight);
    }

    if(result == RESULT_OK)
    {
      delay(500);
      result = SetAllLightsOff();
    }

    return result;
  }
}
