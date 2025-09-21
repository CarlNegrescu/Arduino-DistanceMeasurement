///
/// @file DebugUtils.h
///
/// @brief Definition of debug utility functions and types
///
/// @author Carl Negrescu
/// @date May 20, 2025
///
///
#pragma once

#if !defined(_DEBUGUTILS_H_)
#define _DEBUGUTILS_H_

#define __ASSERT_USE_STDERR
#include <assert.h>
#include <WString.h>
#include <stdarg.h>

/// @brief Resets the board
void Reset();

class Logger
{
  public:
    enum Level
    {
      DEBUG = 0,
      INFO,
      WARNING,
      ERROR,
      FATAL,
      OFF
    };

    typedef void (*LogOutputProc)(const char *message);

  public:
    /// @brief Sets the log output level
    ///
    /// @param level The log output level
    ///
    static void SetLogLevel(Level level);

    /// @brief Gets the log output level
    ///
    /// @retval The log output level
    ///
    static Level GetLogLevel();

    /// @brief Outputs a formatted log message with the DEBUG log level
    ///
    /// @param format Pointer to the format string
    ///
    static void Debug(const char* format, ...);

    /// @brief Outputs a formatted log message with the DEBUG log level
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Debug(const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message with the INFO log level
    ///
    /// @param format Pointer to the format string
    ///
    static void Info(const char* format, ...);

    /// @brief Outputs a formatted log message with the INFO log level
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Info(const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message with the WARNING log level
    ///
    /// @param format Pointer to the format string
    ///
    static void Warning(const char* format, ...);

    /// @brief Outputs a formatted log message with the WARNING log level
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Warning(const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message with the ERROR log level
    ///
    /// @param format Pointer to the format string
    ///
    static void Error(const char* format, ...);

    /// @brief Outputs a formatted log message with the ERROR log level
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Error(const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message with the FATAL log level
    ///
    /// @param format Pointer to the format string
    ///
    static void Fatal(const char* format, ...);

    /// @brief Outputs a formatted log message with the FATAL log level
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Fatal(const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message
    ///
    /// @param level  Enum indicating the logging level of this message
    /// @param format Pointer to the format string
    ///
    static void Log(Level level, const char* format, ...);

    /// @brief Outputs a formatted log message
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param level  Enum indicating the logging level of this message
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    ///
    static void Log(Level level, const __FlashStringHelper* format, ...);

    /// @brief Outputs a formatted log message
    ///
    /// @param level  Enum indicating the logging level of this message
    /// @param format Pointer to the format string
    /// @param args   Arguments for format specification
    ///
    static void Log(Level level, const char *format, va_list args);

    /// @brief Outputs a formatted log message
    ///
    /// @note For this method the format string is located in the program
    /// space (flash memory)
    ///
    /// @param level  Enum indicating the logging level of this message
    /// @param format Pointer to the format string located in the program
    /// space (flash memory)
    /// @param args   Arguments for format specification
    ///
    static void Log(Level level, const __FlashStringHelper *format, va_list args);

    /// @brief Sets the log output function
    ///
    /// @param logOutputProc Pointer to output function
    ///
    static void SetOutputFunction(LogOutputProc logOutputProc);

    /// @brief Flushes the output stream
    ///
    static void Flush();

  private:
    /// @brief Constructor
    ///
    Logger();

    // Hiding the copy constructor
    Logger(const Logger&);
    // And the copy operator
    void operator = (const Logger&);

    /// @brief Gets the logger object instance
    ///
    /// @retval The logger instance
    ///
    static Logger& Instance();

    /// @brief Converts the log output level to a string
    ///
    /// @note This method returns a const string is located in
    /// the program space (flash memory)
    ///
    /// @param level  The logging level to convert to string
    ///
    /// @retval The log output level as a const string located
    /// in the program space (flash memory)
    ///
    static const __FlashStringHelper *ToString(Level level);

    /// @brief Converts the log output level to a string and copies it
    /// into the buffer
    ///
    /// @param level        The logging level to convert
    /// @param buffer       Pointer to the output buffer
    /// @param bufferSize   The output buffer size
    ///
    /// @retval The length of the resulting string
    ///
    static uint32_t ToCharArray(Level level, char *buffer, uint32_t bufferSize);

    /// @brief Converts the log output level to a string and copies it
    /// into the buffer. The log level string will be bracketed with the
    ///  prefix and suffix strings if these are not null
    ///
    /// @param level        The logging level to convert
    /// @param prefix       The prefix string
    /// @param suffix       The suffix string
    /// @param buffer       Pointer to the output buffer
    /// @param bufferSize   The output buffer size
    ///
    /// @retval The length of the resulting string
    ///
    static uint32_t ToCharArray(Level level, const char *prefix, const char *suffix, char *buffer, uint32_t bufferSize);

  private:
    /// @brief The default ouput function
    ///
    /// @param message The message to be printed
    ///
    static void DefaultLogOutputProc(const char *message);

  private:
    Level         _level;
    LogOutputProc _logOutputProc;
};

#endif
