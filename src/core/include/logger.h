#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>

#include "config.h"

#define LOG_OPTION_DEBUG debug
#define LOG_OPTION_RELEASE release

#if (LOG_OPTION == LOG_OPTION_DEBUG)

#define LOG_FATAL(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__, "[FATAL]", xuzy::Logger::Level::Fatal, Message, __VA_ARGS__))
#define LOG_ERROR(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__, "[ERROR]", xuzy::Logger::Level::Error, Message, __VA_ARGS__))
#define LOG_WARN(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__, "[WARN]", xuzy::Logger::Level::Warn, Message, __VA_ARGS__))
// #define LOG_INFO(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__,"[INFO]", xuzy::Logger::Level::Info, Message, __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__, "[DEBUG]", xuzy::Logger::Level::Debug, Message, __VA_ARGS__))
#define LOG_TRACE(Message, ...) (xuzy::Logger::log(__LINE__, __FILE__, "[TRACE]", xuzy::Logger::Level::Trace, Message, __VA_ARGS__))

#define LOG_INFO(Message, ...) (xuzy::Logger::log_no_line_filename("[INFO]", xuzy::Logger::Level::Info, Message, __VA_ARGS__))

#elif (LOG_OPTION == LOG_OPTION_RELEASE)

#define LOG_FATAL(Message, ...)
#define LOG_ERROR(Message, ...)
#define LOG_WARN(Message, ...)
#define LOG_INFO(Message, ...)
#define LOG_DEBUG(Message, ...)
#define LOG_TRACE(Message, ...)

#endif

namespace xuzy
{
    class Logger final
    {
    public:
        enum class Level
        {
            Fatal = 0,
            Error,
            Warn,
            Info,
            Debug,
            Trace,
            Count
        };

    private:
        static Logger *m_one_and_only_instance;

        // A mutex to protect thread safe of p_output_
        static std::mutex m_mutex_output;

        // Shreshold for logger
        static Level m_level;

        static std::ostream *p_output_;

        std::string m_log_filename;

        Logger() = default;

        // Print a message with a certain logger level on the output.
        void print(enum Level t_message_level, const std::string &t_message);

        void print(int t_line_number,
                   const char *t_source_file_name,
                   enum Level t_message_level,
                   const std::string &t_message);

    public:
        static Logger &get_logger();

        // Only the get_logger class method is allowed to give a logger.
        // Deleted copy constructor.
        Logger(const Logger &) = delete;
        // Deleted copy assignment.
        void operator=(const Logger &) = delete;
        // Default move constructor.
        Logger(Logger &&) noexcept = default;
        // Default move assignment.
        Logger &operator=(Logger &&) noexcept = default;

        ~Logger() = default;

        void set_level(const Level t_level);

        void set_log_location(const std::string &t_app_name,
                              const std::string &t_location);

        // A function that allows the setting of the logger output.
        // Nothing by default.
        static void set_output(std::ostream *t_output = nullptr);

        // Convert log level to output string
        static const char *to_string(const Level &t_log_level);

        void FATAL(const std::string &t_message,
                      int t_line_number,
                      const char *t_source_file_name);

        void ERROR(const std::string &t_message,
                   int t_line_number,
                   const char *t_source_file_name);

        void WARN(const std::string &t_message,
                  int t_line_number,
                  const char *t_source_file_name);

        void INFO(const std::string &t_message);

        void DEBUG(const std::string &t_message,
                   int t_line_number,
                   const char *t_source_file_name);

        void TRACE(const std::string &t_message,
                   int t_line_number,
                   const char *t_source_file_name);

        //////////////////////////////////////////////
    private:
        static void print_timestamp();

    public:
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
                std::scoped_lock lock(m_mutex_output);
                print_timestamp();
                printf("%11s: ", t_message_level_str);
                printf(t_message, args...);
                printf("\n\t\t\t\t     on line %d in %s", t_line_number, t_source_file_name);
                printf("\n");
            }
        }

        template <typename... Args>
        static void log_no_line_filename(
            const char *t_message_level_str,
            Level t_message_level,
            const char *t_message,
            Args... args)
        {
            if (m_level >= t_message_level)
            {
                std::scoped_lock lock(m_mutex_output);
                print_timestamp();
                printf("%11s: ", t_message_level_str);
                printf(t_message, args...);
                printf("\n");
            }
        }
    };

}