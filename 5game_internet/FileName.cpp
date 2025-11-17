#define WIN32_LEAN_AND_MEAN  // 阻止 Windows.h 引入旧版 winsock.h 等冗余组件
#define _WIN32_WINNT 0x0601  // 目标 Windows 7 及以上（确保 Filter 等类型被声明）

#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<math.h>
#include<graphics.h>
//#include<Windows.h>
#include<mmsystem.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//#define MAX 44000000;
//#define MIN -88000000;






#pragma comment(lib,"winmm.lib")


int type;
int qiju[15][15];
MOUSEMSG Msg;
IMAGE black;
IMAGE white;
int deter = 0;
const int M = 6;
const int MAX = 44000000;
const int MIN = -88000000;



#include"jizhun.h"

#include"client1.h"
//#include"server.h"
#include"server1.h"


#include"tcp_client.h"
#include"tcp_server.h"
#include"tcp_comm.h"




#include<Windows.h>



int main()
{

	int AIpower= -1;
	int luruwendang=0;
	char lianjie[100] = { 0 };
	int xianshou=0;
	int zhujifou = 0;
	FILE* qipu;
	fopen_s(&qipu,"qipu\\lj.txt", "w");

	
beginer:
	mciSendString("close mymusic", NULL, 0, NULL);
	mciSendString("open music/001.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("play mymusic", NULL, 0, NULL);
	xianshou = 0;
	deter = 0;
	initqipann();
	loadimage(&black, "black.png", 32, 32);
	loadimage(&white, "white.png", 32, 32);
	initGAME(1);
	
	while (1)
	{
		Msg = GetMouseMsg();
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 450 && Msg.y>380)
		{
			printf("what");
			type = 1;
			
			break;
		}
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 530 && Msg.y>460)
		{
			type = 2;
			initGAME(0);
			if (xianshou=xianhoushou())
			{
				printf("aaaaaaa\n");
			}
			initGAME(2);
			if (AIpower = xianhoushou())
			{
				printf("AI is loaded");
			}
			printf("why");
			break;
		}
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 610 && Msg.y>540)
		{
			printf("whose");
			type = 5;
			InputBox(lianjie, 100, "请输入相对路径：", "文件导入", NULL, 0, 0, false);
			printf("\n\n%c %c", lianjie[15], lianjie[16]);
			if (lianjie[16] != '.'&&lianjie[15]!='.')
			{
				settextcolor(BLACK);
				settextstyle(512, BLACK, _T("宋体"));
				outtextxy(0,256,"错误");
				mciSendString("close mymusic", NULL, 0, NULL);
				mciSendString("open music/toggle4.mp3 alias mymusic", NULL, 0, NULL);
				mciSendString("play mymusic", NULL, 0, NULL);
				Sleep(10000);
				goto beginer;
			}
			fopen_s(&qipu, lianjie, "r");
			break;
		}
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 690 && Msg.y>620)
		{
			printf("whereby");
			type = 6;
			initGAME(3);
			if (zhujifou = xianhoushou())
			{
				printf("kai!!!!!%d\n",zhujifou);
				//进房时zhujifou=1，开房时=0
			}
			initGAME(4);
			//getchar();
			break;
		}
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 880 && Msg.x < 1024 && Msg.y < 1024 && Msg.y>955)
		{
			type = 0;
			printf("how");
			break;
		}
	}
	if (type != 0)
	{
		mciSendString("close mymusic", NULL, 0, NULL);
		initqipan();
		if (type != 5)
		{
			qipu = openwrite();
		}
	}
	else
	{
		return 0;
	}
	IMAGE beijing;
	loadimage(&beijing, "beijing.png", 250, 60);
	int xx;
	int yy;
	int xnum,ynum;
	int* xpp = &xnum;
	int* ypp = &ynum;
	int* xppp = &xx;
	int* yppp = &yy;

	if (type == 5)
	{
		if (luruwendang = duizhanqieru(qipu) == 256)
		{
			goto failer;
		}
		else
		{
			settextstyle(64, BLACK, _T("宋体"));
			outtextxy(224, 930, "录入结束，对战继续");
			type = 1;
		}
		if (luruwendang == 1)
		{
			putimage(400, 12, &beijing);
			printf("%d\n", deter);
			settextstyle(64, BLACK, _T("宋体"));
			outtextxy(400, 10, "白方落子");
			goto huisu;
		}
		if (luruwendang == 2)
		{
			goto huisu1;
		}
		else
		{
			goto beginer;
		}
	}
	if (xianshou)
	{
		yinyue();
		qiju[7][7] = 1;
		putimage(111 + 7 * 59 - 16, 74 + 7 * 60 - 16, &black);
		if (type != 5)
		{
			writein(7, 7, 1, qipu);
		}
	}

	if (type == 6)
	{
		int jskfjj = 1;
		/*while(1)*/
		if (zhujifou)
		{
			int yuiaisdas = 1;
			auto firstRoom = findOutRoom();
			initGAME(4);

			// 检查是否找到房间
			if (!firstRoom.first.empty() && !firstRoom.second.empty()) {
				std::cout << "连接到第一个房间：IP=" << firstRoom.first
					<< ", 端口=" << firstRoom.second << std::endl;
				// 此处可添加TCP连接逻辑
			}
			else {
				std::cout << "未找到可连接的房间" << std::endl;
			}
			//getchar();
			const char* tcp_ip = firstRoom.first.c_str();
			int port = std::stoi(firstRoom.second);
			socket_t client_fd = tcp_connect_server(tcp_ip,port);
			

			initGAME(4);
		}
		else
		{
			int hsiudhaohd = 1;
			/*openMyRoom();*/


			if (g_roomThread.joinable()) {
				std::cout << "游戏房间线程已在运行，无需重复创建" << std::endl;
			}
			else {
				// 重置全局指针并创建线程
				if (g_roomListener != nullptr) {
					delete g_roomListener;
					g_roomListener = nullptr;
				}
				g_roomThread = std::thread(openMyRoom);
				std::cout << "游戏房间线程已启动" << std::endl;
			}
			initGAME(4);
			getchar();

			closeMyRoomThread();  // 退出前确保线程关闭
			std::cout << "主程序退出" << std::endl;

			int fuchhh;
			fuchhh = 111;

			getchar();
		}
		goto beginer;
	}
	

	while (1)
	{
		
		putimage(400, 12, &beijing);
		printf("%d\n", deter);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 10, "白方落子");
		/*putimage(400, 12, &beijing);*/
		/*setbkcolor(getpixel(400, 10));
		outtextxy(400, 10, "        ");*/
		
	huisu:
		while (1)
		{
			Msg = GetMouseMsg();
			if (Msg.uMsg == WM_LBUTTONDOWN)
			{
				xx = Msg.x;
				yy = Msg.y;

				break;
			}
		}
		/*printf("%d %d\n", xx, yy);*/
		if (luru(2, xx, yy,xpp,ypp))
		{
			goto huisu;
		}
		determine(xnum, ynum, 2);
		if (type != 5)
		{
			writein(xnum, ynum, 2, qipu);
		}
		/*if(type=5)*/
		/*printf("duandain%d %d\n",xnum,ynum);
		printf("luozi%d %d", xx, yy);*/
		/*printf("1deter=%d\n", deter);*/
		if (deter != 0)
		{
			break;
		}
		mciSendString("close mymusic", NULL, 0, NULL);
		yinyue();
	huisu1:
		putimage(400, 12, &beijing);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 10, "黑方落子");
		switch (type)
		{
		case 1:
			while (1)
			{
				Msg = GetMouseMsg();
				if (Msg.uMsg == WM_LBUTTONDOWN)
				{
					xx = Msg.x;
					yy = Msg.y;

					
					if (luru(1, xx, yy, xpp, ypp))
					{
						goto huisu1;
					}
				break;
				}
			}
			break;
		case 2:
			hefenpanduan(type, xppp, yppp,AIpower);


			

			printf("%d %d\n", xx, yy);
			qiju[xx][yy] = 1;
			xnum = xx;
			ynum = yy;
			
			
			putimage(111 + xx * 59 - 16, 74 + yy * 60 - 16, &black);
			
			printf("whenwhenwhen");
			break;
		default:
			break;
		}
		/*printf("落子点%d %d\n", xx, yy);
		printf("判断点%d %d\n", xnum, ynum);*/
		
		determine(xnum, ynum, 1);
		if (type != 5)
		{
			writein(xnum, ynum, 1, qipu);
		}
		printf("2deter=%d\n",deter);
		if (deter != 0)
		{
			break;
		}
		mciSendString("close mymusic", NULL, 0, NULL);
		yinyue();
	}
	failer:
	switch (deter)
	{
	case 2:
		settextstyle(128, BLACK, _T("宋体"));
		outtextxy(256, 380, "白方胜利");
		break;
	case 1:
		settextstyle(128, BLACK, _T("宋体"));
		outtextxy(256, 380, "黑方胜利");
		break;
	}
	while (1)
	{
		Msg = GetMouseMsg();
		if (Msg.uMsg == WM_LBUTTONDOWN)
		{
			fclose(qipu);
			goto beginer;
		}
	}
	
	getchar();
	return 0;
}
