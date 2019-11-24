#include "pch.h"

// ���ݻ������Ĵ�С
#define WSA_BUFFER_LENGHT 512
typedef struct _tag_ClientContent {
	OVERLAPPED Overlapped; // �ص�IO�ṹ
	SOCKET sock;			// SOCKET���			
	SOCKADDR_IN addr;		// ��ַ
	WSABUF dataBuf;			// ���ݻ����������ڷ���/��������
	HANDLE hEvent;			// �¼�����
}CLIENTCONTENT, *LPCLIENTCONTENT;

int ListToEventArray(const std::list<LPCLIENTCONTENT> &_lsClientContent, HANDLE ** _pEvent)
{
	if (*_pEvent)return -1;
	int nSize = _lsClientContent.size();
	HANDLE *pHandle = new HANDLE[nSize];
	int nClientContentIndex = 0;
	for (LPCLIENTCONTENT pClientContent : _lsClientContent)
	{
		*(pHandle + nClientContentIndex) = pClientContent->hEvent;
		nClientContentIndex++;
	}
	*_pEvent = pHandle;
	return nSize;
}
LPCLIENTCONTENT GetClientContent(const std::list<LPCLIENTCONTENT> &_lsClientContent, const HANDLE & _hEvent)
{
	for (LPCLIENTCONTENT pClient : _lsClientContent)
		if (pClient->hEvent == _hEvent)return pClient;
	return NULL;
}

int main()
{
	WSADATA wsaData{ 0 };
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// �׽��ּ���
	std::list<LPCLIENTCONTENT> lsClientContent;

	// �������󶨲������׽���
	LPCLIENTCONTENT pListen = new CLIENTCONTENT;
	pListen->sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (pListen->sock == INVALID_SOCKET || pListen->sock == SOCKET_ERROR)return -1;
	pListen->addr.sin_family = AF_INET;
	pListen->addr.sin_port = htons(4567);
	pListen->addr.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(pListen->sock, (SOCKADDR*)&pListen->addr, sizeof(SOCKADDR_IN));
	::listen(pListen->sock, 5);

	// �������������¼�����
	pListen->hEvent = WSACreateEvent();
	pListen->Overlapped.hEvent = pListen->hEvent;

	// �����������˸���Ȥ�������¼�
	WSAEventSelect(pListen->sock, pListen->hEvent, FD_ACCEPT | FD_CLOSE);
	lsClientContent.push_back(pListen);

	while (true)
	{
		// �ȴ������¼�����
		HANDLE *pEvent = NULL;
		int nArraySize = ListToEventArray(lsClientContent, &pEvent);
		DWORD dwWaitForRst = WSAWaitForMultipleEvents(nArraySize, pEvent, FALSE,
			WSA_INFINITE, FALSE);
		if (dwWaitForRst == WSA_WAIT_FAILED ||
			dwWaitForRst == WSA_WAIT_IO_COMPLETION ||
			dwWaitForRst == WSA_WAIT_TIMEOUT) 
		{
			DWORD dwError = WSAGetLastError();
			delete[] pEvent;
			continue;
		}
		for (int nCurrentEventIndex = 0;
			nCurrentEventIndex < nArraySize;
			nCurrentEventIndex++)
		{
			// ��鵱ǰ�¼��Ƿ񱻴���
			DWORD dwRst = WSAWaitForMultipleEvents(1,
				&pEvent[nCurrentEventIndex],
				FALSE,
				10,
				FALSE);
			if (dwRst != WSA_WAIT_TIMEOUT && dwRst != WSA_WAIT_FAILED)
			{
				if (pEvent[nCurrentEventIndex] == pListen->hEvent)
				{// ���ܿͻ�������
					int nAddrSie = sizeof(sockaddr_in);
					LPCLIENTCONTENT pClient = new CLIENTCONTENT;
					pClient->sock = WSAAccept(pListen->sock,
						(sockaddr*)&pClient->addr,
						&nAddrSie,
						NULL,
						WSA_FLAG_OVERLAPPED);
					if (pClient->sock == INVALID_SOCKET ||
						pClient->sock == SOCKET_ERROR)
					{
						continue;
					}

					// �����ͻ����¼�
					pClient->hEvent = WSACreateEvent();
					pClient->Overlapped.hEvent = pClient->hEvent;
					
					// �����ͻ��˸���Ȥ�������¼�
					WSAEventSelect(pClient->sock,
						pClient->hEvent,
						FD_READ | FD_CLOSE);
					lsClientContent.push_back(pClient);
				}
				else {// ���տͻ����������ݣ�֧�ֽ��ճ���WSA_BUFFER_LENGHT������
					LPCLIENTCONTENT pClient = GetClientContent(lsClientContent,
						pEvent[nCurrentEventIndex]);
					if (pClient == NULL)continue;
					DWORD dwNumberOfBytesRecvd = 0;
					DWORD dwFlags = 0;
					DWORD dwTransfer = 0;
					pClient->dataBuf.buf = new char[WSA_BUFFER_LENGHT] {0};
					pClient->dataBuf.len = WSA_BUFFER_LENGHT;
					std::string szRecviceBuffer;
					DWORD dwRecviceSize = 0;
					do {
						memset(pClient->dataBuf.buf, 0, WSA_BUFFER_LENGHT);

						// ��������
						int nRecvRst = WSARecv(pClient->sock, &pClient->dataBuf,
							1,
							&dwNumberOfBytesRecvd,
							&dwFlags,
							&pClient->Overlapped,
							NULL);

						// �ȴ��������
						WSAGetOverlappedResult(pClient->sock,
							&pClient->Overlapped,
							&dwTransfer,
							TRUE,
							&dwFlags);
						dwRecviceSize += dwNumberOfBytesRecvd;
						szRecviceBuffer += std::string(pClient->dataBuf.buf);
					} while (dwNumberOfBytesRecvd == WSA_BUFFER_LENGHT);
					
					if (dwRecviceSize == 0)
					{// �Ͽ�����
						lsClientContent.remove(pClient);
						delete[] pClient->dataBuf.buf;
						closesocket(pClient->sock);
						WSACloseEvent(pClient->hEvent);
						delete pClient;
					}
					else {// �������ݣ�֧�ַ��ͳ���WSA_BUFFER_LENGHT������
						delete[] pClient->dataBuf.buf;
						pClient->dataBuf.len = szRecviceBuffer.length();
						pClient->dataBuf.buf = new char[pClient->dataBuf.len + 1]{ 0 };
						strcpy_s(pClient->dataBuf.buf, pClient->dataBuf.len + 1, szRecviceBuffer.c_str());
						DWORD dwNumberOfBytesSend = 0;

						// ��������
						int nSendRst = WSASend(pClient->sock,
							&pClient->dataBuf,
							1,
							&dwNumberOfBytesSend,
							0,
							&pClient->Overlapped,
							NULL);

						// �ȴ��������
						WSAGetOverlappedResult(pClient->sock,
							&pClient->Overlapped,
							&dwTransfer,
							TRUE,
							&dwFlags);

						// �����ڴ�
						delete[] pClient->dataBuf.buf;
						pClient->dataBuf.buf = NULL;
						pClient->dataBuf.len = 0;
					}
				}

				// ���õ�ǰ�¼�Ϊ���ź�״̬
				WSAResetEvent(pEvent[nCurrentEventIndex]);
			}
		}
		delete[] pEvent;
		pEvent = NULL;
	}

	system("pause");
	return 0;
}