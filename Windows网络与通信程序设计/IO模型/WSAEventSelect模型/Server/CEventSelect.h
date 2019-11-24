#ifndef C_EVENT_SELECT_H
#define C_EVENT_SELECT_H

#include <list>
#include <tuple>
#include <exception>
#include <string>
#include <thread>
#include <process.h>
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

class CEventSelect
{
public:
	CEventSelect(DWORD dwHighVersion, DWORD dwLowVersion);
	virtual ~CEventSelect();

public:
	BOOL CreateServer(int af, USHORT usPort, ULONG ulAddr);
	VOID Select(BOOL blRead, BOOL blWrite, BOOL blException, DWORD dwTimeOut);

protected:
	INT Send(SOCKET so, const std::string &cszMsg);
	std::string Receive(SOCKET so);
	BOOL Accept(std::tuple<SOCKET, sockaddr_in, HANDLE> *pClientInstance);

protected:
	virtual VOID EventDisconnect(std::tuple<SOCKET, sockaddr_in> tupClient);
	virtual VOID EventAccept(std::tuple<SOCKET, sockaddr_in> tupClient);
	virtual VOID EventReceive(std::tuple<SOCKET, sockaddr_in, std::string> tupClient);

protected:
	static int TuplesToArray(const std::list<std::tuple<SOCKET, sockaddr_in, HANDLE>> * pTuples, SOCKET ** pSocket, sockaddr_in ** pAddr, HANDLE **pHandle);

protected:
	std::list<std::tuple<SOCKET, sockaddr_in, HANDLE>> lstupClient;
	std::tuple<SOCKET, sockaddr_in, HANDLE> tupServer;
};

#endif // C_EVENT_SELECT_H