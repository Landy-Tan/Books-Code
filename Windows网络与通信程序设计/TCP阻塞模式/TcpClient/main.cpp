#include <iostream>
#include <exception>
#include <string>
#include <WinSock2.h>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try {
		// 加载Winsock库
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)throw exception("加载Winsock库失败");
		cout << "加载Winsock库" << "成功" << endl;

		// 创建套接字
		SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (so == INVALID_SOCKET || so == SOCKET_ERROR)throw exception("创建套接字失败");
		cout << "创建套接字" << "成功" << endl;

		// 连接服务器
		sockaddr_in addr;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4567);
		if(connect(so, (sockaddr*)&addr, sizeof(addr)) != 0)throw exception("连接服务器失败");
		cout << "连接服务器" << "成功" << endl;

		// 接收服务器数据
		char szRecvBuffer[256]{ 0 };
		if (recv(so, szRecvBuffer, 256, 0) <= 0)throw exception("接收服务器数据失败");
		cout << "接收服务器数据" << "成功" << endl;
		cout << "recv:" << szRecvBuffer << endl;

		// 发送数据给服务器
		string szHello = "Hello, This TcpClient demo.";
		if (send(so, szHello.c_str(), szHello.length(), 0) != szHello.length())throw exception("发送数据给服务器失败");
		cout << "发送数据给服务器" << "成功" << endl;
		cout << "send:" << szHello.c_str() << endl;

		// 关闭套接字
		closesocket(so);


		system("pause");

		// 释放Winsock库
		WSACleanup();
		return 0;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
}