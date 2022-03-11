#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "parser.h"
#include "eval_define.h"
#include "look_up_symbol.h"


/*
Built in car function, returns the car of the passed in args
*/
Value *builtInCar(Value *args) {
    if(isNull(args) || isNull(car(args)) || car(args)->type != CONS_TYPE || !isNull(cdr(args))){
        printf("Evaluation error: no arguments supplied to car\n");
        texit(0);
    }

   return (car(car(args)));
}

/*
built in function of cdr, returns the cdr of the passed in args
*/
Value *builtInCdr(Value *args) {

    if(isNull(args) || isNull(car(args))){// || isNull(cdr(car(args))) ){
        printf("Evaluation error: not enough arguments supplied to cdr\n");
        texit(0);
    }

    return cdr(car(args));
}

/* 
built in function of plus, returns all the added values passed in args
*/
Value *builtInPlus(Value *args){
    // printf("going inot plus");
    // display(args);
    Value *node = talloc(sizeof(Value));
    node->type = INT_TYPE;
    Value* curr = args;
    int sum = 0;
    double sumD = 0;
    if(isNull(args)){
        node->d = 0;
    } else{
        while(!isNull(curr) && !isNull(car(curr))){
            if(car(curr)->type != DOUBLE_TYPE && car(curr)->type != INT_TYPE){
                printf("Evaluation error: wrong type of argument to apply to +\n");
                texit(0);
            }
            if(car(curr)->type == DOUBLE_TYPE && node->type == INT_TYPE){
                node->type = DOUBLE_TYPE;
                sumD = sum;
                
            }
            if(node->type == DOUBLE_TYPE){
                if(car(curr)->type == DOUBLE_TYPE){
                    sumD += car(curr)->d;
                } else{
                    sumD += car(curr)->i;
                }   

            }else{
                if(car(curr)->type == DOUBLE_TYPE){
                    sum += car(curr)->d;
                } else{
                    sum += car(curr)->i;
                } 
                

            }

            curr = cdr(curr);
        }
    }

    if(node->type == DOUBLE_TYPE){
        node->d = sumD;
    } else{
        node->i = sum;
    }
    
    return node;
}

/*
built in cons function, appends all the arguments and returns the linked list
*/
Value *builtInCons(Value *args){

    if(isNull(args) || isNull(car(args)) || isNull(cdr(args)) || !isNull(cdr(cdr(args)))){
        printf("Evaluation error: wrong arguments for cons\n");
        texit(0);
    }

    Value *curr = args;
    Value* node = talloc(sizeof(Value));
    node->type = CONS_TYPE;

    (node->c).car = car(args);
    (node->c).cdr = car(cdr(args));

    return node;
}

/*
Built in checkNull function which evaluates if the passed in argument evaluates to null
*/
Value *builtInNull(Value *args){

    if(isNull(args)){
        printf("Evaluation error: no arguments supplied to null?\n");
        texit(0);
    }
    if(!isNull(cdr(args))){
        printf("Evaluation error: null? takes one argument\n");
        texit(0);
    }
    Value *bool_val = makeNull();
    bool_val->type = BOOL_TYPE;
    bool_val->s = talloc(3*sizeof(char));

    if (isNull(car(args)) || (car(args)->type == CONS_TYPE && (car(car(args)))->type == NULL_TYPE)){

        memcpy(bool_val->s, "#t", strlen("#t")+1);
    } else{
        memcpy(bool_val->s, "#f", strlen("#f")+1);
    }
    return bool_val;
}

/*
evaluates its arguments left to right, and immediately returns #f upon evaluating a first non-truthy argument
*/
Value *builtInAnd(Value *args, Frame *f){
    Value *v = talloc(sizeof(Value));
    v->type = BOOL_TYPE;
    v->s = talloc(3*sizeof(char));
    Value *curr = args;
    while(!isNull(curr) && !isNull(car(curr))){
        Value* retVal = eval(car(curr), f);

        if(!(retVal->type == BOOL_TYPE && !strcmp(retVal->s, "#t"))){
            memcpy(v->s, "#f", strlen("#f")+1);
            return v;
        }
        curr = cdr(curr);
        
    }
    memcpy(v->s, "#t", strlen("#t")+1);
    return v;
}

