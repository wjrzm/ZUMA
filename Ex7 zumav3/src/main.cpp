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


//������ײ��
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

//�������kdtree
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

	printf("\n\n\nREAD ME!!\n\n\n��ӭ����������װ棡��\n\n\n"
		"Ϊ�������õ���Ϸ����\n"
		"������Ϊ������һ����Ϸ�����\n"
		"���������е�С����������Ļ�Ϲ�����С��\n"
		"����С����ɫ��ͬ��ᱻ����һ��С����ʧ���һ��\n"
		"�����е�С����ʧҲ��������\n"
		"����������Ļ�Ϲ���������С��\n"
		"��Ϊ��ʤ\n"
		"\n\n\n�밴������1�ٰ��»س���������Ϸ����\n"
	);

	int key = 0;
	scanf_s("%d", &key);
	if (key != 1) {
		return 0;
	}

	//��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();
	//��ĸ���
	int num_pts = DEF_NUM_PTS;
	//ָ��kdtree
	struct kdnode* root = NULL;
	//��������С����
	DataType NearestBall;
	float dis;
	//������
	DataType Ball;

	srand(time(0));

	//���򻭳�����kdtree��(����������ֵ�Բ�ص�)
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
	
	//����ײ��
	DataType cball;
	cball.c = rand() % 6;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
	float ballDirction = -1;
	float speed = 7;

	TCHAR Tname[10];
	char name[10];
	InputBox(Tname, 9, _T("����������"));

	int iLength;
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, Tname, -1, NULL, 0, NULL, NULL);
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, Tname, -1, name, iLength, NULL, NULL);

	//�¼�ѭ����������ꡢʱ���¼�
	MOUSEMSG m;
	bool ballMoving = FALSE;

	while (root)
	{
		BeginBatchDraw();
		// ��ȡһ�������Ϣ

		while (MouseHit())
		{
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				break;

			case WM_LBUTTONDOWN:
				//��������˶�������ʼ�˶�
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
					cout << "\n\n�Ѿ�������Ϸ" << endl;
					cout << "\n��Ϸ�����ѱ�����GameRanking.txt��" << endl;
				}
					
				system("PAUSE");
				return 0;	// ������Ҽ��˳�����
			}
		}

		//�����ײ���Ƿ񳬳���Χ
		if (cball.x > WINDOWWITH + BALLRADIUS || cball.x <-BALLRADIUS || cball.y > WINDOWHEIGHT + BALLRADIUS || cball.y < -BALLRADIUS)
		{
			cball.c = rand() % 6;
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
			ballMoving = FALSE;
			continue;
		}

	//	//�ƶ���������ײ��
		if (ballMoving == TRUE)
		{
			cleardevice();
			drawColBall(&cball, cball.x + (speed * ballDirction), cball.y - speed);
			cball.xspeed = (speed * ballDirction);
			cball.yspeed = ((-1) * speed);

			//�����������̾���
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
		gettextstyle(&f);						// ��ȡ��ǰ��������
		f.lfHeight = 48;						// ��������߶�Ϊ 48
		_tcscpy_s(f.lfFaceName, _T("����"));		// ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
		f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
		settextstyle(&f);						// ����������ʽ
		
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
		cout << "�Ѿ�������Ϸ" << endl;
	}
	system("PAUSE");

	return 0;
}
