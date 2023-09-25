#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "lib.h"

// create a function to extract operands
int* extract_operand(char* input, int* j){
    int* numbers = (int*)malloc(100*4);
    char currentValue[20]; // Assuming a maximum length for the values
    int currentIndex = 0;
    bool isNegative = false;
    *j = 0;

    for (int i = 0; i < strlen(input); i++) {
        char currentChar = input[i];

        if (isdigit(currentChar) || (currentChar == '-' && (i == 0 || input[i - 1] == ' '))) {
            // Append the character to the current value
            currentValue[currentIndex] = currentChar;
            currentIndex++;
        } else if (currentIndex > 0) {
            // Null-terminate the current value and convert it to an integer
            currentValue[currentIndex] = '\0';
            int numericValue = atoi(currentValue);

            // Print the numeric value
            //printf("->Value: %d\n", isNegative ? -numericValue : numericValue);

        if (isNegative){
            numbers[(*j)++] = -numericValue;
        }
        else numbers[(*j)++] = numericValue;

            // Reset the current value
            currentIndex = 0;
            isNegative = false;
        }

        if (currentChar == '-') {
            isNegative = true;
        }
    }

    // Check if there's a pending value at the end of the string
    if (currentIndex > 0) {
        currentValue[currentIndex] = '\0';
        int numericValue = atoi(currentValue);
        //printf("------->Value: %d\n", isNegative ? -numericValue : numericValue);

        if (isNegative){
            numbers[(*j)++] = -numericValue;
        }
        else numbers[(*j)++] = numericValue;
    
    }
    return numbers;
}


// create a function to extract operators
char* extract_operator(const char *msg, int*j){
    char* list = (char*)malloc(100*1);
    *j =0;
    for (int i = 0; msg[i]!='\0'; i++){
        char c = msg[i];
        if(c == '+'){
            list[(*j)++] = c;
        }
        if(c == '-' && msg[i-1] != '*'){
            if (msg[i-1] != '+' && msg[i-1] != '*' && msg[i-1] != '/'){
                list[(*j)++] = '.';
            }

        }  

        if(c == '*'){
            list[(*j)++] = c;
        }
        if(c == '/'){
            list[(*j)++] = c;
        } 
    }
    return list;
}

// calculate numbers with 4 standard operators
int four_operators(char* msg){
    int count=0, count1=0;
    int* list_num = extract_operand(msg, &count);
    char* list_c = extract_operator(msg, &count1);

    int res;
    int i=0;
    int check = 0;

    while(i<(count1)){
        if (list_c[i] == '*' || list_c[i] == '/'){
            int num1 = list_num[i];
            int num2 = list_num[i+1];
            char operator = list_c[i];
      
            if (operator == '*'){
                res = num1*num2;
            }
            else {
                res = num1/num2;
            }

            list_num[i+1] = res;
            list_num[i] = -222; // nothing
            list_c[i] = 'x'; // nothing

            check++;
        }
        i++;
    }

    i = 0;
    while(i<(count1)){
        if (list_c[i] == '+' || list_c[i] == '-'){
            int num1 = list_num[i];
            int state = i;
            // ignore 'x' until get '+' or '-' on list_c
            while(list_c[state+1] == 'x'){
                state++;
            }
            int num2 = list_num[state+1];
            char op = list_c[i];


            // num1 and num2 are probably negative numbers
            if (op == '+'){
                res = num1+num2;
            }else{
                res = num1+num2;
            }

            // save te result into list_num to replace the index of num2
            list_num[state+1] = res;
        } 
        i++;
    }
    free(list_c);
    free(list_num);
    return res;
 

}

// extract and replace equations having brackets
void sub_string(char* msg, char* msg_res){
    int len = strlen(msg);
    int flag=0;
    int index;
    char list[10][2][200];
    int j=0;

    char new[200];
    for (int i = 0; i<len+1; i++){
        if (msg[i] == '('){
            flag = 1;
            index = 0;
        }

        // if encountering ')', stop adding elements
        if (msg[i-1] == ')' && flag == 1){
            flag = -1;
            new[index] = '\0';


            char new1[200];
            int len = strlen(new);
            memmove(new1+1, new, len+1);

            // add '0' to new1 to express 0-1 = -1 or 01 = 1
            new1[1] = '0';
            new1[0] = '(';
            new1[len+1] = '\0';
    

            
            memmove(list[j][0], new1, strlen(new1)+3);

            // replace the passage with new1's sub_string
            REPLACE_PASS(msg, new, new1, msg);

            int res = four_operators(list[j][0]);

            // if result <0 --> add '1' to express 1*-1 = -1 
            char result[50];
            if (res < 0){
                sprintf(result, "1*%d", res);
            }
            else sprintf(result, "%d", res);

            // copy result 's string to list[j][0] which is the string needed to be replaced
            memmove(list[j][1], result, strlen(result)+1);
            list[j][1][strlen(result)] = '\0';

            j++;
        }

        // if encountering '(', start adding elements
        if (flag == 1){
            new[index] = msg[i];
            index++;
        }

    }

    for (int i = 0; i<j; i++){
        REPLACE_PASS(msg, list[i][0], list[i][1], msg_res);
    }

}

// check if equations having brackets
int isHavingBracket(char* msg){
    int len = strlen(msg);
    for (int i = 0; i<len+1; i++){
        if (msg[i] == '('){
            return 1;
        }
    }
    return 0;
}

// print calculations on termina;
void printTerminal(char* msg, int res){
    printf("**********************\n");
    printf("%s = %d\n", msg, res);
    printf("**********************\n");
}

int main(){ 
    while(1){
    char msg[200];
    char msg1[200];
    printf("enter: ");

    fgets(msg1, sizeof(msg1), stdin);
    int len = strlen(msg1);
    msg1[len-1] = '\0';
    
    // add '0' to msg to express 0-1 = -1
    memmove(msg+1, msg1, len+1);
    msg[0] = '0';
    msg[len] = '\0';



    if (isHavingBracket(msg) == 0){
        printTerminal(msg1, four_operators(msg));
    }
    else if (isHavingBracket(msg) == 1){
        char msg_result[200];
        sub_string(msg, msg_result);
        int count, count1;

        printTerminal(msg1, four_operators(msg_result));
    }
    }
}