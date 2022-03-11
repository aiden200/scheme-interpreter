#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"


#ifndef _EVAL_LAMBDA
#define _EVAL_LAMBDA

Value *evalLambda(Value *args, Frame *f);


#endif