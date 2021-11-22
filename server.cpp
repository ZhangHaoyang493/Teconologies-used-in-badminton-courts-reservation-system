#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <vector>

#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

using namespace std;

vector<sockaddr> clientIP;

char connectSuccessfully[] = "OK! Server connect successfully!";
char confirmMes[] = "Confirm Message!";
/*
void handleConfirm(SOCKET &sock, sockaddr clntAddr) {
    
    sendto(sock, connectSuccessfully, strlen(connectSuccessfully) + 1, 0, &clntAddr, sizeof(sockaddr));
}
*/
void handleMessage(char* buffer, char* IP, char* message) {
    int i, j = 0;

    for (i = 0; buffer[i] != ' ' && buffer[i]; i++)
        IP[i] = buffer[i];
    while (buffer[i] == ' ' && buffer[i])
        i++;
    for (i; buffer[i]; i++)
        message[j++] = buffer[i];
    message[j] = '\0';
}

int findIP(char* IP) {
    uint32_t IPnum = inet_addr(IP);

    for (int i = 0; i < clientIP.size(); i++) {
        if ((((sockaddr_in*)(&clientIP[i]))->sin_addr).s_addr == IPnum)
            return i;    
    }

    return -1;
}

int main(){
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定套接字
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;  //使用IPv4地址
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址。 inet_addr("192.168.1.111"); //获取IP地址
    servAddr.sin_port = htons(1234);  //端口
    bind(sock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
    cout << "Server is ready.." << endl;
    //接收客户端请求
    sockaddr clntAddr;  //客户端地址信息
    int nSize = sizeof(sockaddr);
    
    while(1){
        char buffer[BUF_SIZE] = {0};  //缓冲区
        char IP[BUF_SIZE] = {0}, message[BUF_SIZE] = {0};
        
        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
        cout << buffer << endl;
        if (strcmp(buffer, confirmMes) == 0) {
            sendto(sock, connectSuccessfully, strlen(connectSuccessfully) + 1, 0, &clntAddr, sizeof(sockaddr));
            clientIP.push_back(clntAddr);
        }

        else {
            handleMessage(buffer, IP, message);
            // 一定要注意+1
            sendto(sock, message, strlen(message) + 1, 0, &clientIP[findIP(IP)], nSize);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}