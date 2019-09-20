//Raj Kotak
//Help Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help2.h"

FILE *ptr;
char chr;
char description[100];


//moved function definitions to hearder,

//Remove main when done testing(James will remove it). Call help() and pass "all" to print all the help commands or pass a specific word to print that command.
/*
int main(){
    
    char flag[20];
    
    printf("Enter the command to be printed \n");
    scanf("%s", flag);
    help(flag);
}
*/

//This fuction takes a string as a parameter and decides on what command to print.(pass "all" to print all help commands).
void help(char* helpCommand){
    if(helpCommand == NULL)
    {
        printAll();
    }
    else if(strcmp(helpCommand,"all") != 0){
        printOne(helpCommand);
    }
    else if(strcmp(helpCommand,"all") == 0){
        printAll();
    }
    else
    {
        printf("Not a vaild command!");
    }
    
}

//This function is called from help function if it is determined that a single command needs to be printed. 
//It splits the string and checks if it matches a commmand word(it moves the pointer to check if every word matches the command) and then prints that command.
//It will return no command found if no word is matched.

void printOne(char flag[]){

    char *check = flag;
    int cntr = 0;
    int cntr2;
    while(*check != '\0'){
        check++;
        cntr = cntr + 1;
    }
    
    char toCompare[cntr+1];

    for(cntr2 = 0; cntr2 < cntr; cntr2++){
        toCompare[cntr2] = flag[cntr2];
    }
    toCompare[cntr2] = '\0';
    
    FILE *abc; 
    int descriptionctr = 0;
    ptr = fopen("file.txt", "r");
    if(ptr == NULL){
        printf("Cannot open file\n");
        exit(0);
    }
    chr = fgetc(ptr);
    //int flag = 0;
    int cmdWd = sizeof(toCompare);
    int trvse = 0;
    int stop = 0;
    while(chr != EOF  && stop != 1){
        if(strcmp("^",&chr) == 0){
            abc = ptr;
            chr = fgetc(ptr);
            while(strcmp(")",&chr) != 0){
                if(trvse < cmdWd){
                    if(toCompare[trvse] == chr){
                        trvse++;
                        chr = fgetc(ptr);
                        if(strcmp(")",&chr) == 0){
                            chr = fgetc(ptr);
                            printf("%s",toCompare);
                            while(strcmp("!",&chr) != 0 && chr != EOF){
                                if(strcmp(",",&chr) == 0){
                                    printf("\n");
                                }
                                else{
                                printf("%c",chr);
                                }
                                chr = fgetc(ptr);
                            }
                            printf("\n");
                            //break;
                            stop = 1;
                        }
                        continue;
                    }
                    else{
                        trvse = 0;
                        break;
                    }
                }
            }
            
        }
        else{
        chr = fgetc(ptr);
        }
    }
    if(chr == EOF){
    printf("Command not found.\n");
    }
    fclose(ptr);

}


//This function is called from help function if it is determined that all commands needs to be printed.

void printAll(){
    FILE *abc; 
    int descriptionctr = 0;
    ptr = fopen("file.txt", "r");
    if(ptr == NULL){
        printf("Cannot open file\n");
        exit(0);
    }
    chr = fgetc(ptr);
    while(chr != EOF){
        if(chr == '^' || chr == ')' || chr == '!'){
            if(chr == '!'){
                printf("\n");
                chr = fgetc(ptr);
                continue;
            }
            else{
                chr = fgetc(ptr);
                continue;
            }

        }
        if(strcmp(",",&chr) == 0){
            printf("\n");
        }
        else{
            printf("%c",chr);
        }
        chr = fgetc(ptr);
    }
    fclose(ptr);
}

