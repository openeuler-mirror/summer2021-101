#include "test.hh"

Test::Test()
{
    char pwd[1024];
    getcwd(pwd, 1024);
    this->cwd = pwd;
    cwd = cwd + "/plugins/fsearch/dirs/";
    update();
}

void Test::update()
{
    getAllFiles();
    return;
}

string Test::getinfo()
{

    return "{    \"name\": \"fsearch\", \
    \"introduction\": \"search files\",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.6\",\
    \"keyword\": \"f\",\
    \"icon\": \"./File.png\",\
    \"path\": \"./test\",\
    \"CMDmethod\": true,\
    \"config\": \"./plugins/fsearch/fsearchConfig\",\
    \"website\" : \"https://gitlab.summer-ospp.ac.cn/summer2021/210010449\"}";
}

string Test::preExec(string s)
{
    if (s.compare("") == 0) {
        return "{\
                    \"icon\": \"icon.svg\",\
                    \"panels\": []\
                }\
                ";
    }

    //  构造回答
    Document dom;
    dom.SetObject();
    Value icon;
    icon.SetString("File.png", dom.GetAllocator());
    dom.AddMember("icon", icon, dom.GetAllocator());

    // 数量太多(>20)则不显示
    int count = 0;

    Value panels;
    panels.SetArray();

    for (auto f : this->dirs) {
        if (fSelect(f, s)) {
            Value onePanel;
            onePanel.SetObject();
            Value cmd;
            cmd.SetString("/usr/bin/xdg-open", dom.GetAllocator());
            onePanel.AddMember("cmd", cmd, dom.GetAllocator());

            Value argvs;
            argvs.SetArray();
            Value onearg;
            onearg.SetString(f.c_str(), dom.GetAllocator());
            argvs.PushBack(onearg, dom.GetAllocator());
            onePanel.AddMember("argvs", argvs, dom.GetAllocator());

            Value info;
            info.SetString("Powered by MarmaladeCat");
            onePanel.AddMember("info", info, dom.GetAllocator());

            panels.PushBack(onePanel, dom.GetAllocator());
            count++;
            if (count > 20) {
                panels.Clear();
                break;
            }
        }
    }

    for (auto f : this->files) {
        if (fSelect(f, s)) {
            Value onePanel;
            onePanel.SetObject();
            Value cmd;
            cmd.SetString("/usr/bin/xdg-open", dom.GetAllocator());
            onePanel.AddMember("cmd", cmd, dom.GetAllocator());

            Value argvs;
            argvs.SetArray();
            Value onearg;
            onearg.SetString(f.c_str(), dom.GetAllocator());
            argvs.PushBack(onearg, dom.GetAllocator());
            onePanel.AddMember("argvs", argvs, dom.GetAllocator());

            Value info;
            info.SetString("Powered by MarmaladeCat");
            onePanel.AddMember("info", info, dom.GetAllocator());

            panels.PushBack(onePanel, dom.GetAllocator());
            count++;
            if (count > 20) {
                panels.Clear();
                break;
            }
        }
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
    struct stat st;
    if (!stat(s.c_str(), &st)) {
        if (S_ISDIR(st.st_mode)) {
            cmd.SetString("/usr/bin/xdg-open", dom.GetAllocator());
        } else {
            cmd.SetString("/usr/bin/xdg-open", dom.GetAllocator());
        }
    }
    dom.AddMember("cmd", cmd, dom.GetAllocator());

    Value argvs;
    argvs.SetArray();
    Value path;
    path.SetString(s.c_str(), dom.GetAllocator());
    argvs.PushBack(path, dom.GetAllocator());
    dom.AddMember("argvs", argvs, dom.GetAllocator());

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    dom.Accept(writer); // Accept() traverses the DOM and generates Handler events.
    const char* ret = sb.GetString();

    return ret;
}

void Test::clearCache()
{
    // 清除缓存
    this->dirs.clear();
    this->files.clear();
}

void Test::getAllFolders()
{
    // 从文件中读取所有搜索目录
    ifstream in;
    in.open(cwd + "searchDirs.txt", ios::in);

    if (in.bad())
        return;

    string line;
    while (getline(in, line)) {
        if (line.find_first_of("#/") == string::npos)
            continue;
        line.erase(0, line.find_first_of("#/"));
        line.erase(line.find_last_not_of("\t\n\r /") + 1);

        if (line.at(0) == '#')
            continue;
        struct stat st;

        if (!stat(line.c_str(), &st)) {
            if (S_ISDIR(st.st_mode)) {
                this->dirs.push_back(line);
            }
        }
    }
    in.close();
}

// 缓存全部需要搜索的文件和目录
void Test::getAllFiles()
{
    clearCache();
    getAllFolders();

    for (int i = 0; i < this->dirs.size(); i++) {
        string dir = this->dirs[i];
        string target = dir + "/";
        DIR* d = opendir(target.c_str());
        struct dirent* dent = NULL;
        while ((dent = readdir(d)) != NULL) {
            if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0)) {
                if (dent->d_type == DT_DIR) {
                    this->dirs.push_back(dir + "/" + string(dent->d_name));

                } else {
                    this->files.push_back(dir + "/" + string(dent->d_name));
                }
            }
        }
        closedir(d);
    }
}

// 匹配，是否能够中选放入面板展示，以后可以改进这里，使插件更智能
bool Test::fSelect(string f, string s)
{
    int pos1 = f.rfind("/");
    int pos2 = f.size();
    if (pos1 == -1) {
        return false;
    }
    string fname = f.substr(pos1, pos2 - pos1);
    if (fname.find(s.c_str(), 0) != string::npos) {
        return true;
    } else {
        return false;
    }
}