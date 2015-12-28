#include<stdio.h>
#include"malloc.h"

#define malloc(x) mymalloc(x, __FILE__, __LINE__);
#define free(x) myfree(x, __FILE__, __LINE__);

int main(int argc, char** argv){
	
	void* test1;
	test1 = malloc(100);
	void* test2;
	test2 = malloc(200);
	void* test3;
	test3 = malloc(300);
	void* test4;
	test4 = malloc(400);
	void* test5;
	test5 = malloc(500);
	void* test6;
	test6 = malloc(600);
	
	//Uncomment to view list:
	//printList();
	
	return 0;
}