/*
evaluates its arguments left to right, immediately returns #t upon evaluating a first truthy argument
*/
Value *builtInOr(Value *args, Frame *f){
    Value *v = talloc(sizeof(Value));
    v->type = BOOL_TYPE;
    v->s = talloc(3*sizeof(char));
    Value *curr = args;
    while(!isNull(curr) && !isNull(car(curr))){
        Value* retVal = eval(car(curr), f);
        if((retVal->type == BOOL_TYPE && !strcmp(retVal->s, "#t"))){
            memcpy(v->s, "#t", strlen("#t")+1);
            return v;
        }
        curr = cdr(curr);
        
    }
    memcpy(v->s, "#f", strlen("#f")+1);
    return v;
}

/*
checks condition, returns VOID_TYPE if nothing to return
*/
Value *builtInCond(Value *args, Frame *f){
    Value *curr = args;
    
    while(!isNull(curr) && !isNull(car(curr))){
        // display(cdr(car(curr)));
        // printf("%u", (car(car(curr)))->type);
        // fflush( stdout );
        if((car(car(curr))->type == SYMBOL_TYPE && !strcmp(car(car(curr))->s, "else"))||(eval(car(car(curr)),f )->type == BOOL_TYPE && !strcmp(eval(car(car(curr)),f )->s, "#t"))){
            // printf("in");
            // fflush( stdout );
            return eval(car(cdr(car(curr))),f);//this is evaluating car of cdr, which might be wrong.
        }
        curr = cdr(curr);
    }
    Value *void_val = talloc(sizeof(Value));
    void_val->type = VOID_TYPE;
    return void_val;
}

/*
This takes zero or more arguments, evaluates each of them, and returns the result of evaluating the final argument.
*/
Value *builtInBegin(Value *args, Frame *f){
    
    if(isNull(args) || isNull(car(args))){
        Value *return_val = talloc(sizeof(Value));
        return_val->type = VOID_TYPE;
        return return_val;
    }
    Value *curr = args;
    Value *return_v;
    while(!isNull(curr) && !isNull(car(curr))){
        return_v = eval(car(curr), f);
        curr = cdr(curr);
    }

    return return_v;

}

/*
modifies existing variable
*/
Value *builtInSet(Value *args, Frame *f){
    Frame *curr_frame = f;
    Value *param = car(args);
    Value *void_node = talloc(sizeof(Value));
    void_node->type = VOID_TYPE;
    while(curr_frame != NULL){
        Value *curr = curr_frame->bindings;
        while(!isNull(curr) && !isNull(car(curr))){
            
            if((car(curr)->type == CONS_TYPE) && !strcmp(car(car(curr))->s, param->s)){
                (car(curr)->c).cdr = eval(car(cdr(args)), f);

                return void_node;
            }
            curr = cdr(curr);
        }
        curr_frame = curr_frame->parent;
        
    }


    return void_node;
}


/*
Creates a new frame everytime a variable is defined, when all the variables are defined evaluate the cdr expression of args and return it.
*/
Value *builtInLetStar(Value *args, Frame *f){
    Value *params = car(args);
    Frame *curr = talloc(sizeof(Frame));
    curr = f;
    // curr->bindings = makeNull();
    while(!isNull(params) &&!isNull(car(params))){
        Frame *next = talloc(sizeof(Frame));
        next->parent = curr;
        next->bindings = makeNull();
        Value *node = talloc(sizeof(Value));
        node->type = CONS_TYPE;
        (node->c).car = car(car(params));
        (node->c).cdr = eval(car(cdr(car(params))), curr);
        next->bindings = cons(node, next->bindings);
        params = cdr(params);
        curr = next;
    }


    return eval(car(cdr(args)), curr);

}

