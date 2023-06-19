#pragma once
#include <iostream>
#include <functional>

#include "io.h"
#include "log.h"

namespace saheki::dialogs
{
    template <typename char_type = char >
    _GLIBCXX_NODISCARD bool confirm(std::string question, bool default_option = true) noexcept
    {
        std::basic_string<char_type> valid_yes[5] = {"y", "Y", "yes", "YES", "Yes"};
        std::basic_string<char_type> valid_no[5] = {"n", "N", "no", "NO", "No"};

        while (true)
        {
            // display your question
            std::cout << ":: " << question;
            if (default_option)
            {
                std::cout << " [Y/n]: ";
            }
            else
            {
                std::cout << " [y/N]: ";
            }

            auto answer = io::input<char_type>();

            if (answer == "")
                return default_option;

            for (auto it = 0; it != 5; ++it)
            {
                if (answer == valid_yes[it])
                    return true;
            }
            for (auto it = 0; it != 5; ++it)
            {
                if (answer == valid_yes[it])
                    return false;
            }
        }
    }

    template <typename char_type = char >
    _GLIBCXX_NODISCARD std::basic_string<char_type> question_failable(const std::basic_string<char_type> &demand, const std::basic_string<char_type> &fail,
                                   const std::function<bool(std::basic_string<char_type>)> &fn)
    {

        std::basic_string<char_type> answer;
        
        std::cout << ":: " << demand << " ";
        while (true)
        {
            answer = io::input_line<char_type>();
            if (fn(answer))
            {
                return answer;
            }
            else
            {
                std::cout << ":: try again " << fail << " ";
            }
        }
    }

} // namespace end