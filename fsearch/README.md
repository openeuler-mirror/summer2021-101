# 文件搜索插件

## 搜索哪些文件

在dirs目录下，有searchDirs.txt文件，其中一行代表一个搜索的目录。

我们提供了文件搜索配置工具，可以在插件管理页面中找到它。

## 如何搜索文件

对全部搜索目录下的目录和文件与输入匹配，并提供xdg-open来打开文件。注意配置好您PC中的xdg-open工具默认的打开方式。

## 重要感谢
这里使用了著名C++开源json库rapidJson来构造与解析json数据,功能强大、效率很高、易于上手
[文档](http://rapidjson.org/zh-cn/)
[code](https://github.com/Tencent/rapidjson/)