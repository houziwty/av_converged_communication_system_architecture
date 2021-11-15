#ifndef LOG_H
#define LOG_H

class Logger
{
public:
    Logger();
    ~Logger();

public:
    int init(const char* name);
};//class Logger

#endif//LOG_H