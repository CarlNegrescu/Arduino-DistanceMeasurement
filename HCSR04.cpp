///
/// @file HCSR04.h
///
/// @brief HCSR04 class implementation
///
/// @author Carl Negrescu
/// @date May 29, 2025
///

#include "HCSR04.h"

namespace CNEGR
{
  const uint32_t       minTriggerPulseDurationUs  = 10;                          ///< The minimum trigger pulse duration in microseconds
  const uint32_t       minDistanceMm              = 20;                          ///< The minimum distance the sensor can detect in millimeters
  const uint32_t       maxDistanceMm              = 4000;                        ///< The maximum distance the sensor can detect in millimeters
  const SignalPolarity triggerPolarity            = SignalPolarity::ActiveHigh;  ///< The trigger signal polarity
  const SignalPolarity echoPolarity               = SignalPolarity::ActiveHigh;  ///< The echo signal polarity
  const uint32_t       burstSignalFrequencyHz     = 40000;                       ///< The burst signal frequency in Hz
  const uint32_t       burstSignalLength          = 8;                           ///< The length of the burst signal in number of pulses

  /// The duration  of the burst signal in microseconds
  const uint32_t       burstSignalDurationUs      = (burstSignalLength * 1000 * 1000) / burstSignalFrequencyHz;

  /// @brief Constructor.
  HCSR04::HCSR04(): DistanceSensor(minTriggerPulseDurationUs, minDistanceMm, maxDistanceMm, triggerPolarity, echoPolarity)
  {
  }
}
