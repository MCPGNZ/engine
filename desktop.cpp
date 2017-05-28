#include "Desktop.h"
#include <string>
#include <iostream>

#include "logger.h"

#include "folder.h"
#include <shlobj.h>

namespace pk
{
    const LPCSTR program_manager = "Progman";
    const LPCSTR def_view = "SHELLDLL_DefView";
    const LPCSTR sys_list = "SysListView32";

    Desktop::Desktop(void)
    {
        HRESULT hr = CoInitialize(NULL);
        logger::assert_hr(hr, "Desktop", "ctor", "CoInitialize");

        query_listview_hwnd();
        query_explorer_handle();

        query_shell_folder();
        query_folder_view((void**) &folder_view);

        hdc = GetDC(HWND_DESKTOP);
    }

    Desktop& Desktop::Interface()
    {
        static Desktop desktop;
        return desktop;
    }

    Folder& Desktop::IFolder()
    {
        auto desktop = Interface();

        static Folder folder{nullptr, nullptr};
        folder.shell_folder = desktop.shell_folder;
        folder.folder_view = desktop.folder_view;
        folder.storage = CComQIPtr<IStorage>{desktop.shell_folder};

        return folder;
    }

    vec2i Desktop::resolution() const
    {
        return vec2i{
            GetSystemMetrics(SM_CXFULLSCREEN),
            GetSystemMetrics(SM_CYFULLSCREEN)
        };
    }

    bool Desktop::autoarrange() const
    {
        return GetWindowLong(listView, GWL_STYLE) & LVS_AUTOARRANGE;
    }
    void Desktop::autoarrange(const bool state)
    {
        if(autoarrange() == state)
        {
            return;
        }

        auto style = GetWindowLong(listView, GWL_STYLE);
        bool current_state = style & LVS_AUTOARRANGE;

        SetWindowLong(listView, GWL_STYLE, current_state ?
            (style & ~LVS_AUTOARRANGE) : (style & LVS_AUTOARRANGE));
    }

