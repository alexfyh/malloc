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


void split_block(struct mab * header, size_t size)
{
			int newSize = (header->size-size)-mab_size;
			char * init_position = (char *) header;
			int position_offset = mab_size/sizeof(char);
			struct mab * newHeader = (struct mab * ) (init_position + position_offset + size);
			//int * final_position = init_position + position_offset + size;
			//struct mab * newHeader =(struct mab *) (init_position + position_offset + size);
			newHeader->size = newSize;
			newHeader->allocated = 0;
			newHeader->next = header->next;
			newHeader->prev = header;
			//header = 
			header->next = newHeader;
			header->size = size;
			return;

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
		if(header->size-size>mab_size+4)
		{
			split_block(header,size);
		
		}
		return (void *) (header+1);
	}
	block = sbrk(total_size);
	if (block == (void*) -1) 
	{
		printf("%s\n","Sbreak() function failed" );
		return NULL;
	}
	
	header = (struct mab *)block;
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
	//printf("%p\n", tail->next);
	
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
	void * ptr7 = malloc(2048);
	
	show();

	free(ptr1);
	free(ptr2);
	
	show();
	
	void * ptr8 = malloc(50);
	show();
	free(ptr4);
	free(ptr6);
	show();

	void * ptr9 = malloc(400);
	show();
	
	//free(ptr3);
	/*
	free(ptr7);
	show();

	void * ptr9 = malloc(2000);
	show();

	void * ptr10 = malloc(3000);
	show();
	*/
	
	
}






