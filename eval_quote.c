#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "parser.h"

/*
quotes are literal values, print them directly by returning the value node without evaluating them.
*/
Value *evalQuote(Value *args){

    return args;
}