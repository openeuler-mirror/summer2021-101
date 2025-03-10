# 插件开发规范

插件开发应当继承于`plugins_def/include/BasePlugin.hh`基类，并在此基础上实现其中的三个接口函数：

- string getinfo()
- string preExec(string s)
- string realExec(string s)

在此基础上，将派生类生成动态链接库，即可形成插件供主程序使用。

## 插件基本信息

保存于json文件中。格式规范如下

```json
{
    "name": "Hello_py",
    "keyword": "hel",
    "version": "1.2.3",
    "cmd": true,
}
```

name字段为插件名字，区别不同插件。

## 插件信息接口

此字段为getinfo的返回值，json格式，基本模板如下

```json
{
    "name": "Hello py",
    "introduction": "a hello world of python",
    "author": "MarmaladeCat",
    "version": "1.2.1",
    "keyword": "hel",
    "icon": "./Hello.png",
    "path": "./test",
    "CMDmethod": true,
    "website": "https://gitlab.summer-ospp.ac.cn/summer2021/210010449"
}
```

该信息展示于插件管理界面

## 插件预执行接口

此接口为preExec使用，传入参数为用户输入，返回参数用于匹配选项面板，模板如下：

```json
{
    "icon": "icon.svg",
    "panels": [{
            "cmd": "/usr/bin/python3.8",
            "argvs": [
                "Hello.py",
                "1234"
            ],
            "info": "Powered by MarmaladeCat"
        },
        {
            "cmd": "/usr/bin/gedit",
            "argvs": [
                "Hello.py"
            ],
            "info": "Powered by HHH"
        }
    ]
}
```

panels字段为一个列表，主程序将为每一项生成一个匹配面板。

## 插件执行接口

此接口为realExec使用，传入为预执行返回的argvs转空格间隔字符串，返回json模板：

```json
{
    "status": true,
    "cmd": "/usr/bin/python3.8",
    "argvs": ["Hello.py", "1234"]
}
```

如若插件基本信息中的cmd为true，则此处会对返回值作处理，产生一个新的进程并执行相关命令和参数。

# 插件开发举例

test.hh文件中声明一个子类，继承于BasePlugin

```c++
#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include <unistd.h>

class Test : public BasePlugin {
public:
    virtual string getinfo();

    virtual string preExec(string s);

    virtual string realExec(string s);
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
```

test.cc文件中，实现三个虚函数



```c++
#include "test.hh"

string Test::getinfo()
{
    return "{    \"name\": \"Hello_py\", \
    \"introduction\": \"a hello world of python\",\
    \"author\": \"MarmaladeCat\",\
    \"version\": \"1.2.1\",\
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
    std::cout << s << std::endl;

    // 允许相对路径

    return "{\"status\": true,\
    \"cmd\": \"/usr/bin/python3.8\",\
    \"argvs\": [\"Hello.py\", \"1234\"]}";
}

```

编译生成动态链接库，即可完成

```shell
$ tree ./
./
├── exampleJsons
│   ├── getinfo.example.json
│   ├── preExec.example.json
│   └── realExec.example.json
├── Hello.png
├── include
│   ├── BasePlugin.hh
│   └── test.hh
├── Makefile
├── README.md
├── test
│   ├── a.out
│   ├── a.txt
│   ├── Hello.json
│   ├── Hello.png
│   ├── Hello.py
│   └── res
│       ├── images
│       │   └── go.png
│       └── main.cc
└── test.cc

# 目标文件为 插件名字.插件版本.so
$ cat Makefile 
TARGET_DIR      = test
TARGET          = $(TARGET_DIR)/Hello_py.1.2.1.so

CC                      = g++ -std=c++11
LIB                     =
LDFLAGS         =
DEFINES         =
INCLUDE         = -I./include
CFLAGS          =  $(DEFINES) $(INCLUDE)
CXXFLAGS        = $(CFLAGS)
SHARE           = -fPIC -shared -o

OBJ_DIR         = ./obj
OBJ                     = $(patsubst %.cc, $(OBJ_DIR)/%.o, $(wildcard *.cc))

RM                      = rm -rf

all : $(TARGET)

objs : $(OBJ)

clean :
        @echo Cleaning....
        @if [ -d $(OBJ_DIR) ]; then \
                $(RM) $(OBJ_DIR) ;\
        fi
        @if [ -d $(TARGET_DIR) ]; then \
                rm -rf  $(TARGET) ;\
        fi
        @echo Cleaning.....finished!
        @echo


$(OBJ_DIR)/%.o : %.cc
        @mkdir -p $(OBJ_DIR)
        @echo Compiling: $< 
        $(CC) $(CFLAGS) -c $< $(SHARE) $@ 
        @echo

$(TARGET) : $(OBJ)
        @mkdir -p $(TARGET_DIR)
        @echo Compiling: $@
        $(CC) $(CXXFLAGS) $(SHARE) $@ $(OBJ) $(LDFLAGS) $(LIB)
        
$ make
Compiling: test.cc
g++ -std=c++11  -I./include -c test.cc -fPIC -shared -o obj/test.o 

Compiling: test/Hello_py.1.2.1.so
g++ -std=c++11  -I./include -fPIC -shared -o test/Hello_py.1.2.1.so  ./obj/test.o

# 将必要的文件放入目标目录中，
$ tree test
test
├── a.out
├── a.txt
├── Hello.json
├── Hello.png
├── Hello.py
├── Hello_py.1.2.1.so
└── res
    ├── images
    │   └── go.png
    └── main.cc
    
# 并撰写插件基本信息
$ cat Hello.json 
{
    "name": "Hello_py",
    "keyword": "hel",
    "version": "1.2.3",
    "cmd": true,
    "elf.so": "Hello_py.1.2.3.so"
}
# 安装时，选中该文件，将安装该目录下的全部文件
```





