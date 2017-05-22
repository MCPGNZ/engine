#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <functional>

namespace pk
{
    struct logger
    {
    private:
        enum struct type
        {
            LOG_INFO,
            LOG_WARNING,
            LOG_ERROR,
        };

        static void log(const std::string& message, type type = type::LOG_INFO);

    public:
        #pragma region Static variables
        static std::function<void(const std::string& message)> callback;
        #pragma endregion

        #pragma region Static methods
        template <typename... Args>
        static void info(Args&&... arg_pack)
        {
            std::string data;
            expand(data, std::forward<Args>(arg_pack)...);

            log(data, type::LOG_INFO);
        }

        template <typename... Args>
        static void warning(Args&&... arg_pack)
        {
            std::string data;
            expand(data, std::forward<Args>(arg_pack)...);

            log(data, type::LOG_WARNING);
        }

        template <typename... Args>
        static void error(Args&&... arg_pack)
        {
            std::string data;
            expand(data, std::forward<Args>(arg_pack)...);

            log(data, type::LOG_ERROR);
        }
        static void error(const char* class_name, const char* method_name, const char* reason)
        {
            log("[" + std::string{class_name} +"]::[" + std::string{method_name} +"]::[" + std::string{reason} +"]", type::LOG_ERROR);
        }

        static void assert_cond(bool exprestion, const char* class_name, const char* method_name, const char* reason)
        {
            if(!exprestion)
            {
                log("[" + std::string{class_name} +"]::[" + std::string{method_name} +"]::[" + std::string{reason} +"]", type::LOG_ERROR);
            }
        }
        static void assert_hr(long hr, const char* class_name, const char* method_name, const char* reason)
        {
            if(hr < 0)
            {
                error(class_name, method_name, reason);
            }
        }
        #pragma endregion

        #pragma region Helper Functions
        template <typename Arg1, typename... Args>
        static void expand(std::string& data, Arg1&& arg, Args&&... arg_pack)
        {
            std::stringstream stream;
            stream << arg;
            data += stream.str();

            expand(data, std::forward<Args>(arg_pack)...);
        }

        template <typename Arg>
        static void expand(std::string& data, Arg&& arg)
        {
            std::stringstream stream;
            stream << arg;
            data += stream.str();
        }
        #pragma endregion
    };
}