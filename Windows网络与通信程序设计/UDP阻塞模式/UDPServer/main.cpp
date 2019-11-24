#include <iostream>
#include <WinSock2.h>
#include <exception>
#include <string>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)throw exception("初始化Winsock库失败");
		cout << "初始化Winsock库成功" << endl;

		SOCKET so = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (so == INVALID_SOCKET)throw exception("创建套接字失败");
		cout << "创建套接字成功" << endl;

		sockaddr_in addrServer;
		addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
		addrServer.sin_family = AF_INET;
		addrServer.sin_port = htons(6789);
		if (bind(so, (sockaddr*)&addrServer, sizeof(sockaddr_in)) != 0)throw exception("绑定套接字失败");
		cout << "绑定套接字成功" << endl;

		sockaddr_in addrClient;
		int naddrClientSize = sizeof(sockaddr_in);
		char szRecvBuffer[256]{ 0 };
		int nRecvSize = recvfrom(so, szRecvBuffer, 256, 0, (sockaddr*)&addrClient, &naddrClientSize);
		if (nRecvSize <= 0)throw exception("接收数据失败");
		cout << "recv:" << szRecvBuffer << endl;

		string szHello = "This UDP server demo.";
		if (sendto(so, szHello.c_str(), szHello.length(), 0, (sockaddr*)&addrClient, naddrClientSize) != szHello.length())throw exception("发送数据失败");
		cout << "send:" << szHello.c_str() << endl;

		closesocket(so);
		so = INVALID_SOCKET;
		system("pause");
		WSACleanup();
		return 0;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
}