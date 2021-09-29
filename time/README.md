# 时间插件
这里使用管道调用Python程序输出来实现反馈，Python语言爱好者编写插件可以参考这个插件
```C++
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

```

```python
import time
import sys

if __name__ == "__main__":
    print(time.asctime(time.localtime(time.time()))+"\n")

```


## 重要感谢
这里使用了著名C++开源json库rapidJson来构造与解析json数据,功能强大、效率很高、易于上手
[文档](http://rapidjson.org/zh-cn/)
[code](https://github.com/Tencent/rapidjson/)