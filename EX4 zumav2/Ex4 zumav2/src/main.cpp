#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "LinkList.h"
#include "ball.h"
#include <math.h>
#include <stdio.h>
#include "assert.h"
#define WINDOWWITH 1000
#define WINDOWHEIGHT 1000

int count = 0;

/*typedef struct Point
{
	double x;
	double y;
}Point;
*/

typedef struct
{
	float X;
	float Y;
} PointF;

PointF bezier_interpolation_func(float t, PointF* points, int count)
{
	assert(count > 0);
	PointF tmp_points[100];
	for (int i = 1; i < count; ++i)
	{
		for (int j = 0; j < count - i; ++j)
		{
			if (i == 1)
			{
				tmp_points[j].X = (float)(points[j].X * (1 - t) + points[j + 1].X * t);
				tmp_points[j].Y = (float)(points[j].Y * (1 - t) + points[j + 1].Y * t);
				continue;
			}
			tmp_points[j].X = (float)(tmp_points[j].X * (1 - t) + tmp_points[j + 1].X * t);
			tmp_points[j].Y = (float)(tmp_points[j].Y * (1 - t) + tmp_points[j + 1].Y * t);
		}
	}
	return tmp_points[0];
}

void draw_bezier_curves(PointF* points, int count, PointF* out_points, int out_count)
{
	float step = 1.0 / out_count;
	float t = 0;
	for (int i = 0; i < out_count; i++)
	{
		PointF temp_point = bezier_interpolation_func(t, points, count);    // �����ֵ��
		t += step;
		out_points[i] = temp_point;
	}
}

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

	for (i = 0; i < 100; ++i) {
		//b.c = rand() % 6;
		b.c = i%6;
		ListInsert(head, i, b);

	}
}

//�������������λ��
void updateBallPos(Node* head,double *x,double *y)
{
	Node* p = head;
	//double x = 10 + count;
	//double y = WINDOWHEIGHT/2;
	int j = count;
	while (p->next != NULL)
	{
		
		p = p->next;
		
		p->data.x = x[j] ;
		//tempx = tempx + double(BALLRADIUS * 2);
		p->data.y = y[j];
		j++;
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
	Sleep(80);
}

int main()
{
	
	double x[100] = { 0 };
	double y[100] = { 0 };
/*
	Point p0, p1, p2, p3, pot;//��ʼ�� ���Ƶ�1 ���Ƶ�2 ��ֹ��3 ���Ƶ�
	double k, b;//б�� �� �ؾ�
	p0.x = 10;//����������ʼ��x
	p0.y = 200;//����������ʼ��y
	p1.x = 10;//���Ƶ���������������
	p1.y = 200;//���Ƶ���������������
	p2.x = 10;//���Ƶ���������������
	p2.y = 200;//���Ƶ���������������
	p3.x = 200;//�����������յ�x
	p3.y = 200;//�����������յ�y

	double t = 0.1;//���߲���t
	double temp = 1 - t;
	
	int j = 0;

	for (t = 0; t < 3; t += 0.03)
	{
		
		temp = 1 - t;
		pot.x = p0.x * temp * temp * temp + 3 * p1.x * t * temp * temp + 3 * p2.x * t * t * temp + p3.x * t * t * t;
		printf("%f,", pot.x);
		x[j] = pot.x;
		j++;
	}
	
	j = 0;
	printf("/n");

	for (t = 0; t < 3; t += 0.03)
	{
		temp = 1 - t;
		pot.y = p0.y * temp * temp * temp + 3 * p1.y * t * temp * temp + 3 * p2.y * t * t * temp + p3.y * t * t * t;
		printf("%f,", pot.y);
		y[j] = pot.y;
		j++;
	}
	*/

	PointF in[3] = { {50,950},{500,50},{950,950} }; // �����

	int num = 100;     // �������
	PointF out[100];    // ���������

	draw_bezier_curves(in, 3, out, num);// ���ױ���������

	for (int j = 0; j < num; j++)    // ���·����
	{
		x[j] = out[j].X;
		y[j] = out[j].Y;
	}

	// ��ʼ����ͼ����
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();

	//��ʼ������
	Node* head = NULL;
	head = CreateEmptyList();
	initBallList(head);
	updateBallPos(head,x,y);
	drawBallList(head);

	int id;
	bool sameColor;
	Node* q = NULL;
	ColorDetection(head, &sameColor, &q);
	if (sameColor )
	{
		PListDelete(q->next);
		PListDelete(q);
		updateBallPos(head,x,y);
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
		
		updateBallPos(head, x,y);
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
				updateBallPos(head,x,y);
			}
			else
			{
				ListInsert(head, id, cball);
				//ListDelete(head, id);
				updateBallPos(head,x,y);
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
					updateBallPos(head,x,y);
				}

			}
		}
		
		Sleep(20);
		count++;
		if (count == 99)
			count = 0;
	}
	EndBatchDraw();

}
	
