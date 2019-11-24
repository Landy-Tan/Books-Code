#include "CEventSelect.h"


CEventSelect::CEventSelect(DWORD dwHighVersion, DWORD dwLowVersion)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(dwLowVersion, dwHighVersion), &wsaData) != 0)throw std::exception("Failed to load Winsock library.");
}

CEventSelect::~CEventSelect()
{
	closesocket(std::get<0>(tupServer));
	WSACloseEvent(std::get<2>(tupServer));
	for (std::tuple<SOCKET, sockaddr_in, HANDLE> tupSocket : lstupClient)
	{
		closesocket(std::get<0>(tupSocket));
		WSACloseEvent(std::get<2>(tupSocket));
	}
	WSACleanup();
}

BOOL CEventSelect::CreateServer(int af, USHORT usPort, ULONG ulAddr)
{
	SOCKET soServer = socket(af, SOCK_STREAM, IPPROTO_TCP);
	if (soServer == INVALID_SOCKET)return FALSE;
	sockaddr_in addrServer;
	addrServer.sin_family = af;
	addrServer.sin_port = usPort;
	addrServer.sin_addr.S_un.S_addr = ulAddr;
	if (::bind(soServer, (sockaddr*)&addrServer, sizeof(sockaddr_in)) != 0)return FALSE;
	if (::listen(soServer, 5) != 0) return FALSE;
	HANDLE hServer = WSACreateEvent();
	WSAEventSelect(soServer, hServer, FD_ACCEPT);
	tupServer = std::make_tuple(soServer, addrServer, hServer);
	lstupClient.push_back(tupServer);
	return TRUE;
}

VOID CEventSelect::Select(BOOL blRead, BOOL blWrite, BOOL blException, DWORD dwTimeOut)
{
	SOCKET *pSocket = NULL;
	HANDLE *pHandle = NULL;
	sockaddr_in *pAddr = NULL;
	int nArraySize = CEventSelect::TuplesToArray(&lstupClient, &pSocket, &pAddr, &pHandle);
	if (nArraySize > WSA_MAXIMUM_WAIT_EVENTS)return;
	if (WSAWaitForMultipleEvents(nArraySize, pHandle, FALSE, dwTimeOut, FALSE) == WAIT_TIMEOUT)return;
	for (int nCurentHandleIndex = 0; nCurentHandleIndex < nArraySize; nCurentHandleIndex++)
	{
		if (WSAWaitForMultipleEvents(1, pHandle + nCurentHandleIndex, FALSE, 5, FALSE) == WAIT_TIMEOUT)continue;
		WSANETWORKEVENTS NetworkEvents;
		if (WSAEnumNetworkEvents(*(pSocket + nCurentHandleIndex), *(pHandle + nCurentHandleIndex), &NetworkEvents) != 0)continue;
		if (NetworkEvents.lNetworkEvents & FD_READ &&  NetworkEvents.iErrorCode[FD_READ_BIT] == 0)
		{
			EventReceive(std::make_tuple(*(pSocket + nCurentHandleIndex), *(pAddr + nCurentHandleIndex), Receive(*(pSocket + nCurentHandleIndex))));
		}
		else if (NetworkEvents.lNetworkEvents & FD_ACCEPT && NetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0)
		{
			std::tuple<SOCKET, sockaddr_in, HANDLE> tupClient;
			if (!Accept(&tupClient))continue;
			lstupClient.push_back(tupClient);
			EventAccept(std::make_tuple(std::get<0>(tupClient), std::get<1>(tupClient)));
		}
		else if (NetworkEvents.lNetworkEvents & FD_CLOSE/* && NetworkEvents.iErrorCode[FD_CLOSE_BIT] == 0*/)// 无论是正常退出，还是退出，都应该执行相应的处理
		{
			for (std::list<std::tuple<SOCKET, sockaddr_in, HANDLE>>::iterator itBegin = lstupClient.begin();
				itBegin != lstupClient.end();
				itBegin++)
			{
				if (std::get<0>(*itBegin) == *(pSocket + nCurentHandleIndex))
				{
					EventDisconnect(std::make_tuple(*(pSocket + nCurentHandleIndex), *(pAddr + nCurentHandleIndex)));
					closesocket(*(pSocket + nCurentHandleIndex));
					WSACloseEvent(std::get<2>(*itBegin));
					lstupClient.erase(itBegin);
					break;
				}
			}
		}
	}
	delete[] pSocket;
	delete[] pHandle;
	delete[] pAddr;
}

