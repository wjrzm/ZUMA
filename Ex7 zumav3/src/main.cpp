#pragma warning(disable:4996)
#include<iostream>
#include<stack>
#include <stdio.h>
#include <easyx.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include "kdtree.h"
#include <math.h>
#include "ball.h"
#include <crtdbg.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <windows.h>

using namespace std;

#define WINDOWWITH 1024
#define WINDOWHEIGHT 720
#define DEF_NUM_PTS 10
#define BALLRADIUS 10

struct node {
	int scores;
	char names[10];
};

COLORREF ballColorTable[] = { BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN };


//绘制碰撞球
void drawColBall(DataType* b, float x, float y)
{
	b->x = x;
	b->y = y;
	setcolor(ballColorTable[b->c]);
	setfillcolor(ballColorTable[b->c]);
	solidcircle(b->x, b->y, BALLRADIUS);
}

void Mysort(int* array, int size, string names[10])//ascending sort
{
	int lastpos, nextpos;
	int temp = 0;
	string stemp;
	for (nextpos = size - 1; size != 1 && nextpos; size--, nextpos = lastpos)
	{
		lastpos = 0;
		for (int j = 0; j < nextpos; j++)
		{
			if (array[j] > array[j + 1])
			{
				temp = array[j];
				stemp.assign(names[j]);
				array[j] = array[j + 1];
				names[j].assign(names[j + 1]);
				array[j + 1] = temp;
				names[j + 1].assign(stemp);
				lastpos = j;
			}
		}
	}
}

//中序遍历kdtree
static void InOrderTraverse(kdnode* T)
{
	if (T)
	{
		if (T->data.x >= WINDOWWITH) {
			T->data.x = T->data.x - WINDOWWITH;
		}
		if (T->data.y <= 0) {
			T->data.y = T->data.y + WINDOWWITH;
		}

		InOrderTraverse(T->left);
		setcolor(ballColorTable[T->data.c]);
		setfillcolor(ballColorTable[T->data.c]);
		solidcircle(T->data.x++, T->data.y--, BALLRADIUS);
		InOrderTraverse(T->right);
	}
}

static float rd(void) {
	return (float)(BALLRADIUS + rand() % (WINDOWHEIGHT - 2 * BALLRADIUS));
}

void Moveball(kdnode* T, kdnode* root)
{
	int xs, ys;
	if (T)
	{
		Moveball(T->left, root);
		if (esearch(root, T->data))
		{
			xs = T->data.xspeed;
			ys = T->data.yspeed;
			T->data.xspeed = (esearch(root, T->data))->data.xspeed;
			T->data.yspeed = (esearch(root, T->data))->data.yspeed;
			(esearch(root, T->data))->data.xspeed = xs;
			(esearch(root, T->data))->data.yspeed = ys;
		}
		T->data.x = int(T->data.x + T->data.xspeed);
		if (T->data.x + BALLRADIUS >= WINDOWWITH || T->data.x - BALLRADIUS <= 0)
			T->data.xspeed = (-1) * T->data.xspeed;
		T->data.y = int(T->data.y + T->data.yspeed);
		if (T->data.y + BALLRADIUS >= WINDOWHEIGHT || T->data.y - BALLRADIUS <= 0)
			T->data.yspeed = (-1) * T->data.yspeed;
		Moveball(T->right, root);
	}
}

