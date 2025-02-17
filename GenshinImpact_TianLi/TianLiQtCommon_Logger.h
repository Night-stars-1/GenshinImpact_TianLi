#pragma once
#include <QWidget>
#include "ui_TianLiQtCommon_Logger.h"
#include "Logger/TianLi.Logger/TianLi.Logger.h"
#pragma comment(lib, "TianLi.Logger.lib")
class TianLiQtCommon_Logger : public QWidget
{
    Q_OBJECT
private:
    TianLiQtCommon_Logger(QWidget* parent = Q_NULLPTR);
public:
    static TianLiQtCommon_Logger& GetInstance();
    TianLiQtCommon_Logger(const TianLiQtCommon_Logger&) = delete;
    TianLiQtCommon_Logger(TianLiQtCommon_Logger&&) = delete;
    ~TianLiQtCommon_Logger();
private:
    Ui::TianLiQtCommon_Logger ui;
private:
    void append(QString time, QString type, QString func, QString msg);
    void log(QString type, QString func, QString msg);
public:
    void Trace(QString func, QString msg);
    void Debug(QString func, QString msg);
    void Info(QString func, QString msg);
    void Warn(QString func, QString msg);
    void Error(QString func, QString msg);
    //template <typename... Args>
        //inline void Trace(Args&&... args)
        //{
        //    std::string_view level = "Trace";
        //    //log(level, std::forward<Args>(args)...);
        //}

        //template <typename... Args>
        //inline void debug([[maybe_unused]] Args&&... args)
        //{
        //    std::string_view level = "Debug";
        //    //log(level, std::forward<Args>(args)...);
        //}
    //	
    //    template <typename... Args>
    //    void log(std::string_view level, Args&&... args)
    //    {
    //        std::unique_lock<std::mutex> trace_lock(m_trace_mutex);
    //
    //        constexpr int buff_len = 128;
    //        char buff[buff_len] = { 0 };
    //
    //
    //        if (!m_log_list.isEmpty()) 
    //        {
    //            m_log_list.clear();
    //        }
    //        stream_args(m_log_list, buff, std::forward<Args>(args)...);
    //
    //    }
    //    template <bool ToGbk = false, typename T, typename... Args>
    //    inline void stream_args(QStringList& os, T&& first, Args&&... rest)
    //    {
    //        stream<ToGbk, T>()(os, std::forward<T>(first));
    //        stream_args<ToGbk>(os, std::forward<Args>(rest)...);
    //    }
    //    template <bool>
    //    inline void stream_args(QStringList& os)
    //    {
    //        os << "\n";
    //    }
    //    template <bool ToGbk, typename T, typename = void>
    //    struct stream
    //    {
    //        inline void operator()(QStringList& os, T&& first)
    //        {
    //            os << first << " ";
    //        }
    //    };
    //    template <typename T>
    //    struct stream<true, T, typename std::enable_if<std::is_constructible<std::string, T>::value>::type>
    //    {
    //        inline void operator()(QStringList& os, T&& first)
    //        {
    //            os << first << " ";
    //        }
    //    };
    //private:
    //	QStringList m_log_list;
    //    std::mutex m_trace_mutex;
};
class LoggerAux
{
public:
    LoggerAux(const std::string& func_name)
        : m_func_name(func_name),
        m_start_time(std::chrono::steady_clock::now())
    {
        TianLiQtCommon_Logger::GetInstance().Trace(m_func_name.c_str(), "enter function");
    }
    ~LoggerAux()
    {
        auto duration = std::chrono::steady_clock::now() - m_start_time;
        TianLiQtCommon_Logger::GetInstance().Trace(m_func_name.c_str(), "leave - run " + QString::number(
            std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()) + " ms");
    }
private:
    std::string m_func_name;
    std::chrono::time_point<std::chrono::steady_clock> m_start_time;
};

#define _LOG_SHOW

#ifdef _DEBUG
#define _Cat_(a, b) a ## b
#define _Cat(a, b) _Cat_(a, b)
#define _CatVarNameWithLine(Var) _Cat(Var, __LINE__)
//#define Log Logger::get_instance()
#define LogTraceScope LoggerAux _CatVarNameWithLine(_func_aux_)
#define LogTraceFunction LogTraceScope(__FUNCTION__)
#define LogTraceFunctionWithArgs 
#define TianLi_Logger TianLiQtCommon_Logger::GetInstance() 
//#define LogInfo(x) TianLi_Logger.Info(__FUNCTION__, x)
//#define LogDebug(x) TianLi_Logger.Debug(__FUNCTION__, x)
//#define LogTrace(x) TianLi_Logger.Trace(__FUNCTION__, x)
//#define LogWarn(x) TianLi_Logger.Warn(__FUNCTION__, x)
//#define LogError(x) TianLi_Logger.Error(__FUNCTION__, x)
#else
#ifdef _LOG_SHOW
#define _Cat_(a, b) a ## b
#define _Cat(a, b) _Cat_(a, b)
#define _CatVarNameWithLine(Var) _Cat(Var, __LINE__)
#define Log Logger::get_instance()
#define LogTraceScope LoggerAux _CatVarNameWithLine(_func_aux_)
#define LogTraceFunction LogTraceScope(__FUNCTION__)
#define LogTraceFunctionWithArgs 
#define TianLi_Logger TianLiQtCommon_Logger::GetInstance() 
//#define LogInfo(x) TianLi_Logger.Info(__FUNCTION__, x)
//#define LogDebug(x) TianLi_Logger.Debug(__FUNCTION__, x)
//#define LogTrace(x) TianLi_Logger.Trace(__FUNCTION__, x)
//#define LogWarn(x) TianLi_Logger.Warn(__FUNCTION__, x)
//#define LogError(x) TianLi_Logger.Error(__FUNCTION__, x)
#else
#define LogTraceFunction ;
#define TianLi_Logger ;
//#define LogInfo(x) ;
//#define LogDebug(x) ;
//#define LogTrace(x) ;
//#define LogWarn(x) ;
//#define LogError(x) ;
#endif // _LOG_SHOW
#endif // _DEBUG
