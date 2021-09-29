

#include "test.hh"

Test::Test()
{
}

void Test::update()
{
    return;
}

string Test::getinfo()
{

    return "{    \"name\": \"time\", \
    \"introduction\": \"open browser to search \",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.2\",\
    \"keyword\": \"time\",\
    \"icon\": \"time.png\",\
    \"path\": \"./test\",\
    \"CMDmethod\": false,\
    \"website\" : \"https://gitlab.summer-ospp.ac.cn/summer2021/210010449\"}";
}

string Test::preExec(string s)
{

    Document dom;
    dom.SetObject();
    Value icon;
    icon.SetString("time.png", dom.GetAllocator());
    dom.AddMember("icon", icon, dom.GetAllocator());

    Value panels;
    panels.SetArray();

    Value onePanel;
    onePanel.SetObject();
    Value cmd;
    cmd.SetString("Time Now", dom.GetAllocator());
    onePanel.AddMember("cmd", cmd, dom.GetAllocator());

    FILE* fp = NULL;
    char buf[4096] = { 0 };
    char result[4096] = { 0 };
    if ((fp = popen("python3 ./plugins/time/time.py", "r")) != NULL) {

        if (fgets(buf, 4096, fp) != NULL) {
            memcpy(result, buf, strlen(buf));
        }
        pclose(fp);
        fp = NULL;
    }

    Value argvs;
    argvs.SetArray();
    Value onearg;
    onearg.SetString(result, dom.GetAllocator());
    argvs.PushBack(onearg, dom.GetAllocator());
    onePanel.AddMember("argvs", argvs, dom.GetAllocator());
    Value info;
    info.SetString("Powered by Python3 time.py", dom.GetAllocator());
    onePanel.AddMember("info", info, dom.GetAllocator());
    panels.PushBack(onePanel, dom.GetAllocator());

    dom.AddMember("panels", panels, dom.GetAllocator());

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    dom.Accept(writer);
    const char* ret = sb.GetString();

    return string(ret);
}

string Test::realExec(std::string s)
{

    return "";
}