    bool Desktop::grid_allign() const
    {
        return SendMessage(listView, LVM_GETEXTENDEDLISTVIEWSTYLE, (WPARAM) LVS_EX_SNAPTOGRID, (LPARAM) 0);
    }
    void Desktop::grid_allign(const bool state)
    {
        if(grid_allign() == state)
        {
            return;
        }
        SendMessage(listView, LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM) LVS_EX_SNAPTOGRID, (LPARAM) state);
    }

    /*
    std::string Desktop::get_folder_name(LPITEMIDLIST item) const
    {
        STRRET str;
        CComHeapPtr<char> data;

        shell_folder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);

        StrRetToStr(&str, item, &data);

        return data;
    }

    int Desktop::folder_count() const
    {
        int count;
        auto hr = folder_view->ItemCount(0, &count);

        logger::assert_hr(hr, "Desktop", "folder_count", "ItemCount");
        return count;
    }

    Folder Desktop::folder(const int i)
    {
        HRESULT hr;
        LPITEMIDLIST item;
        hr = folder_view->Item(i, &item);

        logger::assert_hr(hr, "Desktop", "folder_count", "Item");

        Folder result{shell_folder};
        result.item = item;
        hr = shell_folder->BindToStorage(item, nullptr, IID_IStorage, (void**) &result.storage.p);

        logger::assert_hr(hr, "Desktop", "folder_count", "BindToStorage");

        return result;
    }

    Folder Desktop::find(const std::string& name)
    {
        for(int i = 0; i < folder_count(); ++i)
        {
            STRRET str;
            CComHeapPtr<char> data;

            LPITEMIDLIST item;
            folder_view->Item(i, &item);

            shell_folder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
            StrRetToStr(&str, item, &data);

            if(std::string{data} == name)
            {
                Folder result{shell_folder};
                result.item = item;
                auto hr = shell_folder->BindToStorage(item, nullptr, IID_IStorage, (void**)&result.storage.p);
                hr = shell_folder->BindToObject(item, nullptr, IID_IShellFolder, (void**) &result.shell_folder.p);
                return result;
            }
        }
        return Folder{nullptr};
    }

    bool Desktop::exists(const std::string& name)
    {
        std::string result_name;

        for(int i = 0; i < folder_count(); ++i)
        {
            LPITEMIDLIST item;
            folder_view->Item(i, &item);

            STRRET str;
            CComHeapPtr<char> data;
            HRESULT hr = shell_folder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
            StrRetToStr(&str, item, &data);

            result_name = data;
            if(result_name == name)
            {
                return true;
            }
        }

        return false;
    }

    LPITEMIDLIST Desktop::item(const std::string& name)
    {
        for(int i = 0; i < folder_count(); ++i)
        {
            auto result = folder(i);
            if(result.name() == name)
            {
                return result.item;
            }
        }

        return nullptr;
    }

    Folder Desktop::create(const std::string& name)
    {
        CComQIPtr<IStorage> storage{shell_folder};
        CComPtr<IStorage> data;

        std::wstring x{name.begin(), name.end()};

        Folder result{shell_folder};

        HRESULT hr = storage->CreateStorage(x.c_str() , STGM_FAILIFTHERE, 0, 0, &result.storage.p);
        logger::assert_hr(hr, "Desktop", "create", "CreateStorage");

        return result;
    }

    */

    Color Desktop::color(const vec2i position) const
    {
        auto value = GetPixel(hdc, position.x, position.y);
        return Color{GetRValue(value), GetGValue(value), GetBValue(value), 255};
    }

    void Desktop::query_shell_folder()
    {
        HRESULT hr = SHGetDesktopFolder(&shell_folder);
        if(FAILED(hr))
        {
            logger::error("Desktop", "create_shell_folder", "SHGetDesktopFolder");
        }
    }
    void Desktop::query_listview_hwnd()
    {
        progman = FindWindow(program_manager, NULL);
        if(progman == nullptr)
        {
            logger::error("Desktop", "find_listview_hwnd", "Progman");
        }

        defView = FindWindowEx(progman, 0, def_view, NULL);
        if(defView == nullptr)
        {
            logger::error("Desktop", "find_listview_hwnd", "SHELLDLL_DefView");
        }

        listView = FindWindowEx(defView, 0, sys_list, NULL);
        if(listView == nullptr)
        {
            logger::error("Desktop", "find_listview_hwnd", "SysListView32");
        }
    }
    void Desktop::query_explorer_handle()
    {
        DWORD processID = 0;

        GetWindowThreadProcessId(listView, &processID);
        if(!processID)
        {
            logger::error("Desktop", "find_explorer_handle", "GetWindowThreadProcessId");
        }

        explorer = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, processID);
        if(!explorer)
        {
            logger::error("Desktop", "find_explorer_handle", "OpenProcess");
        }
    }
    void Desktop::query_folder_view(void** ppv, const IID& riid /*= IID_IFolderView */)
    {
        CComPtr<IShellWindows> spShellWindows;
        HRESULT hr = spShellWindows.CoCreateInstance(CLSID_ShellWindows);

        CComVariant vtLoc(CSIDL_DESKTOP);
        CComVariant vtEmpty;
        long lhwnd;
        CComPtr<IDispatch> spdisp;
        hr = spShellWindows->FindWindowSW(
            &vtLoc, &vtEmpty,
            SWC_DESKTOP, &lhwnd, SWFO_NEEDDISPATCH, &spdisp);

        CComPtr<IShellBrowser> spBrowser;
        hr = CComQIPtr<IServiceProvider>(spdisp)->
            QueryService(SID_STopLevelBrowser,
            IID_PPV_ARGS(&spBrowser));

        CComPtr<IShellView> spView;
        hr = spBrowser->QueryActiveShellView(&spView);

        hr = spView->QueryInterface(riid, ppv);
    }

    void Desktop::read_icon_size()
    {
        char szIconSize[1024] = "";
        read_reg_string(HKEY_CURRENT_USER, "Control Panel\\Desktop\\WindowMetrics", "Shell Icon Size", szIconSize);
        //icon_size = atoi(szIconSize);
    }
    bool Desktop::read_reg_string(HKEY hkeymom, char* pkeyname, char* pvalname, char* pvaldata)
    {
        HKEY hkey = 0;
        DWORD dw;

        if(RegOpenKeyEx(hkeymom, pkeyname, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
        {
            strcpy(pvaldata, "");

            logger::error("Desktop", "read_reg_string", "RegOpenKeyEx");
            return false;
        }

        LONG rtn = RegQueryValueEx(hkey, pvalname, NULL, &dw, NULL, NULL);
        if((rtn != ERROR_SUCCESS) || dw != REG_SZ)
        {
            RegCloseKey(hkey);
            strcpy(pvaldata, "");

            logger::error("Desktop", "read_reg_string", "RegQueryValueEx");
            return false;
        }

        dw = 1024;
        RegQueryValueEx(hkey, pvalname, NULL, NULL, (BYTE*) pvaldata, &dw);
        RegCloseKey(hkey);

        return true;
    }
}