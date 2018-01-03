
#ifndef _LOGGING_HPP
#define  _LOGGING_HPP

#include "LogStream.hpp"

enum class LogLevel{TRACE = 0, DEBUG, INFO, WRN,ERR,FATAL};

class LoggerImpl;

class Logger
{
public:
Logger(const char* fileName, int lineNum, const char* func, LogLevel level);
~Logger();

LogStream& stream();

Logger(const Logger&) = delete;
Logger& operator=(const Logger&) = delete;

private:
LoggerImpl* impl_;
};

namespace log
{
extern LogLevel g_logLevel;
static inline LogLevel logLevel(){return g_logLevel;}
}

#define LOG_TRACE if(log::logLevel() <=  LogLevel::TRACE) Logger(__FILE__,__LINE__, __func__, LogLevel::TRACE).stream()
#define LOG_DBG    if(log::logLevel() <=  LogLevel::DEBUG) Logger(__FILE__,__LINE__, __func__, LogLevel::DEBUG).stream()
#define LOG_INFO   if(log::logLevel() <=  LogLevel::INFO) Logger(__FILE__,__LINE__, __func__, LogLevel::INFO).stream()
#define LOG_WRN   if(log::logLevel() <=  LogLevel::WRN) Logger(__FILE__,__LINE__, __func__, LogLevel::WRN).stream()
#define LOG_ERROR if(log::logLevel() <=  LogLevel::ERR) Logger(__FILE__,__LINE__, __func__, LogLevel::ERR).stream()
#define LOG_FATAL if(log::logLevel() <=  LogLevel::FATAL) Logger(__FILE__,__LINE__, __func__, LogLevel::FATAL).stream()


#endif

