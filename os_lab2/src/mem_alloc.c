#include "mem_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* memory */
#define MEMORY_SIZE 512			//size of initial free memory
char memory[MEMORY_SIZE]; 

/* Structure declaration for a free block */
typedef struct free_block{
  int size; 
  struct free_block *next; 
} free_block_s, *free_block_t; 

/* Structure declaration for an occupied block */
typedef struct{
  int size; 
} busy_block_s, *busy_block_t; 


/* Pointer to the first free block in the memory */
free_block_t first_free; 


#define ULONG(x)((long unsigned int)(x))
#define max(x,y) (x>y?x:y)


void memory_init(void){

	//placing the descriptor at the beginning of memory address
	free_block_t free_block = (free_block_t) heap_base();

	//descriptor of the first free block
	free_block->size = MEMORY_SIZE;
	free_block->next = NULL;

	//pointer to the first free block
	first_free = free_block;
}

char *memory_alloc(int size){

	//searching the first fit free block
	free_block_t current_search_block = first_free->next;
	int free_block_size = 0;
	int busy_block_discriptor_size = sizeof(busy_block_s);
	busy_block_t new_busy_block = NULL;
	int found_free_block = 0;
	while(current_search_block != NULL){
		if((current_search_block->size - busy_block_discriptor_size) >= size){
			found_free_block = 1;
			free_block_size = current_search_block->size - busy_block_discriptor_size;

			//Check if this free block can be sub divided?
			if((free_block_size - size) > (busy_block_discriptor_size + 1)){

				//yes it is, now save the free block descriptor in the second block and allocate size and next element to this free block
				free_block_t new_free_block = (free_block_t) (((char *)current_search_block) + size + busy_block_discriptor_size + 1);	//here 1 is added to reach to the start of next block
				new_free_block->size = current_search_block->size - (size + busy_block_discriptor_size);

				//adding new free block to Free Block List
				new_free_block->next = current_search_block->next;
				free_block_t search_next_block = first_free;
				while(search_next_block->next != NULL){
					if(search_next_block->next == current_search_block)
						break;
					else
						search_next_block = search_next_block->next;
				}
				search_next_block->next = new_free_block;

				//give current_search_block a new size
				current_search_block->size = current_search_block->size - new_free_block->size;
			}else{

				//remove the current_search_block from the free block list
				free_block_t search_next_block = first_free;
				while(search_next_block->next != NULL){
					if(search_next_block->next == current_search_block)
						break;
					else
						search_next_block = search_next_block->next;
				}
				search_next_block->next = current_search_block->next;
			}

			//add busy block descriptor to the current_search_block
			new_busy_block = (busy_block_t) current_search_block;
			new_busy_block->size = current_search_block->size;

			//move the pointer to the next byte of the end of busy_block_descriptor
			new_busy_block++;
		}else
			current_search_block = current_search_block + 1;
	}
	if(found_free_block == 0)
		return NULL;

	//print_alloc_info(new_busy_block, new_busy_block->size);

	//return the new_busy_block pointer
	return (char *)new_busy_block;
}

