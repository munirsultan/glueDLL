#include "glthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/*
 * Node structure is the part of the Data, of DLL. 
 * this node structure act as glue 
 * hense the name glue link list 
 * */
typedef struct emp_ 
{
	char name[29];
	unsigned int salary;
	char designation[30];
	unsigned int emp_id;
	glthread_node_t glnode; //containing the Lptr and Rptr
} emp_t;


void print_emp_details(emp_t *emp)
{
    printf("Employee name = %s\n", emp->name);
    printf("salary = %u\n", emp->salary);
    printf("designation = %s\n", emp->designation);
    printf("emp_id = %u\n", emp->emp_id);
}

int main(int argc, char **argv)
{
	emp_t *emp1 = calloc(1, sizeof(emp_t));	//calloc dynamically set memory block to 0.
	strncpy(emp1->name, "Neha", strlen("Neha"));
	emp1->salary = 500000;
	strncpy(emp1->designation, "HR", strlen("HR"));
	emp1->emp_id = 21;

	glthread_node_init((&emp1->glnode));	//Lptr and Rptr set to NULL

	emp_t *emp2  = calloc(1, sizeof(emp_t));
	strncpy(emp2->name, "Abhishek", strlen("Abhishek"));
	emp2->salary = 150000;
	strncpy(emp2->designation, "SE3", strlen("SE3"));
	emp2->emp_id = 32;

	glthread_node_init((&emp2->glnode));

	emp_t *emp3  = calloc(1, sizeof(emp_t));
	strncpy(emp3->name, "Arun", strlen("Arun"));
	emp3->salary = 60000;
	strncpy(emp3->designation, "SE4", strlen("SE4"));
	emp3->emp_id = 27;

	glthread_node_init((&emp3->glnode));

	/* Now create a glthread */
	glthread_t *emp_list = calloc(1,sizeof(glthread_t));
#if 0
	printf("size of emp_t=%d \n\n",sizeof(emp_t));
	printf("size of emp_t->name=%d \n",sizeof(emp2->name));
	printf("size of emp_t->salary=%d \n",sizeof(emp2->salary));
	printf("size of emp_t->designation=%d \n",sizeof(emp2->designation));
	printf("size of emp_t->Emp_id=%d \n",sizeof(emp2->emp_id));
	printf("size of glthread_node_t=%d \n",sizeof(glthread_node_t));
#endif
	printf("offset = %d \n", offsetof(emp_t, glnode)); //30+1+4+30+1+4=
	init_glthread(emp_list, offsetof(emp_t, glnode));

	/* Now insert the records in glthread */
	glthread_add(emp_list, &emp1->glnode);
	glthread_add(emp_list, &emp2->glnode);
	glthread_add(emp_list, &emp3->glnode);

	/* walk over glthread */
	emp_t *ptr = NULL;
	ITERATE_GL_THREADS_BEGIN(emp_list, emp_t, ptr)
	{
		print_emp_details(ptr);
	} ITERATE_GL_THREADS_ENDS;

	/*Let us remove one record at random */
	glthread_remove(emp_list, &emp2->glnode);
	printf("\nprinting again . . .\n");

	ITERATE_GL_THREADS_BEGIN(emp_list, emp_t, ptr)
	{
		print_emp_details(ptr);
	} ITERATE_GL_THREADS_ENDS;


	/* Free all Dynamicall allocations */
	ITERATE_GL_THREADS_BEGIN(emp_list, emp_t, ptr)
	{
		glthread_remove(emp_list, &ptr->glnode);
		free(ptr);
	} ITERATE_GL_THREADS_ENDS;
	free(emp_list);

	return 0;
}
