#include <iostream>
#include <WinSock2.h>
#include <cstring>
#include <thread>

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

using namespace std;

char connectSuccessfully[] = "OK! Server connect successfully!";

void sendMessage(char* IPcopy, char* buffer, char* IP, SOCKET &sock, sockaddr_in &servAddr) {
    while (1) {
        printf("Input the message: ");
        // gets(char* str)函数可以方便的向str中存入字符串
        gets(buffer);
        char* message = strcat(IPcopy, buffer);
        sendto(sock, message, strlen(message) + 1, 0, (sockaddr*)&servAddr, sizeof(servAddr));
        // 防止IPcopy改变，重新拷贝
        strcpy(IPcopy, IP);
    }
    
}

void receiveMessage(SOCKET &sock, char* buffer, sockaddr &fromAddr, int &addrLen) {
    while (1) {
        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &fromAddr, &addrLen);
        buffer[strLen] = 0;
        printf("Message you receive: %s\n", buffer);
        buffer[0] = '\0';
    }
    
}

void confirmConnect(sockaddr_in &servAddr, const SOCKET &sock) {
    sockaddr fromAddr;
    int addrLen = sizeof(fromAddr);

    char confirmMes[100] = "Confirm Message!";
    char receive[BUF_SIZE] = "";
    // 这里一定要记得加1，否则发过去的字符串结尾没有'\0' //
    int m = sendto(sock, confirmMes, strlen(confirmMes) + 1, 0, (struct sockaddr*)&servAddr, sizeof(sockaddr));
    int mesLen = recvfrom(sock, receive, BUF_SIZE, 0, &fromAddr, &addrLen);

    if (strcmp(connectSuccessfully, receive) == 0)
        cout << "Connect to server successfully!" << endl;   
}

int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    //服务器地址信息
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr("192.168.1.111");
    servAddr.sin_port = htons(1234);

    // 和服务器进行确认连接
    confirmConnect(servAddr, sock);


    //不断获取用户输入并发送给服务器，然后接受服务器数据
    sockaddr fromAddr;
    int addrLen = sizeof(fromAddr);
    char buffer[BUF_SIZE] = {0};

    thread receive(receiveMessage, ref(sock), buffer, ref(fromAddr), ref(addrLen));
    
    // 输入目的主机的IP地址
    char IP[BUF_SIZE] = "", IPcopy[BUF_SIZE] = "";
    cout << "Input the IP of destination host: ";
    gets(IP);
    strcat(IP, " ");
    strcpy(IPcopy, IP);

    thread send(sendMessage, IPcopy, buffer, IP, ref(sock), ref(servAddr));
    receive.join(); 
    send.join();


    /*while(1) {
        
        
        
        
        
        
    }*/
    
    closesocket(sock);
    WSACleanup();
    return 0;
}