void memory_free(char *p){
  print_free_info(p);

  //type cast p to busy_block_t
  busy_block_t memory_to_free = (busy_block_t) p;

  //move to the head of busy block by neglecting 1 from the typed cast p
  memory_to_free--;

  //check if previous block is free
  int previous_block_free = 0;
  free_block_t previous_free_block = first_free;
  while(previous_free_block->next != NULL){
	  if((char *)memory_to_free - (((char *)previous_free_block->next) + previous_free_block->size + 1) == 0){	//here 1 is added to reach to the start of next block
		  //if yes, find the size of this free block and update the previous free block descriptor size
		  previous_free_block->size = previous_free_block->size + memory_to_free->size;
		  previous_block_free = 1;
		  break;
	  }else
		  previous_free_block = previous_free_block->next;
  }

  //check if next block is free
  free_block_t start_of_next_block = (free_block_t)(((char *) memory_to_free) + memory_to_free->size + 1);
  free_block_t next_free_block = first_free;
  while(next_free_block->next != NULL){
	  //check if the previous and the next blocks are free
	  if(next_free_block->next == start_of_next_block && previous_block_free == 1){
		  //if yes, merge all three blocks
		  previous_free_block->size = previous_free_block->size + next_free_block->size;
		  //adjust this new free block in the list of free blocks
		  previous_free_block->next = next_free_block->next;
		  break;
	  }else if(next_free_block->next == start_of_next_block && previous_block_free == 0){	//check if only the next block is free and not the previous one
		  /****** if yes, merge last two blocks and add to the free blocks list *****/

		  //add a free block descriptor in the recently freed block
		  free_block_t released_memory_FBD = (free_block_t) memory_to_free;
		  //merge next free block with this recently freed block
		  released_memory_FBD->size = memory_to_free->size + next_free_block->next->size;
		  //add this free block in to the list of free blocks
		  released_memory_FBD->next = next_free_block->next->next;
		  next_free_block->next = released_memory_FBD;
		  break;
	  }
  }
  /* ... */
}


void print_info(void) {
  //fprintf(stderr, "Memory : [%lu %lu] (%lu bytes)\n", (long unsigned int) 0, (long unsigned int) (memory+MEMORY_SIZE), (long unsigned int) (MEMORY_SIZE));
  fprintf(stderr, "Free block : %lu bytes; busy block : %lu bytes.\n", ULONG(sizeof(free_block_s)), ULONG(sizeof(busy_block_s))); 
}

void print_free_info(char *addr){
  if(addr)
    fprintf(stderr, "FREE  at : %lu \n", ULONG(addr - memory)); 
  else
    fprintf(stderr, "FREE  at : %lu \n", ULONG(0)); 
}

void print_alloc_info(char *addr, int size){
  if(addr){
    fprintf(stderr, "ALLOC at : %lu (%d byte(s))\n", 
	    ULONG(addr - memory), size);
  }
  else{
    fprintf(stderr, "Warning, system is out of memory\n"); 
  }
}

void print_free_blocks(void) {
  free_block_t current; 
  fprintf(stderr, "Begin of free block list :\n"); 
  for(current = first_free; current != NULL; current = current->next)
    fprintf(stderr, "Free block at address %lu, size %u\n", ULONG((char*)current - memory), current->size);
}

char *heap_base(void) {
  return memory;
}


void *malloc(size_t size){
  static int init_flag = 0; 
  if(!init_flag){
    init_flag = 1; 
    memory_init(); 
    //print_info(); 
  }      
  return (void*)memory_alloc((size_t)size); 
}

void free(void *p){
  if (p == NULL) return;
  memory_free((char*)p); 
  print_free_blocks();
}

void *realloc(void *ptr, size_t size){
  if(ptr == NULL)
    return memory_alloc(size); 
  busy_block_t bb = ((busy_block_t)ptr) - 1; 
  printf("Reallocating %d bytes to %d\n", bb->size - (int)sizeof(busy_block_s), (int)size); 
  if(size <= bb->size - sizeof(busy_block_s))
    return ptr; 

  char *new = memory_alloc(size); 
  memcpy(new, (void*)(bb+1), bb->size - sizeof(busy_block_s) ); 
  memory_free((char*)(bb+1)); 
  return (void*)(new); 
}


#ifdef MAIN
int main(int argc, char **argv){

  /* The main can be changed, it is *not* involved in tests */
  memory_init();
  print_info(); 
  print_free_blocks();
  int i ; 
  for( i = 0; i < 10; i++){
    char *b = memory_alloc(rand()%8);
    memory_free(b); 
    print_free_blocks();
  }




  char * a = memory_alloc(15);
  a=realloc(a, 20); 
  memory_free(a);


  a = memory_alloc(10);
  memory_free(a);

  printf("%lu\n",(long unsigned int) (memory_alloc(9)));
  return EXIT_SUCCESS;
}
#endif 
