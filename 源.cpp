#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)
#pragma warning (disable:4244)
#include<iostream>
#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<cstring>
#include<string>
#include<algorithm>
#include<iomanip>
#include<math.h>
#include<fstream>
#include<graphics.h>
using namespace std;

int board[30][50] = { 0 };//-1表示有雷，0表示空，正数表示周围雷数
int condition[30][50] = { 0 };//0表示未翻开，1表示已翻开，2表示标记

int ROW_MAX = 13, COL_MAX =13, MINE_NUM = 28;
int mine_left = MINE_NUM;
bool playing = true, win = true, first = true;

IMAGE gamesetting_picture, mainmenu_picture, blue_rectangle_picture, flag_picture, blank_rectangle_picture;
IMAGE redmine_picture, whitemine_picture;

void mainmenu();
void play();
void record();
void readrecord();
void gamesetting();
void run(int, int, int);
void mine_creator(int, int);
void initialize();
bool save_or_not();
void judge();
void loadimages();


void loadimages()
{
	loadimage(&mainmenu_picture, _T("resources\\mainmenu.jpg"));
	loadimage(&gamesetting_picture, _T("resources\\gamesetting界面.jpg"));
	loadimage(&blue_rectangle_picture, _T("resources\\蓝色格子.png"));
	loadimage(&flag_picture, _T("resources\\魔力鸟.png"));
	loadimage(&blank_rectangle_picture, _T("resources\\blank.jpg"));
	loadimage(&whitemine_picture, _T("resources\\白雷.jpg"));
	loadimage(&redmine_picture, _T("resources\\红雷.jpg"));
}
int main()
{
	initgraph(1199, 666);
	loadimages();
	mainmenu();
	int abcd = _getch();
	closegraph();
	return 0;
}
void mainmenu()
{
	Resize(NULL, 1199, 666);
	putimage(0, 0, &mainmenu_picture);
	
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	settextstyle(76, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(490, 180, _T("主菜单"));
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(444, 320, _T("开始新游戏"));
	outtextxy(476, 420, _T("读取记录"));
	outtextxy(476, 520, _T("退出游戏"));
	
	int thechosennumber = 0;
	int oldcondition = 0, newcondition = 0;
	while (1)
	{
		oldcondition = newcondition;
		ExMessage m = getmessage(EX_MOUSE);
		if (m.x >= 444 && m.y >= 320 && m.x <= 756 && m.y <= 380)
		{
			newcondition = 1;
			goto mainmenu_point;
		}
		if (m.x >= 476 && m.y >= 420 && m.x <= 724 && m.y <= 480)
		{
			newcondition = 2;
			goto mainmenu_point;
		}
		if (m.x >= 476 && m.y >= 520 && m.x <= 724 && m.y <= 580)
		{
			newcondition = 3;
			goto mainmenu_point;
		}
		newcondition = 0;
	mainmenu_point:
		if (newcondition != oldcondition && newcondition == 1 || newcondition == 1 && m.message == WM_LBUTTONDOWN)
		{
			settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			settextcolor(RED);
			outtextxy(444, 320, _T("开始新游戏"));
			if (m.message == WM_LBUTTONDOWN)
			{
				thechosennumber = 1;
				break;
			}
		}
		if (newcondition != oldcondition && newcondition == 2 || newcondition == 2 && m.message == WM_LBUTTONDOWN)
		{
			settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			settextcolor(RED);
			outtextxy(476, 420, _T("读取记录"));
			if (m.message == WM_LBUTTONDOWN)
			{
				thechosennumber = 2;
				break;
			}
		}
		if (newcondition != oldcondition && newcondition == 3 || newcondition == 3 && m.message == WM_LBUTTONDOWN)
		{
			settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			settextcolor(RED);
			outtextxy(476, 520, _T("退出游戏"));
			if(m.message==WM_LBUTTONDOWN)
			{
				thechosennumber = 3;
				break;
			}
		}
		if (newcondition != oldcondition && newcondition == 0)
		{
			settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			settextcolor(YELLOW);
			outtextxy(444, 320, _T("开始新游戏"));
			outtextxy(476, 420, _T("读取记录"));
			outtextxy(476, 520, _T("退出游戏"));
		}
	}
	switch (thechosennumber)
	{
	case 1:
	{
		initialize();
		gamesetting();
		play();
		return;
	}
	case 2:
	{
		initialize();
		readrecord();
		
		play();
		return;
	}
	case 3:
		exit(0);
	}
}
void play()
{
	cleardevice();
	Resize(NULL, COL_MAX * 20 + 260, ROW_MAX * 20 + 100);
	putimage(0, 0, &gamesetting_picture);

	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);
	settextcolor(BLACK);
	settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
	outtextxy(10, 10, _T("开始新游戏"));
	outtextxy(10, 60, _T("保存游戏"));
	outtextxy(10, 110, _T("读取存档"));
	outtextxy(10, 160, _T("返回主菜单"));
	outtextxy(10, 210, _T("退出游戏"));
	int thechosennumber = 0;
	int oldcondition = 0, newcondition = 0;
	int oldpx = -1, oldpy = -1, newpx = -1, newpy = -1;
	int last_mine_left = mine_left;
	settextcolor(BLUE);
	settextstyle(25, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
	outtextxy(200 + 20 * COL_MAX - 200, 20 + 20 * ROW_MAX + 20, _T("剩余雷数"));
	settextstyle(20, 0, _T("宋体"), 0, 0, 10, false, false, false);
	outtextxy(200 + 20 * COL_MAX - 105, 20 + 20 * ROW_MAX + 20, _T(":"));
	settextstyle(35, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
	TCHAR mine[10];
	_stprintf_s(mine, _T("%d"), mine_left);
	outtextxy(200 + 20 * COL_MAX - 80, 20 + 20 * ROW_MAX + 5, mine);
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			run(i, j, 0);
	while (true)
	{
		if (last_mine_left != mine_left)
		{
			settextcolor(BLUE);
			IMAGE mine_left_blank;
			loadimage(&mine_left_blank, _T("resources\\覆盖雷数空白.jpg"));
			putimage(200 + 20 * COL_MAX - 84, 20 + 20 * ROW_MAX + 5, &mine_left_blank);
			settextstyle(35, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
			TCHAR mine[10];
			_stprintf_s(mine, _T("%d"), mine_left);
			outtextxy(200 + 20 * COL_MAX - 80, 20 + 20 * ROW_MAX + 5, mine);
			last_mine_left = mine_left;
		}
		ExMessage m = getmessage(EX_MOUSE);
		if (m.x >= 200 && m.x <= 200 + 20 * COL_MAX && m.y >= 20 && m.y <= 20 + 20 * ROW_MAX && playing)
		{
			oldpx = newpx, oldpy = newpy;
			int i = 0, j = 0;
			for (i = 0; i < COL_MAX; i++)
				if (m.x >= 200 + i * 20 && m.x <= 220 + i * 20)
					break;
			for (j = 0; j < ROW_MAX; j++)
				if (m.y >= 20 + j * 20 && m.y <= 40 + j * 20)
					break;
			newpx = i, newpy = j;
			if (newpx != oldpx || newpy != oldpy)
			{
				if (oldpx != -1 && (condition[oldpy][oldpx] == 0 || condition[oldpy][oldpx] == 2))
					run(oldpy, oldpx, 0);
				if (condition[newpy][newpx] == 0 || condition[newpy][newpx] == 2)
					run(newpy, newpx, 4);
			}
			if (m.message == WM_LBUTTONDOWN && condition[newpy][newpx] == 0)
				run(newpy, newpx, 1);
			if (m.message == WM_RBUTTONDOWN && condition[newpy][newpx] == 0 && !first)
			{
				run(newpy, newpx, 2);
				continue;
			}
			if (m.message == WM_RBUTTONDOWN && condition[newpy][newpx] == 2)
				run(newpy, newpx, 3);
		}
		if (!playing)
		{
			settextstyle(25, 0, _T("Algerian"), 0, 0, 10, false, 0, 0);
			settextcolor(RED);
			if (win)
			{
				outtextxy(100 + 20 * COL_MAX - 40, 70 + 20 * ROW_MAX - 5, _T("WIN"));
				for (int i = 0; i < ROW_MAX; i++)
					for (int j = 0; j < COL_MAX; j++)
						if (board[i][j] != -1 && condition[i][j] == 0)
							run(i, j, 1);
			}
			if (!win)
				outtextxy(100 + 20 * COL_MAX - 40, 70 + 20 * ROW_MAX - 5, _T("LOSE"));//需要更改
		}
		if (!(m.x >= 200 && m.x <= 200 + 20 * COL_MAX && m.y >= 20 && m.y <= 20 + 20 * ROW_MAX && playing))
		{
			if (oldpx != -1 && condition[oldpy][oldpx] != 1)
				run(oldpy, oldpx, 0);
			if (newpx != -1 && condition[newpy][newpx] != 1)
				run(oldpy, oldpx, 0);
			newpx = -1, newpy = -1, oldpx = -1, oldpy = -1;
			oldcondition = newcondition;
			if (m.x >= 10 && m.y >= 10 && m.x <= 150 && m.y <= 38)
			{
				newcondition = 1;
				goto play_point;
			}
			if (m.x >= 10 && m.y >= 60 && m.x <= 120 && m.y <= 88)
			{
				newcondition = 2;
				goto play_point;
			}
			if (m.x >= 10 && m.y >= 110 && m.x <= 120 && m.y <= 138)
			{
				newcondition = 3;
				goto play_point;
			}
			if (m.x >= 10 && m.y >= 160 && m.x <= 150 && m.y <= 188)
			{
				newcondition = 4;
				goto play_point;
			}
			if (m.x >= 10 && m.y >= 210 && m.x <= 120 && m.y <= 238)
			{
				newcondition = 5;
				goto play_point;
			}
			newcondition = 0;
		play_point:
			if (newcondition != oldcondition && newcondition == 1 || newcondition == 1 && m.message == WM_LBUTTONDOWN)
			{
				settextcolor(GREEN);
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				outtextxy(10, 10, _T("开始新游戏"));
				if (m.message == WM_LBUTTONDOWN)
				{
					thechosennumber = 1;
					break;
				}
			}
			if (newcondition != oldcondition && newcondition == 2 || newcondition == 2 && m.message == WM_LBUTTONDOWN)
			{
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				settextcolor(GREEN);
				outtextxy(10, 60, _T("保存游戏"));
				if (m.message == WM_LBUTTONDOWN)
				{
					thechosennumber = 2;
					break;
				}
			}
			if (newcondition != oldcondition && newcondition == 3 || newcondition == 3 && m.message == WM_LBUTTONDOWN)
			{
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				settextcolor(GREEN);
				outtextxy(10, 110, _T("读取存档"));
				if (m.message == WM_LBUTTONDOWN)
				{
					thechosennumber = 3;
					break;
				}
			}
			if (newcondition != oldcondition && newcondition == 4 || newcondition == 4 && m.message == WM_LBUTTONDOWN)
			{
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				settextcolor(GREEN);
				outtextxy(10, 160, _T("返回主菜单"));
				if (m.message == WM_LBUTTONDOWN)
				{
					thechosennumber = 4;
					break;
				}
			}
			if (newcondition != oldcondition && newcondition == 5 || newcondition == 5 && m.message == WM_LBUTTONDOWN)
			{
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				settextcolor(GREEN);
				outtextxy(10, 210, _T("退出游戏"));
				if (m.message == WM_LBUTTONDOWN)
				{
					thechosennumber = 5;
					break;
				}
			}
			if (newcondition != oldcondition && newcondition == 0)
			{
				settextcolor(BLACK);
				settextstyle(29, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
				outtextxy(10, 10, _T("开始新游戏"));
				outtextxy(10, 60, _T("保存游戏"));
				outtextxy(10, 110, _T("读取存档"));
				outtextxy(10, 160, _T("返回主菜单"));
				outtextxy(10, 210, _T("退出游戏"));
			}
		}
	}
	switch (thechosennumber)
	{
	case 1:
	{
		cleardevice();
		if (!first)
		{
			putimage(0, 0, &gamesetting_picture);
			settextcolor(RED);
			settextstyle(40, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
			outtextxy(10 * COL_MAX + 130 - 150, 10 * ROW_MAX + 50 - 55, _T("是否保存当前游戏？"));
			bool flag1 = save_or_not();
			if (flag1)
				record();
		}
		initialize();
		gamesetting();
		play();
		return;
	}
	case 2:
	{
		cleardevice();
		putimage(0, 0, &gamesetting_picture);
		record();
		setbkmode(TRANSPARENT);
		settextcolor(RED);
		settextstyle(40, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
		outtextxy(10 * COL_MAX + 130 - 150, 10 * ROW_MAX + 50 - 55, _T("是否返回主菜单？"));
		bool flag2 = save_or_not();
		if (flag2)
		{
			initialize();
			mainmenu();
		}
		if (!flag2)
			play();
		return;
	}
	case 3://需要更改
	{
		cleardevice();
		putimage(0, 0, &gamesetting_picture);
		bool flag3 = true;
		if (!first)
		{
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			settextstyle(35, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
			outtextxy(10 * COL_MAX + 130 - 200, 10 * ROW_MAX + 50 - 55, _T("将覆盖当前游戏，是否继续？"));//是否保存当前游戏？
			flag3 = save_or_not();
		}
		if (flag3)
		{
			cleardevice();
			initialize();
			readrecord();
		}
		play();
		return;
	}
	case 4:
	{
		cleardevice();
		putimage(0, 0, &gamesetting_picture);
		if (!first)
		{
			settextcolor(RED);
			settextstyle(40, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
			outtextxy(10 * COL_MAX + 130 - 150, 10 * ROW_MAX + 50 - 55, _T("是否保存当前游戏？"));
			bool flag4 = save_or_not();
			if (flag4)
				record();
		}
		cleardevice();
		initialize();
		mainmenu();
		return;
	}
	case 5:
	{
		cleardevice();
		putimage(0, 0, &gamesetting_picture);
		if (!first)
		{
			settextcolor(RED);
			settextstyle(40, 0, _T("华文行楷"), 0, 0, 10, false, 0, 0);
			outtextxy(10 * COL_MAX + 130 - 150, 10 * ROW_MAX + 50 - 55, _T("是否保存当前游戏？"));
			bool flag5 = save_or_not();
			if (flag5)
				record();
		}
		exit(0);
	}
	}
}
void gamesetting()
{
	Resize(NULL, 1199, 666);
	cleardevice();
	putimage(0, 0, &gamesetting_picture);
	setbkmode(TRANSPARENT);

	setlinestyle(PS_SOLID, 4);
	setlinecolor(RED);
	settextcolor(RED);
	roundrect(485, 120, 685, 200, 30, 25);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(314, 135, _T("行数"));
	settextstyle(50, 0, _T("宋体"), 0, 0, 10, false, false, false);
	outtextxy(440, 135, _T(":"));
	settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
	TCHAR s1[5];
	_stprintf_s(s1, _T("%d"), ROW_MAX);
	outtextxy(505, 115, s1);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(700, 135, _T("行"));
	settextstyle(36, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(750, 160, _T("（范围7-30）"));

	setlinestyle(PS_SOLID, 4);
	setlinecolor(RED);
	settextcolor(RED);
	roundrect(485, 220, 685, 300, 30, 25);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(314, 235, _T("列数"));
	settextstyle(50, 0, _T("宋体"), 0, 0, 10, false, false, false);
	outtextxy(440, 235, _T(":"));
	settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
	TCHAR s2[5];
	_stprintf_s(s2, _T("%d"), COL_MAX);
	outtextxy(505, 215, s2);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(700, 235, _T("列"));
	settextstyle(36, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(750, 260, _T("（范围7-50）"));

	setlinestyle(PS_SOLID, 4);
	setlinecolor(RED);
	settextcolor(RED);
	roundrect(485, 320, 685, 400, 30, 25);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(314, 335, _T("雷数"));
	settextstyle(50, 0, _T("宋体"), 0, 0, 10, false, false, false);
	outtextxy(440, 335, _T(":"));
	settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
	TCHAR s3[5];
	_stprintf_s(s3, _T("%d"), MINE_NUM);
	outtextxy(505, 315, s3);
	settextstyle(66, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(700, 335, _T("个"));
	settextstyle(36, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(750, 360, _T("（占总格数的10%-30%，"));
	outtextxy(790, 400, _T("默认为17%（推荐））"));
	setfillcolor(RGB(172, 166, 166));
	solidroundrect(720, 480, 830, 535, 30, 25);
	settextstyle(60, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	outtextxy(720, 480, _T("确定"));
	int oldcondition = 0, newcondition = 0;
	int coltemp = 0, rowtemp = 0, minetemp = 0;
	ExMessage m;
	while (true)
	{
		oldcondition = newcondition;
		m = getmessage(EX_MOUSE | EX_KEY);
	loop1:
		if (m.x >= 485 && m.y >= 120 && m.x <= 685 && m.y <= 200)
		{
			newcondition = 1;
			goto gamesetting_point;
		}
		if (m.x >= 485 && m.y >= 220 && m.x <= 685 && m.y <= 300)
		{
			newcondition = 2;
			goto gamesetting_point;
		}
		if (m.x >= 485 && m.y >= 320 && m.x <= 685 && m.y <= 400)
		{
			newcondition = 3;
			goto gamesetting_point;
		}
		if (m.x >= 720 && m.y >= 480 && m.x <= 830 && m.y <= 535)
		{
			newcondition = 100;
			goto gamesetting_point;
		}
		newcondition = 0;
	gamesetting_point:
		if (newcondition != oldcondition && newcondition == 1 || newcondition == 1 && m.message == WM_LBUTTONDOWN)
		{
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			settextcolor(YELLOW);
			settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
			outtextxy(505, 115, s1);
			roundrect(485, 120, 685, 200, 30, 25);
			if (m.message == WM_LBUTTONDOWN)
			{
				setfillcolor(WHITE);
				solidrectangle(490, 125, 680, 195);
				setlinecolor(BLUE);
				settextcolor(BLUE);
				settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
				roundrect(485, 120, 685, 200, 30, 25);
				while (true)
				{
					m = getmessage(EX_MOUSE | EX_KEY);
					if (m.message == WM_LBUTTONDOWN && !(m.x >= 485 && m.y >= 120 && m.x <= 685 && m.y <= 200))
					{
						if (rowtemp > 30)
							rowtemp = 30;
						if (rowtemp < 7 && rowtemp > 0)
							rowtemp = 7;
						if (rowtemp > 0)
							ROW_MAX = rowtemp;
						setlinecolor(RED);
						settextcolor(RED);
						settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
						roundrect(485, 120, 685, 200, 30, 25);
						_stprintf_s(s1, _T("%d"), ROW_MAX);
						setfillcolor(WHITE);
						solidrectangle(490, 125, 680, 195);
						outtextxy(505, 115, s1);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						MINE_NUM = int(0.17 * ROW_MAX * COL_MAX);
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						settextcolor(RED);
						outtextxy(505, 315, s3);
						rowtemp = 0;
						goto loop1;
					}
					if (((m.vkcode >= 0x30 && m.vkcode <= 0x39) || (m.vkcode >= 0x60 && m.vkcode <= 0x69)) && m.prevdown == 0)
					{
						int abcd = _getch();
						int delta;
						if (m.vkcode >= 0x30 && m.vkcode <= 0x39)
							delta = m.vkcode - 0x30;
						if (m.vkcode >= 0x60 && m.vkcode <= 0x69)
							delta = m.vkcode - 0x60;
						rowtemp = rowtemp * 10 + delta;
						if (rowtemp > 30)
							rowtemp = 30;
						_stprintf_s(s1, _T("%d"), rowtemp);
						setfillcolor(WHITE);
						solidrectangle(490, 125, 680, 195);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 115, s1);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						MINE_NUM = int(0.17 * rowtemp * COL_MAX);
						if (MINE_NUM > 999)
							MINE_NUM = 999;
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						settextcolor(RED);
						outtextxy(505, 315, s3);
					}
					if (m.vkcode == 0x08 && m.prevdown == 0)
					{
						int abcd = _getch();
						rowtemp = (rowtemp - rowtemp % 10) / 10;
						if (rowtemp < 0)
							rowtemp = 0;
						_stprintf_s(s1, _T("%d"), rowtemp);
						setfillcolor(WHITE);
						solidrectangle(490, 125, 680, 195);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 115, s1);

						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						MINE_NUM = int(0.17 * rowtemp * COL_MAX);
						if (MINE_NUM > 999)
							MINE_NUM = 999;
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						settextcolor(RED);
						outtextxy(505, 315, s3);
					}
				}
			}
		}
		if (newcondition != oldcondition && newcondition == 2 || newcondition == 2 && m.message == WM_LBUTTONDOWN)
		{
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			settextcolor(YELLOW);
			settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
			outtextxy(505, 215, s2);
			roundrect(485, 220, 685, 300, 30, 25);
			if (m.message == WM_LBUTTONDOWN)
			{
				setfillcolor(WHITE);
				solidrectangle(490, 225, 680, 295);
				setlinecolor(BLUE);
				settextcolor(BLUE);
				settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
				roundrect(485, 220, 685, 300, 30, 25);
				while (true)
				{
					m = getmessage(EX_MOUSE | EX_KEY);
					if (m.message == WM_LBUTTONDOWN && !(m.x >= 485 && m.y >= 220 && m.x <= 685 && m.y <= 300))
					{
						if (coltemp > 50)
							coltemp = 50;
						if (coltemp < 7 && coltemp > 0)
							coltemp = 7;
						if (coltemp > 0)
							COL_MAX = coltemp;
						setlinecolor(RED);
						settextcolor(RED);
						settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
						roundrect(485, 220, 685, 300, 30, 25);
						_stprintf_s(s2, _T("%d"), COL_MAX);
						setfillcolor(WHITE);
						solidrectangle(490, 225, 680, 295);
						outtextxy(505, 215, s2);

						MINE_NUM = int(0.17 * ROW_MAX * COL_MAX);
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						settextcolor(RED);
						outtextxy(505, 315, s3);
						coltemp = 0;
						goto loop1;
					}
					if (((m.vkcode >= 0x30 && m.vkcode <= 0x39) || (m.vkcode >= 0x60 && m.vkcode <= 0x69)) && m.prevdown == 0)
					{
						int abcd = _getch();
						int delta;
						if (m.vkcode >= 0x30 && m.vkcode <= 0x39)
							delta = m.vkcode - 0x30;
						if (m.vkcode >= 0x60 && m.vkcode <= 0x69)
							delta = m.vkcode - 0x60;
						coltemp = coltemp * 10 + delta;
						if (coltemp > 50)
							coltemp = 50;
						_stprintf_s(s2, _T("%d"), coltemp);
						setfillcolor(WHITE);
						solidrectangle(490, 225, 680, 295);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 215, s2);

						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						MINE_NUM = int(0.17 * ROW_MAX * coltemp);
						if (MINE_NUM > 999)
							MINE_NUM = 999;
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						settextcolor(RED);
						outtextxy(505, 315, s3);
					}
					if (m.vkcode == 0x08 && m.prevdown == 0)
					{
						int abcd = _getch();
						coltemp = (coltemp - coltemp % 10) / 10;
						_stprintf_s(s2, _T("%d"), coltemp);
						setfillcolor(WHITE);
						solidrectangle(490, 225, 680, 295);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 215, s2);

						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						MINE_NUM = int(0.17 * ROW_MAX * coltemp);
						if (MINE_NUM > 999)
							MINE_NUM = 999;
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						settextcolor(RED);
						outtextxy(505, 315, s3);
					}
				}
			}
		}
		if (newcondition != oldcondition && newcondition == 3 || newcondition == 3 && m.message == WM_LBUTTONDOWN)
		{
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			settextcolor(YELLOW);
			settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
			outtextxy(505, 315, s3);
			roundrect(485, 320, 685, 400, 30, 25);
			if (m.message == WM_LBUTTONDOWN)
			{
				setfillcolor(WHITE);
				solidrectangle(490, 325, 680, 395);
				setlinecolor(BLUE);
				settextcolor(BLUE);
				settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
				roundrect(485, 320, 685, 400, 30, 25);
				while (true)
				{
					m = getmessage(EX_MOUSE | EX_KEY);
					if (m.message == WM_LBUTTONDOWN && !(m.x >= 485 && m.y >= 320 && m.x <= 685 && m.y <= 400))
					{
						if (minetemp > int(0.3 * ROW_MAX * COL_MAX))
							minetemp = int(0.3 * ROW_MAX * COL_MAX);
						if (minetemp < int(0.1 * ROW_MAX * COL_MAX) && minetemp > 0)
							minetemp = int(0.1 * ROW_MAX * COL_MAX);
						if (minetemp > 0)
							MINE_NUM = minetemp;
						setlinecolor(RED);
						settextcolor(RED);
						settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
						roundrect(485, 320, 685, 400, 30, 25);
						_stprintf_s(s3, _T("%d"), MINE_NUM);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						outtextxy(505, 315, s3);
						minetemp = 0;
						goto loop1;
					}
					if (((m.vkcode >= 0x30 && m.vkcode <= 0x39) || (m.vkcode >= 0x60 && m.vkcode <= 0x69)) && m.prevdown == 0)
					{
						int abcd = _getch();
						int delta;
						if (m.vkcode >= 0x30 && m.vkcode <= 0x39)
							delta = m.vkcode - 0x30;
						if (m.vkcode >= 0x60 && m.vkcode <= 0x69)
							delta = m.vkcode - 0x60;
						minetemp = minetemp * 10 + delta;
						if (minetemp > int(0.3 * (ROW_MAX * COL_MAX)))
							minetemp = int(0.3 * (ROW_MAX * COL_MAX));
						_stprintf_s(s3, _T("%d"), minetemp);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 315, s3);
					}
					if (m.vkcode == 0x08 && m.prevdown == 0)
					{
						int abcd = _getch();
						minetemp = (minetemp - minetemp % 10) / 10;
						_stprintf_s(s3, _T("%d"), minetemp);
						setfillcolor(WHITE);
						solidrectangle(490, 325, 680, 395);
						setlinecolor(BLUE);
						settextcolor(BLUE);
						outtextxy(505, 315, s3);
					}
				}
			}
		}
		if (newcondition != oldcondition && newcondition == 100 || newcondition == 100 && m.message == WM_LBUTTONDOWN)
		{
			settextcolor(BLUE);
			settextstyle(60, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			outtextxy(720, 480, _T("确定"));
			if (m.message == WM_LBUTTONDOWN)
			{
				cleardevice();
				mine_left = MINE_NUM;
				return;
			}
		}
		if (newcondition != oldcondition && newcondition == 0)
		{
			setlinestyle(PS_SOLID, 4);
			setlinecolor(RED);
			settextcolor(RED);
			settextstyle(78, 0, _T("CandyBits BT"), 0, 0, 10, false, false, false);
			roundrect(485, 120, 685, 200, 30, 25);
			_stprintf_s(s1, _T("%d"), ROW_MAX);
			outtextxy(505, 115, s1);
			roundrect(485, 220, 685, 300, 30, 25);
			_stprintf_s(s2, _T("%d"), COL_MAX);
			outtextxy(505, 215, s2);
			roundrect(485, 320, 685, 400, 30, 25);
			_stprintf_s(s3, _T("%d"), MINE_NUM);
			outtextxy(505, 315, s3);
			settextstyle(60, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
			outtextxy(720, 480, _T("确定"));
		}
	}
}
void run(int row,int col,int operation)//0表示原样输出，1表示翻开，2表示标记，3表示取消标记，4表示预选中
{	
	if (first && operation == 1)
	{
		mine_creator(row, col);
		first = false;
	}
	if (operation == 0)
	{
		if (condition[row][col] == 0 || condition[row][col] == 2)
		{
			putimage(200 + 20 * col, 20 + 20 * row, &blue_rectangle_picture);			
			if (condition[row][col] == 2)
				putimage(200 + 20 * col + 3, 20 + 20 * row + 3, &flag_picture);
		}
		if (condition[row][col] == 1)
		{			
			putimage(200 + 20 * col, 20 + 20 * row, &blank_rectangle_picture);
			setlinecolor(BLUE);
			rectangle(200 + 20 * col, 20 + 20 * row, 220 + 20 * col, 40 + 20 * row);
			if (board[row][col] > 0)
			{
				settextstyle(18, 0, _T("宋体"), 0, 0, 10, false, false, false);
				settextcolor(BLUE);//可以改动
				TCHAR temp[10];
				_stprintf_s(temp, _T("%d"), board[row][col]);
				outtextxy(200 + 20 * col + 6, 20 + 20 * row + 2, temp);
			}
			if (board[row][col] == -1)
				putimage(200 + 20 * col, 20 + 20 * row, &whitemine_picture);
		}
		goto end;
	}	
	if (operation == 1 && condition[row][col] == 0)
	{
		condition[row][col] = 1;
		if (board[row][col] == -1)
		{	
			putimage(200 + 20 * col, 20 + 20 * row, &blank_rectangle_picture);
			putimage(200 + 20 * col, 20 + 20 * row, &redmine_picture);
			Sleep(int(1500 / MINE_NUM));
			putimage(200 + 20 * col, 20 + 20 * row, &whitemine_picture);
			Sleep(int(1500 / MINE_NUM));
			for (int i = 0; i < ROW_MAX; i++)
				for (int j = 0; j < COL_MAX; j++)
					if (board[i][j] == -1 && condition[i][j] != 1)
					{
						condition[i][j] = 1;
						putimage(200 + 20 * j, 20 + 20 * i, &blank_rectangle_picture);
						putimage(200 + 20 * j, 20 + 20 * i, &redmine_picture);
						Sleep(int(1500 / MINE_NUM));
						putimage(200 + 20 * j, 20 + 20 * i, &whitemine_picture);
						Sleep(int(1500 / MINE_NUM));
					}
		}
		if (board[row][col] == 0)
		{
			putimage(200 + 20 * col, 20 + 20 * row, &blank_rectangle_picture);
			setlinecolor(BLUE);
			rectangle(200 + 20 * col, 20 + 20 * row, 220 + 20 * col, 40 + 20 * row);
			if (row > 0 && col > 0 && board[row - 1][col - 1] == 0 && condition[row - 1][col - 1] == 0)run(row - 1, col - 1, 1);
			if (col > 0 && board[row][col - 1] == 0 && condition[row][col - 1] == 0)run(row, col - 1, 1);
			if (row <= ROW_MAX - 2 && col > 0 && board[row + 1][col - 1] == 0 && condition[row + 1][col - 1] == 0)run(row + 1, col - 1, 1);
			if (row <= ROW_MAX - 2 && board[row + 1][col] == 0 && condition[row + 1][col] == 0)run(row + 1, col, 1);
			if (row <= ROW_MAX - 2 && col <= COL_MAX - 2 && board[row + 1][col + 1] == 0 && condition[row + 1][col + 1] == 0)run(row + 1, col + 1, 1);
			if (col <= COL_MAX - 2 && board[row][col + 1] == 0 && condition[row][col + 1] == 0)run(row, col + 1, 1);
			if (row > 0 && col <= COL_MAX - 2 && board[row - 1][col + 1] == 0 && condition[row - 1][col + 1] == 0)run(row - 1, col + 1, 1);
			if (row > 0 && board[row - 1][col] == 0 && condition[row - 1][col] == 0)run(row - 1, col, 1);

			for (int i = 0; i < ROW_MAX; i++)
				for (int j = 0; j < COL_MAX; j++)
					if (board[i][j] == 0 && condition[i][j] == 1)
					{
						if (i > 0 && j > 0 && condition[i - 1][j - 1] == 0)
							run(i - 1, j - 1, 1);
						if (j > 0 && condition[i][j - 1] == 0)
							run(i, j - 1, 1);
						if (i < ROW_MAX - 1 && j>0 && condition[i + 1][j - 1] == 0)
							run(i + 1, j - 1, 1);
						if (i < ROW_MAX - 1 && condition[i + 1][j] == 0)
							run(i + 1, j, 1);
						if (i < ROW_MAX - 1 && j < COL_MAX - 1 && condition[i + 1][j + 1] == 0)
							run(i + 1, j + 1, 1);
						if (j < COL_MAX - 1 && condition[i][j + 1] == 0)
							run(i, j + 1, 1);
						if (i > 0 && j < COL_MAX - 1 && condition[i - 1][j + 1] == 0)
							run(i - 1, j + 1, 1);
						if (i > 0 && condition[i - 1][j] == 0)
							run(i - 1, j, 1);
					}
		}
		if (board[row][col] > 0)
		{
			putimage(200 + 20 * col, 20 + 20 * row, &blank_rectangle_picture);
			settextstyle(18, 0, _T("宋体"), 0, 0, 10, false, false, false);
			settextcolor(BLUE);//可以改动
			TCHAR temp[10];
			_stprintf_s(temp, _T("%d"), board[row][col]);
			outtextxy(200 + 20 * col + 6, 20 + 20 * row + 2, temp);
		}
		setlinestyle(PS_SOLID, 1);
		setlinecolor(BLUE);
		//setlinecolor(RGB(219, 218, 213));
		rectangle(200 + 20 * col , 20 + 20 * row , 220 + 20 * col , 40 + 20 * row );
		goto end;
	}
	if (operation == 2 && condition[row][col] == 0)
	{		
		putimage(200 + 20 * col + 3, 20 + 20 * row + 3, &flag_picture);
		condition[row][col] = 2;
		mine_left--;
		goto end;
	}
	if (operation == 3 && condition[row][col] == 2)
	{
		putimage(200 + 20 * col, 20 + 20 * row, &blue_rectangle_picture);
		condition[row][col] = 0;
		mine_left++;
		goto end;
	}
	if (operation == 4)
	{
		setlinestyle(PS_SOLID, 1);
		setlinecolor(YELLOW);
		rectangle(200 + 20 * col+2, 20 + 20 * row+2, 220 + 20 * col-2, 40 + 20 * row-2);
		goto end;
	}
end:
	judge();
	return;
}
void initialize()
{
	first = true;
	playing = true;
	win = true;
	mine_left = MINE_NUM;
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 50; j++)
		{
			board[i][j] = 0;
			condition[i][j] = 0;
		}
	ROW_MAX = 13, COL_MAX = 13, MINE_NUM = 28;
}
void mine_creator(int row, int col)
{
	int jishuqi = 0;
	while (jishuqi != MINE_NUM)
	{
		int row1 = rand() % ROW_MAX;
		int col1 = rand() % COL_MAX;
		if ((row1 != row || col1 != col) && board[row1][col1] != -1)
		{
			board[row1][col1] = -1;
			jishuqi++;
		}
	}
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			if (board[i][j] != -1)
			{
				if (i > 0 && j > 0 && board[i - 1][j - 1] == -1)board[i][j]++;
				if (j > 0 && board[i][j - 1] == -1)board[i][j]++;
				if (i <= ROW_MAX - 2 && j > 0 && board[i + 1][j - 1] == -1)board[i][j]++;
				if (i <= ROW_MAX - 2 && board[i + 1][j] == -1)board[i][j] ++;
				if (i <= ROW_MAX - 2 && j <= COL_MAX - 2 && board[i + 1][j + 1] == -1)board[i][j]++;
				if (j <= COL_MAX - 2 && board[i][j + 1] == -1)board[i][j]++;
				if (i > 0 && j <= COL_MAX - 2 && board[i - 1][j + 1] == -1)board[i][j]++;
				if (i > 0 && board[i - 1][j] == -1)board[i][j]++;
			}
	return;
}
void record()
{
	ofstream fout("扫雷记录.txt");
	fout << ROW_MAX << " " << COL_MAX << " " << MINE_NUM << " " << mine_left << " " << playing << " " << win << " " << first << endl;
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
		{
			fout << board[i][j];
			if (j < COL_MAX - 1)
				fout << " ";
			if (j == COL_MAX - 1)
				fout << endl;
		}
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
		{
			fout << condition[i][j];
			if (j < COL_MAX - 1)
				fout << " ";
			if (j == COL_MAX - 1)
				fout << endl;
		}
	fout.close();
}
void readrecord()
{
	ifstream fin("扫雷记录.txt");
	fin >> ROW_MAX >> COL_MAX >> MINE_NUM >> mine_left >> playing >> win >> first;
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			fin >> board[i][j];
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			fin >> condition[i][j];
	fin.close();
}
void judge()
{	
	bool judge1 = true;
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			if (board[i][j] == -1 && condition[i][j] == 1)
			{
				judge1 = false;
				break;
			}
	if (!judge1)
	{
		playing = false;
		win = false;
		return;
	}
	bool judge2 = true;
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			if (board[i][j] != -1 && condition[i][j] != 1)
				judge2 = false;
	if (judge2)
	{
		playing = false;
		win = true;
		return;
	}
	bool judge3 = true;
	for (int i = 0; i < ROW_MAX; i++)
		for (int j = 0; j < COL_MAX; j++)
			if (board[i][j] == -1 && condition[i][j] != 2)
				judge3 = false;
	if (judge3 && !first)
	{
		playing = false;
		win = true;
		return;
	}
}
bool save_or_not()
{
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("华文行楷"), 0, 0, 10, false, false, false);
	settextcolor(BLACK);
	/*rectangle(10 * COL_MAX + 50, 10 * ROW_MAX + 70, 10 * COL_MAX + 90, 10 * ROW_MAX + 105);
	rectangle(10 * COL_MAX + 190, 10 * ROW_MAX + 70, 10 * COL_MAX + 230, 10 * ROW_MAX + 105);*/

	outtextxy(10 * COL_MAX + 50, 10 * ROW_MAX + 70, _T("是"));
	outtextxy(10 * COL_MAX + 190, 10 * ROW_MAX + 70, _T("否"));
	int thechosennumber = 0;
	int oldcondition = 0, newcondition = 0;
	while (1)
	{
		oldcondition = newcondition;
		ExMessage m = getmessage(EX_MOUSE);
		if (m.x >= 10 * COL_MAX + 50 && m.y >= 10 * ROW_MAX + 70 && m.x <= 10 * COL_MAX + 90 && m.y <= 10 * ROW_MAX + 105)
		{
			newcondition = 1;
			goto modechoose_point;
		}
		if (m.x >= 10 * COL_MAX + 190 && m.y >= 10 * ROW_MAX + 70 && m.x <= 10 * COL_MAX + 230 && m.y <= 10 * ROW_MAX + 105)
		{
			newcondition = 2;
			goto modechoose_point;
		}
		newcondition = 0;
	modechoose_point:
		if (newcondition != oldcondition && newcondition == 1 || newcondition == 1 && m.message == WM_LBUTTONDOWN)
		{
			settextcolor(RED);
			outtextxy(10 * COL_MAX + 50, 10 * ROW_MAX + 70, _T("是"));
			if (m.message == WM_LBUTTONDOWN)
			{
				thechosennumber = 1;
				break;
			}
		}
		if (newcondition != oldcondition && newcondition == 2 || newcondition == 2 && m.message == WM_LBUTTONDOWN)
		{
			settextcolor(RED);
			outtextxy(10 * COL_MAX + 190, 10 * ROW_MAX + 70, _T("否"));
			if (m.message == WM_LBUTTONDOWN)
			{
				thechosennumber = 2;
				break;
			}
		}
		if (newcondition != oldcondition && newcondition == 0)
		{
			settextcolor(BLACK);
			outtextxy(10 * COL_MAX + 50, 10 * ROW_MAX + 70, _T("是"));
			outtextxy(10 * COL_MAX + 190, 10 * ROW_MAX + 70, _T("否"));
		}
	}
	switch (thechosennumber)
	{
	case 1:
		return true;
	case 2:
		return false;
	}
}
