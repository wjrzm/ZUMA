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

//绘图函数示例
void drawExample(void)
{
	// 设置绘图色为红色
	setcolor(RED);
	setfillcolor(RED);

	// 画矩形
	rectangle(100, 100, 300, 300);
	solidcircle(200, 200, 100);

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

//绘制球链链表，请补全该函数
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

//初始化球链链表，即给球链链表中添加一些球，请补全该函数
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

	// 初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
	cleardevice();

	//创建一个空的球链表
	Node* head = NULL;
	head = CreateEmptyList();
	
	//初始化球链链表
	initBallList(head);

	//在窗口中绘制球链表
	drawBallList(head);
	//绘图示例函数，补全drawBallList（）函数后请删除
	//drawExample();


	//画碰撞球
	ball cball;
	cball.c = rand() % 6;
	drawColBall(&cball, WINDOWWITH / 2, WINDOWHEIGHT);

	//等待任意键退出
	_getch();
	//销毁球链链表和绘图窗口
	DestroyList(head);
	closegraph();
	
	return 0;
}
	
