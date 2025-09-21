///
/// @file DebugUtils.h
///
/// @brief Definition of debug utility functions and types
///
/// @author Carl Negrescu
/// @date May 20, 2025
///
///
#include <Arduino.h>
#include "DebugUtils.h"

/// @brief Resets the board
void Reset()
{
  asm volatile ("jmp 0");
}

const uint32_t LOG_BUFFER_SIZE = 128;

/// @brief Constructor
///
Logger::Logger()
  :_level(INFO),
   _logOutputProc(&DefaultLogOutputProc)
{
}


/// @brief Gets the logger object instance
///
/// @retval The logger instance
///
Logger& Logger::Instance()
{
    static Logger logger;
    return logger;
}

/// @brief Sets the log output level
///
/// @param level The log output level
///
void Logger::SetLogLevel(Level level)
{
  Instance()._level = level;
}

/// @brief Gets the log output level
///
/// @retval The log output level
///
Logger::Level Logger::GetLogLevel()
{
  return Instance()._level;
}

/// @brief Outputs a formatted log message with the DEBUG log level
///
/// @param format Pointer to the format string
///
void Logger::Debug(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::DEBUG, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the DEBUG log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param format Pointer to the format string located in the program
/// space (flash memory)
///
void Logger::Debug(const __FlashStringHelper* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::DEBUG, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the INFO log level
///
/// @param format Pointer to the format string
///
void Logger::Info(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::INFO, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the INFO log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param format Pointer to the format string located in the program
/// space (flash memory)
///
void Logger::Info(const __FlashStringHelper* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::INFO, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the WARNING log level
///
/// @param format Pointer to the format string
///
void Logger::Warning(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::WARNING, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the WARNING log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param format Pointer to the format string located in the program
/// space (flash memory)
///
void Logger::Warning(const __FlashStringHelper* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::WARNING, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the ERROR log level
///
/// @param format Pointer to the format string
///
void Logger::Error(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::ERROR, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the ERROR log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param format Pointer to the format string located in the program
/// space (flash memory)
///
void Logger::Error(const __FlashStringHelper* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::ERROR, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the FATAL log level
///
/// @param format Pointer to the format string
///
void Logger::Fatal(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::FATAL, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message with the FATAL log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param format Pointer to the format string located in the program
/// space (flash memory)
///
void Logger::Fatal(const __FlashStringHelper* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(Level::FATAL, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message using the specified log level
///
/// @param level  Enum indicating the logging level of this message
/// @param format Pointer to the format string
///
void Logger::Log(Level level, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  Log(level, format, args);
  va_end(args);
}

/// @brief Outputs a formatted log message using the specified log level
///
/// @param level  Enum indicating the logging level of this message
/// @param format Pointer to the format string
/// @param args   Arguments for format specification
///
void Logger::Log(Level level, const char *format, va_list args)
{
  if (level >= GetLogLevel())
  {
    char logBuffer[LOG_BUFFER_SIZE];
    // logBuffer[0] = '[';

    // String s(ToString(level));
    // s.toCharArray(&logBuffer[1], sizeof(logBuffer)-1);
    // int len = strnlen(&logBuffer[0], sizeof(logBuffer));
    // if (len < sizeof(logBuffer)-3)
    // {
    //   logBuffer[len] = ']';
    //   logBuffer[len+1] = ' ';
    //   len += 2;
    // }

    uint32_t len = ToCharArray(level, "[", "] ", logBuffer, sizeof(logBuffer));

    vsnprintf(&logBuffer[len], (sizeof(logBuffer) - len), format, args);
    Instance()._logOutputProc(logBuffer);
  }
}

/// @brief Outputs a formatted log message using the specified log level
///
/// @note For this method the format string is located in the program
/// space (flash memory)
///
/// @param level  Enum indicating the logging level of this message
/// @param format Pointer to the format string located in the program
/// space (flash memory)
/// @param args   Arguments for format specification
///
void Logger::Log(Level level, const __FlashStringHelper *format, va_list args)
{
  if (level >= GetLogLevel())
  {
    char logBuffer[LOG_BUFFER_SIZE];

    uint32_t len = ToCharArray(level, "[", "] ", logBuffer, sizeof(logBuffer));
    // logBuffer[0] = '[';
    // uint32_t len = ToCharArray(level, &logBuffer[1], sizeof(logBuffer)-1);
    // // String s(ToString(level));
    // // s.toCharArray(&logBuffer[1], sizeof(logBuffer)-1);
    // // int len = strnlen(&logBuffer[0], sizeof(logBuffer));
    // if (len < sizeof(logBuffer)-3)
    // {
    //   logBuffer[len] = ']';
    //   logBuffer[len+1] = ' ';
    //   len += 2;
    // }

    vsnprintf_P(&logBuffer[len], (sizeof(logBuffer) - len), reinterpret_cast<const char *>(format), args);
    Instance()._logOutputProc(logBuffer);
  }
}

/// @brief Sets the log output function
///
/// @param logOutputProc Pointer to output function
///
void Logger::SetOutputFunction(LogOutputProc logOutputProc)
{
  if (logOutputProc == nullptr)
  {
    Instance()._logOutputProc = &DefaultLogOutputProc;
  }
  else
  {
    Instance()._logOutputProc = logOutputProc;
  }
}

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
const __FlashStringHelper* Logger::ToString(Level level)
{
  static const char LEVEL_DEBUG[] PROGMEM   = "DEBUG";
  static const char LEVEL_INFO[] PROGMEM    = "INFO";
  static const char LEVEL_WARNING[] PROGMEM = "WARNING";
  static const char LEVEL_ERROR[] PROGMEM   = "ERROR";
  static const char LEVEL_FATAL[] PROGMEM   = "FATAL";
  static const char LEVEL_OFF[] PROGMEM     = "OFF";
  static const char LEVEL_INVALID[] PROGMEM = "INVALID";

  const char* const LOG_LEVEL_STRINGS[] PROGMEM =
  {
      LEVEL_DEBUG,
      LEVEL_INFO,
      LEVEL_WARNING,
      LEVEL_ERROR,
      LEVEL_FATAL,
      LEVEL_OFF,
      LEVEL_INVALID
  };

  if ((level >= Level::DEBUG) && (level <= Level::OFF))
    return reinterpret_cast<const __FlashStringHelper *>(LOG_LEVEL_STRINGS[level]);
  else
    return reinterpret_cast<const __FlashStringHelper *>(LOG_LEVEL_STRINGS[Level::OFF+1]);
}

/// @brief Converts the log output level to a string and copies it
/// into the buffer
///
/// @param level        The logging level to convert
/// @param buffer       Pointer to the output buffer
/// @param bufferSize   The output buffer size
///
/// @retval The length of the resulting string
///
uint32_t Logger::ToCharArray(Level level, char *buffer, uint32_t bufferSize)
{
  const __FlashStringHelper* s = ToString(level);
  uint32_t length = strlen_P((PGM_P)s);

  if ((buffer == nullptr) || (bufferSize < length))
    return 0;

  strncpy_P(buffer, (PGM_P)s, bufferSize-1);

  return length;
}


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
uint32_t Logger::ToCharArray(Level level, const char *prefix, const char *suffix, char *buffer, uint32_t bufferSize)
{
  const __FlashStringHelper* s = ToString(level);
  uint32_t length = strlen_P((PGM_P)s);

  uint32_t prefixLength = prefix != nullptr ? strlen(prefix) : 0;
  uint32_t suffixLength = suffix != nullptr ? strlen(suffix) : 0;
  uint32_t totalLength = prefixLength + length + suffixLength;

  if ((buffer == nullptr) || (bufferSize < totalLength))
    return 0;

  buffer[0] = '\0';

  if (prefixLength != 0)
    strcpy(buffer, prefix);

  strcat_P(buffer, (PGM_P)s);

  if (suffixLength != 0)
    strcat(buffer, suffix);

  return totalLength;
}

/// @brief Flushes the output stream
///
void Logger::Flush()
{
  Serial.flush();
}

/// @brief The default ouput function
///
/// @param message The message to be printed
///
void Logger::DefaultLogOutputProc(const char *message)
{
  Serial.println(message);
}

/// @brief Prints the assert diagnostic informations and then aborts program execution
///
/// @param func     The function name from where the assert was invoked
/// @param file     The file name from where the assert was invoked
/// @param lineno   The line number in the file from where the assert was invoked
/// @param exp      The expression which caused the assert to fail
///
void __assert(const char *func, const char *file, int lineno, const char *exp)
{
  // Remove the file path and keep only the file name
  const char *fname = strrchr(file, '\\');
  fname = fname != nullptr ? fname + 1 : file;

  // Send the diagnostic informations through the logger interface
  Logger::Fatal(F("*******************************************************************"));
  Logger::Fatal(F("%s (%d): %s() => assert(%s) failed!"), fname, lineno, func, exp);
  Logger::Fatal(F("*******************************************************************"));
  Logger::Flush();

  // Abort program execution
  abort();
}
