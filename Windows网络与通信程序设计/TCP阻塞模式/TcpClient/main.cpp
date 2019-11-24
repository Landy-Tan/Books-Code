#include <iostream>
#include <exception>
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

		// �����׽���
		SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (so == INVALID_SOCKET || so == SOCKET_ERROR)throw exception("�����׽���ʧ��");
		cout << "�����׽���" << "�ɹ�" << endl;

		// ���ӷ�����
		sockaddr_in addr;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4567);
		if(connect(so, (sockaddr*)&addr, sizeof(addr)) != 0)throw exception("���ӷ�����ʧ��");
		cout << "���ӷ�����" << "�ɹ�" << endl;

		// ���շ���������
		char szRecvBuffer[256]{ 0 };
		if (recv(so, szRecvBuffer, 256, 0) <= 0)throw exception("���շ���������ʧ��");
		cout << "���շ���������" << "�ɹ�" << endl;
		cout << "recv:" << szRecvBuffer << endl;

		// �������ݸ�������
		string szHello = "Hello, This TcpClient demo.";
		if (send(so, szHello.c_str(), szHello.length(), 0) != szHello.length())throw exception("�������ݸ�������ʧ��");
		cout << "�������ݸ�������" << "�ɹ�" << endl;
		cout << "send:" << szHello.c_str() << endl;

		// �ر��׽���
		closesocket(so);


		system("pause");

		// �ͷ�Winsock��
		WSACleanup();
		return 0;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
}