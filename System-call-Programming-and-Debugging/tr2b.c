#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
  const char* from = argv[1];
  const char* to = argv[2];

  int check;
  int len = strlen(from);
  
  if (argc != 3)
    {
      fprintf(stderr, "Incorrect number of arguments\n");
      exit(1);
    }
  if (len != strlen(to))
    {
      fprintf(stderr, "Incorrect size of arguments\n");
      exit(1);
    }
  if (len == 0)
    {
      fprintf(stderr, "Argument cannot be empty\n");
      exit(1);
    }

  int i = 0;
  int j = 0;
  char a;
  for (;i<len-1;i++)
    {
      a = from[i];
      for (j=i+1;j<len;j++)
	{
	  if (a == from[j])
	    {
	      fprintf(stderr, "2 elements in 'from' are same\n");
	      exit(1);
	    }
	}
    }

  int c = getchar();

  while (c != EOF)
    {
      i=0;
      j=0;

      for (;j==0 && i<len; i++)
	{
	  if (from[i] == c)
	    {
	      check = putchar(to[i]);
	      if (check == EOF)
		{
		  fprintf(stderr, "Error writing output.");
		  exit(1);
		}
	      j=1;
	    }
	}
      if (j == 0)
	{
	  check = putchar(c);
	  if (check == EOF)
	    {
	      fprintf(stderr, "Error writing output.");
	      exit(1);
	    }
	}
      c = getchar();
    }
  
  if (ferror(stdin))
    {
      fprintf(stderr, "Error reading from file");
      exit(1);
    }
  return 0;
}
