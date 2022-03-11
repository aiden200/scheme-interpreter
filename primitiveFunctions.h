#ifndef _PRIMITIVEFUNCTIONS
#define _PRIMITIVEFUNCTIONS

Value *builtInCar(Value *args);

Value *builtInCdr(Value *args);

Value *builtInPlus(Value *args);

Value *builtInCons(Value *args);

Value *builtInNull(Value *args);

Value *builtInAnd(Value *args, Frame *f);

Value *builtInOr(Value *args, Frame *f);

Value *builtInCond(Value *args, Frame *f);

Value *builtInBegin(Value *args, Frame *f);

Value *builtInSet(Value *args, Frame *f);

Value *builtInLetStar(Value *args, Frame *f);

Value *builtInLetrec(Value *args, Frame *f);

Value *builtInMinus(Value *args);

Value *builtInStar(Value *args);

Value *builtInSlash(Value *args);

Value *builtInModulo(Value *args);

Value *builtInSmaller(Value *args);

Value *builtInGreater(Value *args);

Value *builtInEqual(Value *args);

#endif