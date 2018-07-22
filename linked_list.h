#ifndef _LIST_H_
#define _LIST_H_

#include <inttypes.h>
#include "linked_config.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct list_t list_t;

	typedef struct list_node_t list_node_t;

	/*
	* New List Memory Allocation
	*/
	list_t *list_new(
		);

	/*
	* List Memory FREE
	*/
	void list_delete(
		);

	/*
	* The First Item of List
	*/
	list_node_t *list_first(
		list_t *list
		);

	/*
	* The Last Item of List
	*/
	list_node_t *list_last(
		list_t *list
		);

	/*
	* Return List's Next_Node's Pointer
	*/
	list_node_t *list_next(
		list_node_t *node
		);

	/*
	* Return List's Previous_Node's Pointer
	*/
	list_node_t *list_prev(
		list_node_t *node
		);

	/*
	* Return Node is Empty
	*/
	int list_is_empty(
		list_t *list
		);

	/*
	* Insert a Item Node to Target List's Head
	*/
	void
		list_insert_head(
		list_t *list,
		list_node_t *node
		);

	/*
	* Insert a Item Node to Target List's Tail
	*/
	void
		list_insert_tail(
		list_t *list,
		list_node_t *node
		);

	/*
	* Insert a Item Node to before Target Node
	*/
	void
		list_insert_before(
		list_t *list,
		list_node_t *node,
		list_node_t *new_node
		);

	/*
	* Insert a Item Node to after Target Node
	*/
	void
		list_insert_after(
		list_t *list,
		list_node_t *node,
		list_node_t *new_node
		);

	/*
	* Delete a Node from Target List
	*/
	void
		list_remove(
		list_t *list,
		list_node_t *node
		);

	/*
	* Clear List's Head and Tail
	*/
	void
		list_head_tail_clear(
		list_t *list
		);

	/*
	* New List's Node Memory Allocation
	*/
	list_node_t *
		list_node_new(
		);

	/*
	* Node's Memory Free
	*/
	void
		list_node_delete(
		list_node_t *node
		);

	/*
	* Set a Node's meaningful Data
	*/
	void
		list_node_set_data(
		list_node_t *node,
		void *data
		);

	/*
	* Return Target Node's meaningful Data Pointer(Casting Needed)
	*/
	void *
		list_node_get_data(
		list_node_t *node
		);

#ifdef __cplusplus
}
#endif

#endif

