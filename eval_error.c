#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
//#include "interpreter.c"

/*
Checks if the passed in list argument is valid for if statement
*/
void errorCheckIf(Value *first){

    Value *checkValue = first;
    for(int i = 0; i < 3; i++){
        if(isNull(checkValue)){
            printf("Evaluation error: 'if' passed fewer than 3 arguments\n");
            texit(0);
        }
        checkValue = cdr(checkValue);
    }

}

/*
Checks if the passed in Value argument is valid for let 
*/
void errorCheckLet(Value *args){
    
    if(car(args)->type != CONS_TYPE){
        printf("Evaluation error: Let argument not a list in list.\n");
        texit(0);
    }

}

/*
Checks if the passed in first argument is valid 
*/
void errorCheck(Value *first){
    if(isNull(first) || (strcmp(first->s, "if") && strcmp(first->s, "let") && strcmp(first->s, "quote"))){
        printf("Evaluation error: first argument not recognized.\n");
        texit(0);
    }

}


/*
Checks if the args in quote have only one argument
*/
void errorCheckQuote(Value *args){
    if(isNull(car(args)) || !isNull(car(cdr(args)))){
        printf("Evaluation error: bad arguments in quote\n");
        texit(0);
    }
}
/*
Exits when first is not valid
*/
void evalationError(){
    printf("Evaluation error: input not recognized.\n");
    texit(0);
}