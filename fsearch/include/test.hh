#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <dirent.h>
#include <fstream>
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
    string cwd;
    // 所有搜索目录
    vector<string> dirs;
    // 所有搜索文件
    vector<string> files;

    /**
     * \brief 清除缓存
     * 
     */
    void clearCache();

    /**
     * \brief 缓存所有搜索文件
     * 
     */
    void getAllFiles();

    /**
     * \brief 读取全部搜索目录
     * 
     */
    void getAllFolders();

    /**
     * \brief 匹配字符串能否进入显示面板
     * \param f 待匹配文件
     * \param s 用户输入
     * 
     * \return 返回 true显示，false不显示
     */
    bool fSelect(string f, string s);
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