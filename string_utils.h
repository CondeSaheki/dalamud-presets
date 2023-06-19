#pragma once
#include <sstream>
#include <string>
#include <vector>

namespace saheki
{
    template <typename char_type = char>
    std::basic_string<char_type> first_lower(const std::basic_string<char_type> &str)
    {
        std::basic_string<char_type> data = str;
        data[0] = std::tolower(data[0]);
        return data;
    }

    // template <typename char_type, typename type>
    // std::basic_string<char_type> table2(std::vector<type> &vec, const int &first, const int &secound, const std::fuction<void(std::basic_stringstream<char_type> &ss, typename std::vector<type>::iterator &it)> &fn1,
    //                                     std::fuction<void(std::basic_stringstream<char_type> &ss, typename std::vector<type>::iterator &it)> &fn2)
    // {
    //     std::basic_stringstream<char_type> ss;
    //     for (auto it = vec.begin(); it != vec.end() - 1; ++it)
    //     {
    //         ss << std::left << std::setw(first);
    //         fn1(ss, it);
    //         ss << std::left << std::setw(secound);
    //         fn2(ss, it);
    //         ss << std::endl;
    //     }
    //     ss << std::left << std::setw(first);
    //     fn1(ss, vec.end() - 1);
    //     ss << std::left << std::setw(secound);
    //     fn2(ss, vec.end() - 1);
    //     return ss.str();
    // }

}
