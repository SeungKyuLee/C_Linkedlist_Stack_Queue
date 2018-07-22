#include <stdio.h>
#include <string.h>
#include "linked_queue.h"
#include "linked_stack.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
	queue_t *que = queue_new();
	stack_t *stack = stack_new();

	int test_num[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int i = 0; i < 10; i++)
	{
		int* q_value = (int*)malloc(sizeof(int));
		*q_value = test_num[i];

		int* s_value = (int*)malloc(sizeof(int));
		*s_value = test_num[i];

		queue_push(que, (int*)q_value);
		stack_push(stack, (int*)s_value);
	}

	for (int i = 0; i < 10; i++)
		printf("%d ", *(int*)(queue_pop_node_data(que)));

	printf("\n");

	for (int i = 0; i < 10; i++)
		printf("%d ", *(int*)(stack_pop_node_data(stack)));

	printf("\n");


	return 0;
}