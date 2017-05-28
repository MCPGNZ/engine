#include "Console.h"
#include "stl_extentions.h"
#include "MessageDispatcher.h"

namespace pk
{
    Console::Console(void)
    {
        using namespace messages;

        _token += MessageDispatcher::Register([&](const console::invoke& data)
        {
            invoke(data._command);
        });

        _token += MessageDispatcher::Register([&](const console::add& data)
        {
            add(data._command, data._function);
        });
    }

    void Console::invoke(const std::string& command)
    {
        if(_commands.find(command) == _commands.cend())
        {
            return;
        }

        _commands[command]();
    }
  
    void Console::add(const std::string& command, const std::function<void(void)> function)
    {
        _commands[command] = function;
    }
}