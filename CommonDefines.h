///
/// @file CommonDefines.h
///
/// @brief Common definitions
///
/// @author Carl Negrescu
/// @date May 19, 2025
///
///
#pragma once

#if !defined(_COMMONDEFINES_H_)
#define _COMMONDEFINES_H_

namespace CNEGR
{
  #define MAX_COMPONENT_NAME_LENGTH 32

  enum SignalPolarity
  {
    ActiveHigh,
    ActiveLow,
  };
}
#endif // _COMMONDEFINES_H_
