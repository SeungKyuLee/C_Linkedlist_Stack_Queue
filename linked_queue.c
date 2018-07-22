#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#include "linked_queue.h"
#include "linked_config.h"

struct queue_t {
	list_t *list;
	ssize_t size;
};

typedef struct data_t
{
	void *data;
	int32_t len;
} data_t;

/*
 * New Queue Structure Memory Allocation
 */
queue_t *
queue_new(
)
{
	queue_t *que;

	que = (queue_t *)calloc(1, sizeof(queue_t));
	if (NULL == que)
		return NULL;
	que->list = list_new();
	if (NULL == que->list)
	{
		FREE(que);
		return NULL;
	}

	return que;
}

/*
* All Item and Queue Memeory Free
*/
void
queue_delete(
queue_t *que
)
{
	list_node_t *node, *next_node;

	assert(que);
	assert(que->list);

	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(que->list);

	FREE(que);
}

/*
* Return Queue is Empty
*/
int
queue_is_empty(
queue_t *que
)
{
	assert(que);
	assert(que->list);

	return list_is_empty(que->list);
}

/*
* Return Queue Size
*/
ssize_t
queue_get_size(
queue_t *que
)
{
	assert(que);

	return que->size;
}

/*
* Return first Item from Queue
* (An Item is Enqueuing is First of List)
*/
queue_node_t *
queue_first(
queue_t *que
)
{
	assert(que);
	assert(que->list);

	return list_last(que->list);
}

/*
* Return last Item from Queue
* (An Item is Enqueuing is First of List)
*/
queue_node_t *
queue_last(
queue_t *que
)
{
	assert(que);
	assert(que->list);

	return list_first(que->list);
}

/*
* Return target after Target Item Node
*/
queue_node_t *
queue_next(
queue_t *que,
queue_node_t *node
)
{
	assert(que);
	assert(que->list);
	assert(node);

	return list_prev(node);
}

/*
* Return target previous Target Item Node
*/
queue_node_t *
queue_prev(
queue_t *que,
queue_node_t *node
)
{
	assert(que);
	assert(que->list);
	assert(node);

	return list_next(node);
}

/*
* Return Data from target Item Node
*/
void *
queue_node_get_data(
queue_node_t *node
)
{
	assert(node);

	return list_node_get_data(node);
}

/*
* An Item is Enqueuing in Target Queue
*/
int
queue_push(
queue_t *que,
void *data
)
{
	list_node_t *node;

	assert(que);
	assert(que->list);

	node = list_node_new();
	if (NULL == node)
		return FAIL;
	list_node_set_data(node, data);
	list_insert_head(que->list, node);
	++que->size;

	return SUCCESS;
}

/*
* An Item's Data is Poping in Target Queue
*/
void *
queue_pop_node_data(
queue_t *que
)
{
	list_node_t *node;
	void *data;

	assert(que);
	assert(que->list);

	node = list_last(que->list);
	if (NULL == node)
		return NULL;
	data = list_node_get_data(node);
	list_remove(que->list, node);
	list_node_delete(node);
	--que->size;

	return data;
}

/*
* An Item is Poping in Target Queue
*/
list_node_t *
queue_pop_node(
queue_t *que
)
{
	list_node_t *node;

	assert(que);
	assert(que->list);

	node = list_last(que->list);
	if (NULL == node)
		return NULL;
	list_remove(que->list, node);
	--que->size;

	return node;
}

/*
* An Item is Deleting from Queue
*/
void
queue_remove_node(
queue_t *que,
queue_node_t *node
)
{
	assert(que);
	assert(que->list);
	assert(node);

	list_remove(que->list, node);
	list_node_delete(node);
	--que->size;
}

/*
* Free All nodes in Queue
*/
void
queue_clear(
queue_t *que
)
{
	list_node_t *node, *next_node;
	assert(que);
	assert(que->list);
	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
}

/***************************************************
* *************  Multi Threading ***************** *
***************************************************/
#ifndef _WIN32

/* POSIX Domain Synchronization */
#include <pthread.h>
struct queue_mt_t {
list_t *list;
ssize_t size;
pthread_mutex_t mutex;
pthread_cond_t cond;
ssize_t bytes;
};

#else

/* Windows NT Synchronization */
#include <process.h>
#include <Windows.h>

struct queue_mt_t {
	list_t *list;
	ssize_t size;
	CRITICAL_SECTION   mutex; //pthread_mutex_t -> HANDLE type
	CONDITION_VARIABLE cond;  //pthread_cond_wait -> Condition_Variable
	ssize_t bytes;
};
#endif


