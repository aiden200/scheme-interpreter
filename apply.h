#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

#ifndef _APPLY
#define _APPLY

Value *apply(Value *function, Value *args);

#endif