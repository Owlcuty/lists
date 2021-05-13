#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_s
{
	char* fio;
	char* group;
	struct subjs_s
	{
		int matan;
		int lab;
		int term;
		int info;
	} subjs;
	struct s_s* next;
} s_t;

enum Element
{
	FIO,
	GROUP,
};

enum SUBJECTS
{
	MATAN,
	INFO,
	LAB,
	TERM
};

const char* Subj[4] = {"Матан", "Инфор", "Лабор", "Терма"};

int my_atoi(char* s, size_t size)
{
	char* s_ = (char*)calloc(size, 1);
	strncpy(s_, s, size);
	int ret = atoi(s_);
	free(s_);
	return ret;
}

s_t* parse_csv(char* buff)
{
	int size = 0;
	char* next = strchr(buff, ';');
	size_t sizelen = next - buff;
	size = my_atoi(buff, sizelen);
	char* bcur = next + 1;

	s_t* parsed = (s_t*)calloc(1, sizeof(*parsed));

	size_t ind = 0;
	for (s_t* scur = parsed; ind < size; ind++)
	{
		enum Element el = FIO;
		int end = 0;
		while (!end)
		{
			char* tmp = strchr(bcur, ';');
			next = strchr(bcur, ',');
			if (next == NULL)
			{
				end = 1;
				next = tmp;
			}
			else if (next > tmp)
			{
				end = 1;
				next = tmp;
			}
			size_t tmpsize = next - bcur;
			char* obj = (char*)calloc(tmpsize + 1, 1);
			strncpy(obj, bcur, tmpsize);
			if (el == FIO)
				scur->fio = obj;
			else if (el == GROUP)
				scur->group = obj;
			else
			{
					int score = 0;
					char* sc_start = strchr(bcur, ':');
					score = my_atoi(sc_start + 1, next - sc_start - 1);
					for (int i = 0; i < 4; i ++)
					{
						if (!memcmp(bcur, Subj[i], sc_start - bcur))
						{
							switch(i)
							{
								case MATAN:
									scur->subjs.matan = score;
									break;
								case LAB:
									scur->subjs.lab = score;
									break;
								case TERM:
									scur->subjs.term = score;
									break;
								case INFO:
									scur->subjs.info = score;
									break;
							}	
						}
					}
			}
			el ++;
			bcur = next + 1;
		}
		if (ind != end)
		{
			scur->next = (s_t*)calloc(1, sizeof(s_t));
			scur = scur->next;
		}
	}

	
	return parsed;
}

s_t* read_csv(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	int t = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buff = (char*)calloc(t + 1, 1);
	fread(buff, t, 1, file);
	buff[t] = 0;

	s_t* ret = parse_csv(buff);

	return ret;
}

void list_insert_after(s_t* node, const s_t src)
{
	s_t* tmp = NULL;
	s_t* new = (s_t*)calloc(1, sizeof(s_t));
	if (node->next)
	{
		new->next = node->next;
	}
	node->next = new;
	new->fio = src.fio;
	new->group = src.group;
	new->subjs.matan = src.subjs.matan;
	new->subjs.lab = src.subjs.lab;
	new->subjs.term = src.subjs.term;
	new->subjs.info = src.subjs.info;
}

void list_append(s_t* node, const s_t src)
{
	s_t* cur;
	for (cur = node; cur->next; cur = cur->next);
	list_insert_after(cur, src);
}

void list_delete_after(s_t* node)
{
	if (!node->next)
		return;
	s_t* cur = node->next;
	node->next = cur->next;
	free(cur);
}

s_t* list_delete_head(s_t* head)
{
	s_t* tmp = head->next;
	free(head);
	return tmp;
}

void list_node_print(s_t* node)
{
	if (!node)
	{
		return;
	}
	s_t* scur = node;
		printf("%d:: fio{%s}, group{%s}, matan{%d}, l{%d}, t{%d}, i{%d}\n\n", __LINE__, scur->fio, scur->group, scur->subjs.matan, scur->subjs.lab, scur->subjs.term, scur->subjs.info);

}

void list_print(s_t* list)
{
	for (s_t* scur = list; scur; scur = scur->next)
	{
		list_node_print(scur);
	}
}

void list_free(s_t* list)
{
	for (s_t* cur = list; cur;)
	{
		s_t* tmp = cur->next;
		free(tmp);
		cur = cur->next;
	}
}

s_t* list_findByFIO(s_t* list, const char* fio)
{
	printf("%d:: HERE\n", __LINE__);
	s_t* cur;
	for (cur = list; cur && strcmp(fio, cur->fio); cur = cur->next);
	if (!cur)
	{
		return NULL;
	}
	return cur;
}

s_t* list_findByFIO_prev(s_t* list, const char* fio)
{
	s_t* cur, *tmp = list;
	for (cur = list; cur && strcmp(fio, cur->fio); cur = cur->next)
	{
		tmp = cur;
	}
	if (!cur)
	{
		return NULL;
	}
	return tmp;
}

