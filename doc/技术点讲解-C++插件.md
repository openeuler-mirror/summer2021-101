# C++插件

这里主要讲解C++插件的开发方法

## 定义虚基类

```C++
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
```

这里定义了基类BasePlugin，然后定义了内部的多个虚函数，相当于定义了整个插件的接口

## 定义插件类

```C++
#ifndef __TEST_HH__
#define __TEST_HH__

#include "BasePlugin.hh"
#include <unistd.h>

class Test : public BasePlugin {
public:
    virtual string getinfo();

    virtual string preExec(string s);

    virtual string realExec(string s);

    virtual void update();
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

```C++
#include "test.hh"

void Test::update()
{
    return;
}

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
                \"Hello World!\"\
            ],\
            \"info\": \"Powered by MarmaladeCat\"\
        } \
    ]\
}\
";
}

string Test::realExec(std::string s)
{
    // 允许相对路径

    return "{\"status\": true,\
    \"cmd\": \"/usr/bin/python3.8\",\
    \"argvs\": [\"Hello.py\", \"Hello World!\"]}";
}
```

这里定义了具体的插件类Test，继承于虚基类BasePlugin，并对其中的虚函数作了实际定义。这样确保了我们可以实例化一明为BasePlugin类实为Test类的插件，调用时候，调用的是子类Test中的各个函数，从而实现了插件的需求。

## 程序编译

使用`-fPIC -shared -o `生成动态链接库

## 示例

文件组织方式：

```shell
$ tree ./
./
├── exampleJsons
│   ├── getinfo.example.json
│   ├── preExec.example.json
│   └── realExec.example.json
├── include
│   ├── BasePlugin.hh
│   └── test.hh
├── Makefile
├── README.md
├── test
│   ├── a.txt
│   ├── Hello.json
│   ├── Hello.png
│   ├── Hello.py
│   ├── icon.svg
│   └── res
└── test.cc
```



MakeFile：

```makefile
TARGET_DIR 	= test
TARGET  	= $(TARGET_DIR)/Hello_py.1.2.1.so

CC			= g++ -std=c++11
LIB			=
LDFLAGS 	=
DEFINES 	=
INCLUDE 	= -I./include
CFLAGS  	=  $(DEFINES) $(INCLUDE)
CXXFLAGS	= $(CFLAGS)
SHARE   	= -fPIC -shared -o

OBJ_DIR 	= ./obj
OBJ			= $(patsubst %.cc, $(OBJ_DIR)/%.o, $(wildcard *.cc))

RM 			= rm -rf

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
```













