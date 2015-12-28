#include<stdio.h>
#include"malloc.h"

#define malloc(x) mymalloc(x, __FILE__, __LINE__);
#define free(x) myfree(x, __FILE__, __LINE__);

int main(int argc, char** argv){
	
	char* string = (char *)malloc(12);
	string[0] = 'h';
	string[1] = 'e';
	string[2] = 'l';
	string[3] = 'l';
	string[4] = 'o';
	string[5] = ' ';
	string[6] = 'w';
	string[7] = 'o';
	string[8] = 'r';
	string[9] = 'l';
	string[10] = 'd';
	string[11] = '\0';
	printf("%s\n",string);
	free(string);
	
	//Uncomment to view list:
	//printList();
	
	return 0;
}