/*
Defines mutually recursive functions, returns the evaluated function of the cdr of args with the new frame created
*/
Value *builtInLetrec(Value *args, Frame *f){
    Frame *env2 = talloc(sizeof(Frame));
    env2->parent = f;
    env2->bindings = makeNull();
    Value *curr = car(args);
    while(!isNull(curr) && !isNull(car(curr))){
        Value *node = talloc(sizeof(Value));
        Value *dummy = talloc(sizeof(Value));
        dummy->type = UNSPECIFIED_TYPE;
        node->type = CONS_TYPE;
        (node->c).car = car(car(curr));
        (node->c).cdr = dummy;
        env2->bindings = cons(node, env2->bindings);
        curr = cdr(curr);
    }
    curr = car(args);
    while(!isNull(curr) && !isNull(car(curr))){
        if(eval(car(cdr(car(curr))), env2)->type == UNSPECIFIED_TYPE){
            printf("Evaluation error: cannot use functions that were just defined in letrec\n");
            texit(0);
        }
        curr = cdr(curr);
    }


    curr = car(args);
    while(!isNull(curr) && !isNull(car(curr))){
        Value *node = cons(car(car(curr)), cons(car(cdr(car(curr))), makeNull()));
        builtInSet(node, env2);
        curr = cdr(curr);
    }
    
    curr = cdr(args);
    Value *return_val = talloc(sizeof(Value));
    while(!isNull(curr) && !isNull(car(curr))){
        return_val = eval(car(curr), env2);

        curr = cdr(curr);
    }
    return return_val;

}


/*
built in subtraction
*/
Value *builtInMinus(Value *args){
    Value *node = talloc(sizeof(Value));
    node->type = INT_TYPE;
    Value* curr = args;
    int first = 1;
    int sum = 0;
    double sumD = 0;
    if(isNull(args)){
        node->d = 0;
    } else{
        while(!isNull(curr) && !isNull(car(curr))){
            if(car(curr)->type != DOUBLE_TYPE && car(curr)->type != INT_TYPE){
                printf("Evaluation error: wrong type of argument to apply to +\n");
                texit(0);
            }
            if(car(curr)->type == DOUBLE_TYPE && node->type == INT_TYPE){
                node->type = DOUBLE_TYPE;
                sumD = sum;
                
            }
            if(node->type == DOUBLE_TYPE){
                if(first == 1){
                    sumD = car(curr)->d;
                    first = 0;
                } else{

                    if(car(curr)->type == DOUBLE_TYPE){
                        sumD -= car(curr)->d;
                    } else{
                        sumD -= car(curr)->i;
                    }   
                }

            }else{
                if(first == 1){
                    sum = car(curr)->i;
                    first = 0;
                } else{

                    if(car(curr)->type == DOUBLE_TYPE){
                        sum -= car(curr)->d;
                    } else{
                        sum -= car(curr)->i;
                    } 
                }
            }

            curr = cdr(curr);
        }
    }

    if(node->type == DOUBLE_TYPE){
        node->d = sumD;
    } else{
        node->i = sum;
    }
    
    return node;
}

/*
built in Multiplication
*/
Value *builtInStar(Value *args){
    Value *node = talloc(sizeof(Value));
    node->type = INT_TYPE;
    Value* curr = args;
    int sum = 1;
    double sumD = 1;
    if(isNull(args)){
        node->d = 0;
    } else{
        while(!isNull(curr) && !isNull(car(curr))){
            if(car(curr)->type != DOUBLE_TYPE && car(curr)->type != INT_TYPE){
                printf("Evaluation error: wrong type of argument to apply to +\n");
                texit(0);
            }
            if(car(curr)->type == DOUBLE_TYPE && node->type == INT_TYPE){
                node->type = DOUBLE_TYPE;
                sumD = sum;
                
            }
            if(node->type == DOUBLE_TYPE){
                if(car(curr)->type == DOUBLE_TYPE){
                    sumD *= car(curr)->d;
                } else{
                    sumD *= car(curr)->i;
                }   

            }else{
                if(car(curr)->type == DOUBLE_TYPE){
                    sum *= car(curr)->d;
                } else{
                    sum *= car(curr)->i;
                } 
            }
            curr = cdr(curr);
        }
    }

    if(node->type == DOUBLE_TYPE){
        node->d = sumD;
    } else{
        node->i = sum;
    }
    
    return node;
}

