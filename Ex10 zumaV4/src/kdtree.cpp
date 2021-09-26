#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <algorithm>    
#include <stack>    
#include <math.h> 
#include <windows.h>
#include "kdtree.h"
using namespace std;

// A method to create a kdnode of K D tree
struct kdnode* newNode(DataType data,unsigned depth)
{
    struct kdnode* temp = (kdnode*)malloc(sizeof(kdnode));

    temp->data = data;
    temp->split = depth % k;
    temp->left = temp->right = NULL;
    if (temp->data.yspeed == 0)
    {
        temp->data.xspeed = rand() % 3 - 1;
        temp->data.yspeed = rand() % 3 - 1;
    }
    return temp;
}

// Inserts a new node and returns root of modified tree 
// The parameter depth is used to decide axis of comparison 
static kdnode* insertRec(kdnode* root, DataType data, unsigned depth)
{
    // Tree is empty? 
    if (root == NULL)
        return newNode(data,depth);

    // Calculate current dimension (cd) of comparison 
    unsigned cd = depth % k;

    // Compare the new point with root on current dimension 'cd' 
    // and decide the left or right subtree 
    if (cd ==0)
    {
        if (data.x < (root->data.x))
            root->left = insertRec(root->left, data, depth + 1);
        else
            root->right = insertRec(root->right, data, depth + 1);
    }
    else
    {
        if (data.y < (root->data.y))
            root->left = insertRec(root->left, data, depth + 1);
        else
            root->right = insertRec(root->right, data, depth + 1);
    }


    return root;
}

// Function to insert a new point with given point in 
// KD Tree and return new root. It mainly uses above recursive 
// function "insertRec()" 
kdnode* kdnode_insert(kdnode* root, DataType data)
{
    return insertRec(root, data, 0);
}

// A utility function to find minimum of three integers 
kdnode* minNode(kdnode* x, kdnode* y, kdnode* z, int d)
{
    kdnode* res = x;
    if (d == 0)
    {
        if (y != NULL && y->data.x < res->data.x)
            res = y;
        if (z != NULL && z->data.x < res->data.x)
            res = z;
    }
    else
    {
        if (y != NULL && y->data.y < res->data.y)
            res = y;
        if (z != NULL && z->data.y < res->data.y)
            res = z;
    }
    return res;
}

// Recursively finds minimum of d'th dimension in KD tree 
// The parameter depth is used to determine current axis. 
static kdnode* findMinRec(kdnode* root, int d, unsigned depth)
{
    // Base cases 
    if (root == NULL)
        return NULL;

    // Current dimension is computed using current depth and total 
    // dimensions (k) 
    unsigned cd = depth % k;

    // Compare point with root with respect to cd (Current dimension) 
    if (cd == d)
    {
        if (root->left == NULL)
            return root;
        return findMinRec(root->left, d, depth + 1);
    }

    // If current dimension is different then minimum can be anywhere 
    // in this subtree 
    return minNode(root,
        findMinRec(root->left, d, depth + 1),
        findMinRec(root->right, d, depth + 1), d);
}

// A wrapper over findMinRec(). Returns minimum of d'th dimension 
kdnode* findMin(kdnode* root, int d)
{
    // Pass current level or depth as 0 
    return findMinRec(root, d, root->split);
}

// A utility method to determine if two Points are same 
// in K Dimensional space 
static bool arePointsSame(DataType data1, DataType data2)
{
    // Compare individual pointinate values 
    if (data1.x == data2.x && data1.y == data2.y) return true;
    return false;
}

// Copies data2 to data1 
void copyPoint(DataType &data1, DataType data2)
{
    data1 = data2;
}

// Function to delete a given data from tree with root 
// as 'root'.  depth is current depth and passed as 0 initially. 
// Returns root of the modified tree. 
static kdnode* deleteNodeRec(kdnode* root, DataType data, int depth)
{
    // Given point is not present 
    if (root == NULL)
        return NULL;

    // Find dimension of current node 
    int cd = depth % k;

    // If the point to be deleted is present at root 
    if (arePointsSame(root->data, data))
    {
        // 2.b) If right child is not NULL 
        if (root->right != NULL)
        {
            // Find minimum of root's dimension in right subtree 
            kdnode* Min = findMin(root->right, cd);

            // Copy the minimum to root 
            copyPoint(root->data, Min->data);

            // Recursively delete the minimum 
            root->right = deleteNodeRec(root->right, Min->data, depth + 1);
        }
        else if (root->left != NULL) // same as above 
        {
            kdnode* Min = findMin(root->left, cd);
            copyPoint(root->data, Min->data);
            root->right = deleteNodeRec(root->left, Min->data, depth + 1);
            root->left = NULL;
        }
        else // If node to be deleted is leaf node 
        {
            free(root);
            root = NULL;
            return NULL;
        }
        return root;
    }

    // 2) If current node doesn't contain point, search downward 
    if (cd == 0)
    {
        if (data.x < root->data.x)
            root->left = deleteNodeRec(root->left, data, depth + 1);
        else
            root->right = deleteNodeRec(root->right, data, depth + 1);
    }
    else
    {
        if (data.y < root->data.y)
            root->left = deleteNodeRec(root->left, data, depth + 1);
        else
            root->right = deleteNodeRec(root->right, data, depth + 1);
    }

    return root;
}

