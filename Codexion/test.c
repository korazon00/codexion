
#include "codexion.h"


int main()
{
	int	heap[10];
	// heap[0] = 0;
	// heap[1] = 1;
	// heap[2] = 2;
	// heap[3] = 30;
	// heap[4] = 6;
	int size = 0;
	// printf("0 === %d\n", heap[0]);
	// printf("1 === %d\n", heap[1]);
	// printf("2 === %d\n", heap[2]);


	// printf("=====================\n\n");
	push(heap, &size, 10);
	push(heap, &size, 4);
	push(heap, &size, 15);
	push(heap, &size, 2);

	// printf("000000000000: %d, size: %d\n", heap[0], size);
	// printf("111111111111: %d, size: %d\n", heap[1], size);
	// printf("222222222222: %d, size: %d\n", heap[2], size);
	// printf("333333333333: %d, size: %d\n\n\n", heap[3], size);

	printf("%d\n", pop(heap, &size));
	printf("%d\n", pop(heap, &size));

	// printf("%d\n", heap[0]);
	// printf("%d\n", heap[1]);
	// printf("%d",size);
}