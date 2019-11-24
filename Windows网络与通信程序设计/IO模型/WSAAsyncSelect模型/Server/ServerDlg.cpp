
// ServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_szMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)MessageBox(_T("初始化Winsock库失败"));

}

CServerDlg::~CServerDlg()
{
	::closesocket(m_soServer);
	WSACleanup();
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szMessage);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, OnSocketProc)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	try {
		m_soServer = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_soServer == INVALID_SOCKET)throw _T("创建SOCKET失败");
		m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
		m_addrServer.sin_port = htons(7896);
		m_addrServer.sin_family = AF_INET;
		if (::bind(m_soServer, (sockaddr*)&m_addrServer, sizeof(sockaddr_in)) != 0)throw _T("绑定套接字失败");
		if (::listen(m_soServer, 5) != 0)throw _T("监听套接字失败");
		WSAAsyncSelect(m_soServer, this->m_hWnd, WM_SOCKET, FD_ACCEPT);
	}
	catch (const LPTSTR pcszEx)
	{
		MessageBox(pcszEx);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CServerDlg::OnSocketProc(WPARAM wParam, LPARAM lParam)
{
	try {
		SOCKET soClient = wParam;
		sockaddr_in addrClient;
		std::list<std::pair<SOCKET, sockaddr_in>>::iterator _First;
		if (soClient != m_soServer) {
			for (_First = m_lsClient.begin(); _First != m_lsClient.end(); _First++) {
				if (std::get<0>(*_First) == soClient) {
					memcpy(&addrClient, &(std::get<1>(*_First)), sizeof(sockaddr_in));
					break;
				}
			}
		}

		int error = WSAGETSELECTERROR(lParam);
		if (error)
		{// 有错误发生
			m_szMessage.AppendFormat(_T("Error ID:%d\r\n"), error);
			UpdateData(FALSE);
			return -1;
		}
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
		{
			SOCKET soAccept;
			sockaddr_in addrAccept;
			int addrSize = sizeof(sockaddr_in);
			soAccept = ::accept(soClient, (sockaddr*)&addrAccept, &addrSize);
			if (soAccept == INVALID_SOCKET)throw _T("接受客户端连接请求失败");
			WSAAsyncSelect(soAccept, this->m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
			m_lsClient.push_back(std::make_pair(soAccept, addrAccept));
			m_szMessage.AppendFormat(_T("Client:%s connect.\r\n"), CString(inet_ntoa(addrAccept.sin_addr)));
		}
		break;
		case FD_READ:
		{
			std::string szRecviceMsg;
			const USHORT cunRecviceBufferSize = 256;
			char szRecviceBuffer[cunRecviceBufferSize]{ 0 };
			INT nRecviceAllSize = 0;
			INT nRecviceSize = 0;
			do {
				memset(szRecviceBuffer, 0, cunRecviceBufferSize);
				nRecviceSize = ::recv(soClient, szRecviceBuffer, cunRecviceBufferSize, 0);
				nRecviceAllSize += nRecviceSize;
				szRecviceMsg += szRecviceBuffer;
			} while (nRecviceSize == cunRecviceBufferSize);
			if (nRecviceAllSize > 0) {
				// 接收正常
				m_szMessage.AppendFormat(_T("Client:%s :%s\r\n"), CString(inet_ntoa(addrClient.sin_addr)), CString(szRecviceMsg.c_str()));
			}
			else {
				// 接收异常
				SendMessage(WM_SOCKET, wParam, FD_CLOSE);
			}
		}
			break;
		case FD_CLOSE:
		{
			m_lsClient.erase(_First);
			closesocket(soClient);
			m_szMessage.AppendFormat(_T("Client:%s deconnect.\r\n"), CString(inet_ntoa(addrClient.sin_addr)));
		}
			break;
		default:
			break;
		}
		UpdateData(FALSE);
		return 0;
	}
	catch (const LPTSTR pszEx)
	{
		MessageBox(pszEx);
		return -1;
	}
}

