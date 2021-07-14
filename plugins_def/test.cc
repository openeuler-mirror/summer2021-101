#include "test.hh"

/**
 * \brief 获取程序的相关信息，用于管理面板展示,
 *         
 * \return 返回程序相关信息，返回json格式 内容参考getinfo.example.json
 * 
*/
string Test::getinfo() const
{
    return "{    \"name\": \"Hello py\", \
    \"introduction\": \"a hello world of python\",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.1\",\
    \"keyword\": \"hel\",\
    \"icon\": \"./Hello.png\",\
    \"path\": \"./test\",\
    \"CMDmethod\": true,\
    \"website\" : \"https://gitlab.summer-ospp.ac.cn/summer2021/210010449\"}";
}

/**
 * \brief 预执行，在查询一半阶段，返回提示值
 * \param s 用户输入
 * 
 * \return 返回提示值，json格式,内容参考preExec.example.json
*/
string Test::preExec(string s) const
{
    return "{\"name\":\"test\"}";
}

/**
 * \brief 实际执行，输入用户提示，返回实际执行
 * \param s 用户输入
 * \return 返回json格式，内容参考realExec.example.json
*/
string Test::realExec(std::string s) const
{
    std::cout << s << std::endl;
    return "";
}
