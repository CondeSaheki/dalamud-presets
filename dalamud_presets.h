#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <iostream>
#include <cstring>

#include "io.h"
#include "json.hpp"
#include "log.h"
#include "string_utils.h"

namespace saheki
{
    namespace sfs = std::filesystem;
    namespace json = nlohmann;

    class ffxiv_mod
    {
    public: // constructor
        ffxiv_mod(const std::string &name, const sfs::path &dir, const sfs::path &config)
            : _name(name),
              _dir(dir),
              _config_file(config) {}

        ffxiv_mod(const sfs::path &dir)
        {
            if (sfs::exists(dir) == true)
            {
                _dir = dir;
                _name = dir.filename().string();
                update();
            }
        }

    private: // data
        std::string _name;
        sfs::path _dir;
        sfs::path _config_file;

    public: // access
        const auto &name() { return _name; }
        const auto &dir() { return _dir; }
        const auto &config_file() { return _config_file; }

    public:             // metods
        bool is_valid() // check if this mod is oki
        {
            if (sfs::exists(_dir) == true && sfs::exists(_config_file) == true)
            {
                auto file = json::json::parse(std::ifstream(_config_file));
                return file["Disabled"] != NULL;
            }
            return false;
        }

        void update() // set config file
        {
            if (_config_file == sfs::path() || sfs::exists(_config_file) == false)
            {
                for (const sfs::directory_entry &dir_entry : sfs::recursive_directory_iterator(_dir))
                {
                    if (dir_entry.path().filename().string() == _name + ".json" || dir_entry.path().filename().string() == first_lower(_name) + ".json")
                    {
                        _config_file = dir_entry.path();
                        break;
                    }
                }
            }
        }

        bool status() // return disabled
        {
            auto file = json::json::parse(std::ifstream(_config_file));
            return file["Disabled"];
        }

        // change mod status

        void enable()
        {
            auto file = json::json::parse(std::ifstream(_config_file));
            file["Disabled"] = false;
            io::outputf<char>(_config_file, file.dump(4));
        }
        void disable()
        {
            auto file = json::json::parse(std::ifstream(_config_file));
            file["Disabled"] = true;
            io::outputf<char>(_config_file, file.dump(4));
        }

    private:
    };

    std::vector<ffxiv_mod> ffxiv_mod_scan(const sfs::path &p)
    {
        std::vector<ffxiv_mod> vec;
        for (const sfs::directory_entry &dir_entry : sfs::directory_iterator(p))
        {
            auto tmp = ffxiv_mod(dir_entry.path());
            if (tmp.is_valid() == true)
            {
                vec.emplace_back(tmp);
            }
            else
            {
                loging::warnning("failed with \"", tmp.name(), "\"");
            }
        }
        return vec;
    }

    json::json to_json(ffxiv_mod &mod)
    {
        json::json j;
        j["name"] = mod.name();
        j["config_file"] = mod.config_file();
        j["dir"] = mod.dir();
        return j;
    }

    class preference
    {
    public:
        ffxiv_mod mod;
        std::vector<std::tuple<std::string, bool>> presets;

    public:
        preference(ffxiv_mod &_mod) : mod(_mod), presets() {}
    };

    json::json to_json(preference &pref)
    {
        json::json j;
        j["mod"] = to_json(pref.mod);
        j["presets"] = pref.presets;

        return j;
    }

} //namespace