/*
* Sync Queue Memory Allocation
*/
queue_mt_t *
queue_mt_new()
{
	queue_mt_t *que;
	que = (queue_mt_t *)calloc(1, sizeof(queue_mt_t));
	if (NULL == que)
		return NULL;
	que->list = list_new();
	if (NULL == que->list)
	{
		FREE(que);
		return NULL;
	}

#ifndef _WIN32

	if (!(que->mutex=CreateMutex(NULL, FALSE, NULL)))
	{
		list_delete(que->list);
		FREE(que);
		return NULL;
	}
	if (pthread_cond_init(&que->cond, NULL) != 0)
	{
		list_delete(que->list);
		FREE(que);
		return NULL;
	}

#else

	//Before Vista
	/*InitializeCriticalSection(&que->mutex);
	InitializeConditionVariable(&que->cond);*/

	if (!(InitializeCriticalSectionAndSpinCount(&(que->mutex), 4000))){
		list_delete(que->list);
		FREE(que);
		return NULL;
	}
	InitializeConditionVariable(&(que->cond));
	
#endif

	return que;
}

/*
* All Item and Queue Memeory Free (Sync)
*/
void
queue_mt_delete(
queue_mt_t *que
)
{
	list_node_t *node, *next_node;

	assert(que);
	assert(que->list);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(que->list);
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(que->list);
	LeaveCriticalSection(&(que->mutex));

#endif

	FREE(que);
}

/*
* Return Sync Queue is Empty
*/
int
queue_mt_is_empty(
queue_mt_t *que
)
{
	assert(que);
	assert(que->list);

	return list_is_empty(que->list);
}

/*
* Return Sync Queue Size
*/
ssize_t
queue_mt_get_size(
queue_mt_t *que
)
{
	ssize_t que_size;

	assert(que);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	que_size = que->size;
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	que_size = que->size;
	LeaveCriticalSection(&(que->mutex));

#endif
	return que_size;
}

/*
* Return first Item from Sync Queue
*/
queue_mt_node_t *
queue_mt_first(
queue_mt_t *que
)
{
	queue_mt_node_t *node;

	assert(que);
	assert(que->list);

#ifndef _WIN32
	pthread_mutex_lock(&que->mutex);
	while (NULL == list_last(que->list))
		pthread_cond_wait(&que->cond, &que->mutex);
	node = list_last(que->list);
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	while (NULL == list_last(que->list))
		SleepConditionVariableCS(&(que->cond), &(que->mutex), INFINITE);
	node = list_last(que->list);
	LeaveCriticalSection(&(que->mutex));

#endif
	return node;
}

/*
* Return next Item from Target Node
*/
queue_mt_node_t *
queue_mt_next(
queue_mt_t *que,
queue_mt_node_t *node
)
{
	queue_mt_node_t *prev;

	assert(que);
	assert(que->list);
	assert(node);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	prev = list_prev(node);
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	prev = list_prev(node);
	LeaveCriticalSection(&(que->mutex));

#endif
	return prev;
}

/*
* Return Data from target Item Node
*/
void *
queue_mt_node_get_data(
queue_mt_t *que,
queue_mt_node_t *node
)
{
	void *data;

	assert(que);
	assert(node);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	data = list_node_get_data(node);
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	data = list_node_get_data(node);
	LeaveCriticalSection(&(que->mutex));

#endif

	return data;
}

/*
* An Item is Enqueuing in Target Sync Queue
*/
int
queue_mt_push_back(
queue_mt_t *que,
void *data
)
{
	list_node_t *node;

	assert(que);
	assert(que->list);

	node = list_node_new();
	if (NULL == node)
		return -1;
	list_node_set_data(node, data);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	list_insert_head(que->list, node);
	++que->size;
	pthread_mutex_unlock(&que->mutex);
	pthread_cond_signal(&que->cond);

#else

	EnterCriticalSection(&(que->mutex));
	list_insert_head(que->list, node);
	++que->size;
	LeaveCriticalSection(&(que->mutex));
	WakeConditionVariable(&(que->cond));

#endif
	return 0;
}

/*
* An Item is Poping in Target Sync Queue
*/
void *
queue_mt_pop_back(
queue_mt_t *que
)
{
	list_node_t *node;
	void *data;

	assert(que);
	assert(que->list);

#ifndef _WIN32

	pthread_mutex_lock(&que->mutex);
	while (NULL == list_last(que->list))
		pthread_cond_wait(&que->cond, &que->mutex);
	node = list_last(que->list);
	data = list_node_get_data(node);
	list_remove(que->list, node);
	list_node_delete(node);
	--que->size;
	pthread_mutex_unlock(&que->mutex);

#else

	EnterCriticalSection(&(que->mutex));
	while (NULL == list_last(que->list))
		SleepConditionVariableCS(&(que->cond), &(que->mutex), INFINITE);
	node = list_last(que->list);
	data = list_node_get_data(node);
	list_remove(que->list, node);
	list_node_delete(node);
	--que->size;
	LeaveCriticalSection(&(que->mutex));

#endif
	return data;
}

/*
* Free All nodes in Sync Queue
*/
void
queue_mt_clear(
queue_mt_t *que
)
{
	list_node_t *node, *next_node;

#ifndef _WIN32

	assert(que);
	assert(que->list);
	pthread_mutex_lock(&que->mutex);
	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	pthread_mutex_unlock(&que->mutex);

#else

	assert(que);
	assert(que->list);
	EnterCriticalSection(&(que->mutex));
	for (node = list_first(que->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	LeaveCriticalSection(&(que->mutex));

#endif
}

