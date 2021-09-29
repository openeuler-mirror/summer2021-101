#ifndef __BASEPLUGIN_HH__
#define __BASEPLUGIN_HH__

#include <stdio.h>

#include <iostream>
#include <string>

using std::string;

class BasePlugin {
public:
    BasePlugin() { }
    virtual ~BasePlugin() { }

    /**
     * \brief 用户打开搜索面板，执行更新操作
     *
     */
    virtual void update() = 0;

    /**
     * \brief 获取程序的相关信息，用于管理面板展示,
     * \return 返回程序相关信息，返回json格式 内容参考getinfo.example.json
     *
     */
    virtual string getinfo() = 0;

    /**
     * \brief 预执行，在查询一半阶段，返回提示值
     * \param s 用户输入
     *
     * \return 返回提示值，json格式,内容参考preExec.example.json
     */
    virtual string preExec(string s) = 0;

    /**
     * \brief 实际执行，输入用户提示，返回实际执行
     * \param s 用户输入
     * \return 返回json格式，内容参考realExec.example.json
     */
    virtual string realExec(string s) = 0;

private:
};

typedef BasePlugin* create_t();
typedef void destroy_t(BasePlugin*);

#endif
