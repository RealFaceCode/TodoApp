#ifndef TIME_HANDLER_HPP
#define TIME_HANDLER_HPP

#include <iostream>
#include <chrono>

namespace TimeHandler
{
    [[maybe_unused]] std::string getCurrentTime()
    {
        time_t now = time(nullptr);
        tm* currentTime = localtime(&now);
        std::string r;
        r += std::to_string(currentTime->tm_year + 1900)
                + ":" + std::to_string(currentTime->tm_mon + 1)
                + ":" + std::to_string(currentTime->tm_mday)
                + " " + std::to_string(currentTime->tm_hour)
                + "-" + std::to_string(currentTime->tm_min)
                + "-" + std::to_string(currentTime->tm_sec);

        return r;
    }

    enum class ClockMode
            {
        none,
        single,
        multi
            };

    class Clock
            {
            private:
                std::chrono::time_point<std::chrono::system_clock> mBegin;
                ClockMode mMode;
                double mCheckTime;

            public:
                [[maybe_unused]] Clock()
                : mBegin(), mMode(ClockMode::none), mCheckTime(0.0)
                {}

                [[maybe_unused]] Clock(const ClockMode& mode, const double& time)
                : mBegin(), mMode(mode), mCheckTime(time)
                {}

                [[maybe_unused]] void start()
                {
                    mBegin = std::chrono::high_resolution_clock::now();
                }

                [[maybe_unused]] void start(const ClockMode& mode, const double& time)
                {
                    mBegin = std::chrono::high_resolution_clock::now();
                    mMode = mode;
                    mCheckTime = time;
                }

                [[maybe_unused]] void stop()
                {
                    mMode = ClockMode::none;
                    mCheckTime = 0.0;
                }

                [[maybe_unused]] bool check()
                {
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsedTime = end - mBegin;
                    double elapsedSeconds = elapsedTime.count();

                    if(elapsedSeconds >= mCheckTime)
                    {
                        switch (mMode) {
                            case ClockMode::none:
                                return false;
                                case ClockMode::single:
                                    return true;
                                    case ClockMode::multi:
                                        mBegin = std::chrono::high_resolution_clock::now();
                                        return true;
                        }
                    }

                    return false;
                }

                [[maybe_unused]] double getDeltaTime()
                {
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsedTime = end - mBegin;
                    return elapsedTime.count();
                }
            };

    class Timer
            {
            private:
                std::chrono::time_point<std::chrono::system_clock> mBegin;
                std::chrono::time_point<std::chrono::system_clock> mEnd;

            public:
                [[maybe_unused]] Timer()
                : mBegin(), mEnd()
                {}

                [[maybe_unused]] void start()
                {
                    mBegin = std::chrono::high_resolution_clock::now();
                }

                [[maybe_unused]] double stop()
                {
                    mEnd = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsedTime = mEnd - mBegin;
                    return elapsedTime.count();
                }
            };
}
#endif // TIME_HANDLER_HPP
