#include <stdio.h>
#include <stdlib.h>
#include <string.h >

#define uint32_t unsigned int

/**
 * @param  s   char
 * @param  len char length
 * @return     unsigned int 
 */
uint32_t strhex2dec(char s[], int len)
{
    uint32_t i,temp=0,n;
	for(i=0;i<len;i++)
	{
		if(s[i]>='A'&&s[i]<='F')
			n=s[i]-'A'+10;
		else if(s[i]>='a'&&s[i]<='f')
			n=s[i]-'a'+10;
		else 
			n=s[i]-'0';
		
		temp=temp*16+n;
	}
    return temp;
}


#if 1
int main()
{
    char s[10];
    gets(s);
    uint32_t n=strhex2dec(s, strlen(s));
    printf("%d\n",n);
    return 0;
}
#endif