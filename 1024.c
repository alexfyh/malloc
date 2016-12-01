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

static struct mab * head = NULL;
static struct mab * tail = NULL;
static struct mab * pointer =NULL;
static int indice =0;

struct mab * first_fit(size_t size)
{
	struct mab * current = head;
	while(current)
	{
		if(current->allocated==0 && current->size>=size)
			return current;
		current = current->next;
	}
	return NULL;
	

	
}

void * malloc ( size_t size)
{
	size_t total_size = mab_size + size;
	void * block;
	struct mab * header;
	if(size==0)
	{
		return NULL;
	}
	header = first_fit(size);
	if(header)
	{
		header->allocated = 1;
		/*if(header->size-size>mab_size)
		{
			header->size = size;
			int newSize = header->size-size-mab_size;
			int * init_position = (int *) header;
			int position_offset = mab_size/sizeof(int);
			struct mab * newHeader =(struct mab *) (init_position + position_offset + size);
			newHeader->size = newSize;
			newHeader->allocated = 0;
			newHeader->next = header->next;
			newHeader->prev = header;
			header->next = newHeader;

		}*/
		return (void *) (header+1);
	}
	block = sbrk(total_size);
	if (block == (void*) -1) 
	{
		printf("%s\n","Sbreak() function failed" );
		return NULL;
	}
	
	header = block;
	header->size = size;
	//printf("%s %d\n","Agregando" , size );
	header->allocated = 1;
	header->next = NULL;
	if (head==NULL)
	{
		head = header;
	}
	if (tail!=NULL)
	{
		struct mab * temp;

		//printf("%d\n",header->size );
		tail->next = header;
		header->prev = tail;
		header->next = NULL;
		

	}
	
	
	tail = header;
		
	
	//tail->next =  NULL;

	return (void*)(header + 1);



}

void free (void * block)
{
	struct mab * current_mab, * previous_mab, * next_mab;

	if (!block)
		return;
	current_mab = (struct mab *)block - 1;
	previous_mab = current_mab->prev;
	next_mab = current_mab->next;
	//printf("%d\n",current_mab->size );
	current_mab->allocated = 0;
	if(!(next_mab==NULL) && (next_mab->allocated==0))
	{
		//printf("%d\n",next_mab->size );
		current_mab->size = current_mab->size + mab_size + next_mab->size; 
		current_mab->next = next_mab->next;
		next_mab->next->prev = current_mab;
		//printf("%s\n", "NEXT");
	}
	if(!(previous_mab==NULL) && (previous_mab->allocated==0))
	{
		//printf("%d\n",previous_mab->size );
		previous_mab->size = previous_mab->size + mab_size + current_mab->size;
		previous_mab->next = current_mab->next;
		next_mab->prev = previous_mab;
		//printf("%s\n", "PREV" );
	}
	return;
}

void show()
{
	
	//tail->prev->next = NULL;
	if(head==NULL)
	{
		return;
	}
	pointer = head;
	printf("%s\n","================================================");
	indice = 1;
	while(pointer!=NULL)
	{
		printf("%s%d%s%d%s%d\n","MEM_BLOCK  ",indice, "    Tamano  ",pointer->size,"  Estado:  ",pointer->allocated );
		pointer = pointer->next;
		indice++;
	}
	printf("%s\n","================================================" );
	
	return;


}
int main(int argc, char const *argv[])
{
	void * ptr1 = malloc(32);
	void * ptr2 = malloc(64);
	void * ptr3 = malloc(128);
	void * ptr4 = malloc(256);
	void * ptr5 = malloc(16);
	void * ptr6 = malloc(512);
	show();
	free(ptr1);
	show();
	free(ptr2);
	show();
	free(ptr3);
	free(ptr4);
	free(ptr5);
	show();
	//free(ptr3);
	malloc(100);
	malloc(100);

	show();
	/*ptr5 = (int *)malloc(56);
	int * ptr6;
	ptr6 = (int *)malloc(16);
	int * ptr7;
	ptr7 = (int *)malloc(256);

	int * ptr8;
	ptr8 = (int *)malloc(169);
	malloc(200);
	malloc(360);
	
	show();
	//malloc(100);
	//show();

*/
	
	
}






