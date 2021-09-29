#include "test.hh"

string Test::getinfo()
{
    return "{    \"name\": \"Hello\", \
    \"introduction\": \"a hello world of python\",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.5\",\
    \"keyword\": \"hel\",\
    \"icon\": \"./Hello.png\",\
    \"path\": \"./test\",\
    \"CMDmethod\": true,\
    \"website\" : \"https://gitlab.summer-ospp.ac.cn/summer2021/210010449\"}";
}

string Test::preExec(string s)
{

    return "{\
    \"icon\": \"icon.svg\",\
    \"panels\": [{\
            \"cmd\": \"/usr/bin/python3.8\",\
            \"argvs\": [\
                \"Hello.py\",\
                \"1234\"\
            ],\
            \"info\": \"Powered by MarmaladeCat\"\
        },\
        {\
            \"cmd\": \"/usr/bin/gedit\",\
            \"argvs\": [\
                \"Hello.py\"\
            ],\
            \"info\": \"Powered by MarmaladeCat\"\
        }\
    ]\
}\
";
}

string Test::realExec(std::string s)
{
    // 允许相对路径

    return "{\"status\": true,\
    \"cmd\": \"/usr/bin/gedit\",\
    \"argvs\": [\"Hello.py\", \"1234\"]}";
}
