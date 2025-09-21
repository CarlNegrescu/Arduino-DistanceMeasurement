///
/// @file Result.h
///
/// @brief Definition of standard function return value
///
/// @author Carl Negrescu
/// @date May 20, 2025
///
#pragma once

#if !defined(_RESULT_H_)
#define _RESULT_H_

#ifdef __cplusplus
extern "C"
{
#endif
  /// @brief Enum of the standard function return value
  ///
  ///  Generally if a function can fail then it should return one of these values
  ///  to indicate success or failure of the operation. Note that RESULT_OK is
  ///  defined as zero, so any non-zero value indicates an error.
  ///
  ///  Result names should be generic so that they can be reused across many
  ///  diverse classes. When choosing which value to return for a given function,
  ///  the idea is that the Result name should distinguish between the possible
  ///  return values and be reasonably mnemonic, but need not describe the error in
  ///  detail. The details about exactly what it means when a particular Result
  ///  name is returned by a given function should be specified in the comments
  ///  above the function's declaration.
  ///
  typedef enum
  {
      // Note the values are assigned here to make it quick and easy to lookup a result code from a number when debugging.
      RESULT_OK           = 0,  ///< Operation was successful.
      RESULT_ERROR        = 1,  ///< Operation was not successful. The specific reason for failure is unknown.

      RESULT_NOT_SUP      = 2,  ///< The requested operation is not supported.
      RESULT_NOT_IMPL     = 3,  ///< Requested function is not yet implemented.
      RESULT_NOT_READY    = 4,  ///< Not performed because client not ready. Possible temporary failure, try again later.
      RESULT_NOT_EXECUTED = 5,  ///< The function for which this is the result has not been executed.
      RESULT_NOT_ALLOWED  = 6,  ///< The request could not be performed. Possible temporary failure, try again later.

      RESULT_BAD_PARAM    = 7,  ///< A function parameter failed validation.
      RESULT_PARSE_ERROR  = 8,  ///< Request not performed because data could not be transformed to the required type.
      RESULT_PERM_DENIED  = 9,  ///< The request could not be performed due to a lack of permission.
      RESULT_EXISTS       = 10, ///< The requested operation was not performed because the output already exists.

      RESULT_BUSY         = 11,  ///< Operation not performed because one or more required resources are in use.
      RESULT_DEV_ERR      = 12,  ///< The specified device did not respond, or is in some kind of error state.
      RESULT_TIMEOUT      = 13,  ///< The requested operation did not complete within the timeout period.
      RESULT_ABORTED      = 14,  ///< The operation was aborted.

      RESULT_NO_DATA      = 15,  ///< Requested data was not available, or could not be sent or received for some reason.
      RESULT_NO_MEM       = 16,  ///< Requested operation not performed because there was not enough memory available.
      RESULT_NO_RESOURCE  = 17,  ///< Operation not performed because one or more required resources are not available.

      RESULT_HW_FAILURE   = 18,  ///< Faulty hardware has prevented the operation from completing.
      RESULT_HW_DISABLED  = 19,  ///< The request could not be performed due to HW being disabled. Possible temporary failure, try again later.
      RESULT_OVERFLOW     = 20,  ///< An overflow has occurred, or would occur.
      RESULT_UNDERFLOW    = 21,  ///< An underflow has occurred, or would occur.

      RESULT_CRC_ERROR    = 22,  ///< A CRC check failed.
      RESULT_NOT_VALID    = 23,  ///< A validation check failed.

      ///
      /// !!!!!!!!!!! New result values shall be added HERE !!!!!!!!!!!
      /// !!!!!!!!!!! and in ResultToStr() in Result.cpp    !!!!!!!!!!!
      ///

      /// The RESULT_MAX_VAL value is a marker for the end of the enum and should always be here
      /// It is used to ensure that ResultToStr() it is in step with the contents of this enum
      /// and should *not* be used as a return value.
      ///
      RESULT_MAX_VAL
  } Result;

  ///  @brief Convert Result value to string
  ///
  ///  A global function to convert a Result value to a string.
  ///  The text returned is just the enum name itself eg "RESULT_OK".
  ///
  const char *ResultToStr(Result r);

#ifdef __cplusplus
}
#endif

#endif
