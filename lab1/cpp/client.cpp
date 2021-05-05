//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ctime>
#else
#include <sys/time.h>
#endif
long long GetCurrentTimeMsec()
{
#ifdef _WIN32
	struct timeval tv;
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif
}

void Delay(int time)
{
	clock_t now = clock();
	while (clock() - now < time)
		;
}
#define BUFLEN 1000				   // 缓冲区大小
#pragma comment(lib, "ws2_32.lib") // 加载winsock 2.2 Llibrary

int connectAndSendto();
int main()
{
	printf("[info]\n开始udp丢包率测试\n");
	printf("=======================================\n");

	connectAndSendto();

	printf("运行结束，任意键退出。\n");
	getchar();
	return 0;
}

int connectAndSendto()
{
	struct sockaddr_in socksend;
	SOCKET sock;
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); //设置winsock版本2.2
	memset(&socksend, 0, sizeof(socksend));
	char host[] = "127.0.0.1";						 //目的地址
	u_short serverport = 30011;						 //目的地址端口
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
	socksend.sin_family = AF_INET;
	socksend.sin_port = htons(serverport);
	socksend.sin_addr.s_addr = inet_addr(host);
	char send_message[BUFLEN + 1];
	for (int i = 0; i < 10000; i++)
	{
		long long send_time = GetCurrentTimeMsec();
		sprintf(send_message, "UDP test id:%d,sendtime=%lld", i, send_time);
		int sendlen = sendto(sock, send_message, BUFLEN, 0, (SOCKADDR *)&socksend, sizeof(socksend));
		if (sendlen == SOCKET_ERROR)
		{
			printf("[error] 第%3d次发送失败", i);
		}
		printf("[info] 第%d次发送成功 发送内容: %s\n", i, send_message);
		//Delay(1);
	}
	closesocket(sock);
	WSACleanup();
	return 1;
}