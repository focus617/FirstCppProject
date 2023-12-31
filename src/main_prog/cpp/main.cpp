#include <string>
#include <sstream>
#include <thread>

#include <glog/logging.h>

#include "config.h"
#include "logger.h"
#include "xuzy_math.h"
#include "singleton.h"
#include "app.h"
#include "args.h"
#include "restful.h"

using xuzy::Logger;

void test_logger();
void test_singleton();
extern int test_httplib_client();
extern void thread_trial();
extern void thread_active_object();

const std::string VERSION{"0.0.1"};
const std::string APP_NAME{"Restful-Server"};

int main(int argc, char *argv[])
{
    // parse_commandline(argc, argv);

    xuzy::App *app = new http::Restful(APP_NAME);
    
    CLI_Parser *p_cli_parser = new CLI_Parser();
    app-> set_cli_parser(p_cli_parser);

    xuzy::App::main(argc, argv, VERSION, app);

    LOG(INFO) << "Main Thread Stopped.";

    /*
        test_logger();
        test_singleton();

        thread_trial();
        thread_active_object();
    */

    return EXIT_SUCCESS;
}

void load_from_configure_file(const std::string &t_filename)
{
    LOG_INFO("%s(): Load configure from file: %s", __FUNCTION__, t_filename.c_str());
}

void set_log_level(const LogLevel &t_new_level)
{
    Logger::Level log_level;

    switch (t_new_level)
    {
    case LogLevel::Fatal:
        log_level = Logger::Level::Fatal;
        break;
    case LogLevel::Error:
        log_level = Logger::Level::Error;
        break;
    case LogLevel::Warn:
        log_level = Logger::Level::Warn;
        break;
    case LogLevel::Info:
        log_level = Logger::Level::Info;
        break;
    case LogLevel::Debug:
        log_level = Logger::Level::Debug;
        break;
    case LogLevel::Trace:
        log_level = Logger::Level::Trace;
        break;
    default:
        log_level = Logger::Level::Trace;
    }
    Logger::get_logger().set_level(log_level);
    return;
}

void test_logger()
{
    int i{0};
    LOG_TRACE("%d: Test Log Level %s", i++, "Trace");
    LOG_DEBUG("%d: Test Log Level %s", i++, "Debug");
    LOG_INFO("%d: Test Log Level %s", i++, "Info");
    LOG_WARN("%d: Test Log Level %s", i++, "Warn");
    LOG_ERROR("%d: Test Log Level %s", i++, "Error");
    LOG_FATAL("%d: Test Log Level %s", i++, "Fatal");

    auto message1 = std::stringstream();
    message1 << i << ": Test Log Level with new logger.";

    int a = 12;
    int b = 56;
    auto message2 = std::stringstream();
    message2 << a << " + " << b << " = " << xuzy::add(a, b);

    // Getting the logger instance.
    auto &logger = Logger::get_logger();
    Logger::set_output(&std::cout);

    logger.FATAL(message1.str(), __LINE__, __FILE__);
    logger.ERROR(message1.str(), __LINE__, __FILE__);
    logger.WARN(message1.str(), __LINE__, __FILE__);
    logger.INFO(message2.str());
    logger.DEBUG(message1.str(), __LINE__, __FILE__);
    logger.TRACE(message1.str(), __LINE__, __FILE__);
}

void test_singleton()
{
    Singleton *s1 = Singleton::get_instance(10);
    s1->print_unique_id();
    std::cout << "S1 memory location: " << s1 << std::endl;

    Singleton *s2 = Singleton::get_instance(5);
    s2->print_unique_id();
    std::cout << "S2 memory location: " << s2 << std::endl;
}