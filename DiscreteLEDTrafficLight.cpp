///
/// @file DiscreteLEDTrafficLight.h
///
/// @brief DiscreteLEDTrafficLight class implementation
///
/// @author Carl Negrescu
/// @date May 23, 2025
///
#include "DiscreteLEDTrafficLight.h"

namespace CNEGR
{
  /// @brief Constructor.
  DiscreteLEDTrafficLight::DiscreteLEDTrafficLight()
    :_initDone(false),
    _redLightPin(NOT_A_PIN),
    _yellowLightPin(NOT_A_PIN),
    _greenLightPin(NOT_A_PIN),
    _pinsPolarity(SignalPolarity::ActiveHigh)
  {
    _name[0] = '\0';
  }

  /// @brief Destructor.
  DiscreteLEDTrafficLight::~DiscreteLEDTrafficLight()
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
  Result DiscreteLEDTrafficLight::Init(const Config& configuration)
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

    _redLightPin    = configuration.redLightPin;
    _yellowLightPin = configuration.yellowLightPin;
    _greenLightPin  = configuration.greenLightPin;
    _pinsPolarity   = configuration.pinsPolarity;

    // Configure all the pins as  outputs.
    pinMode(_redLightPin, OUTPUT);
    pinMode(_yellowLightPin, OUTPUT);
    pinMode(_greenLightPin, OUTPUT);

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
  bool DiscreteLEDTrafficLight::IsInitialized() const
  {
    return _initDone;
  }

  /// @brief Deinitialization function for the device.
  ///
  void DiscreteLEDTrafficLight::Deinit()
  {
    // Turn off all the lights
    SetAllLightsOff();

    // Clear the name
    _name[0] = '\0';

    // Set all pins to inputs which basically puts them in a
    // high-impedence state (low power consumption)
    pinMode(_redLightPin, INPUT);
    pinMode(_yellowLightPin, INPUT);
    pinMode(_greenLightPin, INPUT);

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
  Result DiscreteLEDTrafficLight::TurnOn(LightSelector whichLight)
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
  Result DiscreteLEDTrafficLight::TurnOff(LightSelector whichLight)
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
  Result DiscreteLEDTrafficLight::SetState(LightSelector whichLight, LightState state)
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
        SetPintState(_redLightPin, true);
        SetPintState(_yellowLightPin, false);
        SetPintState(_greenLightPin, false);
        break;

      case LightSelector::YellowLight:
        SetPintState(_redLightPin, false);
        SetPintState(_yellowLightPin, true);
        SetPintState(_greenLightPin, false);
        break;

      case LightSelector::GreenLight:
        SetPintState(_redLightPin, false);
        SetPintState(_yellowLightPin, false);
        SetPintState(_greenLightPin, true);
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
  Result DiscreteLEDTrafficLight::GetState(LightSelector whichLight, LightState& state)
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    Result result = RESULT_OK;

    switch(whichLight)
    {
      case LightSelector::RedLight:
        state = GetPintState(_redLightPin) ? LightState::On : LightState::Off;
        break;

      case LightSelector::YellowLight:
        state = GetPintState(_yellowLightPin) ? LightState::On : LightState::Off;
        break;

      case LightSelector::GreenLight:
        state = GetPintState(_greenLightPin) ? LightState::On : LightState::Off;
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
  Result DiscreteLEDTrafficLight::SetAllLightsOff()
  {
    if (!IsInitialized())
      return RESULT_NOT_READY;

    SetPintState(_redLightPin, false);
    SetPintState(_yellowLightPin, false);
    SetPintState(_greenLightPin, false);

    return RESULT_OK;
  }

  /// @brief Performs a test of the lights.
  ///
  /// @retval RESULT_OK         The test completed successfully.
  /// @retval RESULT_NOT_READY  The execution failed because the device was not initialized (Init() method wasn't called).
  /// @retval RESULT_DEV_ERR    The device is not present or is in some kind of error state.
  ///
  Result DiscreteLEDTrafficLight::PerformLightsTest()
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

  /// @brief Sets the state of  the specified pin.
  ///
  /// @param pin      The pin number
  /// @param active   The pin state
  ///
  void DiscreteLEDTrafficLight::SetPintState(uint8_t pin, bool active)
  {
    uint8_t state = 0;

    if (active)
      state = (_pinsPolarity == SignalPolarity::ActiveHigh ? HIGH : LOW);
    else
      state = (_pinsPolarity == SignalPolarity::ActiveHigh ? LOW : HIGH);

    digitalWrite(pin, state);
  }

  /// @brief Gets the state of the specified pin.
  ///
  /// @param pin      The pin number
  ///
  /// @retval Returns the pin state
  ///
  bool DiscreteLEDTrafficLight::GetPintState(uint8_t pin)
  {
    bool state;
    bool rawState = digitalRead(pin) != 0;

    if (rawState)
      state = (_pinsPolarity == SignalPolarity::ActiveHigh ? HIGH : LOW);
    else
      state = (_pinsPolarity == SignalPolarity::ActiveHigh ? LOW : HIGH);

    return state;
  }
}
