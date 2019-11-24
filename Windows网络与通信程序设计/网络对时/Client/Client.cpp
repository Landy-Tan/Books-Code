// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"


void Output(const string & _szMsg)
{
	cout << _szMsg.c_str() << endl;
}

SYSTEMTIME GetDateTIme(unsigned long ulTime)
{
	FILETIME ft;
	SYSTEMTIME st;
	st.wYear = 1900;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;
	SystemTimeToFileTime(&st, &ft);
	long long *pll = (long long *)&ft;
	*pll += (long long)10000000 * ulTime;
	FileTimeToSystemTime(&ft, &st);
	return st;
}

int main()
{
	try {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)throw exception("初始化Winsock库失败");
		Output("初始化Winsock库成功");

		SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (so == INVALID_SOCKET)throw exception("创建套接字失败");
		Output("创建套接字成功");

		sockaddr_in addr;
		addr.sin_addr.S_un.S_addr = inet_addr("129.6.15.29");
		addr.sin_port = htons(37);
		addr.sin_family = AF_INET;
		if (connect(so, (sockaddr*)&addr, sizeof(addr)) != 0)throw exception("连接服务器失败");
		Output("连接服务器成功");


		unsigned long ulTime = 0;
		int nRecvSize = recv(so, (char*)&ulTime, sizeof(unsigned long), 0);
		if (nRecvSize <= 0)throw exception("接收数据失败");
		cout << "recv:" << ulTime << endl;
		SYSTEMTIME sysTime = GetDateTIme(ulTime);
		cout << "年 :" << sysTime.wYear << endl;
		cout << "月 :" << sysTime.wMonth << endl;
		cout << "日 :" << sysTime.wDay << endl;
		cout << "时 :" << sysTime.wHour << endl;
		cout << "分 :" << sysTime.wMinute << endl;
		cout << "秒 :" << sysTime.wSecond << endl;
		cout << "毫秒 :" << sysTime.wMilliseconds << endl;
		system("pause");
		closesocket(so);
		WSACleanup();
		return 0;
	}
	catch (exception ex)
	{
		Output(ex.what());
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
