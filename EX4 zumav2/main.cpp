#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "LinkList.h"
#include "ball.h"
#include <math.h>
#include <stdio.h>
#include "assert.h"
#include <atlstr.h>
#include <atlbase.h>
#include <atlstr.h>
using namespace std;

#define WINDOWWITH 1024
#define WINDOWHEIGHT 720



COLORREF ballColorTable[] = {
BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN,WHITE
};

//��ͼ����ʾ��
void drawExample(void)
{
	// ��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);

	// ���ñ���ɫ
	setbkcolor(BLACK);
	// �ñ���ɫ�����Ļ
	cleardevice();

	// ���û�ͼɫΪ��ɫ
	setcolor(RED);
	setfillcolor(RED);

	// ������
	rectangle(100, 100, 300, 300);
	solidcircle(200, 200, 100);

	// ��������˳�
	_getch();
	closegraph();
}

//��ʼ��������
void initBallList(Node* head)
{
	int i;
	ball b;

	for (i = 0; i < 20; ++i) {
		//b.c = rand() % 6;
		b.c = i%6;
		ListInsert(head, i, b);

	}
}

//�������������λ��
void updateBallPos(Node* head,int countx,int county)
{
	Node* p = head;
	double x = BALLRADIUS + countx;
	double y = WINDOWHEIGHT/2 + county;

	while (p->next != NULL)
	{
		p = p->next;
		
		p->data.x = x ;
		x = x + double(BALLRADIUS * 2);
		p->data.y = y;
	}
	
}

//����ײ���
bool collisionDetection(Node* head, ball b,bool* sameColor,int* id)
{
	Node* p = head->next;
	double dist = 0;
	int index = 0;

	while (p != NULL)
	{
		dist = (p->data.x - b.x) * (p->data.x - b.x) + (p->data.y - b.y) * (p->data.y - b.y);
		if (dist < 150)
		{
			if (b.c == p->data.c)
				* (sameColor) = TRUE;
			else
				*(sameColor) = FALSE;
			*id = index;
			return TRUE;
		}

		p = p->next;
		index++;
	}
	return FALSE;
		
}

//������������ɫ��ͬ���
bool ColorDetection(Node* head,bool* sameColor,Node** q)
{
	Node* p = head->next;

	if (p) {
		while (p->next != NULL) {
			if (p->data.c == p->next->data.c) {
				*(sameColor) = TRUE;
				*q = p;
				return TRUE;
			}
			else
				*(sameColor) = FALSE;
			p = p->next;
		}
	}
	return FALSE;
}



//�������
void drawBallList(Node* head)
{
	Node* p;
	ball b;

	p = head;

	while (p->next != NULL)
	{
		p = p->next;
		b = p->data;

		setcolor(ballColorTable[b.c]);
		setfillcolor(ballColorTable[b.c]);
		solidcircle(b.x, b.y, BALLRADIUS);

	}
}

//������ײ��
void drawColBall(ball* b, int x, int y)
{
	b->x = x;
	b->y = y;

	setcolor(ballColorTable[b->c]);
	setfillcolor(ballColorTable[b->c]);
	solidcircle(b->x, b->y, BALLRADIUS);
}

//������������ɫ��ͬ�����
void highLight(Node* head, Node* temp)
{
	temp->data.c = 6;
	temp->next->data.c = 6;
	cleardevice();
	//updateBallPos(head);
	drawBallList(head);
	Sleep(70);
}

int getLength(Node* head)
{
	Node* p = NULL;
	int length = 0;

	if (head->next) {
		p = head->next;
		length++;
		while (p->next) {
			p = p->next;
			length++;
		}
	}

	return length;
}


