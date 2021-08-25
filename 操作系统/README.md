# Exp1-Scheduler

## Introduction
该项目为**电子科技大学(UESTC)计算机学院**操作系统课程实验内容，包括实验 1 的代码和实验 1、2 的实验报告。

## 基本内容

该项目包含两个文件，分别为：
- `Exp1-Scheduler.py` （代码文件，程序运行的完整代码）
- `input.txt` （指令文件，程序可以从该文件中读取指令）


## 运行

默认的运行方式为`读取文件指令方式`，即从`input.txt`中读取指令并执行。`input.txt`中存放了实验指导书中的测试样例，也可对其进行修改。

在代码第`342`行为程序的主函数，提供了了两种运行方式，分别为`终端控制方式`和`读取文件指令方式`，其中，默认注释掉了前者，如需切换，需手动注释掉后者，即下列代码段中的内容：

```python
    # =============读取文件方法=============
    with open('input.txt') as f:
        instruction_list=[]
        instruction = f.readline()
        while instruction:
            instruction_list.append(instruction)
            instruction = f.readline()
    for instruction in instruction_list:
        command=instruction.split()
    # =============读取文件方法=============
```

并取消注释下列代码段中的内容：
```python
    # =============终端输入方法=============
    # while(True):
    #     command=input("shell>").split()
    # =============终端输入方法=============
```
然后重新运行，即为`终端控制方式`，可以通过控制台输入指令控制程序。


## 指令列表
|指令|功能|
|---:|---|
|`init`|创建初始化进程|
|`cr PID priority`|新建进程，PID为进程名，priority为进程优先级
|`de PID`|撤销进程，PID为进程名
|`pr PID`|显示PCB信息，PID为进程名
|`to`|当前进程超时
|`req Rn amount`|当前进程请求资源，n在[1,2,3,4]中取值，amount为请求的量
|`rel Rn amount`|当前进程释放资源，n在[1,2,3,4]中取值，amount为请求的量
|`list ready`|显示就绪队列
|`list block`|显示阻塞队列
|`list res`|显示资源剩余情况

其中，资源名为大写`R+数字`，需注意大小写。
无效的指令会有错误提示，可根据提示调整指令格式。


## 其它

本实验在测试范围内完全符合实验要求。  
如有其它疑问，请联系作者（QQ：1103284182）