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
    // printf("bouta go in\n");
    //display(first);
    if(isNull(first)){
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

/*
Checks error for lambda functions
*/
void errorCheckLambda(Value *args){
    if(isNull(args) || isNull(cdr(args)) || isNull(car(cdr(args)))){
        printf("Evaluation error: lambda input not recognized.\n");
        texit(0);
    }
    // printf("%u", car(car(args))->type);
    
    if((car(car(args))->type != SYMBOL_TYPE && car(car(args))->type != NULL_TYPE)){
        printf("Evaluation error: lambda arguments must be symbols.\n");
        texit(0);
    }
}