#include <mutex>
#include <array>

class Logger {
    using CoStringR = const std::string &;

    public:
        static void info(CoStringR, int level = 0);
        static void error(CoStringR, int level = 0);
        static void progress(CoStringR, int level = 0); 

    private:
        static void out(std::ostream &, char, CoStringR);

        static void addLogLevel(unsigned short);

        enum Sign {
            Info = 0,
            Error,
            Progress,
            Count
        };

        static std::array<char, Sign::Count> m_Signs;
        static std::mutex m_Mutex;
};