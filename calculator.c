#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void display_c(char *arr, int size){
    for (int i = 0; i<size; i++){
        printf("%c - i = %d\n", arr[i], i);
    }
}

void display_n(int *arr, int size){
    for (int i = 0; i<size; i++){
        printf("%d\n", arr[i]);
    }
}

void getOperands(char* msg, int size, int* list, int* total){
    char anti_space[size];
    int j = 0;
    // anti space
    for (int i = 0; i<size; i++){
        if (msg[i] == '+' || msg[i] == '-' || isdigit(msg[i]) == 1 || msg[i] == '*' || msg[i] == '/'){
            anti_space[j++] = msg[i];
        }
    }


    int number_lists[j];
    int check = 0, index = 0, column = 0;

    char tmp[j][j];
    int num;
    // get the number in the string
    for (int i =0; i<j; i++){
        tmp[column][index++] = anti_space[i];
        
        if (isdigit(anti_space[i]) == 1 && isdigit(anti_space[i+1]) != 1){
            tmp[column][index] = '\0';
            index = 0;
            column++;
        }
    }

    // chose '+' or '-'
    int column1 = 0;
    int number_of_operators = 0;
    char operators[column][j];

    int list_number_of_operators[column];
    int list_number_of_operands[column];
    char list_characters_of_operands[column][20];
    int list_characters_of_operands_index = 0;
    for (int i = 0; i<column; i++){
        number_of_operators = 0;
        list_characters_of_operands_index = 0;
        for (int i1 = 0; i1<strlen(tmp[i]); i1++){
            // if the number of '-' is even --> '+'
            if (tmp[i][i1] == '-'){
                number_of_operators++;
            }
            // get the number char
            if (isdigit(tmp[i][i1])){
                list_characters_of_operands[i][list_characters_of_operands_index++] = tmp[i][i1];
            }
        }
        // null terminator
        list_characters_of_operands[i][list_characters_of_operands_index] = '\0';

        list_number_of_operands[i] = atoi(list_characters_of_operands[i]);
        list_number_of_operators[i] = number_of_operators;
    }


    for (int i = 0; i<column; i++){
        if (list_number_of_operators[i] %2 == 1){
            list_number_of_operands[i] = -list_number_of_operands[i];
        }
    }

    *total = column;
    for (int i = 0; i<column; i++){
        list[i] = list_number_of_operands[i];
    }
}

void getOperators(char* msg, int size, char* list, int* total){
    char anti_space[size];
    int index = 0;
    for (int i = 0; i<size; i++){
        if (msg[i] == '+' || msg[i] == '-' || isdigit(msg[i]) == 1 || msg[i] == '*' || msg[i] == '/'){
            anti_space[index++] = msg[i];
        }
    }

    char operators[100];
    int j = 0;
    for (int i = 0; i<size; i++){
        if (isdigit(anti_space[i-1]) && (anti_space[i] == '+' || anti_space[i] == '-')){
            operators[j++] = '+';
        }
        else if (isdigit(anti_space[i-1]) && (anti_space[i] == '*' || anti_space[i] == '/')){
            operators[j++] = anti_space[i];
        }
    }
    *total = j;
    for (int i = 0; i<j; i++){
        list[i] = operators[i];
    }
}

int normal_calculations(char* msg, int size){
    int n_operators, n_operands;
    int operands[20];
    char operators[20];
    getOperators(msg, size, operators, &n_operators);

    getOperands(msg, size, operands, &n_operands);

    for (int i = 0; i<n_operands; i++){
       printf("%d; ", operands[i]);
    }
    printf("\n");
    for (int i = 0; i<n_operators; i++){
       printf("%c; ",operators[i]);
    }
    printf("\n------------\n");

    int i = 0, res;
    int num1, num2;
    while(i < n_operators){
        if (operators[i] == '*' || operators[i] == '/'){
            num1 = operands[i];
            num2 = operands[i+1];
            char op = operators[i];

            if (op == '*'){
                res = num1*num2;
            }
            else if (op == '/'){
                res = num1/num2;
            }
            //printf("--> %d, n1[%d] = %d, n2[%d] = %d\n", res, i, num1, i+1, num2);
            operands[i+1] = res;
            operands[i] = -222; // nothing
            operators[i] = 'f'; // nothing
        }
        i++;
    }

    i = 0;
    while(i < n_operators){
        if (operators[i] == '+' || operators[i] == '-'){
            num1 = operands[i];
            int state = i;
            // ignore 'x' until get '+' or '-' on list_c
            while(operators[state+1] == 'f'){
                state++;
            }
            num2 = operands[state+1];
            char op = operands[i];

            // num1 and num2 are probably negative numbers
            if (op == '+'){
                res = num1+num2;
            }else{
                res = num1+num2;
            }

            //printf("--> %d, n1[%d] = %d, n2[%d] = %d\n", res, i, num1, state+1, num2);
            operands[state+1] = res;
        }
        i++;
    }

    return res;
}

void sub(char *msg, int size){
    int flag = 0;
    char list[10][size];
    int string_i=0, char_i=0;

    for (int i = 0; i<size; i++){
        if (msg[i] == '('){
            flag = 1;
            //list[string_i][char_i++] = msg[i];
        }
        else if (msg[i] == ')'){
            list[string_i][char_i++] = msg[i];
            list[string_i++][char_i] = '\0';
            char_i=0;
            flag = 0;
            //printf("list[string_i-1] = %s\n", list[string_i-1]);
        }
        if (flag == 1){
            list[string_i][char_i++] = msg[i];
        }
    }

    char new[size];
    strcpy(new, msg);
    for (int i = 0; i < string_i; i++){
        int tmp = normal_calculations(list[i], strlen(list[i])+1);
        char temp[100];
        sprintf(temp, "%d", tmp);
        //printf("string = %s\n", temp);
        //memmove();
        
        REPLACE_PASS(new, list[i], temp, new);
        //printf("new = %s\n", new);
    }

    int tmp = normal_calculations(new, strlen(new)+1);
    printf("%s = %d\n", msg, tmp);
}

int main(){
    //char msg[] = "(10*2-10)*2 + 5*2*(4/2)";
    //char msg[] = "(1+2)*2  + 3* -2 +4*(20/5)";
    while(1){
        printf("Enter the calculation formula: ");
        char msg[200];
        fgets(msg, 200, stdin);
        msg[strlen(msg)-1] = '\0';

        int size = strlen(msg)+1;
        sub(msg, size);
    }
}
