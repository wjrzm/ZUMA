#pragma once
#define BALLRADIUS 10

//xy�������ɫindex
typedef struct ball
{
	float x;
	float y;
	int c;
	float xspeed=0;
	float yspeed=0;
} DataType;
struct player
{
	char name[10];
	int mark;
};