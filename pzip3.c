//THIS IS YOUR SAFE COPY



#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#define MAX_RLEN 50
#define BUFSIZE 1000


//thread execution
void* compressFile(void *arg)
{
  printf("It works! %s\n", (char *) arg);
  return NULL;
}


//encoding
char* encode(char* src)
{
  int rLen;
  char count[MAX_RLEN];
  int len = strlen(src);

  char* dest = (char*)malloc(sizeof(char) * (len * 2 + 1));

  int i, j = 0, k;

  for (i = 0; i < len; i++)
  {
	dest[j++] = src[i];

	rLen = 1;
	while (i + 1 < len && src[i] == src[i + 1])
	{
		rLen++;
		i++;
	}

	sprintf(count, "%d", rLen);

	for (k = 0; *(count + k); k++, j++)
	{
		dest[j] = count[k];
	}
  }

  dest[j] = '\0';
  return dest;
}



int main(int argc, char const *argv[])
{
/*
this portion of the code is to use MMAP to put a file into an array. It works by itself, but
not with this block of code.


  	//MMAP section to load file into array
  	unsigned char *f;
  	int size;
  	struct stat s;
  	const char * file_name = argv[2];
 	int fd = open (argv[2], O_RDONLY);

  	int status = fstat (fd, & s);
  	size = s.st_size;

  	f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
*/



//spawning the threads from argv[1]
  int j;
  int num_threads = atoi(argv[1]);
  pthread_t * thread = malloc(sizeof(pthread_t)*num_threads);

  for (j = 0; j < num_threads; j++)
  {
	pthread_create(&thread[j], NULL, compressFile, NULL);

  }



//opening the files from argv[i] > 2

  int i;
  for( i = 2; i < argc; i++)
  {
  	FILE *fp = fopen(argv[i], "r");
  	if (fp == NULL) {
		printf("Cannot open file.\n");
		return 1;
  	}

  	char buff[BUFSIZE];
  	while(fgets(buff, BUFSIZE - 1, fp) != NULL)
	{
 	 	char* res = encode(buff);
  		printf("%s", res);
  		getchar();
  	}

  	fclose(fp);
  }

}
