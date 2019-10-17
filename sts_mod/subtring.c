#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char* string_after(char* str, char* search);

int main(void)
{
    char* str = "Bearer XPTO";
    char* search = "Bearer ";
    char* result = string_after(str, search);
    printf("%s\n", result);
}

char* string_after(char* str, char* search) {
    if ((int)strlen(str) <= (int)strlen(search)) {
        return "";
    }
    
    char* retorno = (char *)malloc((int)strlen(str) * sizeof(char));

    int j = 0;

    for (int i = 0 ; i < strlen(str); i++) {
        if (str[i] == search[i]) {
            continue;
        } else {
            retorno[j++] = str[i];
        }
    }
    
    return retorno;
}