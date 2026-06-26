
#include "codexion.h"

void *create_tread_1(void *arg)
{
	while(1)
		printf("first\n");
	return NULL;
}

void *create_tread_2(void *arg)
{
	while(1)
		printf("second\n");
	return NULL;
}

int main()
{
	pthread_t t1, t2;

	int a = 1;

	pthread_create(&t1, NULL, &create_tread_1, NULL);
	pthread_create(&t2, NULL, &create_tread_2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

}