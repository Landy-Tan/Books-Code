#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try {
		// ����Winsock��
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)throw exception("����Winsock��ʧ��");
		cout << "����Winsock��" << "�ɹ�" << endl;

		// ������ʽ�׽���
		SOCKET soServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (soServer == INVALID_SOCKET || soServer == SOCKET_ERROR)throw exception("������ʽ�׽���ʧ��");
		cout << "������ʽ�׽���" << "�ɹ�" << endl;

		// �������ص�ַ
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4567);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(soServer, (sockaddr*)&addr, sizeof(addr)) != 0)throw exception("�������ص�ַʧ��");
		cout << "�������ص�ַ" << "�ɹ�" << endl;

		// �������״̬
		if (listen(soServer, 5) != 0)throw exception("�������״̬ʧ��");
		cout << "�������״̬" << "�ɹ�" << endl;

		// �ȴ��ͻ�������
		SOCKET soClient;
		sockaddr_in addrClient{ 0 };
		int nAddrSize = sizeof(addrClient);
		soClient = accept(soServer, (sockaddr*)&addrClient, &nAddrSize);
		if (soClient == INVALID_SOCKET || soClient == SOCKET_ERROR)throw exception("�ȴ��ͻ�������ʧ��");
		cout << "�ͻ�������" << "�ɹ�" << endl;

		// ���ͻ��˷�������
		const static string szHello = "Hello, This TcpServer demo.";
		if (send(soClient, szHello.c_str(), szHello.length(), 0) != szHello.length())throw exception("���ͻ��˷�������ʧ��");
		cout << "���ͻ��˷�������" << "�ɹ�" << endl;
		cout << "send:" << szHello.c_str() << endl;

		// ���տͻ�������
		char szRecvBuffer[256]{ 0 };
		int nRecvSize = 0;
		nRecvSize = recv(soClient, szRecvBuffer, 256, 0);
		if (nRecvSize <= 0)throw exception("���տͻ�������ʧ��");
		cout << "���տͻ�������" << "�ɹ�" << endl;
		cout << "recv:" << szRecvBuffer << endl;

		// �رտͻ����׽���
		closesocket(soClient);

		// �رշ������׽���
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