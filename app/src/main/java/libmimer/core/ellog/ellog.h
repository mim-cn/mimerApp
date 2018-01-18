#ifndef ELLOG_H
#define ELLOG_H
#include <iostream>
#include <string>
#include "platform.h"
#include "../Stream.h"
#include "easylogging++.h"

namespace mim {

class UTIL_API ellog
{
    // Formatting codes
    #define RESET       "\033[00m"
    #define BOLD        "\033[1m"
    #define DARK        "\033[2m"
    #define UNDERLINE   "\033[4m"
    #define BLINK       "\033[5m"
    #define REVERSE     "\033[7m"
    #define CONCEALED   "\033[8m"
    // Foreground colors
    #define GREY        "\033[30m"
    #define RED         "\033[31m"
    #define GREEN       "\033[32m"
    #define YELLOW      "\033[33m"
    #define BLUE        "\033[34m"
    #define MAGENTA     "\033[35m"
    #define CYAN        "\033[36m"
    #define WHITE       "\033[37m"
    // Background colors
    #define ON_GREY     "\033[40m"
    #define ON_RED      "\033[41m"
    #define ON_GREEN    "\033[42m"
    #define ON_YELLOW   "\033[43m"
    #define ON_BLUE     "\033[44m"
    #define ON_MAGENTA  "\033[45m"
    #define ON_CYAN     "\033[46m"
    #define ON_WHITE    "\033[47m"
public:
    ellog(const std::string& id = "default", const std::string& filepath = ".", const std::string& confile = "") : _confile(confile), _id(id)
    {
        el::Helpers::setStorage(mim::ellog::shared());
        _logger = el::Loggers::getLogger(id, true);
        if (!_confile.empty())
        {
            _conf = el::Configurations(_confile);
            el::Loggers::reconfigureAllLoggers(_conf);
        }
        else
        {
            el::Configurations idConf;
            defaultformat(idConf);
            if( "default" != id)
            {
#ifdef OS_MSWIN
                std::string path = filepath + "\\" + id + ".log";
                const char* defaultfile = "logs\\myeasylog.log";
#else
                std::string path = filepath + "/" + id + ".log";
                const char* defaultfile = "logs/myeasylog.log";
#endif
                idConf.setGlobally(el::ConfigurationType::Filename, path);
                idConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
				el::Loggers::reconfigureLogger(id, idConf);
				//_logger->configure(idConf);
            }
        }
        // 设置一个日志文件最大字节数 MAX_LOG_FILE_SIZE 10M
        logroll("10485760");
    }
    virtual ~ellog() {}
public:
    inline operator const char*() { return (this->_id).c_str(); }
public:
    static el::base::type::StoragePointer shared();
    static void rolloutHandler(const char* filename, std::size_t size);
    static void crashHandler(int sig);
    static inline int_64 timestamp() { return el::base::utils::DateTime::timestamp(); }
    static std::string datatime(const char* format = el::base::consts::kDefaultDateTimeFormat) {
        el::base::MillisecondsWidth msw = el::base::MillisecondsWidth();
        return el::base::utils::DateTime::getDateTime(format, &msw);
    }
public:
    void defaultformat(el::Configurations& idConf);
    void logroll(const std::string& value);
    bool log(el::Level lev, const std::string& msg);
    bool config(const std::string& confile);
public:
#   define LOGGER_LEVEL_WRITERS_SIGNATURES(FUNCTION_NAME)\
    template <typename T, typename... Args>\
    inline void FUNCTION_NAME(const char * s, const T& t, const Args& ... args) \
    { _logger->FUNCTION_NAME(s, t, args...); } \
    template <typename T>\
    inline void FUNCTION_NAME(const T& t){ _logger->FUNCTION_NAME(t); }

	template <typename T, typename... Args>
	inline void verbose(int i, const char* s, const T& t, const Args&... args) \
	{ _logger->verbose(i, s, t, args...); }

	template <typename T>
	inline void verbose(int i, const T& t) \
	{ _logger->verbose(i, t); }

	LOGGER_LEVEL_WRITERS_SIGNATURES(info)
	LOGGER_LEVEL_WRITERS_SIGNATURES(debug)
	LOGGER_LEVEL_WRITERS_SIGNATURES(warn)
	LOGGER_LEVEL_WRITERS_SIGNATURES(error)
	LOGGER_LEVEL_WRITERS_SIGNATURES(fatal)
	LOGGER_LEVEL_WRITERS_SIGNATURES(trace)
#   undef LOGGER_LEVEL_WRITERS_SIGNATURES

#define clog(LEVEL, OBJ)    CLOG(LEVEL, OBJ)
#define clog_if(condition,LEVEL,OBJ) CLOG_IF(condition, LEVEL, OBJ)
#define LOGGER_COLOR(COLOR, FORMAT) COLOR FORMAT RESET

private:
    el::Configurations  _conf;
    std::string         _confile;
    std::string         _id;
    el::Logger*         _logger;
};

}

#endif // ELLOG_H
