#include "SoundSystem.h"

#include <fmod.hpp>
#include "logger.h"

namespace pk
{
    FMOD::System* SoundSystem::_system;

    SoundSystem::SoundSystem(void)
    {
        auto result = FMOD::System_Create(&_system);
        logger::assert_cond(result == FMOD_OK, "SoundSystem", "Ctor", "System_Create failed");

        int drivers = 0;
        result = _system->getNumDrivers(&drivers);
        logger::assert_cond(result == FMOD_OK, "SoundSystem", "Ctor", "getNumDrivers failed");

        result = _system->init(_maxChannels, FMOD_INIT_NORMAL, nullptr);
        logger::assert_cond(result == FMOD_OK, "SoundSystem", "Ctor", "init failed");
    }

    SoundSystem::~SoundSystem(void)
    {
        _system->release();
        _system = nullptr;
    }

    void SoundSystem::update()
    {
        _system->update();
    }
}