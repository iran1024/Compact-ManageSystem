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

#pragma region ����ȫ�ֱ���

#define MAXSTAR 200				// ��������
IMAGE img;
#define FUNCTIONPOINTER 10		// ����ָ�����鳤��
const float PI = 3.1415926f;
const char *ptr[] = { _T("�� ��˾����"),"�� ��ͬ����","�� ������к�ͬ��Ϣ",
				"�� �ļ�����","�� ��ͬ����",NULL };
const char *settr[] = { _T("�� �л�����"),"�� �л�BGM","�� ��ͣ����","�� ֧������","�� ����",NULL };
const char *omusic[] = { "open ./res/Russ.mp3 alias 0",NULL };
const char *pmusic[] = { "play 0 repeat","play 1 repeat",NULL };
const char *smusic[] = { "stop 9","stop 0","stop 1",NULL };
#pragma endregion
void SetupMouse();

#pragma region ��/�ṹ����
struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};
STAR star[MAXSTAR];
#pragma endregion

#pragma region ��������
void _CreateWindow()					// ��������
{
	HWND hWnd = initgraph(640, 480);
	SetWindowText(hWnd, _T("��ҵ��ͬ����ϵͳ"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, _T("������"));
}
void Exception_Handling(char *str)		// �쳣������
{
	if (MessageBox(NULL, str, _T("��ܰ��ʾ..."), MB_OK | MB_ICONERROR) == IDOK)
		return;
}
void PassWord()							// ��¼����
{
	char l_InputPass[11] = { 0 };
	if ((InputBox(l_InputPass, 11, "�������¼����:", "��ҵ��ͬ����ϵͳ��ȫ���...", "���ڴ˴���д����...", 0, 0, false) == IDOK))
	{
		if (strcmp(l_InputPass, "123") != 0)
		{
			printf("�������,�޷�����ҵ��ͬ����ϵͳ\n\n");
			for (int i = 6; i > 0; i--)
			{
				printf("\t����ʱ:%ds\r", i);
				Sleep(1000);
			}
			exit(-1);
		}
	}
	else
		exit(-1);
}
void Welcome()							// ��ӭ����
{
	IMAGE BACK_IMAGE;
	//settextcolor(RGB(160, 82, 45));
	setbkmode(TRANSPARENT);
	loadimage(&BACK_IMAGE, "./res/3.jpg", 640, 480);
	//Բ��Ϊ(60,200) �뾶Ϊ180��Բ�Ĺ켣�Ͼ��ȵ�50����	
	int x, y;
	for (int i = 0; i < 65; i++) //�켣
	{
		cleardevice();
		BeginBatchDraw();
		putimage(0, 0, &BACK_IMAGE);
		x = 210 + int(150 * sin(PI * 2 * i / 80));
		y = 80 + int(160 * cos(PI * 2 * i / 80));
		settextstyle(i, 0, "�����п�", 0, 0, 0, 1, 0, 0);
		outtextxy(x, y, "��ҵ��ͬ����ϵͳ");
		outtextxy(x + 55, y + 60, "C++�γ����");
		outtextxy(x + 220, y + 120, "����5209");
		//EndBatchDraw();
		FlushBatchDraw();
		Sleep(30);
	}
	EndBatchDraw();
	settextstyle(25, 0, "�����п�", 0, 0, 0, 0, 0, 0);
	int colorR = 0, colorG = 255;  //�����
	int speed = 5;
	int num = 10;
	while (!_kbhit())
	{
		BeginBatchDraw();
		settextcolor(RGB(colorR, 0, colorG));
		outtextxy(150, 350, "�������밴���������������");
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
void InitStar(int i)//��ʼ������
{
	star[i].x = 0;
	star[i].y = rand() % 480;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 1.0 + 0.5);	// �ٶ�Խ�죬��ɫԽ��
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}
void MoveStar(int i)			//�ƶ�����
{
	// ����ԭ��������
	putpixel((int)star[i].x, star[i].y, 0);

	// ������λ��
	star[i].x += star[i].step;
	if (star[i].x > 1200)	InitStar(i);

	// ��������
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
	settextstyle(20, 0, "����");
	const char *tok[] = { "��ʹ�ñ�ϵͳʱ���������¹���:",
		"1.��ϵͳ�İ�Ȩ������������У��ܵ����ð�Ȩ��",
		"  ������֪ʶ��Ȩ������Լ�ı���",
		"2.������: ",
		" (1)ɾ�������������������һ�й��ڰ�Ȩ����Ϣ��",
		" (2)���ۡ�����������Ʒ���κβ��֣�",
		" (3)�������ṩ�������ע������ƽ����",
		" (4)�Ա�����������򹤳̣��練��ࡢ������ȡ�",
		"3.�����δ���ر�Э�����һ������������Ȩ������ֹ��Э�飬",
		"  ��������������ֹʹ�ñ���������ٱ������Ʒ�����и�����",
		"  ����Ҫ��Ը��ֿ�����ʽ��Ч",
		NULL
	};
	outtextxy(10, 45, "�𾴵��û�:");
	for (int i = 0, y = 0; tok[i] != NULL; i++, y += 30)
	{
		outtextxy(25, 80 + y, tok[i]);
		Sleep(80);
	}
	outtextxy(440, 420, "����Ƚ");
	Draw();
}
static int px1 = 0, py1 = 0;
void ClearBuffer()					// �����Դ�
{
	int i = 200;
	DWORD *pMem = GetImageBuffer();// ��ȡ�����Դ�ָ��
	
	while (i--) //���û�м��̰���,ʼ��ѭ�� (kbhit()�޷����е��̲߳���) 
	{
		BeginBatchDraw();
		for (int clr = 0; clr < 2000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				px1 = rand() % 640;
				py1 = rand() % 450;
				if (py1 < 479)// ��ֹԽ��
				{
					pMem[py1 * 640 + px1] = pMem[py1 * 640 + px1 + 1] = BLACK;// ���Դ渳ֵ�������ص�
				}
			}
		}
		EndBatchDraw();	// ��ʾǰ������л�ͼ����
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
	settextstyle(25, 0, _T("���Ŀ���"));
	outtextxy(50, 30, "��˾����:");
	outtextxy(170, 30, CompanyName);
	settextstyle(15, 0, _T("������"));
}
void Setup()
{
	closegraph();
	HWND hWnd = initgraph(640, 450);
	SetWindowText(hWnd, _T("��ҵ��ͬ����ϵͳ"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	loadimage(&img, "./res/111.jpg", 640, 450);
	putimage(0, 0, &img);
	settextstyle(15, 0, _T("������"));
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
	SetWindowText(hWnd, "ѧ���ɼ�����ϵͳ");
	Warning();		// ��ʾ����
	ClearBuffer();	// ���Դ渳ֵ�������ص�
	Welcome();		// �׽���
	ClearBuffer();	// ���Դ渳ֵ�������ص�
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, "������");
	UI();
}
void Mouse(Company *c, void(Company::*p_Fun[FUNCTIONPOINTER])()) // �ڶ�������Ϊ����ָ������
{
	MOUSEMSG msg;//���������Ϣ
	while (MouseHit())//�ж��Ƿ������в���
	{
		msg = GetMouseMsg();//�������Ϣ������msg��
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
	MOUSEMSG msg;//���������Ϣ
	while (MouseHit())//�ж��Ƿ������в���
	{
		msg = GetMouseMsg();//�������Ϣ������msg��
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