/*
built in divison, will return a float if numbers to not devide evenly
*/
Value *builtInSlash(Value *args){
    Value *node = talloc(sizeof(Value));
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    if(numOne->type == INT_TYPE){
        if(numTwo->type == INT_TYPE){
            if(numOne->i % numTwo->i == 0){
                node->type = INT_TYPE;
                node->i = numOne->i/numTwo->i;
            } else{
                node->type = DOUBLE_TYPE;
                node->d = numOne->i/(float)numTwo->i;
            }
        } else{
            node->type = DOUBLE_TYPE;
            node->d = numOne->i/numTwo->d; 
        }
    } else{
        node->type = DOUBLE_TYPE;
        if(numTwo->type == DOUBLE_TYPE){
            node->d = numOne->d/numTwo->d;
        } else{
            node->d = numOne->d/numTwo->i;
        }
        
    }

    return node;

    
}

/*
built in modulo function, takes in an integer.
*/
Value *builtInModulo(Value *args){
    Value *v = talloc(sizeof(Value));
    v->type = INT_TYPE;
    v->i = (car(args)->i) % (car(cdr(args))->i);
    return v;
}

/*
built in functions that checks if left argument is smaller than right
*/
Value *builtInSmaller(Value *args){
    Value *v = talloc(sizeof(Value));
    v->type = BOOL_TYPE;
    v->s = talloc(3*sizeof(char));
    if(car(args)->type == car(cdr(args))->type){
        switch (car(args)->type){
            case INT_TYPE: {
                if(car(args)->i < car(cdr(args))->i){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            case DOUBLE_TYPE: {
                if(car(args)->d < car(cdr(args))->d){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            default: {
                printf("Evaluation error: incorrect comparison type");
                texit(0);
            }
        }
    }
    memcpy(v->s, "#f", strlen("#f")+1);
    return v;
}

/*
built in functions that checks if left argument is greater than right
*/
Value *builtInGreater(Value *args){
    Value *v = talloc(sizeof(Value));
    v->type = BOOL_TYPE;
    v->s = talloc(3*sizeof(char));
    if(car(args)->type == car(cdr(args))->type){
        switch (car(args)->type){
            case INT_TYPE: {
                if(car(args)->i > car(cdr(args))->i){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            case DOUBLE_TYPE: {
                if(car(args)->d > car(cdr(args))->d){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            default: {
                printf("Evaluation error: incorrect comparison type");
                texit(0);
            }
        }
    }
    memcpy(v->s, "#f", strlen("#f")+1);
    return v;
}

/*
built in functions that checks if left argument is the same as the right
*/
Value *builtInEqual(Value *args){
    Value *v = talloc(sizeof(Value));
    v->type = BOOL_TYPE;
    v->s = talloc(3*sizeof(char));
    if(car(args)->type == car(cdr(args))->type){
        switch (car(args)->type){
            case INT_TYPE: {
                if(car(args)->i == car(cdr(args))->i){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            case DOUBLE_TYPE: {
                if(car(args)->d == car(cdr(args))->d){
                    memcpy(v->s, "#t", strlen("#t")+1);
                } else{
                    memcpy(v->s, "#f", strlen("#f")+1);
                }
                return v;
            }
            default: {
                printf("Evaluation error: incorrect comparison type");
                texit(0);
            }
        }
    }
    memcpy(v->s, "#f", strlen("#f")+1);
    return v;
}