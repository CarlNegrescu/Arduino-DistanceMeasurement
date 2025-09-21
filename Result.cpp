///
/// @file Result.h
///
/// @brief Definition of standard function return value
///
/// @author Carl Negrescu
/// @date May 20, 2025
///

#include "Result.h"

const char *ResultToStr(Result r)
{
     /// <The following array of strings must be kept in the same order as
     /// <the corresponding values in the Result enum.
     ///
     static const char * const resName[] = { "RESULT_OK",
                                             "RESULT_ERROR",
                                             "RESULT_NOT_SUP",
                                             "RESULT_NOT_IMPL",
                                             "RESULT_NOT_READY",
                                             "RESULT_NOT_EXECUTED",
                                             "RESULT_NOT_ALLOWED",
                                             "RESULT_BAD_PARAM",
                                             "RESULT_PARSE_ERROR",
                                             "RESULT_PERM_DENIED",
                                             "RESULT_EXISTS",
                                             "RESULT_BUSY",
                                             "RESULT_DEV_ERR",
                                             "RESULT_TIMEOUT",
                                             "RESULT_ABORTED",
                                             "RESULT_NO_DATA",
                                             "RESULT_NO_MEM",
                                             "RESULT_NO_RESOURCE",
                                             "RESULT_HW_FAILURE",
                                             "RESULT_HW_DISABLED",
                                             "RESULT_OVERFLOW",
                                             "RESULT_UNDERFLOW",
                                             "RESULT_CRC_ERROR",
                                             "RESULT_NOT_VALID"
                                            };

    /// Should never get a value out of range, but just in case it is better to
    /// check and return a sensible value.
    ///
    int rr = static_cast<int>(r);
    if ((rr < RESULT_OK) || (rr >= RESULT_MAX_VAL))
    {
        return "Unknown Error Code";
    }
    else
    {
        return resName[rr];
    }
}
