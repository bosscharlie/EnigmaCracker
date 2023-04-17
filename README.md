# Enigma密码机破解

包含两种Rejweski和Turing的两种破解方式

* `Enigma.h`中定义Enigma类，用于模拟Enigma机器的运行和加密过程，在其中配置机器相关的转子及反射器等设置，实现了加密、状态更新等接口
* `EnigmaCracker.h`中配置破解过程中所需的输入，包括循环圈等信息
* `EnigmaCracker.cpp`中具体实现了两种针对Enigma机的破解方法，包括寻找循环圈及预计算等流程

程序运行方式：

```
// 编译生成可执行程序
make
// 运行破解过程
./main
```

得到破解结果，恢复转子顺序与初始设置：

```
Rejewski method:
Possible Solution: Rotors Order: 231 Initial Position: AAA
Turing method:
···（循环信息及初筛可能的密钥组合）
Thread for comb 321 Running Time: 11.968001
Thread for comb 312 Running Time: 11.969246
Thread for comb 123 Running Time: 11.978469
Final Possible Solution: Rotors Order: 231 Initial Position: AAA Letter for J:J
Thread for comb 231 Running Time: 11.988862
Thread for comb 213 Running Time: 12.29959
Thread for comb 132 Running Time: 12.39405
```