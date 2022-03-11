#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "linkedlist.h"
#include "value.h"
#include "talloc.h"

/*
Created a new struct Value that has a NULL_TYPE
Checks if malloc manage to find some memory
*/
Value *makeNull(){
    Value *null_pointer = talloc(sizeof(Value));
    //assert(null_pointer!= NULL && "Error in malloc, not enough memory.\n");
    null_pointer->type = NULL_TYPE;
    return null_pointer;
}

/*
Checks if the passed in value struct is a NULL_TYPE
returns true if true, else false
*/
bool isNull(Value *value){
    //assert(value != NULL && "Error: passed in a NULL value\n");
    if(value->type == NULL_TYPE){
        return true;
    }
    return false;
}

/*
Creates new value struct with a CONS_TYPE. 
Parameter: newCar, newCdr
These parameters each go into the car pointer and the cdr pointer of the new values struct.
Checks if malloc allocated enough memory
*/
Value *cons(Value *newCar, Value *newCdr){
    struct Value *new_cons = talloc(sizeof(struct Value));
    //assert(new_cons!= NULL && "Error in malloc, not enough memory.\n");
    //maybe check if car and cdr are actual value structs
    new_cons->type = CONS_TYPE;
    (new_cons->c).car = newCar;
    (new_cons->c).cdr = newCdr;
    return new_cons;
}

/*
Returns the pointer to the struct value car from the passed in value struct
Checks if there is no car value at the head of an empty list
*/
Value *car(Value *list){
    //assert(!isNull(list) && "Wrong type of list to apply function car");
    return (list->c).car;
}

/*
Returns the pointer to the struct value cdr from the passed in value struct
Checks if there is no cdr value at the head of an empty list
*/
Value *cdr(Value *list){
    //assert(!isNull(list) && "Wrong type of list to apply function cdr");
    return (list->c).cdr;
}

/*
Prints out the linked Value list structure. 
Recursively calls itself if the type is CONS_TYPE.
Else, prints out the appropriate value depending on the passed in value structs type.
Returns nothing/
*/
void display(Value *list){
    switch (list->type) {
        case INT_TYPE:
            printf("%i", list->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", list->d);
            break;
        case STR_TYPE:
            printf("\"%s\"", list->s);
            break;
        case CONS_TYPE:
            if(car(list)->type == NULL_TYPE){
                break;
            }
            display((list->c).car);
            printf(" ");
            display((list->c).cdr); 
        case SYMBOL_TYPE:
            printf("%s", list->s);
            break;
        case CLOSURE_TYPE:
            printf("%s", "#<procedure>");
            break;
        case BOOL_TYPE:
            printf("%s", list->s);
            break;
        case OPEN_TYPE:
            printf("(");
            break;
        case CLOSE_TYPE:
            printf(")");
            break;
        default:
            break;

    }
}



/*
Takes in a value struct linked list. Will return a new value struct linked list
that is the reverse of the original one. These two structures will share memory at car.
*/
Value *reverse(Value *list){
    int length = 0;
    struct Value *new_head = makeNull();
    if(list->type == NULL_TYPE){
        return new_head;
    }
    struct Value *curr_length = list;
   
    while(curr_length->type != NULL_TYPE){
        curr_length = (curr_length->c).cdr;
        length++;
    }
    if(length == 0){
        return list;
    } 
    
    for(int i = 0; i < length; i++){
        struct Value *next = new_head;
        struct Value *curr = list;

        for(int j=0; j<i; j++){
            curr = cdr(curr);                
        }

        //printf("Type: %u\n", curr->type);
        new_head = talloc(sizeof(Value));
        new_head->type = CONS_TYPE;
        (new_head->c).car = car(curr);
        //display(new_head);
        (new_head->c).cdr = next;

    }
    
    return new_head;
}



/*
Returns the length of the passsed in value struct as an int. A length of a struct is defined as 
how many values exist in a struct.
*/
int length(Value *value){
    //assert(value != NULL && "Passed in NULL type, expecting Value type.\n");
    if(value->type == NULL_TYPE){
        return 0;
    }
    int counter = 0;
    if((value->c).car->type != NULL_TYPE){
        counter += 1;
    }
    if((value->c).cdr->type == CONS_TYPE){
        counter += length((value->c).cdr);
    }
    else if((value->c).cdr->type != NULL_TYPE){
        counter += 1;
    }
    return counter;
}



