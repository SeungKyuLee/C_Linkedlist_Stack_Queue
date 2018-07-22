#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include "linked_stack.h"


struct stack_t {
	list_t *list;
	ssize_t size;
};

typedef struct data_t
{
	void *data;
	int32_t len;
} data_t;

/*
* New Stack Structure Memory Allocation
*/
stack_t *
stack_new(
)
{
	stack_t *stack;

	stack = (stack_t *)calloc(1, sizeof(stack_t));
	if (NULL == stack)
		return NULL;
	stack->list = list_new();
	if (NULL == stack->list)
	{
		FREE(stack);
		return NULL;
	}

	return stack;
}

/*
* All Item and Stack Memeory Free
*/
void
stack_delete(
stack_t *stack
)
{
	list_node_t *node, *next_node;

	assert(stack);
	assert(stack->list);

	for (node = list_first(stack->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(stack->list);

	FREE(stack);
}

/*
* Return Stack is Empty
*/
int
stack_is_empty(
stack_t *stack
)
{
	assert(stack);
	assert(stack->list);

	return list_is_empty(stack->list);
}

/*
* Return Stack Size
*/
ssize_t
stack_get_size(
stack_t *stack
)
{
	assert(stack);

	return stack->size;
}

/*
* Return first Item from Stack
* (An Item is pushing First of List)
*/
stack_node_t *
stack_first(
stack_t *stack
)
{
	assert(stack);
	assert(stack->list);

	return list_first(stack->list);
}

/*
* Return last Item from Stack
* (An Item is pushing is First of List)
*/
stack_node_t *
stack_last(
stack_t *stack
)
{
	assert(stack);
	assert(stack->list);

	return list_last(stack->list);
}

/*
* Return target after Target Item Node
*/
stack_node_t *
stack_next(
stack_t *stack,
stack_node_t *node
)
{
	assert(stack);
	assert(stack->list);
	assert(node);

	return list_next(node);
}

/*
* Return target previous Target Item Node
*/
stack_node_t *
stack_prev(
stack_t *stack,
stack_node_t *node
)
{
	assert(stack);
	assert(stack->list);
	assert(node);

	return list_prev(node);
}

/*
* Return Data from target Item Node
*/
void *
stack_node_get_data(
stack_node_t *node
)
{
	assert(node);

	return list_node_get_data(node);
}


/*
* An Item is pushing in Target Stack
*/
int
stack_push(
stack_t *stack,
void *data
)
{
	list_node_t *node;

	assert(stack);
	assert(stack->list);

	node = list_node_new();
	if (NULL == node)
		return -1;
	list_node_set_data(node, data);
	list_insert_head(stack->list, node);
	++stack->size;

	return 0;
}

/*
* An Item's Data is poping in Target Stack
*/
void *
stack_pop_node_data(
stack_t *stack
)
{
	list_node_t *node;
	void *data;

	assert(stack);
	assert(stack->list);

	node = list_first(stack->list);
	if (NULL == node)
		return NULL;
	data = list_node_get_data(node);
	list_remove(stack->list, node);
	list_node_delete(node);
	--stack->size;

	return data;
}

/*
* An Item is poping in Target Stack
*/
list_node_t *
stack_pop_node(
stack_t *stack
)
{
	list_node_t *node;

	assert(stack);
	assert(stack->list);

	node = list_first(stack->list);
	if (NULL == node)
		return NULL;
	list_remove(stack->list, node);
	--stack->size;

	return node;
}

/*
* An Item is Deleting from Stack
*/
void
stack_remove_node(
stack_t *stack,
stack_node_t *node
)
{
	assert(stack);
	assert(stack->list);
	assert(node);

	list_remove(stack->list, node);
	list_node_delete(node);
	--stack->size;
}

/*
* Free All nodes in Stack
*/
void
stack_clear(
stack_t *stack
)
{
	list_node_t *node, *next_node;
	assert(stack);
	assert(stack);
	for (node = list_first(stack->list); NULL != node;
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
struct stack_mt_t {
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
struct stack_mt_t {
	list_t *list;
	ssize_t size;
	CRITICAL_SECTION mutex;//pthread_mutex_t -> HANDLE type
	CONDITION_VARIABLE cond;//pthread_cond_wait -> Condition_Variable
	ssize_t bytes;
};
#endif

/*
* Sync Stack Memory Allocation
*/
stack_mt_t *
stack_mt_new(
)
{
	stack_mt_t *stack;

	stack = (stack_mt_t *)calloc(1, sizeof(stack_mt_t));
	if (NULL == stack)
		return NULL;
	stack->list = list_new();
	if (NULL == stack->list)
	{
		FREE(stack);
		return NULL;
	}

#ifndef _WIN32

	if (pthread_mutex_init(&stack->mutex, NULL) != 0)
	{
		list_delete(stack->list);
		FREE(stack);
		return NULL;
	}
	if (pthread_cond_init(&stack->cond, NULL) != 0)
	{
		list_delete(stack->list);
		FREE(stack);
		return NULL;
	}

#else

	if (!(InitializeCriticalSectionAndSpinCount(&(stack->mutex), 4000)))
	{
		list_delete(stack->list);
		FREE(stack);
		return NULL;
	}
	InitializeConditionVariable(&(stack->cond));

#endif

	return stack;
}

/*
* All Item and Stack Memeory Free (Sync)
*/
void
stack_mt_delete(
stack_mt_t *stack
)
{
	list_node_t *node, *next_node;
	
#ifndef _WIN32

	assert(stack);
	assert(stack->list);

	pthread_mutex_lock(&stack->mutex);
	for (node = list_first(stack->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(stack->list);
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(stack->list);

	EnterCriticalSection(&(stack->mutex));
	for (node = list_first(stack->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	list_delete(stack->list);
	LeaveCriticalSection(&(stack->mutex));

#endif
	FREE(stack);
}

/*
* Return Sync Stack is Empty
*/
int
stack_mt_is_empty(
stack_mt_t *stack
)
{
	assert(stack);
	assert(stack->list);

	return list_is_empty(stack->list);
}

/*
* Return Sync Stack Size
*/
ssize_t
stack_mt_get_size(
stack_mt_t *stack
)
{
	assert(stack);

	return stack->size;
}

/*
* Return first Item from Sync Stack
*/
stack_mt_node_t *
stack_mt_first(
stack_mt_t *stack
)
{
	stack_mt_node_t *node;

#ifndef _WIN32

	assert(stack);
	assert(stack->list);

	pthread_mutex_lock(&stack->mutex);
	while (NULL == list_first(stack->list))
		pthread_cond_wait(&stack->cond, &stack->mutex);
	node = list_first(stack->list);
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(stack->list);

	EnterCriticalSection(&(stack->mutex));
	while (NULL == list_first(stack->list))
		SleepConditionVariableCS(&(stack->cond), &(stack->mutex), INFINITE);
	node = list_first(stack->list);
	LeaveCriticalSection(&(stack->mutex));

#endif

	return node;
}

/*
* Return next Item from Target Node
*/
stack_mt_node_t *
stack_mt_next(
stack_mt_t *stack,
stack_mt_node_t *node
)
{
	stack_mt_node_t *prev;

#ifndef _WIN32

	assert(stack);
	assert(stack->list);
	assert(node);

	pthread_mutex_lock(&stack->mutex);
	prev = list_prev(node);
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(stack->list);
	assert(node);

	EnterCriticalSection(&(stack->mutex));
	prev = list_prev(node);
	LeaveCriticalSection(&(stack->mutex));

#endif

	return prev;
}

/*
* Return Data from target Item Node
*/
void *
stack_mt_node_get_data(
stack_mt_t *stack,
stack_mt_node_t *node
)
{
	void *data;

#ifndef _WIN32

	assert(stack);
	assert(node);

	pthread_mutex_lock(&stack->mutex);
	data = list_node_get_data(node);
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(node);

	EnterCriticalSection(&(stack->mutex));
	data = list_node_get_data(node);
	LeaveCriticalSection(&(stack->mutex));

#endif

	return data;
}

/*
* An Item is pushing in Target Sync Queue
*/
int
stack_mt_push_back(
stack_mt_t *stack,
void *data
)
{
	list_node_t *node;

#ifndef _WIN32

	assert(stack);
	assert(stack->list);

	node = list_node_new();
	if (NULL == node)
		return -1;
	list_node_set_data(node, data);

	pthread_mutex_lock(&stack->mutex);
	list_insert_head(stack->list, node);
	++stack->size;
	pthread_mutex_unlock(&stack->mutex);
	pthread_cond_signal(&stack->cond);

#else

	assert(stack);
	assert(stack->list);

	node = list_node_new();
	if (NULL == node)
		return -1;
	list_node_set_data(node, data);

	EnterCriticalSection(&(stack->mutex));
	list_insert_head(stack->list, node);
	++stack->size;
	LeaveCriticalSection(&(stack->mutex));
	WakeConditionVariable(&(stack->cond));

#endif

	return 0;
}

/*
* An Item is Poping in Target Sync Stack
*/
void *
stack_mt_pop_back(
stack_mt_t *stack
)
{
	list_node_t *node;
	void *data;

#ifndef _WIN32

	assert(stack);
	assert(stack->list);

	pthread_mutex_lock(&stack->mutex);
	while (NULL == list_last(stack->list))
		pthread_cond_wait(&stack->cond, &stack->mutex);
	node = list_last(stack->list);
	data = list_node_get_data(node);
	list_remove(stack->list, node);
	list_node_delete(node);
	--stack->size;
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(stack->list);

	EnterCriticalSection(&(stack->mutex));
	while (NULL == list_last(stack->list))
		SleepConditionVariableCS(&(stack->cond), &(stack->mutex), INFINITE);
	node = list_last(stack->list);
	data = list_node_get_data(node);
	list_remove(stack->list, node);
	list_node_delete(node);
	--stack->size;
	LeaveCriticalSection(&(stack->mutex));

#endif

	return data;
}

/*
* Free All nodes in Sync Stack
*/
void
stack_mt_clear(
stack_mt_t *stack
)
{
	list_node_t *node, *next_node;

#ifndef _WIN32

	assert(stack);
	assert(stack->list);
	pthread_mutex_lock(&stack->mutex);
	for (node = list_first(stack->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	pthread_mutex_unlock(&stack->mutex);

#else

	assert(stack);
	assert(stack->list);

	EnterCriticalSection(&(stack->mutex));
	for (node = list_first(stack->list); NULL != node;
		node = next_node)
	{
		next_node = list_next(node);
		list_node_delete(node);
	}
	LeaveCriticalSection(&(stack->mutex));

#endif
}
