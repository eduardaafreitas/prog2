#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main( int argc, char *argv[]){
    snprintf(buffer, "%%%ds", 10);
    printf("%*s \n", 10, "oiii");
    return 0 ;
}