#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
  int check;
  const char* from = argv[1];
  const char* to = argv[2];
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
  char c[1];
  int t = read(STDIN_FILENO, c, 1);

  while (t == 1)
    {
      i=0;
      j=0;
      for (;j==0 && i<len; i++)
	{
	  if (from[i] == c[0])
	    {
	      check = write(STDOUT_FILENO, &to[i], 1);
	      if (check < 0)
		{
		  fprintf(stderr, "Error writing to output");
		  exit(1);
		}
	      
	      j=1;
	    }
	}
      if (j == 0)
	{
	  check = write(STDOUT_FILENO, c, 1);
	  if (check < 0)
	    {
	      fprintf(stderr, "Error writing to output");
	      exit(1);
	    }
	}
      t = read(STDIN_FILENO, c, 1);
    }
  if (t == -1)
    {
      fprintf(stderr, "Error reading from file");
      exit(1);
    }
  return 0;
}
