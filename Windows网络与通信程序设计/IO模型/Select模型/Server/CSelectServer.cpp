#include "pch.h"
#include "CSelectServer.h"


CSelectServer::CSelectServer()
{
}


CSelectServer::~CSelectServer()
{
}

bool CSelectServer::InstallWinsock(int HVer, int LVer)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(HVer, LVer), &wsaData) != 0)return false;
	return true;
}

void CSelectServer::UninstallWinsock()
{
	WSACleanup();
}

bool CSelectServer::Create(const std::string & _szIp, const unsigned short _cunPort)
{
	try {
		m_soServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_soServer == INVALID_SOCKET)throw std::exception("´´½¨SOCKETÊ§°Ü");
		m_addrServer.sin_family = AF_INET;
		m_addrServer.sin_port = htons(_cunPort);
		m_addrServer.sin_addr.S_un.S_addr = (_szIp.empty() ? INADDR_ANY : inet_addr(_szIp.c_str()));
		if (bind(m_soServer, (sockaddr*)&m_addrServer, sizeof(sockaddr_in)) != 0)throw std::exception("°ó¶¨SOCKETÊ§°Ü");
		if (::listen(m_soServer, 5) != 0)throw std::exception("¼àÌýSOCKETÊ§°Ü");
		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}
}

void CSelectServer::Select(bool _isRead, bool _isWrite, bool _isException, unsigned int _unTimeout)
{
	try {
		timeval tv;
		tv.tv_sec = _unTimeout / 1000;
		tv.tv_usec = _unTimeout % 1000;

		fd_set fd;
		FD_ZERO(&fd);

		FD_SET(m_soServer, &fd);
		for (std::pair<SOCKET, sockaddr_in> pairSockAddr : m_lsClient)
			if (fd.fd_count < 64) { FD_SET(std::get<0>(pairSockAddr), &fd); }
		
		fd_set selFd = fd;
		int selRst = select(0, _isRead ? &selFd : NULL, _isWrite ? &selFd : NULL, _isException ? &selFd : NULL, &tv);

		for (int fdArrayIndex = 0; fdArrayIndex < fd.fd_count; fdArrayIndex++) {
			if (FD_ISSET(fd.fd_array[fdArrayIndex], &selFd)) {
				FD_CLR(fd.fd_array[fdArrayIndex], &selFd);
				if (fd.fd_array[fdArrayIndex] == m_soServer) {// Connect
					// Accept client connect requeit
					SOCKET soClient;
					sockaddr_in addrClient;
					int addrSize = sizeof(sockaddr_in);
					soClient = accept(m_soServer, (sockaddr*)&addrClient, &addrSize);
					if (soClient == INVALID_SOCKET)continue;
					m_lsClient.push_back(std::make_pair(soClient, addrClient));
					ConnectEvent(soClient, addrClient);
				}
				else {// Recvice or deconnect
					// Get client socket and sockaddr_in 
					std::list<std::pair<SOCKET, sockaddr_in>>::iterator _First = m_lsClient.begin();
					SOCKET soClient = INVALID_SOCKET;
					sockaddr_in addrClient;
					for (_First = m_lsClient.begin(); _First != m_lsClient.end(); _First++)
						if (std::get<0>(*_First) == fd.fd_array[fdArrayIndex]) { soClient = std::get<0>(*_First); addrClient = std::get<1>(*_First); break; }
					if (soClient == INVALID_SOCKET)continue;

					// Recvice client msg.
					const unsigned short cunRecviceBufferSize = 256;
					char szRecviceBuffer[cunRecviceBufferSize]{ 0 };
					std::string szRecvice;
					int nRecviceAllSize = 0;
					int nRecviceSize = 0;
					do {
						memset(szRecviceBuffer, 0, cunRecviceBufferSize);
						nRecviceSize = recv(soClient, szRecviceBuffer, cunRecviceBufferSize, 0);
						nRecviceAllSize += nRecviceSize;
						szRecvice += szRecviceBuffer;
					} while (nRecviceSize == cunRecviceBufferSize);
					
					if (nRecviceAllSize <= 0 && nRecviceSize <= 0) { CloseEvent(soClient, addrClient); m_lsClient.erase(_First); }// Recvice msg fail, is deconnect.
					else { RecvectEvent(soClient, addrClient, szRecvice.c_str()); }// Recvice msg success, is msg.
				}
			}
		}
	}
	catch (std::exception ex)
	{
	}
}

void CSelectServer::ConnectEvent(SOCKET _soClient, sockaddr_in _addrClient)
{
	std::cout << "Client " << inet_ntoa(_addrClient.sin_addr) << " connect." << std::endl;
}

void CSelectServer::CloseEvent(SOCKET _soClient, sockaddr_in _addrClient)
{
	std::cout << "Client " << inet_ntoa(_addrClient.sin_addr) << " deconnect." << std::endl;
}

void CSelectServer::RecvectEvent(SOCKET _soClient, sockaddr_in _addrClient, const char * const _pszMsg)
{
	std::cout << "Recvice client " << inet_ntoa(_addrClient.sin_addr) << " msg:" << _pszMsg << std::endl;
}
