#include <stdio.h>
#include <unistd.h>
#define mab_size sizeof(struct mab)
//#include <sys/resource.h>


struct mab {
	int offset;
	int size;
	int allocated;
	struct mab * next;
	struct mab * prev;
};
typedef struct mab Mab;
typedef Mab * MabPtr;

struct mab * head = NULL;
struct mab * tail = NULL;

void * first_fit(size_t size)
{
	struct mab * current = head;
	while(current)
	{
		if(current->allocated==0 && current->size>size)
			return current;
		current = current->next;
	}
	return NULL;
	// Si sobra espacio
	//
	//
	//

	
}

void * malloc ( size_t size)
{
	size_t total_size = mab_size + size;
	void * block;
	struct mab * header;
	if(!size)
	{
		return NULL;
	}
	header = first_fit(size);
	if(header)
	{
		header->allocated = 1;
		return (void *) (header+1);
	}
	block = sbrk(total_size);
	if (block == (void*) -1) 
	{
		printf("%s\n","Sbreak() function failed" );
		return NULL;
	}
	else
	{

	}
	header = block;
	header->size = size;
	header->allocated = 1;
	header->next = NULL;
	if (!head)
		head = header;
	if (tail)
	{
		tail->next = header;
		header->prev = tail;
	}
		
	tail = header;

	return (void*)(header + 1);



}

void free (void * block)
{
	printf("%s\n", "Entro al free");
	struct mab * current_mab, * previous_mab, * next_mab;

	if (!block)
		return;
	current_mab = (struct mab *)block - 1;
	previous_mab = current_mab->prev;
	next_mab = current_mab->next;
	printf("%d\n",current_mab->size );
	current_mab->allocated = 0;
	if(!(next_mab==NULL) && (next_mab->allocated==0))
	{
		printf("%d\n",next_mab->size );
		current_mab->size = current_mab->size + mab_size + next_mab->size; 
		current_mab->next = next_mab->next;
		next_mab->next->prev = current_mab;
		printf("%s\n", "NEXT");
	}
	if(!(previous_mab==NULL) && (previous_mab->allocated==0))
	{
		printf("%d\n",previous_mab->size );
		previous_mab->size = previous_mab->size + mab_size + current_mab->size;
		previous_mab->next = current_mab->next;
		next_mab->prev = previous_mab;
		printf("%s\n", "PREV" );
	}
	return;
}


int main(int argc, char const *argv[])
{
	int * ptr1;
	ptr1 = (int *)malloc(5);
	*ptr1 = 32 ;
	int * ptr2;
	ptr2 = (int *)malloc(10);
	*ptr2 = 1024;
	int * ptr3;
	ptr3 = (int *) malloc(8);
	*ptr3 = 256;
	int * ptr4;
	ptr4 = (int *)malloc(2);
	*ptr1 = 4 ;
	printf("%s\n","Funcion Free" );

	free((void *)ptr3);
	free((void *)ptr2);
	free((void *)ptr4);
	printf("%d\n", ((struct mab *)ptr2 - 1)->size);
	 //printf("%d\n", sizeof(head));
	 //printf("%p\n", a);
	 //first_fit(head,20);
}






