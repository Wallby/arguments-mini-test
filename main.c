#include <arguments_mini.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct info_about_memory_allocation_t
{
	void* a;
	int numBytesAllocated;
};
struct info_about_memory_allocation_t* infoPerMemoryAllocation = NULL;
int numMemoryAllocations = 0; //< really # tracked memory allocations (excluding memory allocations for infoPerMemoryAllocation)
size_t numBytesAllocated;

// NOTE: thanks to https://stackoverflow.com/questions/1208644/how-can-i-get-the-size-of-a-memory-block-allocated-using-malloc
void* __real_malloc(size_t a);
void __real_free(void* a);

void* __wrap_malloc(size_t a)
{
	void* b = __real_malloc(a);

	struct info_about_memory_allocation_t* c = infoPerMemoryAllocation;
	//infoPerMemoryAllocation = new info_about_memory_allocation_t[numMemoryAllocations + 1];
	infoPerMemoryAllocation = (struct info_about_memory_allocation_t*)__real_malloc(sizeof(struct info_about_memory_allocation_t) * (numMemoryAllocations + 1));

	if(c != NULL)
	{
		memcpy(infoPerMemoryAllocation, c, sizeof(struct info_about_memory_allocation_t) * numMemoryAllocations);
		__real_free(c);
	}

	if((b == NULL) | (infoPerMemoryAllocation == NULL))
	{
		fputs("error: out of memory (test will be inaccurate)\n", stdout); //< not sure if that is the only case __real_malloc would return NULL..?
		if(b == NULL)
		{
			__real_free(b);
		}
		if(infoPerMemoryAllocation == NULL)
		{
			__real_free(infoPerMemoryAllocation);
		}
		exit(1); //< i.e. continuing "like this" would not make a whole lot of sense..?
	}

	++numMemoryAllocations;
	infoPerMemoryAllocation[numMemoryAllocations - 1].a = b;
	infoPerMemoryAllocation[numMemoryAllocations - 1].numBytesAllocated = a;
	numBytesAllocated += a;

	return b;
}

void __wrap_free(void* a)
{
	int i;
	for(i = 0; i < numMemoryAllocations; ++i)
	{
		if(infoPerMemoryAllocation[i].a == a)
		{
			break;
		}
	}
	if(i == numMemoryAllocations)
	{
		fputs("warning: attempted to free memory that was never allocated\n", stdout);
		exit(1); //< i.e. continuing "like this" would not make a whole lot of sense..?
	}

	numBytesAllocated -= infoPerMemoryAllocation[i].numBytesAllocated;
	if((numMemoryAllocations - 1) > 0)
	{
		struct info_about_memory_allocation_t* b = infoPerMemoryAllocation;
		//infoPerMemoryAllocation = new info_about_memory_allocation_t[numMemoryAllocations - 1];
		infoPerMemoryAllocation = (struct info_about_memory_allocation_t*)__real_malloc(sizeof(struct info_about_memory_allocation_t) * (numMemoryAllocations - 1));

		int numElementsBefore = i;
		int numElementsAfter = numMemoryAllocations - numElementsBefore - 1;

		if(numElementsBefore > 0)
		{
			memcpy(infoPerMemoryAllocation, b, sizeof(struct info_about_memory_allocation_t) * numElementsBefore);
		}
		if(numElementsAfter > 0)
		{
			memcpy(infoPerMemoryAllocation + numElementsBefore, b + numElementsBefore + 1, sizeof(struct info_about_memory_allocation_t) * numElementsAfter);
		}

		__real_free(b);
	}
	else
	{
		__real_free(infoPerMemoryAllocation);
		infoPerMemoryAllocation = NULL;
	}
	--numMemoryAllocations;

	__real_free(a);
}

/*
void my_on_print(char* a)
{
	fputs(a, stdout);
}
*/

//int main(int, char**)
int main(int unusedArgc, char** unusedArgv)
{
	size_t numBytesAllocatedBefore = numBytesAllocated;

	//char validArgv[][] =
	char* validArgv[] =
		{
			"", //< i.e. 1st parameter will be executable name..?
			"apple",
			"pear",
			"-color=red",
			"-tint=gray",
			"--recycle"
		};
	//int validArgc = length(validArgv);
	int validArgc = 1+4;

	//char invalidArgv[][] =
	char* invalidArgv[] =
		{
			"", //< i.e. 1st parameter will be executable name..?
			"pear=" //< '=' in argument
			//OR
			//"-color" //< missing '=' character
			//OR
			//"-color=red",
			//"-color=yellow" //< twice the same parameter (not allowed)
			//OR
			//"-color=red",
			//"--color" //< twice the same parameter (not allowed)
			//OR
			//"---" //< invalid parameter (starts with --- <- not allowed)
			//OR
			//"-" //< invalid parameter (missing parametername)
			//OR
			//"--" //< invalid parameter (missing parametername)
		};
	//int invalidArgc = length(invalidArgv);
	int invalidArgc = 1+2;

	// NOTE: don't set on_print callback (not testing error logging here)
	// TODO: test callbacks too
	if(am_parse(validArgc, validArgv) != 1)
	{
		fputs("error: am_parse failed with valid argv (false negative)\n", stdout);
		return 1;
	}

	//am_set_on_print(&my_on_print);

	if(am_parse(invalidArgc, invalidArgv) != 0)
	{
		fputs("error: am_parse succeeded with invalid argv (false positive)\n", stdout);
		return 1;
	}

	size_t numBytesAllocatedAfter = numBytesAllocated;
	size_t numBytesLeaked = numBytesAllocatedAfter - numBytesAllocatedBefore;
	if(numBytesAllocatedAfter != numBytesAllocatedBefore)
	{
		printf("error: %zi bytes of memory leaked\n", numBytesLeaked);
	}

	fputs("all tests succeeded\n", stdout);

	return 0;
}
