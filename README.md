 测试海康SDK崩溃
 ===============
 
 1. 编译生成hkcrash.exe,根据测试需要编译x86或者x64
 
 2. 将hkcrash.exe拷贝到海康SDK的库文件目录内
 
 3. 运行hkcrash.exe，设置DVR地址，端口，用户名和密码
 
 4. 反复打开/关闭视频窗口，看HCCore.dll的加载地址。
 
 6.1.4.17 之前的版本，只要HCCore.dll的地址变动，就会crash。
 
 6.1.4.17 已解决了该问题。卸载HCCore.dll之后，也卸载了libeay32.dll。
 