#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char *strtok(char *str, const char *delim);
//
int dw_strtok_demo( void )
{
    char str[80] = "hello world wade ttt";
    const char space[2] = " ";
    char *token = NULL;

    token = strtok(str, space);

    while( token != NULL ) {
        printf("%s\n", token);
        token = strtok(NULL, space);
    }

    return 0;
}

// char *strtok_r(char *str, const char *delim, char **saveptr);
// ./a.out 'a/bbb///cc;xxx:yyy:' ':;' '/'
int main(int argc, char *argv[])
{
    char *str1, *str2, *token, *subtoken;
    char *saveptr1, *saveptr2;
    int j;

    if (argc != 4) {
    fprintf(stderr, "Usage: %s string delim subdelim\n",
        argv[0]);
        exit(EXIT_FAILURE);
    }

    for (j = 1, str1 = argv[1]; ; j++, str1 = NULL) {
        token = strtok_r(str1, argv[2], &saveptr1);
        if (token == NULL)
            break;
            printf("%d: %s\n", j, token);

        for (str2 = token; ; str2 = NULL) {
            subtoken = strtok_r(str2, argv[3], &saveptr2);
            if (subtoken == NULL)
                break;
            printf(" --> %s\n", subtoken);
        }
    }

    exit(EXIT_SUCCESS);
}

int string_parsing(void)
{
    int j,in = 0;
    char buffer[100] = "Fred male 25,John male 62,Anna female 16";
    char *p[20];
    char *buf = buffer;
    char *outer_ptr = NULL;
    char *inner_ptr = NULL;
    while ((p[in] = strtok_r(buf, ",", &outer_ptr)) != NULL)
    {
        buf = p[in];
        while ((p[in] = strtok_r(buf, " ", &inner_ptr)) != NULL)
        {
            in++;
            buf = NULL;
        }
        buf = NULL;
    }
    printf("Here we have %d strings\n", in);
    for (j = 0; j < in; j++)
    {
        printf(">%s<\n", p[j]);
    }
    return 0;
}