// Function to delete a given data from K D Tree with 'root' 
kdnode* kdnode_delete(kdnode* root, DataType data)
{
    // Pass depth as 0 
    return deleteNodeRec(root, data, 0);
}

static float Distance(DataType a, DataType b) {
    float tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    return sqrt(tmp);
}

//向栈中添加经过的结点
void Addpath(kdnode* root, DataType target, stack<kdnode*>& search_path) {
    kdnode* pSearch = root;
    while (pSearch != NULL) {
        search_path.push(pSearch);
        if (pSearch->split == 0) {
            if (target.x < pSearch->data.x) {
                pSearch = pSearch->left;
            }
            else {
                pSearch = pSearch->right;
            }
        }
        else {
            if (target.y < pSearch->data.y) {
                pSearch = pSearch->left;
            }
            else {
                pSearch = pSearch->right;
            }
        }
    }
}

kdnode* easysearch(kdnode* root, DataType target)
{
    kdnode* p, *q;
    if (root)
    {
        float dist;
        dist = Distance(root->data, target);
        if (dist > 0 && dist <= 2 * BALLRADIUS)
        {
            return root;
        }
        p=easysearch(root->left, target);
        q=easysearch(root->right, target);
        if (p != NULL || q != NULL)
        {
            if (p == NULL)
                return q;
            else
                return p;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

//Function to find the nearestdata and the distance 
void searchNearest(kdnode* root, DataType target, DataType& nearestdata, float& distance) {

    //1. 如果Kd是空的，则设dist为无穷大返回    

    //2. 向下搜索直到叶子结点    

    stack<kdnode*> search_path;
    kdnode* pSearch = root;
    DataType nearest;
    float dist;

    Addpath(pSearch, target, search_path);

    //取出search_path最后一个赋给nearest    
    nearest = search_path.top()->data;

    dist = Distance(nearest, target);
    //3. 回溯搜索路径    

    kdnode* pBack;

    while (search_path.size() != 0)
    {
        //取出search_path最后一个结点赋给pBack    
        pBack = search_path.top();
        search_path.pop();

        if (pBack->left == NULL && pBack->right == NULL) /* 如果pBack为叶子结点 */

        {

            if (Distance(nearest, target) > Distance(pBack->data, target))
            {
                nearest = pBack->data;
                dist = Distance(pBack->data, target);
            }

        }

        else

        {

            int s = pBack->split;
            if (s == 0)
            {
                if (fabs(pBack->data.x - target.x) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if (Distance(nearest, target) > Distance(pBack->data, target))
                    {
                        nearest = pBack->data;
                        dist = Distance(pBack->data, target);
                    }
                    if (target.x <pBack->data.x) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
                        pSearch = pBack->right;
                    else
                        pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
                    if (pSearch != NULL)
                        //从pSearch开始添加结点加入到search_path中   
                        Addpath(pSearch, target, search_path);
                }
            }
            else {
                if (fabs(pBack->data.y - target.y) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if (Distance(nearest, target) > Distance(pBack->data, target))
                    {
                        nearest = pBack->data;
                        dist = Distance(pBack->data, target);
                    }
                    if (target.y < pBack->data.y) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
                        pSearch = pBack->right;
                    else
                        pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
                    if (pSearch != NULL)
                        // 从pSearch开始添加结点加入到search_path中    
                        Addpath(pSearch, target, search_path);
                }
            }

        }
    }

    nearestdata = nearest;
    distance = dist;

}


void kdtree_destroy(kdnode* root)
{
    if (root)
    {
        kdtree_destroy(root->left);
        kdtree_destroy(root->right);
        free(root);
        root = NULL;
    }
}

void Quicksort(player* p, int n)
{
    Myqsort(p, 0, n - 1);
}

void Myqsort(player* p, int left, int right)
{
    if (left<right)
    {
        int pivotloc = partition(p,left,right);
        Myqsort(p, left, pivotloc - 1);
        Myqsort(p, pivotloc + 1, right);
    }
}

int partition(player* p, int left,int right)
{
    player temp = p[left];
    int pivotkey = temp.mark;
    while (left < right)
    {
        while (p[right].mark >= pivotkey && left < right)
            right--;
        p[left] = p[right];
        while (p[left].mark <= pivotkey && left < right)
            left++;
        p[right] = p[left];
    }
    p[left] = temp;
    return left;
}

char* wchar2char(const wchar_t* wchar)
{
    char* m_char;
    int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
    m_char = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    return m_char;
}