void Myqsort(node* array, int size)
{
	int p = 0;
	int r = size - 1;
	int i = p - 1;
	int j = p;
	int key = array[r].scores;

	if (p >= r)
		return;

	for (j = p; j < r; ++j) {
		if (array[j].scores <= key) {
			++i;
			node temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}
	node temp = array[i + 1];
	array[i + 1] = array[r];
	array[r] = temp;

	Myqsort(array, i + 1);
	Myqsort(array + i + 2, size - i - 2);
}

void GameRanking(char name[10], int score) {
	FILE* fp = NULL;
	errno_t err = 0;
	err = fopen_s(&fp,"GameRanking.txt", "r+");
	

	if (fp == NULL) {
		puts("Can't open file!");
		exit(1);
	}

	int players;
	

	fscanf_s(fp, "%d", &players);

	
	
	players = players + 1;
	node* a = (node*)malloc(players * sizeof(node));

	for (int i = 0; i < players - 1; ++i) {
		fscanf_s(fp, "%s", a[i].names, 10);
		fscanf_s(fp, "%d", &(a[i].scores));
	}
	strcpy(a[players - 1].names, name);
	a[players - 1].scores = score;

	Myqsort(a, players);
	rewind(fp);

	fprintf(fp, "%d\n\n", players);
	for (int i = 0; i < players; ++i) {
		fprintf(fp, "%s\n", a[i].names);
		fprintf(fp, "%d\n", a[i].scores);
	}
	fclose(fp);
	free(a);
	a = NULL;
}

int main()
{
	int score = 0;
	int number = DEF_NUM_PTS;

	printf("\n\n\nREAD ME!!\n\n\n欢迎来到祖玛简易版！！\n\n\n"
		"为了您更好的游戏体验\n"
		"让我来为您介绍一下游戏规则吧\n"
		"利用您手中的小球来消除屏幕上滚动的小球\n"
		"两个小球颜色相同则会被消除一个小球消失则得一分\n"
		"您手中的小球消失也包括在内\n"
		"消除所有屏幕上滚动的所有小球\n"
		"即为获胜\n"
		"\n\n\n请按下数字1再按下回车键进入游戏！！\n"
	);

	int key = 0;
	scanf_s("%d", &key);
	if (key != 1) {
		return 0;
	}

	//初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();
	//球的个数
	int num_pts = DEF_NUM_PTS;
	//指向kdtree
	struct kdnode* root = NULL;
	//最近球和最小距离
	DataType NearestBall;
	float dis;
	//插入球
	DataType Ball;

	srand(time(0));

	//把球画出来插kdtree上(避免随机出现的圆重叠)
	for (int i = 0; i < num_pts; ++i) {
		Ball.x = rd(), Ball.y = rd(), Ball.c = rand() % 6;
		while (i > 0)
		{
			searchNearest(root, Ball, NearestBall, dis);
			if (dis >= 2 * BALLRADIUS)break;
			Ball.x = rd(), Ball.y = rd();
		}
		root = kdnode_insert(root, Ball);
		setcolor(ballColorTable[Ball.c]);
		setfillcolor(ballColorTable[Ball.c]);
		solidcircle(Ball.x, Ball.y, BALLRADIUS);
	}
	
	//画碰撞球
	DataType cball;
	cball.c = rand() % 6;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
	float ballDirction = -1;
	float speed = 7;

	TCHAR Tname[10];
	char name[10];
	InputBox(Tname, 9, _T("请输入名字"));

	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, Tname, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, Tname, -1, name, iLength, NULL, NULL);

	//事件循环，处理鼠标、时间事件
	MOUSEMSG m;
	bool ballMoving = FALSE;

	while (root)
	{
		BeginBatchDraw();
		// 获取一条鼠标消息

		while (MouseHit())
		{
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				break;

			case WM_LBUTTONDOWN:
				//更新球的运动方向，球开始运动
				if (!ballMoving)
				{
					ballDirction = float(m.x - WINDOWWITH / 2) / float(WINDOWHEIGHT - m.y);
					ballMoving = TRUE;
				}
				break;

			case WM_RBUTTONUP:
				closegraph();
				kdtree_destroy(root);
				if (!_CrtDumpMemoryLeaks()) {
					GameRanking(name, score);
					cout << "\n\n已经结束游戏" << endl;
					cout << "\n游戏数据已保存在GameRanking.txt中" << endl;
				}
					
				system("PAUSE");
				return 0;	// 按鼠标右键退出程序
			}
		}

		//检测碰撞球是否超出范围
		if (cball.x > WINDOWWITH + BALLRADIUS || cball.x <-BALLRADIUS || cball.y > WINDOWHEIGHT + BALLRADIUS || cball.y < -BALLRADIUS)
		{
			cball.c = rand() % 6;
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
			ballMoving = FALSE;
			continue;
		}

	//	//移动、绘制碰撞球
		if (ballMoving == TRUE)
		{
			cleardevice();
			drawColBall(&cball, cball.x + (speed * ballDirction), cball.y - speed);
			cball.xspeed = (speed * ballDirction);
			cball.yspeed = ((-1) * speed);

			//求最近的球及最短距离
			searchNearest(root,cball,NearestBall,dis);
			if (dis <= 2* BALLRADIUS )
			{
				if (cball.c == NearestBall.c)
				{
					root = kdnode_delete(root, NearestBall);
					score++;
					number--;
				}
				else
				{
					root = kdnode_insert(root, cball);
					number++;
				}
				ballMoving = false;
				cball.c = rand() % 6;
				continue;
			}
			InOrderTraverse(root);
		}
		else
		{
			cleardevice();
			InOrderTraverse(root);
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
		}
		Moveball(root, root);
		InOrderTraverse(root);
		Sleep(15);

		TCHAR s[5];
		TCHAR n[5];
		_stprintf_s(s, _T("%d"), score);
		_stprintf_s(n, _T("%d"), number);

		settextcolor(WHITE);
		LOGFONT f;
		gettextstyle(&f);						// 获取当前字体设置
		f.lfHeight = 48;						// 设置字体高度为 48
		_tcscpy_s(f.lfFaceName, _T("黑体"));		// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
		settextstyle(&f);						// 设置字体样式
		
		outtextxy(10, 40, _T("Score:"));
		outtextxy(160, 40, s);
		outtextxy(230, 40, _T("Number:"));
		outtextxy(400, 40, n);

		EndBatchDraw();
	}
	closegraph();
	kdtree_destroy(root);
	if (!_CrtDumpMemoryLeaks())
	{
		cout << "已经结束游戏" << endl;
	}
	system("PAUSE");

	return 0;
}
