///
/// @file StateMachine.h
///
/// @brief StateMachine class implementation
///
/// @author Carl Negrescu
/// @date May 24, 2025
///

#include "DebugUtils.h"
#include "StateMachine.h"

namespace CNEGR
{
  /// @brief Constructor.
  StateMachine::StateMachine()
    :_initDone(false),
     _state(State::Invalid),
     _previousState(State::Invalid),
     _distanceSensor(nullptr),
     _trafficLight(nullptr),
     _previousDistance(UINT32_MAX),
     _previousTime(0),
     _maxDistanceThresholdMm(0),
     _farThresholdMm(0),
     _nearThresholdMm(0),
     _movingDistanceDetectionThresholdMm(0),
     _movingTimeThresholdMs(0),
     _holdingTimeThresholdMs(0)
  {
  }

  /// @brief Destructor.
  StateMachine::~StateMachine()
  {

  }

  /// @brief Initialize the state machine
  /// @param configuration      The configuration data.
  ///
  ///
  void StateMachine::Init(const Config& configuration)
  {
    assert(_initDone == false);
    assert(configuration.distanceSensor != nullptr);
    assert(configuration.trafficLight != nullptr);

    _distanceSensor                     = configuration.distanceSensor;
    _trafficLight                       = configuration.trafficLight;
    _maxDistanceThresholdMm             = configuration.maxDistanceThresholdMm;
    _farThresholdMm                     = configuration.farThresholdMm;
    _nearThresholdMm                    = configuration.nearThresholdMm;
    _movingDistanceDetectionThresholdMm = configuration.movingDistanceDetectionThresholdMm;
    _movingTimeThresholdMs              = configuration.movingTimeThresholdMs;
    _holdingTimeThresholdMs             = configuration.holdingTimeThresholdMs;

    _state = State::Initializing;
    _previousDistance = UINT32_MAX;
    _previousTime = 0;

    _initDone = true;
  }

  /// @brief Update the state machine state.
  ///
  /// @note This method must be called periodically
  /// in the main app loop
  ///
  void StateMachine::Update()
  {
    assert(_initDone == true);

    // Get the current distance
    uint32_t distance = MeasureDistance();
    // and current time
    uint32_t time     = millis();

    // Calculate deltaT and deltaD
    uint32_t deltaT = millis() - _previousTime;
    int32_t  deltaD = (distance > _previousDistance) ?
                            (int32_t)(distance - _previousDistance) :
                            ((int32_t)(_previousDistance - distance) * (-1));

    Logger::Info(F("deltaT is %d ms"), deltaT);
    Logger::Info(F("deltaD is %d mm"), deltaD);

    MovingDirection movingDirection = GetMovingDirection(deltaT, deltaD);

    Logger::Info(F("movingDirection is %s"), ToString(movingDirection));

    // Update the previous distance value
    _previousDistance = distance;

    State nextState = _state;

    Logger::Info(F("Current state is %s"), ToString(_state));

    switch(_state)
    {
      case State::Initializing:
        _previousTime = time;
        nextState = State::Idle;
        if (TestLights() != RESULT_OK)
          nextState = State::Error;
        break;

      case State::Idle:
        _previousTime = time;
        switch(movingDirection)
        {
          case MovingDirection::Stopped:
            SetAllLightsOff();
            break;

          case MovingDirection::Backward:
            nextState = State::SubjectRetreating;
            break;

          case MovingDirection::Forward:
            nextState = State::SubjectApproaching;
            break;
        }
        break;

      case State::SubjectApproaching:
        SetTrafficLights(distance);
        switch(movingDirection)
        {
          case MovingDirection::Stopped:
            if (deltaT > _holdingTimeThresholdMs)
            {
              SetAllLightsOff();
              nextState = State::Idle;
            }
            break;

          case MovingDirection::Backward:
            _previousTime = time;
            nextState = State::SubjectRetreating;
            break;

          case MovingDirection::Forward:
            _previousTime = time;
            nextState = State::SubjectApproaching;
            break;
        }
        break;

      case State::SubjectRetreating:
        SetTrafficLights(distance);
        switch(movingDirection)
        {
          case MovingDirection::Stopped:
            if (deltaT > _holdingTimeThresholdMs)
            {
              SetAllLightsOff();
              nextState = State::Idle;
            }
            break;

          case MovingDirection::Backward:
            _previousTime = time;
            nextState = State::SubjectRetreating;
            break;

          case MovingDirection::Forward:
            _previousTime = time;
            nextState = State::SubjectApproaching;
            break;
        }
        break;

      case State::Invalid:
        nextState = State::Error;
        break;

      case State::Error:
        assert(_state != State::Error);
        break;

      default:
        nextState = State::Error;
        break;
    }

    // Update the previous state and the current state
    _previousState = _state;
    _state = nextState;

    Logger::Info(F("Next state is %s"), ToString(_state));
  }

