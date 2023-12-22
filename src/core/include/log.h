#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>

#define LOG_CRITICAL(Message, ...) (xuzy::Log::Critical(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_ERROR(Message, ...) (xuzy::Log::Error(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_WARN(Message, ...) (xuzy::Log::Warn(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_INFO(Message, ...) (xuzy::Log::Info(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (xuzy::Log::Debug(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_TRACE(Message, ...) (xuzy::Log::Trace(__LINE__, __FILE__, Message, __VA_ARGS__))
namespace xuzy
{
    class Log
    {
    public:
        enum class Level
        {
            Critical = 0,
            Error,
            Warn,
            Info,
            Debug,
            Trace,
            Count
        };

    private:
        static Level m_level;
        static std::mutex m_mutex;

    public:
        static void SetLevel(const Level t_level)
        {
            m_level = t_level;
            LOG_INFO("Set log level to: %s", to_string(m_level));
        }

        static const char *to_string(const Level &t_log_level)
        {
            switch (t_log_level)
            {
            case Log::Level::Critical:
                return "critical";
            case Log::Level::Error:
                return "error";
            case Log::Level::Warn:
                return "warn";
            case Log::Level::Info:
                return "info";
            case Log::Level::Debug:
                return "debug";
            case Log::Level::Trace:
                return "trace";
            default:
                return "error log level";
            }
        }

        template <typename... Args>
        static void Trace(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[TRACE]", Level::Trace, t_message, args...);
        }

        template <typename... Args>
        static void Debug(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[DEBUG]", Level::Debug, t_message, args...);
        }

        template <typename... Args>
        static void Info(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[INFO]", Level::Info, t_message, args...);
        }

        template <typename... Args>
        static void Warn(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[WARN]", Level::Warn, t_message, args...);
        }

        template <typename... Args>
        static void Error(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[ERROR]", Level::Error, t_message, args...);
        }

        template <typename... Args>
        static void Critical(
            int t_line_number, const char *t_source_file_name, const char *t_message, Args... args)
        {
            log(t_line_number, t_source_file_name, "[CRITICAL]", Level::Critical, t_message, args...);
        }

    private:
        template <typename... Args>
        static void log(
            int t_line_number,
            const char *t_source_file_name,
            const char *t_message_level_str,
            Level t_message_level,
            const char *t_message,
            Args... args)
        {
            if (m_level >= t_message_level)
            {
                std::scoped_lock lock(m_mutex);
                print_timestamp();
                printf("%11s: ", t_message_level_str);
                printf(t_message, args...);
                printf("\n\t\t\t\t     on line %d in %s", t_line_number, t_source_file_name);
                printf("\n");
            }
        }

        static void print_timestamp()
        {
            std::time_t current_time = std::time(0);
            std::tm *timestamp = std::localtime(&current_time);

            char buffer[80];
            strftime(buffer, 80, "%c", timestamp);
            printf("%s", buffer);
        }
    };

}