#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <dirent.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace rapidjson;

class Test : public BasePlugin {
public:
    Test();

    virtual void update();

    virtual string getinfo();

    virtual string preExec(string s);

    virtual string realExec(string s);

private:
};

extern "C" BasePlugin* create()
{
    return new Test();
}

extern "C" void destroy(BasePlugin* p)
{
    delete p;
}
#endif