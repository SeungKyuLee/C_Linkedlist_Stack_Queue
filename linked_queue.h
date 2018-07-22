#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "linked_list.h"
#include "linked_config.h"


	typedef list_node_t queue_node_t;/* List Queue */
	typedef struct queue_t queue_t;

	/*
	* New Queue Structure Memory Allocation
	*/
	queue_t *
		queue_new(
		);

	/*
	* All Item and Queue Memeory Free
	*/
	void
		queue_delete(
		queue_t *que
		);

	/*
	* Return Queue is Empty
	*/
	int
		queue_is_empty(
		queue_t *que
		);

	/*
	* Return Queue Size
	*/
	ssize_t
		queue_get_size(
		queue_t *que
		);

	/*
	* Return first Item from Queue
	*/
	queue_node_t *
		queue_first(
		queue_t *que
		);

	/*
	* Return last Item from Queue
	*/
	queue_node_t *
		queue_last(
		queue_t *que
		);

	/*
	* Return target after Target Item Node
	*/
	queue_node_t *
		queue_next(
		queue_t *que,
		queue_node_t *node
		);

	/*
	* Return target previous Target Item Node
	*/
	queue_node_t *
		queue_prev(
		queue_t *que,
		queue_node_t *node
		);

	/*
	* Return Data from target Item Node
	*/
	void *
		queue_node_get_data(
		queue_node_t *node
		);

	/*
	* An Item is Enqueuing in Target Queue
	*/
	int
		queue_push(
		queue_t *que,
		void *data
		);

	/*
	* An Item's Data is Poping in Target Queue
	*/
	void *
		queue_pop_node_data(
		queue_t *que
		);

	/*
	* An Item is Poping in Target Queue
	*/
	list_node_t *
		queue_pop_node(
		queue_t *que
		);

	/*
	* An Item is Deleting from Queue
	*/
	void
		queue_remove_node(
		queue_t *que,
		queue_node_t *node
		);

	/*
	* Free All nodes in Queue
	*/
	void
		queue_clear(
		queue_t *que
		);

	// reentrant msg que which has locking system
	typedef queue_node_t queue_mt_node_t;
	typedef struct queue_mt_t queue_mt_t;

	/*
	* Sync Queue Memory Allocation
	*/
	queue_mt_t *
		queue_mt_new(
		);

	/*
	* All Item and Queue Memeory Free (Sync)
	*/
	void
		queue_mt_delete(
		queue_mt_t *que
		);

	/*
	* Return Sync Queue is Empty
	*/
	int
		queue_mt_is_empty(
		queue_mt_t *que
		);

	/*
	* Return Sync Queue Size
	*/
	ssize_t
		queue_mt_get_size(
		queue_mt_t *que
		);

	/*
	* Return first Item from Sync Queue
	*/
	queue_mt_node_t *
		queue_mt_first(
		queue_mt_t *que
		);

	/*
	* Return next Item from Target Node
	*/
	queue_mt_node_t *
		queue_mt_next(
		queue_mt_t *que,
		queue_mt_node_t *node
		);

	/*
	* Return Data from target Item Node
	*/
	void *
		queue_mt_node_get_data(
		queue_mt_t *que,
		queue_mt_node_t *node
		);

	/*
	* An Item is Enqueuing in Target Sync Queue
	*/
	int
		queue_mt_push_back(
		queue_mt_t *que,
		void *data
		);
	
	/*
	* An Item is Poping in Target Sync Queue
	*/
	void *
		queue_mt_pop_back(
		queue_mt_t *que
		);

	/*
	* Free All nodes in Sync Queue
	*/
	void
		queue_mt_clear(
		queue_mt_t *que
		);

#ifdef __cplusplus
}
#endif

#endif	/* end of header */
