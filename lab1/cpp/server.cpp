#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <regex>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define BUFLEN 1000
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

void makeNewMsg(char *msg, char *timestr, struct sockaddr_in from);
int main()
{
	u_short port = 30011;
	struct sockaddr_in sock_in;
	struct sockaddr_in from;
	int fromsize = sizeof(from);
	SOCKET sock;
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&sock_in, 0, sizeof(sock_in));
	sock_in.sin_port = htons(port);
	sock_in.sin_family = AF_INET; //指定ipv4
	sock_in.sin_addr.s_addr = INADDR_ANY;
	bind(sock, (struct sockaddr *)&sock_in, sizeof(sock_in));

	time_t now = time(NULL);
	printf("[info] UDP测试服务端启动 \n服务器启动时间：%s", ctime(&now));

	char buf[BUFLEN + 1];
	int success_count = 0;
	regex send_time_expr(".*id:(.*?),.*sendtime=(.*?)");
	while (1)
	{
		int recvlen = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, &fromsize);
		long long recv_time = GetCurrentTimeMsec();
		time_t now = time(NULL);
		char *timestr = ctime(&now);
		buf[recvlen] = '\0';
		string recv_msg(buf);
		std::smatch match_msg;
		long long send_time;
		regex_match(recv_msg, match_msg, send_time_expr);
		if (match_msg.size() > 1)
		{
			std::stringstream strstream;
			strstream << match_msg[2].str();
			strstream >> send_time;
			printf("收到第%d条消息：%s\n", success_count++, recv_msg.c_str());
			printf("该条消息的发送-接收时间差为: %lld ms\n", recv_time - send_time);
			if (match_msg[1].str() == "9999")
				break;
		}
	}
	closesocket(sock);
	WSACleanup();
	printf("接收到的数据包数量：%d\n丢包率：%.3f%%\n", success_count, 1.0f * (100 - success_count));
	printf("服务端运行结束\n");
	getchar();
	return 0;
}
