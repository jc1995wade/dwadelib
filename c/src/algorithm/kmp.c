#include <stdio.h>


#include <string.h>


//未改进的KMP算法代码实现
void get_next(int *next, char *T, int len)
{
	next[0] = -1;//-1代表没有重复子串
	int k = -1;
	for (int q = 1; q <= len; q++)
	{
		while (k > -1 && T[k+1] != T[q])//下一个元素不相等，把k向前回溯
		{
			k = next[k];
		}
		if (T[k+1] == T[q])//下一个元素相等，所以最长重复子串+1
		{
			k = k+1;
		}
		next[q] = k;//给next数组赋值
	}
}
int KMP(char *s, int len, char *p, int plen)//利用KMP算法匹配
{
	int *next = new int(plen);
	get_next(next, p, plen);
	int k = -1;
	int i = 0;
	for (; i < len; i++)
	{
 		while (k > -1 && p[k+1]!=s[i])//两串下一个字符不相等，向前回溯（效率高就是在这里，每次匹配失败，k不用直接变为0，从第一个字符开始重新匹配，而是变为最长重复子串的下一个字符，从中间开始匹配即可）。
		{
			k = next[k];
		}
		if(p[k+1] == s[i])//两个串的字符相等，k+1来匹配子串的一个字符
		{
			k++;
		}
		if (k == plen-1)//匹配成功，返回短串在长串的位置。
		{
			return i-plen+1;
 
		}
	}
	return -1;
}
int main()
{
	
	char a[] = "bacbababadababacambabacaddababacasdsd";
	char b[] = "ababaca";
	int m = KMP(a,strlen(a),b,strlen(b));
	printf("%d", m);
	return 0;
}