#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"


//Check if the character indicates the end of the token
int checkEndToken(char c){
    if(c == ')' || c == ' ' || c == ';' || c == EOF || c == '\n' || c == '('){
        return 1;
    } else{
        return 0;
    }
}


//check if the char is a letter
int checkLetter(char c){
    return isalpha(c);
    char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int n = sizeof(letters)/sizeof(letters[0]);
    for(int i =0; i < n; i++){
        if(letters[i] == c){
            return 1;
        }
    }
    return 0;
}


//check if the char is a digit
int checkDigit(char c){
    char digits[] = {'0','1','2','3','4','5','6','7','8','9'};
    int n = sizeof(digits)/sizeof(digits[0]);
    for(int i =0; i < n; i++){
        if(digits[i] == c){
            return 1;
        }
    }
    return 0;
}

//check if the char is an initial
int checkInitial(char nextChar){
    if(checkLetter(nextChar) || nextChar == '!' || nextChar == '$' || nextChar == '%' || nextChar == '&' || nextChar == '*' || nextChar == '/' || nextChar == ':' || nextChar == '<' || nextChar == '=' || nextChar == '>' || nextChar == '?' || nextChar == '~' || nextChar == '_' || nextChar == '^'){
        return 1;
    }
    return 0;
}

//check if the char is subsequent
int checkSubsequent(char c){
    if(checkInitial(c) || checkDigit(c) || c == '.' || c == '+' || c =='-'){
        return 1;
    }
    return 0;
}

