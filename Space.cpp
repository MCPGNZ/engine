#include "Space.h"

#include "File.h"

#include "json.hpp"
#include "json_bindings.h"



namespace pk
{
    void Space::add(int id, const std::string & name)
    {
        _objects.push_back(id);
        _names.push_back(name);
    }
    void Space::remove(int id)
    {}
}