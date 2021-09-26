#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "LinkList.h"
#include "ball.h"

#define WINDOWWITH 600
#define WINDOWHEIGHT 600
#define length 10

COLORREF ballColorTable[] = {
BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN
};

//��ͼ����ʾ��
void drawExample(void)
{
	// ���û�ͼɫΪ��ɫ
	setcolor(RED);
	setfillcolor(RED);

	// ������
	rectangle(100, 100, 300, 300);
	solidcircle(200, 200, 100);

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

//�������������벹ȫ�ú���
void drawBallList(Node* head)
{
	if (head) {
		Node* p = head->next;
		for (int i = 0; i < length; i++) {
			setcolor(ballColorTable[p->data.c]);
			setfillcolor(ballColorTable[p->data.c]);
			solidcircle(p->data.x, p->data.y, BALLRADIUS);
			p = p->next;
		}
	}
	else {
		exit(1);
	}
	
}

//��ʼ���������������������������һЩ���벹ȫ�ú���
void initBallList(Node* head)
{
	if (head) {
		ball arrayball[length] = { 0 };
		for (int i = 0; i < length; i++) {
			arrayball[i] = { 10 + 20*i,300,rand() % 6 };
		}
		for (int j = 0; j < length; j++) {
			ListInsert(head, 0, arrayball[j]);
		}
	}
	else {
		exit(1);
	}
}
	

int main()
{
	srand(time(NULL));

	// ��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();

	//����һ���յ�������
	Node* head = NULL;
	head = CreateEmptyList();
	
	//��ʼ����������
	initBallList(head);

	//�ڴ����л���������
	drawBallList(head);
	//��ͼʾ����������ȫdrawBallList������������ɾ��
	//drawExample();


	//����ײ��
	ball cball;
	cball.c = rand() % 6;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);

	//�ȴ�������˳�
	_getch();
	//������������ͻ�ͼ����
	DestroyList(head);
	closegraph();
	
	return 0;
}
	
