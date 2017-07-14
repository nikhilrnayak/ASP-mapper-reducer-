#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "lib_map_reduce_types.h"		//lib defines the struct used in the prog

int store_buf[500];

void main(int argc, char **argv)
{
	int fd_p[2], i = 0, j = 0, k = 0, index = 0, len;
	fd_p[0] = atoi(argv[1]);					//Get file descriptors as arguments. If used with combiner the pipe fds are passed
	fd_p[1] = atoi(argv[2]);					//If used as a standalone file the usage should be ./reduce 0 1
	char buf[2], flag = 'T', flag2 = 'F';				//char buf for reading chars and flags for programming logic
	mapped_s *mapped_store = (mapped_s *)malloc(sizeof(*mapped_store)), *mapped_i, *mapped_j;	//structs for holding 
	printf("Entered Reduce.\n");
	*(buf + 1) = NULL;
	for(;;)
	{

		*buf = NULL;
		read(fd_p[0], buf, 1);
		if(strlen(buf) == 0)					//exit on eof
			break;
		else if(*buf == '\n')					//check for end of word
		{
			store_buf[i] = mapped_store;
			if(i > 0)
			{
				mapped_i = (mapped_s *)store_buf[i - 1];
				mapped_j = (mapped_s *)store_buf[i];
				if(strlen((char *)mapped_i->word) == strlen((char *)mapped_j->word))
				{
					len = strlen((char *)mapped_i->word);				//check whether the same as the previous word
					for(k = 0; k < len; k++)
					{
						if(((char)*((char *)mapped_i->word + k) == (char)*((char *)mapped_j->word + k)) && (k == len-1))
						{
							mapped_i->count = mapped_i->count + 1;
							free((void *)store_buf[i]);
							mapped_store = (mapped_s *)malloc(sizeof(*mapped_store));				
							store_buf[i] = NULL;
							flag2 = 'T';					//set flag to let know new word is same as the previous word
							j = 0;
						}
						else if((char)*((char *)mapped_i->word + k) != (char)*((char *)mapped_j->word + k))		//if different from previous word break loop
						{
							mapped_store = (mapped_s *)malloc(sizeof(*mapped_store));
							j = 0;
							flag2 = 'F';					//set flag to let know new word is different from the the previous word
							break;
						}
					}
				}

				else						//this else is needed doesn't perform the same operation as the else in for loop
				{
					mapped_store = (mapped_s *)malloc(sizeof(*mapped_store));		//if different from previous word break loop
					j = 0;
					flag2 = 'F';								//set flag to let know new word is different from the the previous word
				}						
	
				if(flag2 == 'F')				//if new word is different then print the previous word
				{
					mapped_i = (mapped_s *)store_buf[i - 1];
					write(1, "(", strlen("("));
					write(1, mapped_i->word, strlen(mapped_i->word));
					printf(", %d)\n", mapped_i->count);
					i++;
				}
			}
			else
			{
				mapped_store = (mapped_s *)malloc(sizeof(*mapped_store));		//this is to get the loop rolling once i is greater than 0 this is never entered
				i++;
				j = 0;
			}
		}
		else if(*buf == ')')							//the following else if's perform string manipulation based on the delims provided in the cond.
		{
			flag = 'T';
			mapped_store->count = 1;
		}
		else if(*buf == ',')
			flag = 'F';
		else if(*buf == '(' || *buf == ' ')
			continue;
		else									//store chars if part of the word string 
		{
			if(flag == 'T')
			{
				realloc(mapped_store, sizeof(*mapped_store) + (j + 1)*sizeof(char));
				*(mapped_store->word + j) = *buf;
				j++;
				*(mapped_store->word+j) = NULL;
			}
		}
	}
	close(fd_p[0]);
	if(i > 0)									//prints the tail element
	{
		mapped_i = (mapped_s *)store_buf[i - 1];
		write(1, "(", strlen("("));
		write(1, mapped_i->word, strlen(mapped_i->word));
		printf(", %d)\n", mapped_i->count);
	}

	printf("Exiting Reduce.\n");
}
