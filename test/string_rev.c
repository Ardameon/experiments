#include <string.h>
#include <ctype.h>
#include <stdio.h>


#define STR_LEN 4096



char *reverse_letter(const char *str)
{
    static char out_str[STR_LEN];
    int i, j;
    
    for (j = 0, i = strlen(str) - 1; i >= 0; i--)
    {
      if (isalpha(str[i])) out_str[j++] = str[i];
    }
    
    out_str[j] = '\0';
    
    return out_str; //coding and coding..
}


int main(int argc, char **argv)
{
	printf("%s\n", reverse_letter(argv[1]));

	return 0; 
}