/*
Reads the output of stdin by each character. Determines the token type and stores the result
in a Value type linked list with the token and its corresponding value as a string

*/
Value *tokenize() {
    // Prepare list of tokens
    Value *tokensList = makeNull();

    // Prepare the character stream
    char nextChar;
    nextChar = (char)fgetc(stdin);
    char word[301];
    int checking = 0;
    int index = 0;
    int onBool = 0;
    int onInt = 0;
    int onDoub = 0;
    int onString = 0;
    int onSymb = 0;
    int inComment = 0;
    int found = 0;
    int decimal = 0;
    // Start tokenizing!
    while (nextChar != EOF) {
        
        if (nextChar == '(') {
            word[0] = '(';
            found = 1;
            index++;

        } else if (nextChar == ')') {
            word[0] = ')';
            found = 1;
            index++;

        } else {
            //If we are checking for the first character of each token
            char checkChar = (char)fgetc(stdin);
            ungetc(checkChar, stdin);
            if(!checking){
                //Checking for booleans
                if(nextChar == '#'){
                    checking = 1;
                    onBool = 1;
                    word[index] = nextChar;
                    index++;
                } else if((nextChar == '-' || nextChar == '+') && checkEndToken(checkChar)){
                    onSymb = 1;
                    found = 1;
                    word[0] = nextChar;
                    index++;
                } else if (nextChar == '-' || nextChar == '+' || nextChar == '.'|| checkDigit(nextChar)){ // checking for numbers 
                    
                    checking = 1;
                    if(nextChar != '.'){
                        onInt = 1;
                    } 
                    
                    onDoub = 1;
                    //printf("nextchar: %c checkChar: %c onInt %d", nextChar, checkChar, onInt);
                    if (checkDigit(nextChar) && checkEndToken(checkChar) && onInt == 1){
                        //printf("in\n");
                        checking = 0;
                        found = 1;
                        onDoub = 0;
                    }
                    //("found: %d\n", found);
                    word[index] = nextChar;
                    index++;

                } else if (nextChar == '\"'){ // checking for strings
                    checking = 1;
                    onString = 1;

                } else if (checkInitial(nextChar)){// checking for symbols
                    checking = 1;
                    onSymb = 1;
                    word[0] = nextChar;
                    index++;
                    if(checkEndToken(checkChar)){
                        checking = 0;
                        found = 1;
                    }
                } else if(nextChar == ' ' || nextChar == '\n'){ // check for whitespace
                    ;
                } else if(nextChar == ';'){ // check for comments
                    inComment = 1;
                    checking =1;
                } else{
                    printf("Syntax Error: Unrecognized syntax: %c , exiting\n", nextChar);
                    texit(0);
                }

            } 
            else{
                //Checking correct boolean syntax
                if(onBool){
                    if (nextChar == 'f' || nextChar == 't'){
                        checking = 0;
                        word[index] = nextChar;
                        char checkChar = (char)fgetc(stdin);
                        if(!checkEndToken(checkChar)){
                            printf("Syntax error (readBoolean): boolean was not #t or #f\n");
                            texit(0);
                        } 
                        ungetc(checkChar, stdin);
                        found = 1;
                        index ++;
                    } else{
                        printf("Syntax error (readBoolean): boolean was not #t or #f\n");
                        texit(0);
                    }
                } else if(onInt || onDoub){
                    //if . is found its a double at the end if int is still valid, it is not a double
                    char checkChar = (char)fgetc(stdin);
                    if((!checkDigit(nextChar) && nextChar != '.') || (onInt == 0 && nextChar == '.')){
                        printf("Syntax error: Invalid Syntax for type Number: %c , exiting\n", nextChar);
                        texit(0);
                    }
                    if(nextChar == '.'){
                        onInt = 0;
                        decimal = index;
                    }
                    if(checkEndToken(checkChar)){
                        found = 1;
                        if(onInt){
                            onDoub = 0;
                        }
                        checking = 0;
                    }
                    word[index] = nextChar;
                    ungetc(checkChar, stdin);
                    index++;
                    if(onDoub && (index - decimal) < 6 && found == 1){
                        for(int i = index; i < (decimal + 7); i++){
                            word[i] = '0';
                            index++;
                        }
                    }

                } else if(onString){
                    //make sure to check for comments
                    //char checkChar = (char)fgetc(stdin);
                    
                    
                    //ungetc(checkChar, stdin);
                    
                    if(nextChar == '\"'){
                        found = 1;
                        checking = 0;
                    } else{
                        word[index] = nextChar;
                        index++;
                    }
                    

                } else if(onSymb){

                    if (!checkSubsequent(nextChar)){
                        printf("Syntax error: Invalid Syntax for type Symbol: %c , exiting\n", nextChar);
                        texit(0);
                    }
                    char checkChar = (char)fgetc(stdin);
                    if(checkEndToken(checkChar)){
                        checking = 0;
                        found = 1;
                    }
                    word[index] = nextChar;
                    ungetc(checkChar, stdin);
                    index++;
                } else if(inComment){
                    if(nextChar == '\n'){
                        checking = 0;
                        inComment = 0;
                    }
                }
                
            }
            //check for other tokens
            //make sure string ignores comments

        }
        //printf("%d\n", index);
        if(found){
            //printf("%d\n", index);
            word[index]= '\0';
            //printf("%c\n", word[0]);
            //printf("%d\n", '\0' == word[1]);
            Value *node = talloc(sizeof(Value));
            if(!onInt && !onDoub){
                node->s = talloc((index+1)*sizeof(char));
                strcpy(node->s, word);
            } else if (onInt){
                char *ptr;
                node->i = strtol(word, &ptr, 10);

            } else{
                char *ptr;
                node->d = strtod(word, &ptr);
            }
            
            Value *next = tokensList;
            tokensList = cons(node, next);
            
            if(onBool){
                node->type = BOOL_TYPE;
                onBool =0;
            } else if(onInt){
                node->type = INT_TYPE;
                onInt =0;
            } else if(onDoub){
                node->type = DOUBLE_TYPE;
                onDoub =0;
            } else if(onString){
                node->type = STR_TYPE;
                onString =0;
            } else if(onSymb){
                node->type = SYMBOL_TYPE;
                onSymb =0;
            } else{
                if (word[0] == '('){
                    node->type = OPEN_TYPE;
                } else if(word[0] == ')'){
                    node->type = CLOSE_TYPE;
                }
                
            }
            index = 0;
            found = 0;
        }
        

        // Read next character
        nextChar = (char)fgetc(stdin);
    }

    // Reverse the tokens list, to put it back in order
    Value *reversedList = reverse(tokensList);
    //display(reversedList);
    return reversedList;
}



/*
Displays the entire list in a format of lexeme:category and a newline. Repeats this untill the current
Node the program is looking at is null. 
Takes in a Value, returns nothing.
*/
void displayTokens(Value *list){
    Value *curr = list;
    while(!isNull(curr)){
        switch (car(curr)->type) {
            case INT_TYPE:
                printf("%i:integer\n", car(curr)->i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double\n", car(curr)->d);
                break;
            case STR_TYPE:
                printf("\"%s\":string\n", car(curr)->s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", car(curr)->s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", car(curr)->s);
                break;
            case OPEN_TYPE:
                printf("(:open\n");
                break;
            case CLOSE_TYPE:
                printf("):close\n");
                break;
            default:
                break;
        }
        curr = cdr(curr);
    }

}