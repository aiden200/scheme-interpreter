#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"



/*
Looks for the given value of the symbol through the frames, throws and error and exits if
it cannot find one, returns the value of the symbol if it does find one.
*/
Value *lookUpSymbol(Value *tree, Frame *frame){
    Frame *currFrame = frame;
    while(currFrame != NULL){
        Value *curr = currFrame->bindings;
        while(!isNull(curr)){            
            if(!strcmp(car(car(curr))->s, tree->s)){
                return cdr(car(curr));
            }
            curr = cdr(curr);
        }
        currFrame = currFrame->parent;
    }
    printf("Evaluation error: symbol %s not recognized.\n", tree->s);
    texit(0); 
    return tree;

}