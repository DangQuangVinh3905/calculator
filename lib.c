#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib.h"



// ham con
void REPLACE_PASS(char* pass2, char* ex, char* replace, char* new){
    //printf("---------start----------start----------start-----------------start---------\n");
    char **msg = (char**)malloc(200*sizeof(char*));
    char **msg1 = (char**)malloc(200*sizeof(char*));
    char **first = (char**)malloc(200*sizeof(char*));
    char **last = (char**)malloc(200*sizeof(char*));
    int i=0;
    uint32_t len_msg;
    char* temp;
    temp = (char*)malloc(strlen(pass2)+1); // temporary
    strcpy(temp, pass2);
    while(1){
        temp = strstr(temp, ex);
        if (temp == NULL) break;
        msg[i] = (char*)malloc(strlen(temp)+1);
        if (strlen(replace)>strlen(ex)) msg1[i] = (char*)malloc(strlen(temp)-strlen(ex)+strlen(replace)+1);
        else msg1[i] = (char*)malloc(strlen(temp)+1);

        strcpy(msg[i], temp);
        strcpy(msg1[i], temp);
        //printf("msg[%d]-->`%s`\n", i, msg[i]);
        
        temp[0] = 'n';
        i++;
    }
    if (i == 0) {
        //printf("------------------%d results------------------------------------------------------------------------------------------------\n", i);
        //printf("`%s`\n", pass2);
        while(i>0){free(msg[i-1]);free(msg1[i-1]);i--;}
        free(msg);free(msg1);
    }
    else{
        int first_last = strlen(pass2) - strlen(msg[0]);
        char* yyy = extract_first_characters_n(pass2, first_last);

        char **msg2 = (char**)malloc(200*sizeof(char*));
        char **store = (char**)malloc(200*sizeof(char*));
        for (int j = 0; j<i; j++){
            int x;
            if (j != i-1) x = strlen(msg[j]) - strlen(msg[j+1]);
            else x = strlen(msg[j]);
            msg2[j] = (char*)malloc(x+1);
            msg2[j] = extract_first_characters_n(msg[j], x);

            if (strlen(replace)>strlen(ex)) msg1[j] = (char*)malloc(strlen(msg2[j])-strlen(ex)+strlen(replace)+1);
            else msg1[j] = (char*)malloc(strlen(msg2[j])+1);
            strcpy(msg1[j], msg2[j]);

            replaceString(msg1[j], ex, replace);

            //printf("`%s`\n", msg2[j]);

            first[j] = (char*)malloc(strlen(ex)+2);
            last[j] = (char*)malloc(strlen(msg2[j])-strlen(ex)+2);
            first[j] = extract_first_characters(msg2[j], ex);
            last[j] = extract_last_characters(msg2[j], ex);

            first[j] = add_first_char(first[j], '|');
            last[j] = add_first_char(last[j], '|');

            // printf("`%s` - `%s`\n", first[j], last[j]);
            store[j] = (char*)malloc(strlen(first[j])+strlen(last[j])+1);
            sprintf(store[j], "%s%s", first[j], last[j]);
            //printf("`%s`\n", store[j]);
        }
        len_msg = strlen(pass2)+i*(-strlen(ex)+strlen(replace));
        int len1_msg = strlen(pass2)+i*2;

        char new1[len1_msg+1];
        //char new[len_msg+1];
        //new = (char*)malloc(len_msg+1);

        //printf("i = %d\n", first_last);

        strcpy(new, yyy);
        strcpy(new1, yyy);
        for (int j = 0; j<i; j++){
            sprintf(new, "%s%s", new, msg1[j]);
            sprintf(new1, "%s%s", new1, store[j]);

        }
        //printf("------------------%d results------------------------------------------------------------------------------------------------\n", i);
        printf("`%s`\n", new1);
        //printf("-------------------%d results are replaced by `%s`---------------------------------------------------------------------\n", i, replace);
        printf("`%s`\n", new);
        strcpy(pass2, new);

        while(i>0){free(msg[i-1]);free(msg1[i-1]);free(msg2[i-1]);free(first[i-1]);free(last[i-1]);i--;}
        free(msg);free(last);free(first);free(msg2);free(msg1);
        free(temp);
        free(yyy);
    }


    

}

char* extract_first_characters(char* pass, char* extract){
    char* msg = (char*)malloc((strlen(extract)+1)*sizeof(char));
    memmove(msg, pass, strlen(extract));
    *(msg+strlen(extract)) = '\0';
    free(extract);
    return msg;
}

char* extract_last_characters(char* pass, char* extract){
    char* msg = (char*)malloc((strlen(pass) - strlen(extract)+1)*sizeof(char));
    memmove(msg, pass-0+strlen(extract), strlen(pass) - strlen(extract));
    *(msg+strlen(pass) - strlen(extract)) = '\0';
    free(extract);
    return msg;
}

void replaceString(char *str, const char *oldWord, const char *newWord) {
    char *pos = strstr(str, oldWord); // Find the position of oldWord in str

    if (pos != NULL) {
        int oldLength = strlen(oldWord);
        int newLength = strlen(newWord);

        // Move characters after the oldWord to make space for newWord
        memmove(pos + newLength, pos + oldLength, strlen(pos + oldLength) + 1);

        // Copy newWord into the position of oldWord
        strncpy(pos, newWord, newLength);
    }
}

char* extract_first_characters_n(char* source, int n) {
    char* destination = malloc((n + 1) * sizeof(char)); // Allocate memory for destination
    strncpy(destination, source, n); // Copy at most n characters
    destination[n] = '\0';
    return destination;
}

char* add_first_char(char* s, char mark){

    char* des = (char*)malloc(strlen(s) + 2);
    memmove(des+1, s,  strlen(s)+1);
    
    des[0] = mark;
    return des;
    free(des);
}