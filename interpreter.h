#ifndef _INTERPRETER
#define _INTERPRETER

void interpret(Value *tree);

Value *eval(Value *expr, Frame *frame);

Value *evalEach(Value *args, Frame *f);

void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame);

#endif

