#pragma once
#include <string>
#include <vector>

#include "vec2.h"

#include <atlcomcli.h>
#include <shtypes.h>

struct IShellFolder;
struct IFolderView;

namespace pk
{
    class Desktop;

    class Folder
    {
        public:
        friend class Desktop;
        enum struct Type
        {
            Desktop,
            Normal
        };

        private:
        IShellFolder* parent_shell_folder;
        IFolderView* parent_folder_view;

        LPITEMIDLIST item = nullptr;

        CComPtr<IShellFolder> shell_folder;
        CComPtr<IFolderView> folder_view;
        CComPtr<IStorage> storage;

        Type type;
        public:
        Folder(void) = delete;
        Folder(IShellFolder* psv, IFolderView* pfv);

        //static Folder Desktop();

        std::string name() const;
        std::string path() const;

        void rename(const std::string& name);
        void icon(const std::string& path);
        void tooltip(const std::string& content);

        static void position(const std::vector<Folder>& folders, const std::vector<vec2i>& positions);

        vec2i position() const;
        void position(const vec2i pos);

        bool created() const;
        bool valid() const;

        bool exists(const std::string& name);

        Folder find(const std::string& name);
        Folder create(const std::string& name);

        void destroy();
        void destroy(const std::string& name);
    };
}