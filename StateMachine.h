///
/// @file StateMachine.h
///
/// @brief The application StateMachine class definition
///
/// @author Carl Negrescu
/// @date May 24, 2025
///
#pragma once

#if !defined(_STATEMACHINE_H_)
#define _STATEMACHINE_H_

#include "IDistanceSensor.h"
#include "ITrafficLight.h"

namespace CNEGR
{
  /// @brief StateMachine class definition
  ///
  class StateMachine
  {
  private:
    enum  State
    {
        Invalid = -1,
        Initializing,
        Idle,
        Error,
        SubjectApproaching,
        SubjectRetreating
    };

    enum MovingDirection
    {
        Stopped,
        Forward,
        Backward
    };

  public:
    struct Config
    {
      IDistanceSensor *distanceSensor;                        ///< The distance sensor to use for distance measurements
      ITrafficLight   *trafficLight;                          ///< The traffic light component to use for signaling
      uint32_t        maxDistanceThresholdMm;                 ///< The maximum distance threshold in millimiters.
                                                              ///< If the measured distance is greater than this value
                                                              ///< then the suject is considered out of range
      uint32_t        farThresholdMm;                         ///< The "far" distance threshold in millimiters.
                                                              ///< If the measured distance is greater than this value
                                                              ///< but lower than the maximum distance then the suject
                                                              ///< is considered to be in the "far" range
      uint32_t        nearThresholdMm;                        ///< The "near" distance threshold in millimiters.
                                                              ///< If the measured distance is greater than this value
                                                              ///< but lower than the "far" distance then the suject
                                                              ///< is considered to be in the "medium" range
                                                              ///< If the measured distance is lower than this value
                                                              ///< then the suject is considered to be in the "short" range
      uint32_t        movingDistanceDetectionThresholdMm;     ///< The minimum amount of distance delta to be be considered
                                                              ///< as a valid movement
      uint32_t        movingTimeThresholdMs;                  ///< The minimum amount of time delta to be be considered
                                                              ///< as a valid movement
      uint32_t        holdingTimeThresholdMs;                 ///< The minimum amount of time that the subject needs to be
                                                              ///< in the same position to detect that the move stopped
    };

  public:
    /// @brief Constructor.
    StateMachine();

    /// @brief Destructor.
    ~StateMachine();

  public:
    /// @brief Initialize the state machine
    ///
    /// @param configuration      The configuration data.
    ///
    void Init(const Config& configuration);

    /// @brief Update the state machine state.
    ///
    /// @note This method must be called periodically
    /// in the main app loop
    ///
    void Update();

  private:
    /// @brief Gets the moving direction based on the time and distance
    /// differences from the previous values
    ///
    /// @param deltaT   The time difference in milliseconds
    /// @param deltaD   The distance difference in millimeters
    ///
    /// @retval Returns the direction in which the subject is moving
    ///
    MovingDirection GetMovingDirection(uint32_t deltaT, int32_t  deltaD);

    /// @brief Measures the distance.
    ///
    /// @retval returns the measured distance in millimeters.
    ///
    uint32_t MeasureDistance();

    /// @brief Sets the traffic lights based on the measured distance.
    ///
    /// @param distance The distance in millimeters
    ///
    void SetTrafficLights(uint32_t distance);

    /// @brief Sets Off all traffic lights
    ///
    void SetAllLightsOff();

    /// @brief Tests the traffic lights
    ///
    Result TestLights();

  private:
    static const char *ToString(MovingDirection movingDirection);
    static const char *ToString(State state);

  private:
    bool            _initDone;                            ///< A flag to indicate whether the state machine was initialized
    State           _state;                               ///< The current state
    State           _previousState;                       ///< The previous state
    IDistanceSensor *_distanceSensor;                     ///< The distance sensor to use for distance measurements
    ITrafficLight   *_trafficLight;                       ///< The traffic light component to use for signaling
    uint32_t        _previousDistance;                    ///< The previous distance measured in millimiters
    uint32_t        _previousTime;                        ///< The previous time measured in milliseconds
    uint32_t        _maxDistanceThresholdMm;              ///< The maximum distance threshold in millimiters.
    uint32_t        _farThresholdMm;                      ///< The "far" distance threshold in millimiters.
    uint32_t        _nearThresholdMm;                     ///< The "near" distance threshold in millimiters.
    uint32_t        _movingDistanceDetectionThresholdMm;  ///< The minimum amount of distance delta to be be considered
                                                          ///< as a valid movement
    uint32_t        _movingTimeThresholdMs;               ///< The minimum amount of time delta to be be considered
                                                          ///< as a valid movement
    uint32_t        _holdingTimeThresholdMs;              ///< The minimum amount of time that the subject needs to be
                                                          ///< in the same position to detect that the move stopped

  };
}
#endif // _STATEMACHINE_H_
