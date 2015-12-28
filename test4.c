#include<stdio.h>
#include"malloc.h"

#define malloc(x) mymalloc(x, __FILE__, __LINE__);
#define free(x) myfree(x, __FILE__, __LINE__);

int main(int argc, char** argv){
	
	void* test1 = malloc(10);
	void* test2 = malloc(13);
	void* test3;
	test3 = malloc(15);
	int* test4;
	test4 = (int *)malloc(16);
	void* test5;
	test5 = malloc(25);
	double* test6 = (double *)malloc(78);
	void* test7 = malloc(25);
	int* test8 = (int *)malloc(98);
	void* test9 = malloc(2);
	void* test10;
	test10 = malloc(3);	
	free(test9);
	free(test8);
	free(test7);	
	void* test11;
	test11 = malloc(3000);	
	free(test1);
	free(test2);
	free(test6);
	
	//Uncomment to view list:
	//printList();
	
	return 0;
}
