#include "LinkList.h"


/*
Args: 
    * None.
Return: 
    * Pointer of the head node.
Description:
    * Create an empty linked list with a head node.
    * The "next" field of the head node should point to NULL.
*/
Node* CreateEmptyList()
{
	Node* head;
	
	head =(Node*) malloc(sizeof(Node));
	if (head) {
		head->next = head->prior = NULL;
	}
	else exit(1);

	return head;
}


/*
Args:
    * addr: pointer of an array.
    * n: length of the array.
Return:
    * Pointer of the head node.
Description:
    * Initialize a linked list with an array.
*/
Node* CreateList(DataType *addr, unsigned int n)
{
	Node *head;

	head = (Node*)malloc(sizeof(Node));
	if (head) {
		head->next = head->prior = NULL;
	}
	else exit(1);

	for (unsigned int i = 0; i < n; i++)
	{
		ListInsert(head, i, addr[i]);
	}

	return head;
}


/*
Args:
    * head: pointer of the head node.
Returns:
    * None
Description:
    * Destroy the linked list.
    * Release all allocated memory.
*/
void DestroyList(Node *head)
{
	Node *p;

	
	while (head->next != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
	free(head);

}


/*
Args:
    * head: pointer of the head node.
    * index: index of the inserted data.
    * data: the inserted data.
Returns:
    * None.
Description:
    *   If the linked list is "head->3->5->2",
    * when you call (head, 0, 6), the linked 
    * list will be "head->6->3->5->2".
*/
void ListInsert(Node *head, unsigned int index, DataType data)
{
	unsigned int j = 0;
	Node* p = head;
 
	while (p && j < index)
	{
		p = p->next;
		++j;
	}
	if (p == NULL)
		return;

	Node *s = (Node*)malloc(sizeof(Node));
	if (s) {
	s->data = data;
	s->prior = p;
	s->next = p->next;
	if(p->next){
		p->next->prior = s;
	}
	p->next = s;
	}
	
}


/*
Args:
    * head: pointer of the head node.
    * index: index of the deleted data.
Returns:
    * The deleted data.
Description:
    *   If the linked list is "head->3->5->2",
    * when you call (head, 2), the linked 
    * list will be "head->3->5".
*/
DataType ListDelete(Node *head, unsigned int index)
{
	unsigned int j = 0;
	Node *p = head;
	Node* q = NULL;
	DataType data;

	while (p->next && j < index)
	{
		p = p->next;
		++j;
	}

	q = p->next;
	p->next = q->next;
	if (q->next) {
		q->next->prior = p;
	}
	data = q->data;
	free(q);

	return data;
}

DataType PListDelete(Node* p)
{
	DataType data;

	if ((p)->next) {
		(p)->next->prior = (p)->prior;
	}
	(p)->prior->next = (p)->next;
	
	
	data = (p)->data;
	free(p);

	return data;
}
