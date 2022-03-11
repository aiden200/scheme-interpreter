#include <stdbool.h>
#include "value.h"

#ifndef _EVAL_ERROR
#define _EVAL_ERROR

void errorCheckIf(Value *first);

void errorCheckLet(Value *args);

void errorCheck(Value *first);

void errorCheckQuote(Value *args);

void evalationError();

void errorCheckLambda(Value *args);

#endif