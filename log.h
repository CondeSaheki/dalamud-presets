#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <array>

#define LEVEL 2

namespace saheki
{
    class loging
    {
    public:
        using char_type = char;
        using string_type = std::basic_string<char_type>;
        std::basic_ostream<char_type> &out = std::cout;

    public:
        template <typename... args>
        static void debug(const args &...Args)
        {
            Get().io(0, Args...);
        }
        template <typename... args>
        static void info(const args &...Args)
        {
            Get().io(1, Args...);
        }
        template <typename... args>
        static void warnning(const args &...Args)
        {
            Get().io(2, Args...);
        }
        template <typename... args>
        static void error(const args &...Args)
        {
            Get().io(3, Args...);
        }
        template <typename... args>
        static void fatal(const args &...Args)
        {
            Get().io(4, Args...);
            exit(1);
        }
        template <typename... args>
        static void message(const args &...Args)
        {
            Get().io(5, Args...);
        }

    private:
        template <typename type>
        constexpr inline void output(const type &val) noexcept
        {
            out << val;
        }

        template <typename first, typename... args>
        constexpr inline void output(const first &First, const args &...Args) noexcept
        {
            output(First);
            output(Args...);
        }

        template <typename... args>
        void io(const int &&level, const args &...Args)
        {
            std::lock_guard<std::mutex> lock(io_lk);
            static const std::array<string_type, 6> type = {"debug", "info", "warnning", "error", "fatal", "message"};
            if (level == 5)
            {
                output(Args...);
                out << std::endl;
                return;
            }
            if (LEVEL <= level && type.size() >= level)
            {
                out << "<" << type[level] << " | ";
                output(Args...);
                out << ">" << std::endl;
            }
        }

    private:
        std::mutex io_lk;

    private: // constructors
        loging()
        {
        }
        static loging &Get()
        {
            static loging instance;
            return instance;
        }

    public: // operators
        loging(const loging &) = delete;
    };
}

#undef LEVEL