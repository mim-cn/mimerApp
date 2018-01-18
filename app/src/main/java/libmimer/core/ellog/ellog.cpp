#include "ellog.h"
INITIALIZE_EASYLOGGINGPP

namespace mim {

el::base::type::StoragePointer ellog::shared()
{
	   return el::Helpers::storage();
}

bool ellog::config(const std::string& confile)
{
    if (_confile.empty())
    {
        _confile = confile;
        _conf = el::Configurations(_confile);
        el::Loggers::reconfigureAllLoggers(_conf);
        return true;
    }
    return false;
}

void ellog::defaultformat(el::Configurations& idConf)
{
    idConf.set(el::Level::Trace,   el::ConfigurationType::Format, LOGGER_COLOR(BOLD,   "[ %level | %datetime ] > %msg"));
    idConf.set(el::Level::Debug,   el::ConfigurationType::Format, "[ %level | %datetime ] : %msg");
    idConf.set(el::Level::Error,   el::ConfigurationType::Format, LOGGER_COLOR(RED,    "[ %level | %datetime ] ! %msg"));
    idConf.set(el::Level::Warning, el::ConfigurationType::Format, LOGGER_COLOR(YELLOW, "[ %level | %datetime ] ? %msg"));
    idConf.set(el::Level::Info,    el::ConfigurationType::Format, LOGGER_COLOR(GREEN,  "[ %level | %datetime ] $ %msg"));
}

void ellog::logroll(const std::string& value)
{
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    // fix ctrl+c crash bug:  https://github.com/muflihun/easyloggingpp/issues/186
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    el::Loggers::setLoggingLevel(el::Level::Global);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Helpers::setCrashHandler(crashHandler);
    el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::MaxLogFileSize, value);
    el::Helpers::installPreRollOutCallback(rolloutHandler);
}

void ellog::rolloutHandler(const char* filename, std::size_t size)
{
    time_t cuurenttime = time(NULL);
    struct::tm*  oneMinuteAgo = localtime(&cuurenttime);
    std::string filenameTemp = filename;
    int pos = filenameTemp.rfind('.');
    filenameTemp = filenameTemp.substr(0, pos);
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
    char backupFile[MAX_PATH] = { 0 };
    snprintf(backupFile, MAX_PATH, "%s_%04d%02d%02d%02d%02d%02d.log", filenameTemp.c_str(),  oneMinuteAgo->tm_year + 1900
		,oneMinuteAgo->tm_mon + 1, oneMinuteAgo->tm_mday, oneMinuteAgo->tm_hour, oneMinuteAgo->tm_min, oneMinuteAgo->tm_sec);
    if(mimer::Stream::Rename(filename, backupFile))
    {
        printf("rename %s to %s ok\n", filename, backupFile);
	}
	else {
        printf("rename %s to %s failed\n", filename, backupFile);
        snprintf(backupFile, MAX_PATH, "%s_%04d%02d%02d%02d%02d%02d.log", filenameTemp.c_str(), oneMinuteAgo->tm_year + 1900
        , oneMinuteAgo->tm_mon + 1, oneMinuteAgo->tm_mday, oneMinuteAgo->tm_hour, oneMinuteAgo->tm_min, oneMinuteAgo->tm_sec + 3);
        if (mimer::Stream::Rename(filename, backupFile))
        {
            printf("again rename %s to %s ok\n", filename, backupFile);
        }
	}
}

void ellog::crashHandler(int sig)
{
    el::Helpers::logCrashReason(sig, true);
    el::Loggers::flushAll();
    el::Helpers::crashAbort(sig); // FOLLOWING LINE IS ABSOLUTELY NEEDED AT THE END IN ORDER TO ABORT APPLICATION
    el::Loggers::flushAll();
}

bool ellog::log(el::Level lev, const std::string& msg)
{
    switch (lev) {
//    case el::Level::Global:
//        LOG(GLOBAL) << msg;
//        break;
    case el::Level::Trace:
        LOG(TRACE) << msg;
        break;
    case el::Level::Debug:
        LOG(DEBUG) << msg;
        break;
    case el::Level::Fatal:
        LOG(FATAL) << msg;
        break;
    case el::Level::Error:
        LOG(ERROR) << msg;
        break;
    case el::Level::Warning:
        LOG(WARNING) << msg;
        break;
//    case el::Level::Verbose:
//        LOG(VERBOSE) << msg;
//        break;
    case el::Level::Info:
        LOG(INFO) << msg;
        break;
    default:
        return false;
        break;
    }
    return true;
}

}
