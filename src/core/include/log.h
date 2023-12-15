#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>

#define LOG_CRITICAL(Message, ...) (xuzy::Log::Critical(__LINE__,__FILE__,Message,__VA_ARGS__))
#define LOG_ERROR(Message, ...) (xuzy::Log::Error(__LINE__,__FILE__,Message,__VA_ARGS__))
#define LOG_WARN(Message, ...) (xuzy::Log::Warn(__LINE__,__FILE__,Message,__VA_ARGS__))
#define LOG_INFO(Message, ...) (xuzy::Log::Info(__LINE__,__FILE__,Message,__VA_ARGS__))
#define LOG_DEBUG(Message, ...) (xuzy::Log::Debug(__LINE__,__FILE__,Message,__VA_ARGS__))
#define LOG_TRACE(Message, ...) (xuzy::Log::Trace(__LINE__,__FILE__,Message,__VA_ARGS__))
namespace xuzy {
class Log
{
    public:
        enum Level { LevelCritical = 0, LevelError, LevelWarn, LevelInfo, LevelDebug, LevelTrace };

    private:
        static Level        m_LogLevel;
        static std::mutex   m_LogMutex;
    
    public:
        static void SetLevel(const Level newLevel){
            m_LogLevel = newLevel;
        }

        template<typename... Args>
        static void Trace(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[TRACE]", LevelTrace, message, args...);
        }

        template<typename... Args>
        static void Debug(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[DEBUG]", LevelDebug, message, args...);
        }

        template<typename... Args>
        static void Info(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[INFO]", LevelInfo, message, args...);
        }

        template<typename... Args>
        static void Warn(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[WARN]", LevelWarn, message, args...);
        }

        template<typename... Args>
        static void Error(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[ERROR]", LevelError, message, args...);
        }

        template<typename... Args>
        static void Critical(int line_number, const char* source_file_name, const char* message, Args... args)
        {
            log(line_number, source_file_name, "[CRITICAL]", LevelCritical, message, args...);
        }

    private:
        template<typename... Args>
        static void log(
            int line_number,
            const char* source_file_name,
            const char* message_level_str, 
            Level message_level,
            const char* message,
            Args... args)
        {
            if(m_LogLevel >= message_level){
                std::scoped_lock lock(m_LogMutex);
                print_timestamp();
                printf("%11s: ", message_level_str);
                printf(message, args...);
                printf(" on line %d in %s", line_number, source_file_name);
                printf("\n");
            }
        }

        static void print_timestamp()
        {
            std::time_t current_time = std::time(0);
            std::tm* timestamp = std::localtime(&current_time);
            
            char buffer[80];
            strftime(buffer, 80, "%c", timestamp);
            printf("%s", buffer);
        }
};

Log::Level Log::m_LogLevel = Log::LevelInfo;
std::mutex Log::m_LogMutex;

}