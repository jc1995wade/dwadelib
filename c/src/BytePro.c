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

/**
 * 对32位的数，即4个字节，大端转换成小端
 * @param  value [big]
 * @return       [lit]
 */
uint32_t reversebytes_uint32t(uint32_t value)
{
    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8  | 
           (value & 0x00FF0000U) >> 8  | (value & 0xFF000000U) >> 24; 
}



#if 1
int main()
{
    char s[10];
    gets(s);
    uint32_t n = strhex2dec(s, strlen(s));
    uint32_t m = reversebytes_uint32t(n);
    
    printf("%d\n",n);
    printf("%x\n",n);
    printf("%x\n", m);
    return 0;
}
#endif