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

    return "{    \"name\": \"urlSearch\", \
    \"introduction\": \"open browser to search \",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.2\",\
    \"keyword\": \"s\",\
    \"icon\": \"search.png\",\
    \"path\": \"./test\",\
    \"CMDmethod\": true,\
    \"website\" : \"https://gitlab.summer-ospp.ac.cn/summer2021/210010449\"}";
}

string Test::preExec(string s)
{

    Document dom;
    dom.SetObject();
    Value icon;
    icon.SetString("search.png", dom.GetAllocator());
    dom.AddMember("icon", icon, dom.GetAllocator());

    Value panels;
    panels.SetArray();

    vector<string> surl = { "https://www.google.com/search?q=", "https://cn.bing.com/search?q=", "https://www.baidu.com/s?wd=" };
    vector<string> sEngine = { "google", "bing", "baidu" };

    for (int i = 0; i < 3; ++i) {
        Value panelGoogle;
        panelGoogle.SetObject();
        Value cmd;
        cmd.SetString(sEngine.at(i).c_str(), dom.GetAllocator());
        panelGoogle.AddMember("cmd", cmd, dom.GetAllocator());
        Value argvs;
        argvs.SetArray();
        Value onearg;
        onearg.SetString((surl.at(i) + s).c_str(), dom.GetAllocator());
        argvs.PushBack(onearg, dom.GetAllocator());
        panelGoogle.AddMember("argvs", argvs, dom.GetAllocator());
        Value info;
        info.SetString("search", dom.GetAllocator());
        panelGoogle.AddMember("info", info, dom.GetAllocator());
        panels.PushBack(panelGoogle, dom.GetAllocator());
    }

    dom.AddMember("panels", panels, dom.GetAllocator());

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    dom.Accept(writer);
    const char* ret = sb.GetString();

    return string(ret);
}

string Test::realExec(std::string s)
{
    Document dom;
    dom.SetObject();

    Value status;
    status.SetBool(true);
    dom.AddMember("status", status, dom.GetAllocator());

    Value cmd;

    cmd.SetString("/usr/bin/xdg-open", dom.GetAllocator());

    dom.AddMember("cmd", cmd, dom.GetAllocator());

    Value argvs;
    argvs.SetArray();
    Value path;
    path.SetString(s.c_str(), dom.GetAllocator());
    argvs.PushBack(path, dom.GetAllocator());
    dom.AddMember("argvs", argvs, dom.GetAllocator());

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    dom.Accept(writer);
    const char* ret = sb.GetString();

    return ret;
}
