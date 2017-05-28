#include "folder.h"

#include <string>
#include <shtypes.h>
#include <atlbase.h>
#include <shlobj.h>

#include <future>

#include "logger.h"
#include "desktop.h"

namespace pk
{
    Folder::Folder(IShellFolder* psv, IFolderView* pfv) :
        parent_shell_folder{psv},
        parent_folder_view{pfv}
    {}

    std::string Folder::name() const
    {
        HRESULT hr;
        STRRET str;
        CComHeapPtr<char> data;

        hr = parent_shell_folder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
        logger::assert_hr(hr, "Folder", "name", "GetDisplayNameOf");

        hr = StrRetToStr(&str, item, &data);
        logger::assert_hr(hr, "Folder", "name", "StrRetToStr");

        return data;
    }

    std::string Folder::path() const
    {
        std::string s((size_t) 256, ' ');
        auto hr = SHGetPathFromIDList(item, const_cast<char*>(s.data()));
        logger::assert_hr(hr, "Folder", "path", "SHGetPathFromIDList");

        return s;
    }

    void Folder::rename(const std::string& data)
    {
        std::wstring wdata{data.begin(), data.end()};
        auto hr = parent_shell_folder->SetNameOf(nullptr, item, wdata.data(), SHGDN_NORMAL, &item);
        logger::assert_hr(hr, "Folder", "rename", "SetNameOf");
    }

    void Folder::icon(const std::string& iconPath)
    {
        auto folderPath = path();

        std::wstring folderPathW{folderPath.cbegin(), folderPath.cend()};
        std::wstring iconPathW{iconPath.cbegin(), iconPath.cend()};

        SHFOLDERCUSTOMSETTINGS fcs = {0};
        fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
        fcs.dwMask = FCSM_ICONFILE;
        fcs.pszIconFile = const_cast<wchar_t*>(iconPathW.data());
        fcs.cchIconFile = 0;
        fcs.iIconIndex = 0;
        auto hr = SHGetSetFolderCustomSettings(&fcs, folderPathW.data(), FCS_FORCEWRITE);
        logger::assert_hr(hr, "Folder", "icon", "SHGetSetFolderCustomSettings");
    }

    void Folder::tooltip(const std::string& content)
    {
        auto folderPath = path();

        std::wstring folderPathW{folderPath.cbegin(), folderPath.cend()};
        std::wstring contentW{content.cbegin(), content.cend()};

        SHFOLDERCUSTOMSETTINGS fcs = {0};
        fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
        fcs.dwMask = FCSM_INFOTIP;
        fcs.pszInfoTip = const_cast<wchar_t*>(contentW.data());
        auto hr = SHGetSetFolderCustomSettings(&fcs, folderPathW.data(), FCS_FORCEWRITE);
        logger::assert_hr(hr, "Folder", "tooltip", "SHGetSetFolderCustomSettings");
    }

    vec2i Folder::position() const
    {
        if(parent_folder_view == nullptr)
        {
            logger::error("Folder", "position", "folder does not have IFolderView");
            return vec2i{};
        }

        POINT point;
        auto hr = parent_folder_view->GetItemPosition(item, &point);
        logger::assert_hr(hr, "Folder", "position", "GetItemPosition");

        return vec2i{point.x, point.y};
    }

    void Folder::position(const vec2i pos)
    {
        if(parent_folder_view == nullptr)
        {
            logger::error("Folder", "position", "folder does not have IFolderView");
            return;
        }

        POINT point{pos.x, pos.y};
        LPCITEMIDLIST list = item;

        auto hr = parent_folder_view->SelectAndPositionItems(1, &list, &point, SVSI_POSITIONITEM);

        logger::assert_hr(hr, "Folder", "position", "SelectAndPositionItems");
    }

    void Folder::position(const std::vector<Folder>& folders, const std::vector<vec2i>& positions)
    {
        std::vector<POINT> points;
        for(const auto p : positions)
        {
            points.push_back(POINT{p.x, p.y});
        }

        std::vector<LPCITEMIDLIST> pidls;
        for(const auto f : folders)
        {
            pidls.push_back(f.item);
        }

        folders[0].parent_folder_view->SelectAndPositionItems(folders.size(), pidls.data(), points.data(), SVSI_POSITIONITEM);
    }

    bool Folder::created() const
    {
        return (item != nullptr);
    }
    bool Folder::valid() const
    {
        return (parent_shell_folder != nullptr);
    }

    bool Folder::exists(const std::string& name)
    {
        return find(name).valid();
    }

    Folder Folder::find(const std::string& name)
    {
        HRESULT hr;
        CComPtr<IEnumIDList> list;

        hr = shell_folder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &list);
        logger::assert_hr(hr, "Folder", "find", "EnumObjects");

        ULONG count = 0;
        LPITEMIDLIST currentItem = nullptr;

        while(list->Next(1, &currentItem, &count) != S_FALSE)
        {
            STRRET str;
            CComHeapPtr<char> data;

            hr = shell_folder->GetDisplayNameOf(currentItem, SHGDN_NORMAL, &str);
            logger::assert_hr(hr, "Folder", "find", "GetDisplayNameOf");

            hr = StrRetToStr(&str, currentItem, &data);
            logger::assert_hr(hr, "Folder", "find", "StrRetToStr");

            auto retreivedName = std::string{data};
            if(name == retreivedName)
            {
                Folder result{shell_folder, folder_view};
                result.item = ILCombine(item, currentItem);

                hr = shell_folder->BindToObject(result.item, nullptr, IID_IShellFolder, (void**) &result.shell_folder.p);
                logger::assert_hr(hr, "Folder", "find", "BindToObject");

                hr = shell_folder->BindToStorage(result.item, nullptr, IID_IStorage, (void**) &result.storage.p);
                logger::assert_hr(hr, "Folder", "find", "BindToStorage");

                return result;
            }
        }

        return Folder{nullptr, nullptr};
    }

    Folder Folder::create(const std::string& name)
    {
        const std::wstring x{name.cbegin(), name.cend()};

        CComPtr<IStorage> data;
        HRESULT hr;

        Folder result{shell_folder, folder_view};

        hr = storage->CreateStorage(x.c_str(), STGM_FAILIFTHERE, 0, 0, &result.storage.p);
        logger::assert_hr(hr, "Desktop", "create", "CreateStorage");

        hr = result.storage->QueryInterface<IShellFolder>(&result.shell_folder.p);
        logger::assert_hr(hr, "Desktop", "create", "QueryInterface<IShellFolder>");

        IPersistFolder2* psf2;
        hr = result.shell_folder->QueryInterface<IPersistFolder2>(&psf2);
        logger::assert_hr(hr, "Desktop", "create", "IPersistFolder2");

        hr = psf2->GetCurFolder(&result.item);
        logger::assert_hr(hr, "Desktop", "create", "GetCurFolder");

        return result;
    }

    void Folder::destroy()
    {
        CComQIPtr<IStorage> parent_storage{parent_shell_folder};

        std::string n = name();
        std::wstring wname{n.cbegin(), n.cend()};
        HRESULT hr = parent_storage->DestroyElement(wname.data());

        logger::assert_hr(hr, "Desktop", "create", "destroy");
    }

    void Folder::destroy(const std::string& name)
    {
        std::wstring wname{name.cbegin(), name.cend()};
        storage->DestroyElement(wname.data());
    }
}