#ifndef __BASEPLUGIN_HH__
#define __BASEPLUGIN_HH__

#include <iostream>
#include <stdio.h>
#include <string>

using std::string;

class BasePlugin {
public:
    BasePlugin() { }
    virtual ~BasePlugin() { }

    virtual string getinfo() const = 0;

    virtual string preExec(string s) const = 0;

    virtual string realExec(string s) const = 0;

private:
};

typedef BasePlugin* create_t();
typedef void destroy_t(BasePlugin*);

#endif