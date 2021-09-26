#pragma once

#include "ball.h"

//2d
#define k 2

// A structure to represent node of kd tree 
struct kdnode
{
    DataType data;
    int split;
    kdnode* left, * right;
};


struct kdnode* newNode(DataType data,unsigned depth);
kdnode* kdnode_insert(kdnode* root, DataType data);
kdnode* findMin(kdnode* root, int d);
kdnode* kdnode_delete(kdnode* root, DataType data);
kdnode* easysearch(kdnode* root, DataType target);
void searchNearest(kdnode* root, DataType target, DataType& nearestdata, float& distance); 
void kdtree_destroy(kdnode* root);
void Quicksort(player* p, int n);
void Myqsort(player* p, int left, int right);
int partition(player* p, int left, int right);
char* wchar2char(const wchar_t* wchar);

