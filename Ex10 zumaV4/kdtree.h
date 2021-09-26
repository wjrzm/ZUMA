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
kdnode* esearch(kdnode* root, DataType target);
void searchNearest(kdnode* root, DataType target, DataType& nearestdata, float& distance); 
void kdtree_destroy(kdnode* root);