int main()
{

	int countx = 0;
	int county = 0;
	int score = 0;
	bool win = FALSE;

	printf("\n\n\nREAD ME!!\n\n\n��ӭ����������װ棡��\n\n\n"
		"Ϊ�������õ���Ϸ����\n"
		"������Ϊ������һ����Ϸ�����\n"
		"���������е�С����������Ļ�Ϲ�����С��\n"
		"����С����ɫ��ͬ��ᱻ����һ��С����ʧ���һ��\n"
		"�����е�С����ʧҲ��������\n"
		"��С��������ײ�֮ǰ\n"
		"����������Ļ�Ϲ���������С��\n"
		"��Ϊ��ʤ\n"
		"����С��������ײ�ʱ��δ����������С��\n"
		"��Ϊʧ��\n"
		"\n\n\n�밴������1�ٰ��»س���������Ϸ����\n"
		);

	int key = 0;
	scanf_s("%d", &key);
	if (key != 1) {
		return 0;
	}

	// ��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();

	//��ʼ������
	Node* head = NULL;
	head = CreateEmptyList();
	initBallList(head);
	updateBallPos(head,countx,county);
	drawBallList(head);

	int id;
	bool sameColor;
	Node* q = NULL;
	ColorDetection(head, &sameColor, &q);
	if (sameColor )
	{
		PListDelete(q->next);
		PListDelete(q);
		score += 2;
		updateBallPos(head,countx,county);
	}

	//����ײ��
	ball cball;
	cball.c = rand() % 6;
	double ballDirction = -1;
	double speed = 10;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);

	//�¼�ѭ����������ꡢʱ���¼�
	MOUSEMSG m;
	bool ballMoving = FALSE;
	BeginBatchDraw();
	
	while (true)
	{
		FlushBatchDraw();
		
		updateBallPos(head, countx,county);
		drawBallList(head);
		
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
					ballDirction = double(m.x - WINDOWWITH / 2) / double(WINDOWHEIGHT - m.y);
					ballMoving = TRUE;
				}
				break;

			case WM_RBUTTONUP:
				DestroyList(head);
				closegraph();
				return 0;	// ������Ҽ��˳�����
			}
		}
		
		//��ʱ������������ײ��
		cleardevice();
		
		bool collision = collisionDetection(head, cball, &sameColor,&id);
		if (collision)
		{
			if (!sameColor)
			{
				ListInsert(head, id, cball);
				updateBallPos(head,countx,county);
			}
			else
			{
				ListInsert(head, id, cball);
				//ListDelete(head, id);
				updateBallPos(head,countx,county);
			}
			
			cball.c = rand() % 6;
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
			ballMoving = FALSE;

		}
		
			drawBallList(head);
		
		
		//�����ײ���Ƿ񳬳���Χ
		if (cball.x > WINDOWWITH || cball.x <0 || cball.y > WINDOWHEIGHT || cball.y < 0)
		{
			cball.c = rand() % 6;
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
			ballMoving = FALSE;
		}
		
		//�ƶ���������ײ��
		if(ballMoving == TRUE) 
		{
			drawColBall(&cball, cball.x += (speed * ballDirction) , cball.y -= speed);
		}
		else
		{
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
		}

		if (collision) {
			bool sameColor;
			Node* q = NULL;
			while (ColorDetection(head, &sameColor, &q)) {
				if (sameColor)
				{
					highLight(head, q);

					PListDelete(q->next);
					//updateBallPos(head);
					PListDelete(q);
					score += 2;
					updateBallPos(head,countx,county);
				}

			}
		}
		int length = getLength(head);
		

		
		Sleep(20);
		countx++;
		if (countx == WINDOWWITH && county != WINDOWHEIGHT/2) {
			countx = (-length * BALLRADIUS * 2);
			county += BALLRADIUS * 4;
		}
		
		CString CS_score,CS_length,CS_win,CS_gameOver;
		CS_score.Format(_T("Score: %d"), score);
		CS_length.Format(_T("Length: %d"), length);
		CS_win.Format(_T("Win!!"));
		CS_gameOver.Format(_T("Game Over"));

		LOGFONT f;
		gettextstyle(&f);						// ��ȡ��ǰ��������
		f.lfHeight = 150;						// ��������߶�Ϊ 48
		_tcscpy_s(f.lfFaceName, _T("Consolas"));		
		f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
		settextstyle(&f);						// ����������ʽ
		RECT zuma = { 0, 0, WINDOWWITH, WINDOWHEIGHT/4 };
		drawtext(_T("ZUMA"), &zuma, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		
		RECT scoren = { WINDOWWITH / 4 , 0 , WINDOWWITH / 2 , WINDOWHEIGHT / 2 };
		RECT lengthn = { WINDOWWITH / 2 , 0, WINDOWWITH * 3 / 4, WINDOWHEIGHT / 2 };
		RECT endn = { 0 , WINDOWHEIGHT / 4 , WINDOWWITH , WINDOWHEIGHT };
		
	
		f.lfHeight = 50;
		settextstyle(&f);
		settextcolor(WHITE);

		drawtext(CS_score, &scoren, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(CS_length, &lengthn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (length == 0) {
			f.lfHeight = 200;
			settextstyle(&f);
			settextcolor(WHITE);
			drawtext(CS_win, &endn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		
		if(county >= WINDOWHEIGHT / 2){
			f.lfHeight = 200;
			settextstyle(&f);
			settextcolor(WHITE);
			drawtext(CS_gameOver, &endn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

	}
	EndBatchDraw();

}
	
