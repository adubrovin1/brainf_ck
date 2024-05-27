#include <stdio.h>
#include <stdlib.h>


//vector etc

typedef struct vector {
    char* arr;
    int cnt;
    int cap;
} vector;

void add(vector *v, char x) {
    if (v->cnt == v->cap) {
        int newCap = v->cap * 2;
        char* newArr = realloc(v->arr, newCap * sizeof(char));
        v->arr = newArr;
        v->cap = newCap;
    }
    v->arr[v->cnt++] = x;
}


//checking is symbol a command

int is_command(char c){
    char possible[] = {'>', '<', '+', '-', '.', ',', '[', ']'};
    for (int i = 0; i < 8; i++){
        if (c == possible[i]){
            return 1;
        }
    }
    return 0;
}


//stack for chars

typedef struct simple_stack{
    int* arr;
    int ptr;
} simple_stack;

void add_to_stack (simple_stack* stack, int value){
    stack->arr[++stack->ptr] = value;
}

int take_from_stack (simple_stack* stack, int* err){
    if ((stack->ptr) == 0){
        *err = 1;
        return -1;
    }
    else{
        (stack->ptr);
        return stack->arr[stack->ptr--];
    }
}


//Interpreting starts here

int main(){
    vector code;
    code.arr = malloc(sizeof(char) * 4);
    code.cnt = 0;
    code.cap = 4;
    int prog_len = 0;
    char temp;
    while ((temp = fgetc(stdin)) != '\n'){
        if (is_command(temp)){
            add(&code, temp);
        }
        else{
            printf("Error - Incorrect symbol: symbol %c (ASCII code %d) on position %d is not a correct brainfuck command.", temp, temp, (code.cnt + 1));
            return 3;
        }
    }

    
    int err = 0;
    int take = 0;
    int* jumps = malloc(sizeof(int) * (code.cnt));
    simple_stack jumps_stack;
    int* temparr = malloc(sizeof(int) * (code.cnt));
    jumps_stack.arr = temparr;
    jumps_stack.ptr = 0;

    for (int i = 0; i < code.cnt; i++){
        if ((code.arr)[i] == '['){
            add_to_stack(&jumps_stack, i);
        }

        if ((code.arr)[i] == ']'){
            take = take_from_stack(&jumps_stack, &err);
            if (err == 1){
                printf("Error - incorrect brackets sequence. Closing bracket on position %d has no opening bracket", (i + 1));
                return 4;
            }
            jumps[i] = take;
            jumps[take] = i;
        }
    }

    if (jumps_stack.ptr != 0){
        take = take_from_stack(&jumps_stack, &err);
        printf("Error - incorrect brackets sequence. Opening bracket on position %d has no closing bracket.", (take + 1));
        return 4;
    }
    

    //30000 cells array (that's a standard of this language)
    char tape[30000] = {0};
    int i = 0;

    int pointer = 0;
    char c;
    //char temp; //been used before
    while (pointer < code.cnt){
        c = (code.arr)[pointer];
        
        if (c == '>'){
            i++;
        }

        if (c == '<'){
            i--;
        }

        if (c == '+'){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on + operation on position %d.", i, (pointer + 1));
                return 5;
            }
            tape[i] = tape[i] + 1;
        }

        if (c == '-'){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on - operation on position %d.", i, (pointer + 1));
                return 5;
            }
            tape[i] = tape[i] - 1;
        }

        if (c == '.'){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on . operation on position %d.", i, (pointer + 1));
                return 5;
            }
            temp = tape[i];
            fprintf(stdout, "%c", temp);
        }

        if (c == ','){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on , operation on position %d.", i, (pointer + 1));
                return 5;
            }
            scanf("%c", &temp);
            tape[i] = temp;
        }

        if (c == '['){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on [ operation on position %d.", i, (pointer + 1));
                return 5;
            }
            if (tape[i] == 0){
                pointer = jumps[pointer];
            }
        }

        if (c == ']'){
            if ((i < 0) || (i >= 30000)){
                printf("Error - incorrect indice: indice %d is not in range from 0 to 29999 (due to language standard array size is limited by 30000). Error occured on ] operation on position %d.", i, (pointer + 1));
                return 5;
            }
            if (tape[i] != 0){
                pointer = jumps[pointer];
            }
        }

        pointer++;
    }

    free(code.arr);
    free(jumps);
    free(temparr);

    return 0;
}