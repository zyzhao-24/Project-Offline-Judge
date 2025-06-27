# PKU Offline Judge

#### 介绍

**本仓库是北京大学程序设计实习课程大作业的代码仓库**

本项目是一个本地编程题考试及评测系统，目前仍处于开发阶段

#### 软件架构

本项目完全基于C++，使用Qt 6.9.0开发
仅兼容windows

#### 安装教程

解压即可，编译需求：
1. Qt版本高于6.9.0
2. 安装了MinGW编译器组件
3. 安装了对应版本的Qt PDF组件
4. 安装了对应版本的Qt Creator及Qt Linguist
5. 构建选项添加install

若希望正确运行本项目，应当：
1. 安装MINGW编译器及python 3解释器
2. 添加编译器/解释器目录到PATH下

项目发布提示：
1. 需要使用windeployqt6而非windeployqt
2. 需要手动复制Qt6PrintSupport.dll到构建目录下

#### 文件后缀名一览
1. 比赛文件：.ctinfo
2. 学生文件包：.sctinfo
3. 学生提交：.sspack

#### 使用说明
参阅大作业报告文件和TestlibInfo.md