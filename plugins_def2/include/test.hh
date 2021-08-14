#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include <unistd.h>

class Test : public BasePlugin {
public:
    virtual string getinfo();

    virtual string preExec(string s);

    virtual string realExec(string s);
};

extern "C" BasePlugin* create()
{
    return new Test;
}

extern "C" void destroy(BasePlugin* p)
{
    delete p;
}
#endif