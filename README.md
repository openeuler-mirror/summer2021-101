# Summer2021-No.101 在openEuler的UKUI桌面中增加一款快速启动的软件

#### 介绍
https://gitee.com/openeuler-competition/summer-2021/issues/I3OBSG


#### 安装教程

1. 下载本软件源码

   1. git clone https://gitee.com/openeuler-competition/summer2021-101.git
   2. cd summer2021-101

2. 编译准备

   1. 安装Gcc、G++、Qmake工具
   2. mkdir build-Launcher
   3. mkdir build-Launcher-emit
   4. mkdir ~/.Launcher
   5. mkdir ~/.Launcher/Launcher-emit
   6. mkdir ~/.Launcher/Launcher

3. 编译主窗体程序

   1. cd build-Launcher
   2. qmake-qt5 ../Launcher/Launcher.pro
   3. make
   4. cp -r Launcher ~/.Launcher/Launcher/
   5. cp -r  plugins/* ~/.Launcher/Launcher/ 
   6. 启动本程序即可

4. 编译触发器

   1. cd build-Launcher-emit
   2. qmake ../Launcher-emit/Launcher-emit.pro
   3. make
   4. cp -r Launcher-emit ~/.Launcher/Launcher-emit
   5. 设置系统快捷键触发这个Launcher-emit程序

5. 安装插件(以time为例)

   1. cd time
   2. make
   3. 在插件管理界面中，选择time/test/time.json即可安装该插件

   



#### 使用说明

所有说明均在doc目录中，可以前往查看。

[指路](./doc/overview.md)

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request



#### 致谢
感谢导师的指导
感谢Qt社区的优秀Qt框架
感谢使用到的rapidJson开源项目