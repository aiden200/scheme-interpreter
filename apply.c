#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "eval_define.h"
#include "parser.h"



/*
Applys function with given arguments and returns the value.
*/
Value *apply(Value *function, Value *args){

    Value *func_args = (function->cl).paramNames;
    Value *reg_args = args;
    // Frame *f = (function->cl).frame;
    Frame *newFrame = talloc(sizeof(struct Frame));
    newFrame->parent = (function->cl).frame;
    newFrame->bindings = makeNull();
    if(function->type == PRIMITIVE_TYPE){
        return (function->pf)(args);
    }

    while(!isNull(func_args) && !isNull(args) && !isNull(car(args))){

        
        Value *node = makeNull();
        node = cons(car(reg_args), node);

        evalDefine(cons(car(func_args), node), newFrame);

        func_args = cdr(func_args);
        reg_args = cdr(reg_args);
    }

    return eval((function->cl).functionCode, newFrame);
}