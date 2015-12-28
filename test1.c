/*
 * Authored by Daniel Tsioni (Section 02, 150007096) and Gavriel Tsioni (Section 01, 150005575)
 * 
 */
 
#include<stdio.h>
#include"malloc.h"

#define malloc(x) mymalloc(x, __FILE__, __LINE__);
#define free(x) myfree(x, __FILE__, __LINE__);

int main(int argc, char** argv){
	malloc(0);
	
	int x;
	free(&x);
	
	char* p = (char *)malloc(200);
	free(p+10);
	
	free(p);
	free(p);
	
	malloc(10000);
	
	//Uncomment to view list:
	//printList();
	
	return 0;
}
