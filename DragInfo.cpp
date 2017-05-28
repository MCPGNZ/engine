#include "DragInfo.h"



namespace pk
{
    DragInfo DragInfo::interpret(void* data)
    {
        return *reinterpret_cast<DragInfo*>(data);
    }
}