#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int f;

int cmp(const void * a, const void * b);
int frobcmp (const char *a, const char *b);


int main(int argc, const char* argv[])
{
  int check;
  char sp = ' ';
  
  char * filein;
  int size;
  int readsize;
  int nsize;
  int fd = STDIN_FILENO;
  struct stat buf;

  int diff;
  int cw = 0;
  int sw = 100;
  int i = 0;
  int j = 0;
  

  //checking for correct arguments
  if (argc == 1)
    {
      f = 0;
    }
  else if (argc == 2 && strcmp(argv[1], "-f") == 0 )
    {
      f = 1;
    }
  else
    {
      fprintf(stderr, "Invalid option\n");
      exit(1);
    }
  
  check = fstat(fd, &buf);
  if (check != 0)
    {
      fprintf(stderr, "Error getting information on a file\n");
      exit(1);
    }
  //reading from stdin depending on whether it is a regular file or not
  if (S_ISREG(buf.st_mode))
    {
      size = buf.st_size;
      filein = malloc(sizeof(char) * size+1);
      if (filein == NULL)
	{
	  fprintf(stderr, "Error allocating memory\n");
	  free(filein);
	  exit(1);
	}
      readsize = read(fd,filein,size);

      if (readsize < 0)
	{
	  fprintf(stderr, "Could not read file\n");
	  free(filein);
	  exit(1);
	}

      //to read from a growing file
      
      check = fstat(fd, &buf);
      if (check != 0)
	{
	  fprintf(stderr, "Error getting information on a file\n");
	  free(filein);
	  exit(1);
	}
      size = buf.st_size;
      diff = size - readsize;

      while(diff > 0)
	{
	  filein = realloc(filein, sizeof(char) * size+1);
	  if (filein == NULL)
	    {
	      fprintf(stderr, "Error allocating memory\n");
	      free(filein);
	      exit(1);
	    }
	  check = lseek (fd, readsize, 0);
	  if (check < 0)
	    {
	      fprintf(stderr, "Error while reading file\n");
	      free(filein);
	      exit(1);
	    }
	  nsize = read(fd, filein+readsize, diff);
	  if (nsize < 0)
	    {
	      fprintf(stderr, "Error while reading file\n");
	      free (filein);
	      exit(1);
	    }
	  readsize += nsize;
	  check = fstat(fd, &buf);
	  if (check != 0)
	    {
	      fprintf(stderr, "Error getting information on a file\n");
	      free(filein);
	      exit(1);
	    }
	  size = buf.st_size;
	  diff = size - readsize;
	}
      filein[readsize]=' ';
    }
  else
    {

      size = 0;
      filein = malloc(sizeof(char) * size+1);
      if (filein == NULL)
	{
	  fprintf(stderr, "Error allocating memory\n");
	  free(filein);
	  exit(1);
	}
      check = read (fd, &filein[0], 1);

      if (check < 0)
	{
	  fprintf(stderr, "Error reading from standard input.\n");
	  free(filein);
	  exit(1); 
	}
      while (check != 0)
	{
	  size++;
	  filein = realloc(filein, sizeof(char) * size+1);
	  if (filein == NULL)
	    {
	      fprintf(stderr, "Error allocating memory\n");
	      free(filein);
	      exit(1);
	    }	  
	  check = read (fd, &filein[size], 1);
	  if (check < 0)
	    {
	      fprintf(stderr, "Error reading from standard input.\n");
	      free(filein);
	      exit(1); 
	    }

	}
      filein[size] = ' ';
      readsize = size;
    }
  

  //allocate space for 100 word pointers so as to not reallocate every time
  char** wordlist = malloc(sizeof(char*) * sw);
  if (wordlist == NULL)
    {
      fprintf(stderr, "Error allocating memory\n");
      free(filein);
      free(wordlist);
      exit(1);
    }
  
  while (i < readsize)
    {
      if (cw > sw-2)
	{
	  sw += 100;
	  wordlist = realloc(wordlist, sizeof(char*) * sw);
	  if (wordlist == NULL)
	    {
	      fprintf(stderr, "Error allocating memory\n");
	      free(filein);
	      free(wordlist);
	      exit(1);
	    }
	}
      wordlist[cw] = &filein[i];
      cw++;
      while (i < readsize && filein[i] != ' ')
	{
	  i++;
	}
      if (i < readsize)
	{
	  i++;
	}
    }
  
  //sort the words by unfrobnicating the letters and comparing
  qsort(wordlist,cw,sizeof(char*),cmp);

  //print all the elements in the sorted word list
  i = 0;
  while(i < cw)
  {
    j = 0;
    while (wordlist[i][j] != ' ')
      {
	check = write(1,&wordlist[i][j],1);
	if (check < 0)
	  {
	    fprintf(stderr, "Error writing to stdout\n");
	    free(filein);
	    free(wordlist);
	    exit(1);
	  }
	j++;
      }
    check = write(1,&sp,1);
    if (check < 0)
      {
	fprintf(stderr, "Error writing to stdout\n");
	free(filein);
	free(wordlist);
	exit(1);
      }
    i++;
  }
  
  free(filein);
  free(wordlist);
}



int cmp(const void * a, const void * b)
{
  return frobcmp(*(char**) a, *(char**) b);
}


int frobcmp ( const char *a, const char *b)
{
 
    int i = 0;
    int j = 0;
    int a1, b1;
    while (1)
    {
      //space is checked as it is used to indicate end of word
        if (*(a+i) == ' ' && *(b+j) == ' ')
            return 0;
        else if (*(a+i) == ' ')
            return -1;
        else if (*(b+j) == ' ')
            return 1;

        a1 = (unsigned char) (*(a+i) ^ 0x2A);
        b1 = (unsigned char) (*(b+j) ^ 0x2A);

	if (f == 1)
	  {
	    a1 = toupper (a1);
	    b1 = toupper (b1);
	  }
	
        if (a1 > b1)
            return 1;
        if (a1 < b1)
            return -1;
        i++;
        j++;
    }
    return 0;
}

