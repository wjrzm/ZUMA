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
		PointF temp_point = bezier_interpolation_func(t, points, count);    // 计算插值点
		t += step;
		out_points[i] = temp_point;
	}
}

COLORREF ballColorTable[] = {
BLUE, GREEN, RED, YELLOW, MAGENTA, BROWN,WHITE
};

//绘图函数示例
void drawExample(void)
{
	// 初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);

	// 设置背景色
	setbkcolor(BLACK);
	// 用背景色清空屏幕
	cleardevice();

	// 设置绘图色为红色
	setcolor(RED);
	setfillcolor(RED);

	// 画矩形
	rectangle(100, 100, 300, 300);
	solidcircle(200, 200, 100);

	// 按任意键退出
	_getch();
	closegraph();
}

//初始化球链表
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

//更新球链中球的位置
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

//球碰撞检测
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

//相邻两个球颜色相同检测
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



//绘制球表
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

//绘制碰撞球
void drawColBall(ball* b, int x, int y)
{
	b->x = x;
	b->y = y;

	setcolor(ballColorTable[b->c]);
	setfillcolor(ballColorTable[b->c]);
	solidcircle(b->x, b->y, BALLRADIUS);
}

//相邻两个球颜色相同则高亮
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
	Point p0, p1, p2, p3, pot;//起始点 控制点1 控制点2 终止点3 绘制点
	double k, b;//斜率 和 截距
	p0.x = 10;//比如这是起始点x
	p0.y = 200;//比如这是起始点y
	p1.x = 10;//控制点坐标可以随便整数
	p1.y = 200;//控制点坐标可以随便整数
	p2.x = 10;//控制点坐标可以随便整数
	p2.y = 200;//控制点坐标可以随便整数
	p3.x = 200;//比如这是起终点x
	p3.y = 200;//比如这是起终点y

	double t = 0.1;//曲线参数t
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

	PointF in[3] = { {50,950},{500,50},{950,950} }; // 输入点

	int num = 100;     // 输出点数
	PointF out[100];    // 输出点数组

	draw_bezier_curves(in, 3, out, num);// 二阶贝塞尔曲线

	for (int j = 0; j < num; j++)    // 输出路径点
	{
		x[j] = out[j].X;
		y[j] = out[j].Y;
	}

	// 初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();

	//初始化球链
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

	//画碰撞球
	ball cball;
	cball.c = rand() % 6;
	double ballDirction = -1;
	double speed = 10;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);

	//事件循环，处理鼠标、时间事件
	MOUSEMSG m;
	bool ballMoving = FALSE;
	BeginBatchDraw();
	
	while (true)
	{
		FlushBatchDraw();
		
		updateBallPos(head, x,y);
		drawBallList(head);
		
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
					ballDirction = double(m.x - WINDOWWITH / 2) / double(WINDOWHEIGHT - m.y);
					ballMoving = TRUE;
				}
				break;

			case WM_RBUTTONUP:
				DestroyList(head);
				closegraph();
				return 0;	// 按鼠标右键退出程序
			}
		}
		
		//定时更新球链、碰撞球
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
		
		
		//检测碰撞球是否超出范围
		if (cball.x > WINDOWWITH || cball.x <0 || cball.y > WINDOWHEIGHT || cball.y < 0)
		{
			cball.c = rand() % 6;
			drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);
			ballMoving = FALSE;
		}
		
		//移动、绘制碰撞球
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
	
