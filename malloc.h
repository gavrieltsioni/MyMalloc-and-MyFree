/*
 * Authored by Daniel Tsioni (Section 02, 150007096) and Gavriel Tsioni (Section 01, 150005575)
 * 
 */

struct head{

	int ident;
	int free;
	int size;
	struct head* next;
	struct head* prev;

} typedef head;

void* mymalloc(int size, char* filename, int linenum);
void myfree(void* space, char* filename, int linenum);
head* getSpace(int size, int smallRequest);
void* allocSpace(int size);
void printList();
void squash();
