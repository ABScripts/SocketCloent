#include <mutex>
#include <array>

class Logger {
    using CoStringR = const std::string &;

    public:
        static void info(CoStringR);
        static void error(CoStringR);
        static void progress(CoStringR); 

    private:
        static void out(std::ostream &, char, CoStringR);

        enum Sign {
            Info = 0,
            Error,
            Progress,
            Count
        };

        static std::array<char, Sign::Count> m_Signs;
        static std::mutex m_Mutex;
};