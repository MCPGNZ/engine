#pragma once
#include <atomic>
#include <thread>
#include <string>
#include <functional>

#include "File.h"

namespace pk
{
    #pragma region Forward declarations
    #pragma endregion

    class FileWatcher
    {
    public:
        #pragma region Types
        enum struct Type
        {
            Sequence,
            Async
        };
        #pragma endregion

        #pragma region Ctors
        FileWatcher(void) = default;
        FileWatcher(const std::filesystem::path& path);


        FileWatcher(const std::filesystem::path& path, const std::function<void(void)>& function, Type type = Type::Sequence);

        ~FileWatcher(void);
        #pragma endregion

        #pragma region Public methods
        void run(const std::filesystem::path& path);
        void run(const std::filesystem::path& path, const std::function<void(void)>& function, Type type = Type::Sequence);

        void stop();

        void update() const;
        void callback(const std::function<void(void)>& function, Type type = Type::Sequence);
        #pragma endregion

    private:
        #pragma region Private variables
        mutable std::atomic_flag runningFlag;
        mutable std::atomic<bool> change;

        std::thread watchThread;
        std::filesystem::path path;
        void* handle;

        std::function<void(void)> sequenceCallback = nullptr;
        std::function<void(void)> asyncCallback = nullptr;

        #pragma endregion

        #pragma region Private methods
        void threadFunc();
        #pragma endregion
    };
}