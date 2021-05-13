
void list_delete_after(s_t* node)
{
	if (!node->next)
		return;
	s_t* cur = node->next;
	node->next = cur->next;
	free(cur->fio);
	free(cur->group);
	free(cur);
}

s_t* list_delete_head(s_t* head)
{
	s_t* tmp = head->next;
	free(head->fio);
	free(head->group);
	free(head);
	return tmp;
}


