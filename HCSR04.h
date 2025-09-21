///
/// @file HCSR04.h
///
/// @brief HCSR04 ultrasonic sensor class definition
///
/// @author Carl Negrescu
/// @date May 20, 2025
///
#pragma once

#if !defined(_HCSR04_H_)
#define _HCSR04_H_

#include "DistanceSensor.h"

namespace CNEGR
{
  /// @brief HCSR04 class definition
  ///
  class HCSR04: public DistanceSensor
  {
  public:
    /// @brief Constructor.
    HCSR04();
  };
}
#endif // _HCSR04_H_
