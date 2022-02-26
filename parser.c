#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"


/*
Prints out the linked Value list structure. 
Recursively calls itself if the type is CONS_TYPE.
Else, prints out the appropriate value depending on the passed in value structs type.
Returns nothing/
*/
void printTree(Value *tree){
    while(!isNull(tree)){
        switch (car(tree)->type) {
            case INT_TYPE:
                printf("%i", car(tree)->i);
                break;
            case DOUBLE_TYPE:
                printf("%f", car(tree)->d);
                break;
            case STR_TYPE:
                printf("\"%s\"", car(tree)->s);
                break;
            case CONS_TYPE:
                printf("(");
                printTree((car(tree)));
                printf(")");
                break;
            case SYMBOL_TYPE:
                printf("%s", car(tree)->s);
                break;
            case BOOL_TYPE:
                printf("%s", car(tree)->s);
                break;
            default:
                break;

        }
        if(!isNull(cdr(tree)) && (car(tree)->type != CLOSE_TYPE)){
            printf(" ");
        }
        
        tree = cdr(tree);
    }
    
}


/*
Creates a parse tree with the given tokens. Returns the parse tree.
Throws and error if there are incorrect amount of parens
The returned parse tree has a linked list value struct type
*/
Value *parse(Value *tokens){
    Value *curr = tokens;
    Value *stack = makeNull();
    int depth = 0;
    while(!isNull(curr)){
        if(car(curr)->type != CLOSE_TYPE){

            if(car(curr)->type == OPEN_TYPE){
                depth--;
            }
            stack = cons(car(curr), stack);

        } else{
            depth++;
            Value *node = makeNull();
            node = cons(makeNull(), node);

            if(isNull(stack)){
                printf("Syntax error: incorrect amount of parenthesis\n");
                texit(0);
            }

            if(isNull(cdr(curr)) && depth !=0){
                printf("Syntax error: incorrect amount of parenthesis\n");
                texit(0);
            }

            while(car(stack)->type != OPEN_TYPE){
                if(isNull(stack)){
                    printf("Syntax error: incorrect amount of parenthesis\n");
                    texit(0);
                }
                node = cons(car(stack), node);
                stack = cdr(stack);
            }
            stack = cons(node, cdr(stack));
            
          
            
        }
        
        curr = cdr(curr);   
    }

    if(depth != 0){
        printf("Syntax error: incorrect amount of parenthesis\n");
        texit(0);
    }

    Value *returnValue = reverse(stack);
    return returnValue;
}





