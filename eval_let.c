#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "parser.h"


/*
Creates a new frame and sets the original frame as the parent. 
In the frame update the let symbols passed in.
Return the eval of the 2nd argument with the new frame created.
*/
Value *evalLet(Value *args, Frame *frame){
    
    Frame *e = frame;
    Frame *f = talloc(sizeof(Frame));
    f->parent = e;
    f->bindings = makeNull();
    Value *curr = car(args);
    
    while(curr == NULL || !isNull(cdr(curr)) ){

        if(car(curr)->type != CONS_TYPE){
            printf("Evaluation error: binding not CONS_TYPE\n");
            texit(0);
        }
        if(car(car(curr))->type != SYMBOL_TYPE){
            printf("Evaluation error: Assigning value to none symbol type\n");
            texit(0);
        }
        if(isNull(car(car(curr))) || isNull(cdr(car(curr)))){
            printf("Evaluation error: null binding in let.\n");
            texit(0);
        }

        Value *node = talloc(sizeof(Value));
        node->type = CONS_TYPE;
        (node->c).car = car(car(curr));
        (node->c).cdr = eval(car(cdr(car(curr))), e);
        Value *next = f->bindings;
        f->bindings = cons(node, next);
        curr = cdr(curr);

    }
    Value *currArgs = cdr(args);

    while(!isNull(cdr(currArgs)) && car(cdr(currArgs))->type != NULL_TYPE){
        eval(car(currArgs), f);
        currArgs = cdr(currArgs);
    }

    return eval(car(currArgs), f);


}
