# C-socket-programme
+ 程序分为客户端程序(client.cpp)和服务器程序(server.cpp)。
+ 这个程序利用C++套接字以及UDP协议实现了信息传输。
+ 程序运行时首先和服务器进行确认连接，然后要求用户输入目标主机的IP地址，然后可以进行发送消息。
+ 客户端程序使用两个线程，一个用于收消息，一个用于发消息。
+ 收消息线程一开始就开始执行，二发送消息的线程在用户指定目的IP地址后才开始执行。
+ 但是该程序目前还存在一些问题：
  + 首先，发现远距离的传输服务器收不到消息，不知道是发不出去还是延迟太大。
  + 其次，界面的排版不好。 
