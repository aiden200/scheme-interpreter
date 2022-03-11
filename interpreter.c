#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "eval_error.h"
#include "eval_if.h"
#include "look_up_symbol.h"
#include "eval_let.h"
#include "parser.h"
#include "eval_quote.h"
#include "eval_define.h"
#include "eval_lambda.h"
#include "apply.h"
#include "primitiveFunctions.h"




/*
Evaluates the tree with the given frame, returns a Value type
*/
Value *eval(Value *tree, Frame *frame) {
   Value *result;
   switch (tree->type)  {
     case INT_TYPE: {
        result = tree; 
        break;
     }
     case DOUBLE_TYPE: {
        result = tree; 
        break;
     }  
     case STR_TYPE: {
        result = tree; 
        break;
     }
    case BOOL_TYPE: {
        result = tree; 
        break;
     }
     case CLOSURE_TYPE: {
        result = tree;
        break;
     }
     case PRIMITIVE_TYPE: {
        result = tree;
        break;
     }
     case NULL_TYPE: {
        result = tree;
     }
     case SYMBOL_TYPE: {
         result = lookUpSymbol(tree, frame);
        break;
     }  
     case CONS_TYPE: {
         if(isNull(car(tree))){
            return tree;
         }

         Value *first = car(tree);
         Value *args = cdr(tree);

         errorCheck(first);
         if (!strcmp(first->s, "if")) {
            errorCheckIf(args);
            result = evalIf(args, frame);

         } else if(!strcmp(first->s, "let")) {

            errorCheckLet(args);
            result = evalLet(args, frame);
         } else if(!strcmp(first->s, "and")) {
            result = builtInAnd(args, frame);
            
         } else if(!strcmp(first->s, "or")) {
            result = builtInOr(args, frame);
            
         } else if(!strcmp(first->s, "begin")) {
            result = builtInBegin(args, frame);
            
         } else if(!strcmp(first->s, "cond")) {
            result = builtInCond(args, frame);
            
         } else if(!strcmp(first->s, "let*")) {
            result = builtInLetStar(args, frame);
            
         } else if(!strcmp(first->s, "letrec")) {
            result = builtInLetrec(args, frame);
            
         } else if(!strcmp(first->s, "quote") || !strcmp(first->s, "'")) {

            errorCheckQuote(args);
            result = evalQuote(args);

         } else if(!strcmp(first->s, "define")) {

            result = evalDefine(args, frame);
         } else if(!strcmp(first->s, "set!")) {

            result = builtInSet(args, frame);
         } else if(!strcmp(first->s, "lambda")) {
            errorCheckLambda(args);
            
            result = evalLambda(args, frame);
            
         } else {

            Value *evaluatedOperator = eval(first, frame);
            Value *evaluatedArgs = makeNull();
            if(!isNull(args) && !isNull(car(args))){
               evaluatedArgs = evalEach(args, frame);
            } 

            return apply(evaluatedOperator, evaluatedArgs);
         }
         break;


      }
      default: {
         printf("Evaluation error: Not supported Value type %u \n", tree->type);
         texit(0);
      }
      
    }    

    return result;
}

/*
Evaluates each argument and returns a linked list with the evaluated arguments.
*/
Value *evalEach(Value *args, Frame *f){

   Value *curr = args;
   Value *list = makeNull();

   while(!isNull(curr) && !isNull(car(curr))){

      list = cons(eval(car(curr), f), list);
      curr = cdr(curr);
   }

   return reverse(list);
}

/*
Interprets the given tree, evaluates each car and prints
*/
void interpret(Value *tree){
   struct Frame *f = talloc(sizeof(Frame));
   f->parent = NULL; 
   f->bindings = makeNull();
   // Create bindings in the global frame for all of
   // the built-in functions.
   bindPrimitiveFunction("car", &builtInCar, f);
   bindPrimitiveFunction("cdr", &builtInCdr, f);
   bindPrimitiveFunction("cons", &builtInCons, f);
   bindPrimitiveFunction("+", &builtInPlus, f);
   bindPrimitiveFunction("null?", &builtInNull, f);
   bindPrimitiveFunction("-", &builtInMinus, f);
   bindPrimitiveFunction("*", &builtInStar, f);
   bindPrimitiveFunction("/", &builtInSlash, f);
   bindPrimitiveFunction("+", &builtInPlus, f);
   bindPrimitiveFunction("modulo", &builtInModulo, f);
   bindPrimitiveFunction("<", &builtInSmaller, f);
   bindPrimitiveFunction(">", &builtInGreater, f);
   bindPrimitiveFunction("=", &builtInEqual, f);

    while(!isNull(tree)){

       Value *return_val = eval(car(tree), f);

        if (return_val->type!= VOID_TYPE){   
           if(return_val->type == CONS_TYPE){
              printTree(return_val);
               printf("\n");
           } else{
              display(return_val);
              printf("\n");
           }
            
        }
        tree = cdr(tree);
      

    }
}

/*
binds premetive function with a char, store it in the global frame
*/
void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame) {

    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *node = talloc(sizeof(Value));
    node->type = SYMBOL_TYPE;
    node->s = talloc((strlen(name) + 1)*sizeof(char));
    memcpy(node->s, name, strlen(name)+1);
    frame->bindings = cons(cons(node, value), frame->bindings);
}