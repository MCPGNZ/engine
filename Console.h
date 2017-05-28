#pragma once
#include <functional>
#include <map>

#include "MessageToken.h"
#include "Singleton.h"

namespace pk
{
    namespace messages
    {
        namespace console
        {
            struct invoke
            {
                const std::string& _command;
            };

            struct add
            {
                const std::string _command;
                std::function<void(void)> _function;
            };
        }
    }

    class Console
    {
    public:
        Console(void);

        void invoke(const std::string& command);
        void add(const std::string& command, const std::function<void(void)>);

    private:
        std::map<std::string, std::function<void(void)>> _commands;
        MessageToken _token;

        friend class ConsoleView;
    };
}