s_t* list_printByGroup(s_t* list, const char* group)
{
	for (s_t* cur = list; cur; cur = cur->next)
	{
		if (!strcmp(group, cur->group))
		{
			list_node_print(cur);
		}
	}
}

s_t* list_write(s_t* list, const char* filename)
{
	FILE* file = fopen(filename, "w");

	for (s_t* cur = list; cur; cur = cur->next)
	{
		fwrite(cur->fio, 1, strlen(cur->fio), file);
		fputc(',', file);
		fwrite(cur->group, 1, strlen(cur->group), file);
		fputc(',', file);
		fprintf(file, "Матан:%d,", cur->subjs.matan);
		fprintf(file, "Инфор:%d,", cur->subjs.info);
		fprintf(file, "Лабор:%d,", cur->subjs.lab);
		fprintf(file, "Терма:%d;", cur->subjs.term);
	}

	fclose(file);
}

enum Action
{
  ReadCSV,
  WriteCSV,
  FreeBD,
  DumpBD,
  DumpBDByGroup,
  AddData,
  DeleteStudent,
  FindStudent,
  ExitAct
};

int main()
{
	s_t* students;
	list_print(students);

	s_t student = {"N W D", "B08", {4, 3, 2, 1}};

	int menu_mod = 1;
	int flag = -1;
	s_t tmp_student = {};
	char* buff = (char*)calloc(1024, 1);
	while(1)
	{
		if (menu_mod)
		{
			printf("%i:: 1>Read csv" "\n", __LINE__);
      		printf("%i:: 2>Write csv" "\n", __LINE__);
      		printf("%i:: 3>Free BD" "\n", __LINE__);
      		printf("%i:: 4>Dump BD" "\n", __LINE__);
      		printf("%i:: 5>Dump BD by group" "\n", __LINE__);
	      	printf("%i:: 6>Add data" "\n", __LINE__);
   	   		printf("%i:: 7>Delete student" "\n", __LINE__);
      		printf("%i:: 8>Find student" "\n", __LINE__);
      		printf("%i:: 9>Exit" "\n", __LINE__);
			menu_mod = 0;
		}

		printf("%i:: Action: ", __LINE__);
    	scanf("%i", &flag);
    	printf("\n");
    switch (flag - 1)
    {
      case ReadCSV:
        printf("%i:: Filename: ", __LINE__);
        scanf("%s", buff);
        printf("\n");
		students = read_csv(buff);
        menu_mod = 1;
        break;
      case WriteCSV:
        printf("%i:: Filename: ", __LINE__);
        scanf("%s", buff);
        printf("\n");
		list_write(students, buff);
        menu_mod = 1;
        break;
      case FreeBD:
		list_free(students);
        menu_mod = 1;
        break;
	  case DumpBD:
		list_print(students);
		menu_mod = 1;
		break;
	  case DumpBDByGroup:
		printf("%i:: Group: ", __LINE__);
		scanf("%s", buff);
		list_printByGroup(students, buff);
		menu_mod = 1;
		break;
      case AddData:	
		printf("%d:: FIO: ", __LINE__);
		scanf("%s", buff);
		strcpy(tmp_student.fio, buff);
		printf("%d:: Group: ", __LINE__);
		scanf("%s", buff);
		strcpy(tmp_student.group, buff);
		printf("%d:: Matan: ", __LINE__);
		scanf("%d", flag);
		tmp_student.subjs.matan = flag;
		printf("%d:: Terma: ", __LINE__);
		scanf("%d", flag);
		tmp_student.subjs.term = flag;
		printf("%d:: Lab: ", __LINE__);
		scanf("%d", flag);
		tmp_student.subjs.lab = flag;
		printf("%d:: Inform: ", __LINE__);
		scanf("%d", flag);
		tmp_student.subjs.info = flag;
        menu_mod = 1;
        break;
      case DeleteStudent:
		printf("%d: FIO: ", __LINE__);
		scanf("%s", buff);
		list_delete_after(list_findByFIO_prev(students, buff));
        menu_mod = 1;
        break;
      case FindStudent:
        printf("%i:: FIO: ", __LINE__);
        scanf("%s", buff);
		list_node_print(list_findByFIO(students, buff));
        menu_mod = 1;
        break;
      case ExitAct:
        printf("%i:: Thanks for using us!" "\n", __LINE__);
        return 0;
      default:
        printf("%i:: Smth wrong" "\n", __LINE__);
        return -1;
    }
    printf("\n");
  }	

	list_append(students, student);
	list_delete_after(students);

	list_print(students);
	
	list_node_print(list_findByFIO(students, "N W D"));

	list_write(students, "res.out");
	list_free(students);
	free(buff);
	return 0;
}