INT CEventSelect::Send(SOCKET so, const std::string & cszMsg)
{
	return ::send(so, cszMsg.c_str(), cszMsg.length(), 0);
}

std::string CEventSelect::Receive(SOCKET so)
{
	int nTotalRecviceSize = 0;
	int nDisposableRecviceSize = 0;
	std::string szTotalRecviceBuffer;
	const WORD cwDisposableRecviceBufferSize = 256;
	char szDisposableRecviceBuffer[cwDisposableRecviceBufferSize];
	do {
		Sleep(20);
		memset(szDisposableRecviceBuffer, 0, cwDisposableRecviceBufferSize * sizeof(char));
		nDisposableRecviceSize = ::recv(so, szDisposableRecviceBuffer, cwDisposableRecviceBufferSize, 0);
		nTotalRecviceSize += nDisposableRecviceSize;
		szTotalRecviceBuffer += szDisposableRecviceBuffer;
	} while (nDisposableRecviceSize == cwDisposableRecviceBufferSize);
	
	return szTotalRecviceBuffer;
}

BOOL CEventSelect::Accept(std::tuple<SOCKET, sockaddr_in, HANDLE> *pClientInstance)
{
	SOCKET soClient;
	sockaddr_in addrClient;
	int addrSize = sizeof(sockaddr_in);
	HANDLE hClient;
	soClient = ::accept(std::get<0>(tupServer), (sockaddr*)&addrClient, &addrSize);
	if (soClient == INVALID_SOCKET)return FALSE;
	hClient = WSACreateEvent();
	WSAEventSelect(soClient, hClient, FD_READ | FD_CLOSE);
	if (pClientInstance)*pClientInstance = std::make_tuple(soClient, addrClient, hClient);
	return TRUE;
}

VOID CEventSelect::EventDisconnect(std::tuple<SOCKET, sockaddr_in> tupClient)
{
	std::cout << "Client :" << inet_ntoa(std::get<1>(tupClient).sin_addr) << " disconnect." << std::endl;
	return VOID();
}

VOID CEventSelect::EventAccept(std::tuple<SOCKET, sockaddr_in> tupClient)
{
	const std::string szIp = inet_ntoa(std::get<1>(tupClient).sin_addr);
	std::cout << "Client :" << inet_ntoa(std::get<1>(tupClient).sin_addr) << " connected." << std::endl;
	return VOID();
}

VOID CEventSelect::EventReceive(std::tuple<SOCKET, sockaddr_in, std::string> tupClient)
{
	std::cout << inet_ntoa(std::get<1>(tupClient).sin_addr) << ":" << std::get<2>(tupClient) << std::endl;
	return VOID();
}

int CEventSelect::TuplesToArray(const std::list<std::tuple<SOCKET, sockaddr_in, HANDLE>>* pTuples, SOCKET ** pSocket, sockaddr_in ** pAddr, HANDLE ** pHandle)
{
	int nArraySize = pTuples->size();
	SOCKET *pOutSocket = new SOCKET[nArraySize]{ 0 };
	sockaddr_in *pOutAddr = new sockaddr_in[nArraySize]{ 0 };
	HANDLE *pOutHandle = new HANDLE[nArraySize]{ 0 };

	int nCurrentTuplesIndex = 0;
	for (auto tup : *pTuples)
	{
		*(pOutSocket + nCurrentTuplesIndex) = std::get<0>(tup);
		*(pOutAddr + nCurrentTuplesIndex) = std::get<1>(tup);
		*(pOutHandle + nCurrentTuplesIndex) = std::get<2>(tup);
		nCurrentTuplesIndex++;
	}
	
	if (pSocket)*pSocket = pOutSocket;
	else delete[] pOutSocket;
	if (pAddr)*pAddr = pOutAddr;
	else delete[] pAddr;
	if (pHandle)*pHandle = pOutHandle;
	else delete[] pOutHandle;

	return nArraySize;
}
