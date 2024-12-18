#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(void){
    char entrada[10];

    while(fgets(entrada, sizeof(entrada), stdin) != NULL){
        if(entrada[0] == '\n') break;
        printf("%s", entrada);
    }
    return 0;
}