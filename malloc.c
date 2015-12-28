/*
 * Authored by Daniel Tsioni (Section 02, 150007096) and Gavriel Tsioni (Section 01, 150005575)
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"malloc.h"

//This is how much space we want to use for our dynamic memory
#define BLOCKSIZE 5000
//This is where we define the size of a large allocation
#define LARGEALLOC 300

static char myblock[BLOCKSIZE];

//This function controls the allocation of space through the use of helper functions
void* mymalloc(int size, char* filename, int linenum){
	
	//This static int will determine whether we've already initialized our 'dynamic memory' array
	static int init = 0;
	static head* root;
	
	
	if ( !init )
	{
		//Create the first node which will represent the entire dynamic memory as free space, and place it at the front
		root = (head *)myblock;
		root->size = BLOCKSIZE - sizeof(head);
		root->free = 1;
		root->ident = 0xAAAAAAAA;
		root->next = NULL;
		root->prev = NULL;
		init = 1;
	}
	
	if(size<=0){
		printf("Error [%s:%d]: ", filename, linenum);
		printf("Invalid allocation size.\n");
		return NULL;
	}
	
	//Get the pointer for the space that fits our size.
	void* space = allocSpace(size);
	
	//If there is none, alert the user.
	if(space == NULL){
		printf("Error [%s:%d]: ", filename, linenum);
		printf("No space found for requested allocation size.\n");
		return NULL;
	}
	
	//printList();	
	return space;
	
}

//This function handles the linked list entries of new allocations.
//Returns a pointer to the free space (not the node, but the space the user can use).
//Obeys the fragmentation rule that we set, which is to allocate from the back of a chunk if the request is small
void* allocSpace(int size){
	
	//Determine if a request is small
	int smallRequest = 0;
	if(size <= LARGEALLOC){
		smallRequest = 1;
	}
	
	//Get a node in the linked list which has the size we need. getSpace follows the fragmentation rules as well, so it will search from the back for small allocations.
	head* freeSpace = getSpace(size, smallRequest);
	
	if(freeSpace == NULL){
		return NULL;
	}
	
	//freeSpace should hold a pointer to a node which can hold our requested size.
	//This is where we update the linked list to represent the new allocated space being added.
	
	//If it is a small request, allocate space from the front of the chunk
	if(!smallRequest){
		freeSpace->free=0;
		
		//This checks if the freespace has enough space for another allocation in the left over space. If not, it just leaves on the remainder
		//In other words, if we were allocating 430 bytes out of 438, those 8 bytes would not have enough space for another node.
		//So in that case, the size is left as 438 and the user if given the pointer anyway, without adding a new entry to the linked list.
		if((freeSpace->size)-size < sizeof(head)+1){
			return freeSpace+sizeof(head);
		}
		
		//Creates a node which will represent the remaining memory
		void* remAlloc;
		remAlloc = freeSpace;	
		remAlloc += ((int)sizeof(head) + (int)size);
		head* newHead = (head *)remAlloc;
		newHead->size = (freeSpace->size) - size - sizeof(head);
		newHead->free=1;
		newHead->ident=0xAAAAAAAA;
		
		//Insert the node into the linked list.
		freeSpace->size=size;
		newHead->next = freeSpace->next;
		freeSpace->next = newHead;
		newHead->prev = freeSpace;
		
		//This ensures the pointer starts at the at free space, and not at the head node.
		void* iter = freeSpace;
		iter+=sizeof(head);	
		freeSpace = (head *)iter;		
		
		return freeSpace;	
	
	}
	
	//Else allocate space from the back of the chunk
	else{
		//Same 'rounding' rules as above.
		if((freeSpace->size)-size < sizeof(head)+1){
			return freeSpace+sizeof(head);
			
		}
		
		void* newAlloc;		
		//This will move the new space from the back of the free space chunk towards the front.
		newAlloc = (void*)freeSpace+sizeof(head)+(freeSpace->size);
		newAlloc -= size + sizeof(head);
		
		//Create the new node of the linked list to represent the new allocation.
		head* newHead = (head *)newAlloc;
		newHead->size = size;
		newHead->free=0;
		newHead->ident=0xAAAAAAAA;		
		freeSpace->size -= (size + sizeof(head));
		
		//Insert the new node into the linked list.
		newHead->next = freeSpace->next;
		freeSpace->next = newHead;
		newHead->prev = freeSpace;
		
		//This ensures the pointer starts at the free space, and not at the head note
		void* iter = newHead;
		iter+=sizeof(head);	
		newHead = (head *)iter;
		
		return newHead;
		
	}
		
}

//This function finds a node in the linked list that is free and could fit our new allocation
//Returns a pointer to the NODE.
//Obeys the fragmentation rule that we set, which is to start from the back if the request is small
head* getSpace(int size, int smallRequest){
	
	head* root;
	root = (head *)myblock;	
	head* current;
	current = root;
	
	int reverse = smallRequest;
	//If we are going backwards, traverse to the end
	if(reverse){
		head* ptr = root;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		current = ptr;
	}
	
	while(current!=NULL){
		if(current->size >= size && current->free){
			return current;
		}	
		//Obey fragmentation rules:
		if(!reverse){
			current = current->next;
		} else {
			current = current->prev;
		}
	}
	
	return NULL;
}

//This debug function prints the linked list.
void printList(){
	head* root = (head*)myblock;
	head* current = root;
	printf("\n----------\nList:\n");
	int sz = 0;
	for(; current!=NULL; current=current->next){
		printf("---node at %p / %p free: %d. size: %d. next: %p. prev: %p\n", current, ((void *)current)+sizeof(head), current->free, current->size, current->next, current->prev);
		sz+=current->size+sizeof(head);
	}
	printf("total size: %d\n", sz);
	printf("----------\n\n");
}

//This function will free memory to be reused.
//It will alert the user to common errors.
void myfree(void* space, char* filename, int linenum){
	
	//This checks if the given pointer falls outside of our dynamic memory.
	//Alert the user if this error is present.
	if(space < (void *)myblock || space > ((void *)myblock+BLOCKSIZE)){
		printf("Error [%s:%d]: ", filename, linenum);
		printf("Freeing of pointer that was never allocated.\n");
		return;		
	}
	
	//Otherwise, go backwards to find the node that represents that space
	space-=sizeof(head);
	head* node = (head *)space;
	
	//If the identification bits are not correct, then the pointer that was given to be freed is not immediately after a node
	//This means it falls in our dynamic memory, but was not returned by malloc.
	//Alert the user if this error is present.
	if(node->ident != 0xAAAAAAAA){
		printf("Error [%s:%d]: ", filename, linenum);
		printf("Freeing of pointer in dynamic memory that was not returned from malloc.\n");
		return;
	}
	
	//If the pointer is immediately after a node, but that node is already marked as free, then this is a redundant freeing.
	//Alert the user if this error is present.
	if(node->free){
		printf("Error [%s:%d]: ", filename, linenum);
		printf("Redundant freeing of pointer.\n");
		return;
	}
	
	//Since all other errors are not present, then the given node can be marked as free.
	node->free=1;
	//This helper function 'squashes' consecutive free nodes.
	squash();
	
}

//This function will combine consecutive free nodes to be one large free node that holds the sum of their designated memory.
void squash(){
	head* root = (head*)myblock;
	head* current = root;
	while(current!=NULL){
		if(current->next==NULL){
			return;
		}
		//Take into account the size of the node.
		if(current->free && current->next->free){
			current->size += current->next->size + sizeof(head);
			current->next = current->next->next;
		} else {			
			current=current->next;	
		}
	}
	
}
