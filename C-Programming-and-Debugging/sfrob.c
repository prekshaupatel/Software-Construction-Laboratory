#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int cmp(const void * a, const void * b);
int frobcmp (const char *a, const char *b);

int main()
{
    
    size_t contentSize;
    int i;
    int j;
    contentSize = 1;
    int numw = 1;
    int numl = 1;
    
    char** wordlist = malloc(sizeof(char*) * numw);
    if(wordlist == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        free(wordlist);
        exit(1);
    }
    
    int c= getchar();
    if(ferror(stdin))
    {
        free(wordlist);
        fprintf(stderr, "Error reading from stdin.");
        exit(1);
    }

    //reading all the letters from the input
    while (c != EOF)
    {
        char* cword = malloc(sizeof(char) * numl);
	//allocate a new word dynamically 
	if(cword == NULL)
        {
            fprintf(stderr, "Failed to allocate memory\n");
            for (i=0; i<numw-1;i++)
             {
                 free(wordlist[i]);
             }
             exit(1);
         }
        numl=1;
	//add letters to the word till space character or end of line is \
	//encountered
	while (c != EOF && c != ' ')
        {
            numl++;
            cword = realloc(cword, sizeof(char) * numl);
            if(cword == NULL)
             {
                fprintf(stderr, "Failed to reallocate memory\n");
                for (i=0; i<numw-1;i++)
                  {
                    free(wordlist[i]);
                  }
                free(wordlist);
                exit(1);
             }
            cword[numl-2]=c;
            c=getchar();

        }
	
	//terminate the word with a space
        cword[numl-1]=' ';
        numw++;
	//add the word to the list of words
        wordlist=realloc(wordlist, sizeof(char*) * numw);
        if(wordlist == NULL)
         {
           fprintf(stderr, "Failed to reallocate memory\n");
           for (i=0; i<numw-1;i++)
                {
                  free(wordlist[i]);
                }
           free(wordlist);
           exit(1);
         }
        wordlist[numw-2]=cword;
	if (c != EOF )
	  {
	    c=getchar();
	  }
    }
    //sort the words by unfrobnicating the letters and comparing
    qsort(wordlist,numw-1,sizeof(char*),cmp);

    //print all the elements in the sorted word list
    i = 0;
    while(i<numw-1)
    {
      j = 0;
      while (wordlist[i][j] != ' ')
	{
	  printf("%c", wordlist[i][j]);
	  j++;
	}
        printf("%c",' ');
        i++;
    }
    
    for (i=0; i<numw-1;i++)
    {
        free(wordlist[i]);
    }
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
    char a1, b1;
    while (1)
    {
      //space is checked as it is used to indicate end of word
        if (*(a+i) == ' ' && *(b+j) == ' ')
            return 0;
        else if (*(a+i) == ' ')
            return -1;
        else if (*(b+j) == ' ')
            return 1;

        a1 = *(a+i) ^ 0x2A;
        b1 = *(b+j) ^ 0x2A;

        if (a1 > b1)
            return 1;
        if (a1 < b1)
            return -1;
        i++;
        j++;
    }
    return 0;
}
