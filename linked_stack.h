#ifndef _STACK_H_
#define _STACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "linked_list.h"
#include "linked_config.h"

	typedef list_node_t stack_node_t;
	typedef struct stack_t stack_t;

	/*
	* New Stack Structure Memory Allocation
	*/
	stack_t *
		stack_new(
		);

	/*
	* All Item and Stack Memeory Free
	*/
	void
		stack_delete(
		stack_t *stack
		);

	/*
	* Return Stack is Empty
	*/
	int
		stack_is_empty(
		stack_t *stack
		);

	/*
	* Return Stack Size
	*/
	ssize_t
		stack_get_size(
		stack_t *stack
		);

	/*
	* Return first Item from Stack
	* (An Item is pushing First of List)
	*/
	stack_node_t *
		stack_first(
		stack_t *stack
		);

	/*
	* Return last Item from Stack
	* (An Item is pushing is First of List)
	*/
	stack_node_t *
		stack_last(
		stack_t *stack
		);

	/*
	* Return target after Target Item Node
	*/
	stack_node_t *
		stack_next(
		stack_t *stack,
		stack_node_t *node
		);

	/*
	* Return target previous Target Item Node
	*/
	stack_node_t *
		stack_prev(
		stack_t *stack,
		stack_node_t *node
		);

	/*
	* Return Data from target Item Node
	*/
	void *
		stack_node_get_data(
		stack_node_t *node
		);
	/*
	* An Item is pushing in Target Stack
	*/
	void*
		stack_top(
		stack_t *stack
		);

	/*
	* An Item is pushing in Target Stack
	*/
	int
		stack_push(
		stack_t *stack,
		void *data
		);

	/*
	* An Item's Data is poping in Target Stack
	*/
	void *
		stack_pop_node_data(
		stack_t *stack
		);

	/*
	* An Item is poping in Target Stack
	*/
	list_node_t *
		stack_pop_node(
		stack_t *stack
		);

	/*
	* An Item is Deleting from Stack
	*/
	void
		stack_remove_node(
		stack_t *stack,
		stack_node_t *node
		);

	/*
	* Free All nodes in Stack
	*/
	void
		stack_clear(
		stack_t *stack
		);

	// reentrant msg stack which has locking system
	typedef stack_node_t stack_mt_node_t;
	typedef struct stack_mt_t stack_mt_t;

	/*
	* Sync Stack Memory Allocation
	*/
	stack_mt_t *
		stack_mt_new(
		);

	/*
	* All Item and Stack Memeory Free (Sync)
	*/
	void
		stack_mt_delete(
		stack_mt_t *stack
		);

	/*
	* Return Sync Stack is Empty
	*/
	int
		stack_mt_is_empty(
		stack_mt_t *stack
		);

	/*
	* Return Sync Stack Size
	*/
	ssize_t
		stack_mt_get_size(
		stack_mt_t *stack
		);

	/*
	* Return first Item from Sync Stack
	*/
	stack_mt_node_t *
		stack_mt_first(
		stack_mt_t *stack
		);

	/*
	* Return next Item from Target Node
	*/
	stack_mt_node_t *
		stack_mt_next(
		stack_mt_t *stack,
		stack_mt_node_t *node
		);

	/*
	* Return Data from target Item Node
	*/
	void *
		stack_mt_node_get_data(
		stack_mt_t *stack,
		stack_mt_node_t *node
		);

	/*
	* An Item is pushing in Target Sync Queue
	*/
	int
		stack_mt_push_back(
		stack_mt_t *stack,
		void *data
		);

	/*
	* An Item is Poping in Target Sync Stack
	*/
	void *
		stack_mt_pop_back(
		stack_mt_t *stack
		);

	/*
	* Free All nodes in Sync Stack
	*/
	void
		stack_mt_clear(
		stack_mt_t *stack
		);

#ifdef __cplusplus
}
#endif

#endif	/* end of header */
