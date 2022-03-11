#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "parser.h"


/*
Creates a new closure type Value with the given params, frame, and body in args.
Returns this closure type.
*/
Value *evalLambda(Value *args, Frame *f){
    Value *closure_val = talloc(sizeof(Value));
    closure_val->type = CLOSURE_TYPE;
    (closure_val->cl).paramNames = makeNull();
    (closure_val->cl).functionCode = car(cdr(args));
    

    (closure_val->cl).frame = f;

    Value *curr = car(args);
    Value *track = makeNull();
    while(!isNull(curr) && !isNull(car(curr))){
        Value *currloop = track;
        while(!isNull(currloop)){
            if(!strcmp(car(curr)->s, car(currloop)->s)){
                printf("Evaluation error: Duplicate Values\n");
                texit(0);
            }
            currloop = cdr(currloop);
        }
        track = cons(car(curr),track);
        (closure_val->cl).paramNames = cons(car(curr), (closure_val->cl).paramNames);

        curr = cdr(curr);
       
    }
    (closure_val->cl).paramNames = reverse((closure_val->cl).paramNames );
    return closure_val;

}