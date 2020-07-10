#pragma once
#include <stdio.h>
#include <Windows.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <setjmp.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)
#include "Declaration.h"

#pragma region 定义全局变量

#define MAXSTAR 200				// 星星总数
IMAGE img;
#define FUNCTIONPOINTER 10		// 函数指针数组长度
const float PI = 3.1415926f;
const char *ptr[] = { _T("◆ 公司管理"),"◆ 合同管理","◆ 输出所有合同信息",
				"◆ 文件操作","◆ 合同排序",NULL };
const char *settr[] = { _T("◆ 切换背景"),"◆ 切换BGM","◆ 暂停播放","◆ 支持作者","◆ 返回",NULL };
const char *omusic[] = { "open ./res/Russ.mp3 alias 0",NULL };
const char *pmusic[] = { "play 0 repeat","play 1 repeat",NULL };
const char *smusic[] = { "stop 9","stop 0","stop 1",NULL };
#pragma endregion
void SetupMouse();

#pragma region 类/结构定义
struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};
STAR star[MAXSTAR];
#pragma endregion

#pragma region 函数定义
void _CreateWindow()					// 创建窗口
{
	HWND hWnd = initgraph(640, 480);
	SetWindowText(hWnd, _T("企业合同管理系统"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, _T("新宋体"));
}
void Exception_Handling(char *str)		// 异常处理函数
{
	if (MessageBox(NULL, str, _T("温馨提示..."), MB_OK | MB_ICONERROR) == IDOK)
		return;
}
void PassWord()							// 登录密码
{
	char l_InputPass[11] = { 0 };
	if ((InputBox(l_InputPass, 11, "请输入登录密码:", "企业合同管理系统安全检测...", "请在此处填写密码...", 0, 0, false) == IDOK))
	{
		if (strcmp(l_InputPass, "123") != 0)
		{
			printf("密码错误,无法打开企业合同管理系统\n\n");
			for (int i = 6; i > 0; i--)
			{
				printf("\t倒计时:%ds\r", i);
				Sleep(1000);
			}
			exit(-1);
		}
	}
	else
		exit(-1);
}
void Welcome()							// 欢迎界面
{
	IMAGE BACK_IMAGE;
	//settextcolor(RGB(160, 82, 45));
	setbkmode(TRANSPARENT);
	loadimage(&BACK_IMAGE, "./res/3.jpg", 640, 480);
	//圆心为(60,200) 半径为180的圆的轨迹上均匀的50个点	
	int x, y;
	for (int i = 0; i < 65; i++) //轨迹
	{
		cleardevice();
		BeginBatchDraw();
		putimage(0, 0, &BACK_IMAGE);
		x = 210 + int(150 * sin(PI * 2 * i / 80));
		y = 80 + int(160 * cos(PI * 2 * i / 80));
		settextstyle(i, 0, "华文行楷", 0, 0, 0, 1, 0, 0);
		outtextxy(x, y, "企业合同管理系统");
		outtextxy(x + 55, y + 60, "C++课程设计");
		outtextxy(x + 220, y + 120, "－－5209");
		//EndBatchDraw();
		FlushBatchDraw();
		Sleep(30);
	}
	EndBatchDraw();
	settextstyle(25, 0, "华文行楷", 0, 0, 0, 0, 0, 0);
	int colorR = 0, colorG = 255;  //红和绿
	int speed = 5;
	int num = 10;
	while (!_kbhit())
	{
		BeginBatchDraw();
		settextcolor(RGB(colorR, 0, colorG));
		outtextxy(150, 350, "···请按任意键继续···");
		colorR += speed;  //-5  +5
		colorG -= speed;
		if (colorR <= 0 || colorR >= 255)
		{
			speed = -speed;
		}
		Sleep(20);
		EndBatchDraw();
	}
	_getch();
	//system("pause");
	//system("cls");
}
void InitStar(int i)//初始化星星
{
	star[i].x = 0;
	star[i].y = rand() % 480;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 1.0 + 0.5);	// 速度越快，颜色越亮
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}
void MoveStar(int i)			//移动星星
{
	// 擦掉原来的星星
	putpixel((int)star[i].x, star[i].y, 0);

	// 计算新位置
	star[i].x += star[i].step;
	if (star[i].x > 1200)	InitStar(i);

	// 画新星星
	putpixel((int)star[i].x, star[i].y, star[i].color);
}
void Draw()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 640;
	}
	while (!kbhit())
	{
		for (int i = 0; i < MAXSTAR; i++)
			MoveStar(i);
		Sleep(20);
	}
	_getch();
}
void Warning()
{
	cleardevice();
	settextcolor(LIGHTRED);
	settextstyle(20, 0, "楷体");
	const char *tok[] = { "在使用本系统时请遵守以下规则:",
		"1.本系统的版权归软件作者所有，受到适用版权法",
		"  及其他知识产权法及条约的保护",
		"2.您不得: ",
		" (1)删除本软件及其他副本上一切关于版权的信息；",
		" (2)销售、出租此软件产品的任何部分；",
		" (3)制作和提供该软件的注册机及破解程序；",
		" (4)对本软件进行逆向工程，如反汇编、反编译等。",
		"3.如果您未遵守本协议的任一条款，软件作者有权立即终止本协议，",
		"  且您必须立即终止使用本软件并销毁本软件产品的所有副本。",
		"  这项要求对各种拷贝形式有效",
		NULL
	};
	outtextxy(10, 45, "尊敬的用户:");
	for (int i = 0, y = 0; tok[i] != NULL; i++, y += 30)
	{
		outtextxy(25, 80 + y, tok[i]);
		Sleep(80);
	}
	outtextxy(440, 420, "－－冉");
	Draw();
}
static int px1 = 0, py1 = 0;
void ClearBuffer()					// 擦除显存
{
	int i = 200;
	DWORD *pMem = GetImageBuffer();// 获取窗口显存指针
	
	while (i--) //如果没有键盘按下,始终循环 (kbhit()无法进行单线程操作) 
	{
		BeginBatchDraw();
		for (int clr = 0; clr < 2000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				px1 = rand() % 640;
				py1 = rand() % 450;
				if (py1 < 479)// 防止越界
				{
					pMem[py1 * 640 + px1] = pMem[py1 * 640 + px1 + 1] = BLACK;// 对显存赋值擦除像素点
				}
			}
		}
		EndBatchDraw();	// 显示前面的所有绘图操作
	}
}
inline void UI()
{
	cleardevice();
	loadimage(&img, "./res/111.jpg", 640, 450);
	putimage(0, 0, &img);
	int y = 10, index = 0;

	while (ptr[index] != NULL)
	{
		outtextxy(240, 80 + y, ptr[index++]);
		y += 30;
	}
	settextstyle(25, 0, _T("华文楷体"));
	outtextxy(50, 30, "公司名称:");
	outtextxy(170, 30, CompanyName);
	settextstyle(15, 0, _T("新宋体"));
}
void Setup()
{
	closegraph();
	HWND hWnd = initgraph(640, 450);
	SetWindowText(hWnd, _T("企业合同管理系统"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	loadimage(&img, "./res/111.jpg", 640, 450);
	putimage(0, 0, &img);
	settextstyle(15, 0, _T("新宋体"));
	int y = 10, index = 0;
	while (settr[index] != NULL)
	{
		outtextxy(240, 80 + y, settr[index++]);
		y += 30;
	}
	_getch();
	while (1)
	{
		SetupMouse();
	}
}
inline void DrawRect(int val)
{
	setlinecolor(BLACK);
	BeginBatchDraw();
	cleardevice();
	UI();
	rectangle(230, 85 + val * 30, 410, 110 + val * 30);
	FlushBatchDraw();
}
void PageLoad()
{
	HWND hWnd = initgraph(640, 450);
	SetWindowText(hWnd, "学生成绩管理系统");
	Warning();		// 警示界面
	ClearBuffer();	// 给显存赋值擦除像素点
	Welcome();		// 首界面
	ClearBuffer();	// 给显存赋值擦除像素点
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, "新宋体");
	UI();
}
void Mouse(Company *c, void(Company::*p_Fun[FUNCTIONPOINTER])()) // 第二个参数为函数指针数组
{
	MOUSEMSG msg;//定义鼠标信息
	while (MouseHit())//判断是否对鼠标有操作
	{
		msg = GetMouseMsg();//把鼠标信息保存在msg中
		switch (msg.uMsg)
		{
			case WM_MOUSEMOVE:
			{
				int y = msg.y;
				int val = (y - 85) / 30;
				if (msg.x > 239 && msg.x < 420 && y > 85 && y < 235)
				{
					DrawRect(val);
				}
			}break;

			case WM_LBUTTONDOWN:
			{
				int y = msg.y;
				int val = (y - 85) / 30;
				if (msg.x > 239 && msg.x < 420 && y > 85 && y < 235)
				{
					(c->*p_Fun[val])();
				}
				//else if (msg.x > 239 && msg.x < 420 && y > 235 && y < 265)
				//{
				//	Setup();
				//}
			}break;
		}
	}
}

void SetupMouse()
{
	MOUSEMSG msg;//定义鼠标信息
	while (MouseHit())//判断是否对鼠标有操作
	{
		msg = GetMouseMsg();//把鼠标信息保存在msg中
		switch (msg.uMsg)
		{
			case WM_MOUSEMOVE:
			{
				int y = msg.y;
				int val = (y - 85) / 30;
				if (msg.x > 239 && msg.x < 420 && y > 85 && y < 265)
				{
					DrawRect(val);
				}
			}break;

			case WM_LBUTTONDOWN:
			{
				int y = msg.y;
				int val = (y - 85) / 30;
				if (msg.x > 239 && msg.x < 420 && y > 85 && y < 235)
				{
				
				}
			}break;
		}
	}
}

#pragma endregion