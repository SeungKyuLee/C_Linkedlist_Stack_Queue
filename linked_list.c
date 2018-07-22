#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "linked_list.h"


struct list_node_t {
	struct list_node_t *prev;
	struct list_node_t *next;
	void *data;
};

struct list_t {
	list_node_t *head;
	list_node_t *tail;
};

/*
 * New List Memory Allocation
 */
list_t *
list_new(
)
{
	list_t *list;

	list = (list_t *)malloc(sizeof(list_t));
	list->head = list->tail = NULL;

	return list;
}

/* 
 * List Memory FREE 
 */
void
list_delete(
list_t *list
)
{
	assert(list);

	FREE(list);
}

/*
 * The First Item of List
 */
list_node_t *
list_first(
list_t *list
)
{
	assert(list);

	return list->head;
}

/*
 * The Last Item of List
 */
list_node_t *
list_last(
list_t *list
)
{
	assert(list);

	return list->tail;
}

/*
 * Return List's Next_Node's Pointer
 */
list_node_t *
list_next(
list_node_t *node
)
{
	assert(node);

	return node->next;
}

/*
 * Return List's Previous_Node's Pointer
 */
list_node_t *
list_prev(
list_node_t *node
)
{
	assert(node);

	return node->prev;
}

/*
 * Return Node is Empty
 */
int list_is_empty(
	list_t *list
	)
{
	assert(list);

	return (NULL == list->head);
}

/*
 * Insert a Item Node to Target List's Head
 */
void
list_insert_head(
list_t *list,
list_node_t *node
)
{
	assert(list && node);

	node->prev = NULL;
	node->next = list->head;
	if (list->head)
		list->head->prev = node;
	else // empty list
		list->tail = node;
	list->head = node;
}

/*
 * Insert a Item Node to Target List's Tail
 */
void
list_insert_tail(
list_t *list,
list_node_t *node
)
{
	assert(list && node);

	node->prev = list->tail;
	node->next = NULL;
	if (list->tail)
		list->tail->next = node;
	else // empty que
		list->head = node;
	list->tail = node;
}

/*
 * Insert a Item Node to before Target Node
 */
void
list_insert_before(
list_t *list,
list_node_t *node,
list_node_t *new_node
)
{
	assert(list && node && new_node);

	new_node->prev = node->prev;
	new_node->next = node;
	if (NULL == node->prev)
		list->head = new_node;
	else
		node->prev->next = new_node;
	node->prev = new_node;
}

/*
 * Insert a Item Node to after Target Node
 */
void
list_insert_after(
list_t *list,
list_node_t *node,
list_node_t *new_node
)
{
	assert(list && node && new_node);

	new_node->prev = node;
	new_node->next = node->next;
	if (NULL == node->next)
		list->tail = new_node;
	else
		node->next->prev = new_node;
	node->next = new_node;
}

/*
 * Delete a Node from Target List
 */
void
list_remove(
list_t *list,
list_node_t *node
)
{
	assert(list && node);

	if (NULL == node->prev)
	{
		list->head = node->next;
		if (node->next)
			node->next->prev = NULL;
	}
	else
		node->prev->next = node->next;

	if (NULL == node->next)
	{
		list->tail = node->prev;
		if (node->prev)
			node->prev->next = NULL;
	}
	else
		node->next->prev = node->prev;
}

/*
 * Clear List's Head and Tail
 */
void
list_head_tail_clear(
list_t *list
)
{
	assert(list);

	list->head = list->tail = NULL;
}

/*
 * New List's Node Memory Allocation
 */
list_node_t *
list_node_new(
)
{
	return (list_node_t *)calloc(1, sizeof(list_node_t));
}

/*
 * Node's Memory Free
 */
void
list_node_delete(
list_node_t *node
)
{
	assert(node);

	FREE(node);
}

/*
 * Set a Node's meaningful Data
 */
void
list_node_set_data(
list_node_t *node,
void *data
)
{
	assert(node);

	node->data = data;
}

/*
 * Return Target Node's meaningful Data Pointer(Casting Needed)
 */
void *
list_node_get_data(
list_node_t *node
)
{
	assert(node);

	return node->data;
}
