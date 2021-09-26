#pragma warning(disable:4996)
#include<iostream>
#include<stack>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include "kdtree.h"
#include "ball.h"
#include <crtdbg.h>//add this header file to detect memory leaks
#include <windows.h>
using namespace std;

#define WINDOWWITH 600
#define WINDOWHEIGHT 600
#define DEF_NUM_PTS 10

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



//�������kdtree
static void InOrderTraverse(kdnode* T)
{
	if (T)
	{
		InOrderTraverse(T->left);
		setcolor(ballColorTable[T->data.c]);
		setfillcolor(ballColorTable[T->data.c]);
		solidcircle(T->data.x, T->data.y, BALLRADIUS);
		InOrderTraverse(T->right);
	}
}

static float rd(void) {
	return (float)(BALLRADIUS + rand() % (WINDOWHEIGHT - 2 * BALLRADIUS));
}


void Moveball(kdnode* T,kdnode* root)
{
	int xs, ys;
	if (T)
	{
		Moveball(T->left,root);
		if (easysearch(root, T->data))
		{
			xs = T->data.xspeed;
			ys = T->data.yspeed;
			T->data.xspeed = (easysearch(root, T->data))->data.xspeed;
			T->data.yspeed = (easysearch(root, T->data))->data.yspeed;
			(easysearch(root, T->data))->data.xspeed = xs;
			(easysearch(root, T->data))->data.yspeed = ys;
		}
		T->data.x = int(T->data.x + T->data.xspeed);
		if (T->data.x + BALLRADIUS >= WINDOWWITH || T->data.x - BALLRADIUS <= 0)
			T->data.xspeed = (-1) * T->data.xspeed;
		T->data.y = int(T->data.y + T->data.yspeed);
		if (T->data.y + BALLRADIUS >= WINDOWHEIGHT || T->data.y - BALLRADIUS <= 0)
			T->data.yspeed = (-1) * T->data.yspeed;
		Moveball(T->right,root);
	}
}

int main()
{
	int grade = 0;
	//��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	TCHAR s[9];
	InputBox(s, 9, _T("����������"));
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
	float speed = 3;



	//�¼�ѭ����������ꡢʱ���¼�
	MOUSEMSG m;
	bool ballMoving = FALSE;

	while (root)
	{
		BeginBatchDraw();
		// ��ȡһ�������Ϣ
		if (MouseHit())
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
					cball.xspeed = speed * (m.x - WINDOWWITH / 2) / sqrtf((m.x - WINDOWWITH / 2) * (m.x - WINDOWWITH / 2) + (WINDOWHEIGHT - m.y) * (WINDOWHEIGHT - m.y));
					cball.yspeed = speed * (WINDOWHEIGHT - m.y) / sqrtf((m.x - WINDOWWITH / 2) * (m.x - WINDOWWITH / 2) + (WINDOWHEIGHT - m.y) * (WINDOWHEIGHT - m.y));
					ballMoving = TRUE;
				}
				break;

			case WM_RBUTTONUP:
				closegraph();
				kdtree_destroy(root);

				if (!_CrtDumpMemoryLeaks())
				{
					cout << "�Ѿ�������Ϸ" << endl;
				}
				
				system("PAUSE");
				FILE* fp;
				errno_t err = 0;
				err = fopen_s(&fp, "GameRanking.txt", "r");
				if (fp == NULL)
				{
					puts("Can't open file.\n");
					exit(-1);
				}
				int times;
				fscanf_s(fp, "%d", &times);
				player stu;
				int count = 0;
				while (true)
				{
					fscanf_s(fp, "%s", &stu.name,10);
					if (fscanf_s(fp, "%d", &stu.mark) == -1)
						break;
					count++;
				}
				player* p = new player[count + 1];
				fseek(fp, 0L, SEEK_SET);
				fscanf_s(fp, "%d", &times);
				times++;
				for (int i = 0;i < count;i++)
				{
					fscanf_s(fp, "%s", p[i].name,10);
					fscanf_s(fp, "%d", &p[i].mark);
				}
				strncpy_s(p[count].name, wchar2char(s),9);
				p[count].mark = grade;
				fclose(fp);
				Quicksort(p, count + 1);
				err = fopen_s(&fp, "GameRanking.txt", "w");
				if (fp == NULL)
				{
					puts("Can't open file.\n");
					exit(1);
				}
				fprintf(fp, "%d\n", times);
				for (int i = 0;i <= count;i++)
				{
					fprintf(fp, "%s\n", p[i].name);
					fprintf(fp, "%d\n", p[i].mark);
				}
				fclose(fp);
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
			drawColBall(&cball, cball.x + cball.xspeed, cball.y + cball.yspeed);
			cball.xspeed = (speed * ballDirction);
			cball.yspeed = ((-1) * speed);
			//�����������̾���
			searchNearest(root,cball,NearestBall,dis);
			if (dis <= 2* BALLRADIUS )
			{
				if (cball.c == NearestBall.c)
				{
					root = kdnode_delete(root, NearestBall);
					grade++;
				}
				else
				{
					root = kdnode_insert(root, cball);
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
		Moveball(root,root);
		InOrderTraverse(root);
		Sleep(5);
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
