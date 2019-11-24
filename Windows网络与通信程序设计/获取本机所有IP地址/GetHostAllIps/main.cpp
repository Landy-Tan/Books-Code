#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// ����Winsock��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// ��ȡ��������
	const unsigned short cunNameBufferSize = 256;
	char szName[cunNameBufferSize]{ 0 };
	int rst = ::gethostname(szName, cunNameBufferSize);
	std::cout << "Host name:" << szName << std::endl;

	// ͨ���������ƻ�ȡIP��ַ
	std::cout << "Host ip address:" << std::endl;
	hostent *pHost = ::gethostbyname(szName);
	in_addr addr;
	for (int i = 0; ; i++)
	{
		char *p = pHost->h_addr_list[i];// ȡ�õ�ַ
		if (p == NULL)break;
		::memcpy(&addr.S_un.S_addr, p, pHost->h_length);// ת����in_addr����
		char *szIp = inet_ntoa(addr);// �ֽ�˳��ת��
		std::cout << szIp << std::endl;
	}
	system("pause");
	
	// �ͷ�Winsock��
	WSACleanup();
	return 0;
}