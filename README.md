# SimulationBaiduNetwork
1234yuanwenting/SimulationBaiduNetwork
这是一个模拟百度网盘项目
设计分为服务器端设计+客户端测试
在Linux环境下，采用Tcp协议和多线程编程来完成上传，下载，查看，删除，重命名文件等功能，
通过strtok函数对接收的命令进行分割，解读客户端的意图，对于部分工作通过fork()出的子进程进行替换，
然后将子进程的结果返回给父进程，再返回给客户
