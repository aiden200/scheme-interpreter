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
     case SYMBOL_TYPE: {
        result = lookUpSymbol(tree, frame);
        break;
     }  
     case CONS_TYPE: {
      Value *first = car(tree);
      Value *args = cdr(tree);

      // Error checking on 'first' goes here...
      errorCheck(first);

      if (!strcmp(first->s, "if")) {
         errorCheckIf(args);
         result = evalIf(args, frame);
      } else if(!strcmp(first->s, "let")) {
         //how does frames come into play with this??
         errorCheckLet(args);
         result = evalLet(args, frame);
      } else if(!strcmp(first->s, "quote") || !strcmp(first->s, "'")) {
         //how does frames come into play with this??
         errorCheckQuote(args);
         result = evalQuote(args);
      } else {
         printf("Evaluation error: 'first' is not recognized first: %s \n", first->s);
         texit(0);
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
Interprets the given tree, evaluates each car and prints
*/
void interpret(Value *tree){
    while(!isNull(tree)){
        struct Frame *f = talloc(sizeof(Frame));
        f->parent = NULL; 
        f->bindings = makeNull();
        printTree(eval(car(tree), f));
        printf("\n");
        tree = cdr(tree);
    }
}