#include "FileWatcher.h"
#include "File.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "logger.h"
#include "File.h"
#include <corecrt_malloc.h>

namespace pk
{
    #pragma region Ctors
    FileWatcher::FileWatcher(const std::filesystem::path& file)
    {
        run(file);
    }
    FileWatcher::FileWatcher(const std::filesystem::path& file, const std::function<void(void)>& function, Type type /*= Type::Sequence*/)
    {
        run(file);
        callback(function, type);
    }

    FileWatcher::~FileWatcher(void)
    {
        stop();
    }
    #pragma endregion

    #pragma region Public methods
    void FileWatcher::run(const std::filesystem::path& file)
    {
        handle = FindFirstChangeNotification(file.parent_path().string().c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        logger::assert_cond(handle != 0 && handle != INVALID_HANDLE_VALUE, "FileWatcher", "ctor", "FindFirstChangeNotification");

        runningFlag.test_and_set();
        watchThread = std::thread([&](){ threadFunc(); });

        path = file;
    }
    void FileWatcher::run(const std::filesystem::path& file, const std::function<void(void)>& function, Type type)
    {
        run(file);
        callback(function, type);
    }
    void FileWatcher::stop()
    {
        if(handle != nullptr)
        {
            FindCloseChangeNotification(handle);
        }

        if(watchThread.joinable())
        {
            runningFlag.clear();
            watchThread.join();
        }
    }

    void FileWatcher::update() const
    {
        if(change.load())
        {
            change.store(false);
            sequenceCallback();
        }
    }
    void FileWatcher::callback(const std::function<void(void)>& function, FileWatcher::Type type /* = Type::sequence */)
    {
        switch(type)
        {
            case FileWatcher::Type::Sequence: sequenceCallback = function; break;
            case FileWatcher::Type::Async:    asyncCallback = function; break;
        }
    }
    #pragma endregion

    #pragma region Private methods
    void FileWatcher::threadFunc()
    {
        FILE_NOTIFY_INFORMATION* buffer = (FILE_NOTIFY_INFORMATION*) _aligned_malloc(sizeof(FILE_NOTIFY_INFORMATION) * 32, sizeof(DWORD));

        while(runningFlag.test_and_set())
        {
            auto wait = WaitForSingleObject(handle, std::numeric_limits<DWORD>::infinity());
            if(wait != WAIT_OBJECT_0)
            {
                continue;
            }
            DWORD bytes;

            if(!ReadDirectoryChangesW(handle, buffer, sizeof(FILE_NOTIFY_INFORMATION) * 32, false, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytes, NULL, NULL))
            {
                continue;
            }

            if(buffer[0].Action == FILE_ACTION_MODIFIED)
            {
                const std::filesystem::path current = {buffer[0].FileName, buffer[0].FileName + buffer[0].FileNameLength / 2};

                if(std::filesystem::is_directory(path) || path.filename() == current)
                {
                    change.store(true);
                    if(asyncCallback != nullptr)
                    {
                        asyncCallback();
                    }
                }
            }
        }

        _aligned_free(buffer);
    }
    #pragma endregion
}