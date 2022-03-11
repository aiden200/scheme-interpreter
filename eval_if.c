#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"


/*
evaluates if the first argument is false, if it is, return the third argument,
if not, return the second.
*/
Value *evalIf(Value *args, Frame *frame){
    Value *evalValue = eval(car(args), frame); 
    if(!strcmp(evalValue->s, "#f")){
        return eval(car(cdr(cdr(args))), frame);
    }
    return eval(car(cdr(args)), frame);
}

