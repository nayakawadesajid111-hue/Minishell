#include "main.h"
/*Insert at first*/
int insert_at_first(list **head, data_t pid, char *cmds, char *status,data_t count)
{
	/* Creating the new node */
	list *new = malloc(sizeof(list));

	/* Check whether new node created or not */
	if (new == NULL)
	{
		return FAILURE;
	}

	/* Fill the parts of the node */
	new->pid = pid;
	new->link = NULL;
	new->count = count;

	strcpy(new->cmd, cmds);
	strcpy(new->state, status);

	/* If list is empty */
	if (*head == NULL)
	{
		/*If *head is empty then create the first node */

		*head = new;
	
		return SUCCESS;
	}
	else
	{
		/* *head is not empty then store value i.e link in the temp variable */
		list *temp = *head;

		/* Stroring that new link in the head inserting the element at first */
		*head = new;

		/* Creating the link to next data */
		new->link = temp;
	}
    
	
	return SUCCESS;
}

/*Print list*/
void print_list(list *head)
{
	if (head == NULL)
	{
		printf(" no such jobs\n");
	}
	else
	{
		reverse(&head);
		list*temp = head ;
		while (temp)
		{
			printf("[%d]    %s                 %s\n",temp->count, temp->state, temp->cmd);
			temp = temp->link;
		}
		reverse(&head);
	}
}

/*Delete first node*/
int delete_first(list **head)
{
	if (*head == NULL)
	{
		return FAILURE;
	}

	list *temp = *head;

	*head = temp->link;

	free(temp);

	return SUCCESS;
}

/*Reverse the list*/
void reverse(list **head)
{
    list *prev = NULL;
    list *curr = *head;
    list *next = NULL;

    while (curr != NULL)
    {
        next        = curr->link;  // save next
        curr->link  = prev;        // reverse link
        prev        = curr;        // move prev forward
        curr        = next;        // move curr forward
    }

    *head = prev;  // prev is new head
}
