#pragma once
#include <wtypes.h>
#include <atlcomcli.h>
#include <shobjidl.h>

#include "vec2.h"
#include "color.h"

class IShellFolder;
class IFolderView;

namespace pk
{
    class Folder;
    class Desktop
    {
        public:
        HWND progman;
        HWND listView;
        HWND defView;
        HANDLE explorer;

        HDC hdc;

        CComPtr<IShellFolder> shell_folder;
        CComPtr<IFolderView>  folder_view;

        Desktop(void);
        public:
        static Desktop& Interface();
        static Folder& IFolder();

        vec2i resolution() const;

        bool autoarrange() const;
        bool grid_allign() const;

        void autoarrange(const bool state);
        void grid_allign(const bool state);

        //int folder_count() const;
        //Folder folder(const int i);
        //Folder find(const std::string& name);

        //bool exists(const std::string& name);
        //LPITEMIDLIST item(const std::string& name);

        //Folder create(const std::string& name);

        Color color(const vec2i position) const;

        private:
        void query_listview_hwnd();
        void query_explorer_handle();
        void query_shell_folder();

        void query_folder_view(void** ppv, const IID& riid = IID_IFolderView);

        // std::string get_folder_name(LPITEMIDLIST item) const;

        void read_icon_size();
        bool read_reg_string(HKEY hkeymom, char* pkeyname, char* pvalname, char* pvaldata);
    };
}
