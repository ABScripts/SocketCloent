#include "../include/logger.h"

#include <iostream>

std::array<char, Logger::Sign::Count> Logger::m_Signs = {'i', '!', '~', '?'};
std::mutex Logger::m_Mutex;

void Logger::out(std::ostream &ostream, char sign, CoStringR message)
{
    m_Mutex.lock();
    ostream << "[" << sign << "]" << " " << message << std::endl;
    m_Mutex.unlock();
}

void Logger::info(CoStringR message,  int level) 
{
    out(std::cout, m_Signs[Sign::Info], message);
}

void Logger::error(CoStringR message, int level) 
{
    out(std::cerr, m_Signs[Sign::Error], message);
}

void Logger::progress(CoStringR message, int level) 
{
    out(std::cout, m_Signs[Sign::Progress], message);
}

void Logger::question(CoStringR message, int level)
{
    out(std::cout, m_Signs[Sign::Question], message);
}

void Logger::addLogLevel(unsigned short logLevel)
{
    for (unsigned short i = 0; i < logLevel; ++i)
    {
        std::cout << "  ";
    }
}