  /// @brief Measures the distance.
  ///
  /// @retval returns the measured distance in millimeters.
  ///
  uint32_t StateMachine::MeasureDistance()
  {
    uint32_t distance = 0;
    Result result = RESULT_OK;
    result = _distanceSensor->MeasureDistance(distance);

    switch(result)
    {
      case RESULT_OK:
        Logger::Info(F("MeasureDistance returned RESULT_OK and distance is %d mm"), distance);
        break;

      case RESULT_TIMEOUT:
        // The object may be out of the sensor's measurement range, nothing to do, so simply return a large value
        Logger::Warning(F("MeasureDistance returned RESULT_TIMEOUT"));
        distance = UINT32_MAX;
        break;

      case RESULT_DEV_ERR:
        // Some sort of device error
        // Reset the board to reinitialize everything and hope that it works after that
        Logger::Error(F("MeasureDistance returned RESULT_DEV_ERR, restarting the application"));
        delay(1000);
        Reset();
        break;

      case RESULT_NOT_READY:
        // Init() must be called before the loop starts executing
        // This is developer error, assert as we can't continue the execution
        Logger::Fatal(F("MeasureDistance returned RESULT_NOT_READY"));
        assert(result != RESULT_NOT_READY);
        break;

      default:
        // We received an unknown/unexpected result
        // This is developer error, assert as we can't continue the execution
        Logger::Fatal(F("MeasureDistance returned %s"), ResultToStr(result));
        assert(false);
        break;
    }

    return distance;
  }

  /// @brief Sets Off all traffic lights
  ///
  void StateMachine::SetAllLightsOff()
  {
    Logger::Info(F("All lights OFF"));
    _trafficLight->SetAllLightsOff();
  }

  /// @brief Tests the traffic lights
  ///
  Result StateMachine::TestLights()
  {
    Logger::Info(F("Testing lights..."));
    Result result = _trafficLight->PerformLightsTest();
    if (result != RESULT_OK)
    {
      Logger::Error(F("Lights test error: %s"), ResultToStr(result));
    }
    else
    {
      Logger::Info(F("Lights test completed successfully"));
    }

    return result;
  }

  /// @brief Sets the traffic lights based on the measured distance.
  ///
  /// @param distance The distance in millimeters
  ///
  void StateMachine::SetTrafficLights(uint32_t distance)
  {
    Result result = RESULT_OK;
    if (distance > _maxDistanceThresholdMm)
    {
      Logger::Info(F("All lights OFF"));
      result = _trafficLight->SetAllLightsOff();
    }
    else if (distance > _farThresholdMm)
    {
      Logger::Info(F("Green light ON"));
      result = _trafficLight->TurnOn(CNEGR::ITrafficLight::GreenLight);
    }
    else if (distance > _nearThresholdMm)
    {
      Logger::Info(F("Yellow light ON"));
      result = _trafficLight->TurnOn(CNEGR::ITrafficLight::YellowLight);
    }
    else
    {
      Logger::Info(F("Red light ON"));
      result = _trafficLight->TurnOn(CNEGR::ITrafficLight::RedLight);
    }

    assert (result == RESULT_OK);
  }


  /// @brief Gets the moving direction based on the time and distance
  /// differences from the previous values
  ///
  /// @param deltaT   The time difference in milliseconds
  /// @param deltaD   The distance difference in millimeters
  ///
  /// @retval Returns the direction in which the subject is moving
  ///
  StateMachine::MovingDirection StateMachine::GetMovingDirection(uint32_t deltaT, int32_t  deltaD)
  {
    MovingDirection result = MovingDirection::Stopped;

    if (deltaT > _movingTimeThresholdMs)
    {
        if (abs(deltaD) > _movingDistanceDetectionThresholdMm)
        {
          result = deltaD > 0 ? MovingDirection::Backward : MovingDirection::Forward;
        }
    }

    return result;
  }

  const char *StateMachine::ToString(MovingDirection movingDirection)
  {
    static const char DIRECTION_STOPPED[]  = "Stopped";
    static const char DIRECTION_FORWARD[]  = "Forward";
    static const char DIRECTION_BACKWARD[] = "Backward";
    static const char DIRECTION_INVALID[]  = "Invalid";

    const char* const DIRECTION_STRINGS[] =
    {
      DIRECTION_STOPPED,
      DIRECTION_FORWARD,
      DIRECTION_BACKWARD,
      DIRECTION_INVALID
    };

    if ((movingDirection >= MovingDirection::Stopped) && (movingDirection <= MovingDirection::Forward))
      return reinterpret_cast<const char *>(DIRECTION_STRINGS[movingDirection]);
    else
      return reinterpret_cast<const char *>(DIRECTION_STRINGS[MovingDirection::Forward + 1]);
  }

  const char *StateMachine::ToString(State state)
  {
    static const char STATE_INVALID[]        = "Invalid";
    static const char STATE_INITIALIZING[]   = "Initializing";
    static const char STATE_IDLE[]           = "Idle";
    static const char STATE_ERROR[]          = "Error";
    static const char STATE_APPROACHING[]    = "SubjectApproaching";
    static const char STATE_RETREATING[]     = "SubjectRetreating";
    static const char STATE_UNKNOWN[]        = "Unknown";

    const char* const STATE_STRINGS[] =
    {
        STATE_INVALID,
        STATE_INITIALIZING,
        STATE_IDLE,
        STATE_ERROR,
        STATE_APPROACHING,
        STATE_RETREATING,
        STATE_UNKNOWN
    };

    if ((state >= State::Invalid) && (state <= State::SubjectRetreating))
      return reinterpret_cast<const char *>(STATE_STRINGS[state + 1]);
    else
      return reinterpret_cast<const char *>(STATE_STRINGS[State::SubjectRetreating + 2]);
  }
}
