#ifndef C_SELECT_SERVER_H
#define C_SELECT_SERVER_H

#include <WinSock2.h>
#include <tuple>
#include <list>
#include <string>
#include <thread>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

class CSelectServer
{
public:
	CSelectServer();
	virtual ~CSelectServer();

public:// Winsock�����
	static bool InstallWinsock(int HVer, int LVer);
	static void UninstallWinsock();

public:// ����SOCKET����������¼�
	bool Create(const std::string & _szIp, const unsigned short _cunPort);
	void Select(bool _isRead, bool _isWrite, bool _isException, unsigned int _unTimeout = 3000);

protected:// �����¼����Ͽ������¼��������¼�
	virtual void ConnectEvent(SOCKET _soClient, sockaddr_in _addrClient);
	virtual void CloseEvent(SOCKET _soClient, sockaddr_in _addrClient);
	virtual void RecvectEvent(SOCKET _soClient, sockaddr_in _addrClient, const char * const _pszMsg);

protected:
	SOCKET m_soServer;
	sockaddr_in m_addrServer;
	std::list<std::pair<SOCKET, sockaddr_in>> m_lsClient;
};

#endif // C_SELECT_SERVER_H