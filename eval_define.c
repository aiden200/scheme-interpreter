#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"


/*
checks if symbol is already in bindings
*/
int checkBindings(char* s, Value *binding){
    while(!isNull(binding)){
        if(!strcmp(s, car(car(binding))->s)){
            return 1;
        }
        binding = cdr(binding);
    }
    return 0;
}



/*
Returns a void type Value after updating the current frame
*/
Value *evalDefine(Value *args, Frame *frame){
    Value *void_node = talloc(sizeof(Value));
    void_node->type = VOID_TYPE;

    Frame *e = frame;
    Value *curr = args;

    if(curr->type != CONS_TYPE){
        printf("Evaluation error: binding not CONS_TYPE\n");
        texit(0);
    }
    if(car(curr)->type != SYMBOL_TYPE){
        printf("Evaluation error: Assigning value to none symbol type\n");
        texit(0);
    }

    
    Value *node = talloc(sizeof(Value));
    node->type = CONS_TYPE;
    (node->c).car = car(curr);

    if(car(cdr(curr))->type == CONS_TYPE && car(car(cdr(curr)))->type != SYMBOL_TYPE){ 
        (node->c).cdr = car(cdr(curr));
    } else{
        (node->c).cdr = eval(car(cdr(curr)), e);
    }

    Value *next = frame->bindings;

    (frame->bindings) = (cons(node, next));
    
    return void_node;
}