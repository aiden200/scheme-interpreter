#include <stdio.h>
#include <stdlib.h>
#include "value.h"

struct Value *head = NULL;


/*
inserts into the active linked list. Each node will have 
a pointer value allocated and a pointer to next.
Returns allocated pointer
*/
void *talloc(size_t size){
    if(head == NULL){
        Value *next = malloc(sizeof(Value));
        next->p = malloc(size);
        head = next;
        (next->c).cdr = NULL;
        return next->p;
    }
    Value *curr = head;
    while((curr->c).cdr != NULL){
        curr = (curr->c).cdr;
    }
    Value *next = malloc(sizeof(Value));
    next->p = malloc(size);
    (curr->c).cdr = next;
    (next->c).cdr = NULL;
    return next->p;
}

/*
Frees all allocated memory called by talloc
returns nothing.
*/
void tfree(){
    Value *curr = head;
    Value *next;
    while(curr != NULL){
        next = (curr->c).cdr;
        free(curr->p);
        free(curr);
        curr = next;
    }
    head = NULL;
}

/*
Frees all memory then exits with the passed in status
Returns nothing.
*/
void texit(int status){
    tfree();
    exit(status);
}
