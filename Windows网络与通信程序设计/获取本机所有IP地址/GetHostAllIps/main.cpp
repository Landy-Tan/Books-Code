#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// 加载Winsock库
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 获取主机名称
	const unsigned short cunNameBufferSize = 256;
	char szName[cunNameBufferSize]{ 0 };
	int rst = ::gethostname(szName, cunNameBufferSize);
	std::cout << "Host name:" << szName << std::endl;

	// 通过主机名称获取IP地址
	std::cout << "Host ip address:" << std::endl;
	hostent *pHost = ::gethostbyname(szName);
	in_addr addr;
	for (int i = 0; ; i++)
	{
		char *p = pHost->h_addr_list[i];// 取得地址
		if (p == NULL)break;
		::memcpy(&addr.S_un.S_addr, p, pHost->h_length);// 转换成in_addr类型
		char *szIp = inet_ntoa(addr);// 字节顺序转换
		std::cout << szIp << std::endl;
	}
	system("pause");
	
	// 释放Winsock库
	WSACleanup();
	return 0;
}