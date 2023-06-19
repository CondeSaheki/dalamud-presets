#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <functional>

#include "json.hpp"
#include "dalamud_presets.h"
#include "io.h"
#include "log.h"
#include "dialogs.h"
#include "string_utils.h"

struct command
{
    std::string name;
    std::string description;
    std::function<void()> func;
    command(const std::string &_name, const std::string &_description, const std::function<void()> &_func) : name(_name), description(_description), func(_func) {}
};

std::string commands_help(std::vector<command> &commands) noexcept
{
    std::stringstream ss;
    ss << "Getting Helped:\n";
    for (auto it = commands.begin(); it != commands.end() - 1; ++it)
    {
        ss << std::left << std::setw(30) << it->name << std::left << std::setw(8) << it->description << std::endl;
    }
    ss << std::left << std::setw(30) << (commands.end() - 1)->name << std::left << std::setw(8) << (commands.end() - 1)->description;
    return ss.str();
}

struct config
{
    std::filesystem::path mods_folder;
    config() = default;
    config(const nlohmann::json &file)
    {
        namespace sfs = std::filesystem;

        if (file["mods folder"] == NULL)
        {
            saheki::loging::fatal("Config file is invalid");
        }
        mods_folder = sfs::path(std::string(file["mods folder"]));
        if (sfs::exists(mods_folder) == false)
        {
            saheki::loging::fatal(mods_folder, " does not exist");
        }
    }
};

int main(int argc, char *argv[])
{
    namespace sfs = std::filesystem;
    namespace json = nlohmann;
    using namespace saheki;

    loging::info("Welcome!");
    loging::message("DO NOT USE THIS WHILE DALAMUD IS RUNNING!\n");
    // loging::message("press \"enter\" to continue");
    // auto discarted_value = io::input<char, char>();
    const sfs::path config_file = "config.json";
    config cfg;

    std::vector<command> commands =
        {
            command("-help", "Show this help message", [&]() {}),
            command("-show", "Show all mods and status",
                    [&]()
                    {
                        loging::info("searching for mods");

                        std::vector<ffxiv_mod> mods = ffxiv_mod_scan(cfg.mods_folder);

                        loging::message("Number of mods is ", mods.size(), "!");
                        std::stringstream ss;
                        for (auto it = mods.begin(); it != mods.end() - 1; ++it)
                        {
                            ss << std::left << std::setw(30) << it->name() << std::left << std::setw(8) << (it->status() ? "Disabled" : "Enabled") << std::endl;
                        }
                        ss << std::left << std::setw(30) << (mods.end() - 1)->name() << std::left << std::setw(8) << ((mods.end() - 1)->status() ? "Disabled" : "Enabled");
                        loging::message(ss.str()); // it->name(), "\t\t", it->status() ? "Disabled" : "Enabled"
                    }),
            command("-enable", "Enable all mods",
                    [&]()
                    {
                        loging::info("searching for mods");

                        std::vector<ffxiv_mod> mods = ffxiv_mod_scan(cfg.mods_folder);

                        loging::info("number of mods is ", mods.size());

                        for (auto it = mods.begin(); it != mods.end(); ++it)
                        {
                            loging::debug(it->name(), " status ", it->status());
                            it->enable();
                        }
                    }),
            command("-disable", "Disable all mods",
                    [&]()
                    {
                        loging::info("searching for mods");

                        std::vector<ffxiv_mod> mods = ffxiv_mod_scan(cfg.mods_folder);

                        loging::info("number of mods is ", mods.size());

                        for (auto it = mods.begin(); it != mods.end(); ++it)
                        {
                            loging::debug(it->name(), " status ", it->status());
                            it->enable();
                        }
                    }),
            command("-p:display", "WIP", [&]() {}),
            command("-p:apply", "WIP", [&]() {}),
            command("-p:edit", "WIP", [&]() {})

        };

    // help message must be set later
    commands[0].func =
        [&]()
    {
        loging::message(commands_help(commands));
        exit(0);
    };
    typename std::vector<command>::const_iterator option = commands.begin();

    // dealing with cli
    {
        loging::info("reading cli");

        if (argc != 2) // more or less args than needed
        {
            loging::message(commands_help(commands));
            exit(0);
        }
        loging::debug("arg is \"", argv[1], "\"");

        for (; option != commands.end(); ++option)
        {
            if (argv[1] == (*option).name)
            {
                break;
            }
        }
        if (option == commands.end())
        {
            loging::message(commands_help(commands));
            exit(0);
        }
    }

    // config
    {
        loging::info("loading configs");

        if (sfs::exists(config_file) == false)
        {
            loging::message("Creating a default config file");
            json::json j;
            /* the string slices here are an quick fix io::input_line unintended behavior */

            auto str = dialogs::question_failable<char>("Enter your ffxiv mods folder", "Path does not exist",
                                                        [](const std::string &s)
                                                        {
                                                            return sfs::exists(std::string(s.begin(), s.end() - 1));
                                                        });
            j["mods folder"] = sfs::path(std::string(str.begin(), str.end() - 1));
            io::outputf<char>(config_file, j.dump(4));
        };
        auto file = json::json::parse(std::ifstream(config_file));
        cfg = config(file);
    }

    // execution
    {
        loging::info("Executing Command \"", option->name, "\"");

        option->func();
    }

    loging::info("done!");
    return 0;
}
