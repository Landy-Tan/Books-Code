#include <iostream>
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

		// 创建流式套接字
		SOCKET soServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (soServer == INVALID_SOCKET || soServer == SOCKET_ERROR)throw exception("创建流式套接字失败");
		cout << "创建流式套接字" << "成功" << endl;

		// 关联本地地址
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4567);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(soServer, (sockaddr*)&addr, sizeof(addr)) != 0)throw exception("关联本地地址失败");
		cout << "关联本地地址" << "成功" << endl;

		// 进入监听状态
		if (listen(soServer, 5) != 0)throw exception("进入监听状态失败");
		cout << "进入监听状态" << "成功" << endl;

		// 等待客户端连接
		SOCKET soClient;
		sockaddr_in addrClient{ 0 };
		int nAddrSize = sizeof(addrClient);
		soClient = accept(soServer, (sockaddr*)&addrClient, &nAddrSize);
		if (soClient == INVALID_SOCKET || soClient == SOCKET_ERROR)throw exception("等待客户端连接失败");
		cout << "客户端连接" << "成功" << endl;

		// 给客户端发送数据
		const static string szHello = "Hello, This TcpServer demo.";
		if (send(soClient, szHello.c_str(), szHello.length(), 0) != szHello.length())throw exception("给客户端发送数据失败");
		cout << "给客户端发送数据" << "成功" << endl;
		cout << "send:" << szHello.c_str() << endl;

		// 接收客户端数据
		char szRecvBuffer[256]{ 0 };
		int nRecvSize = 0;
		nRecvSize = recv(soClient, szRecvBuffer, 256, 0);
		if (nRecvSize <= 0)throw exception("接收客户端数据失败");
		cout << "接收客户端数据" << "成功" << endl;
		cout << "recv:" << szRecvBuffer << endl;

		// 关闭客户端套接字
		closesocket(soClient);

		// 关闭服务器套接字
		closesocket(soServer);

		system("pause");
		WSACleanup();
		return 0;
	}
	catch (const char * pEx)
	{
		std::cout << pEx << std::endl;
	}
}