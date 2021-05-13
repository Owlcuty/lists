#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#define ERRPRINTF(format, ...)	fprintf(stderr, "%d::%s__::__ " format "\n", __LINE__, __PRETTY_FUNCTION__, ## __VA_ARGS__)

typedef int errno_t;
typedef char Elem_t;

typedef struct List_s
{
	Elem_t elem;
	struct List_s* next;
	struct List_s* prev;
} List_t;

List_t* list_Init(List_t* list, Elem_t val)
{
	list = (List_t*)calloc(1, sizeof(*list));
	if (!list)
	{
		return NULL;
	}
	list->elem = val;
	return list;
}

errno_t list_InsertAfter(List_t* list, size_t pos, Elem_t val)
{
	List_t* cur;
	size_t ind;
	for (cur = list, ind = 0; ind < pos && cur->next; ind ++, cur = cur->next);
	if (ind < pos)
	{
		return -1;
	}
	List_t* tmp = cur->next;
	List_t* new = (List_t*)malloc(sizeof(*new));
	cur->next = new;
	new->elem = val;
	new->next = tmp;

	return 0;
}

int list_Delete(List_t* list, size_t pos)
{
	List_t* cur;
	size_t ind;
	for (cur = list, ind = 0; ind < pos && cur; ind++, cur = cur->next);
	if (!cur)
	{
		return -1;
	}
	if (cur->next)
	{
		if (cur->prev)
		{
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
		}
		else
		{
			cur->next->prev = NULL;
		}
	}
	else if (cur->prev)
	{
		cur->prev->next = NULL;
	}
	else
		return -1;
	return 0;
}

errno_t list_Insert(List_t* list, size_t pos, Elem_t val)
{
	List_t* cur;
	size_t ind;
	for (cur = list, ind = 0; ind < pos && cur->next; ind ++, cur = cur->next);
	if (ind == pos - 1)
	{
		List_t* new = (List_t*)malloc(sizeof(*(cur->next)));
		if (!new)
		{
			return errno;
		}
		cur->next = new;
		new->prev = cur;
		new->elem = val;
	}
	else if (ind < pos)
	{
		return -1;
	}
	else
	{
		List_t* new = (List_t*)calloc(1, sizeof(*new));
		if (!new)
		{
			return errno;
		}
		if (cur->prev)
		{
			cur->prev->next = new;
			new->prev = cur->prev;
			cur->prev = new;
		}
		new->next = cur;
		new->elem = val;

	}

	return 0;
}

void list_Free(List_t* list)
{
	for (List_t* cur = list; cur;)
	{
		List_t* tmp = cur->next;
		free(tmp);
		cur = cur->next;
	}
}

void list_Print(List_t* list, size_t size_el)
{ 
	for (List_t* cur = list; cur; cur = cur->next)
	{
		ERRPRINTF("%c ", cur->elem);
	}
	fputc('\n', stdout);
}

errno_t change_list(List_t* list)
{
	size_t ind = 0;
	for (List_t* cur = list; cur; cur = cur->next, ind ++)
	{
		if (cur->elem == '*')
		{
			list_Insert(cur, 0, '1');
			list_Insert(cur, 0, '2');
			list_Print(list, sizeof(Elem_t));
			list_Insert(cur, 0, '3');
			list_Delete(cur, 0);
		}
	}
}

int main()
{
	List_t* list;
	if ((list = list_Init(list, 0)) == NULL)
	{
		perror("init fail:");
		return -1;
	}

	char* text = "j*skh*s**sj";

	{
		size_t ind = 1;
		for (char* cur = text; cur < text + strlen(text); cur++, ind++)
		{
			if (list_Insert(list, ind, *cur))
			{
				ERRPRINTF("InsertAfter fail. Position doesn't exist");
				return -1;
			}
		}
	}
	list_Print(list, sizeof(Elem_t));
	printf("\n");

	if (change_list(list))
	{
		perror("change fail:");
	}

	printf("%s\n", text);
	list_Print(list, sizeof(Elem_t));

	return 0;
}
