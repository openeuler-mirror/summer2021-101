#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include <unistd.h>

class Test : public BasePlugin {
public:
    virtual string getinfo() const;

    virtual string preExec(string s) const;

    virtual string realExec(string s) const;
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