#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strFcn.h"

char* allocStr(char* source) {
    char* ret = malloc(strlen(source) + 1); //allocate mem for string
    strcpy(ret, source); //copy old string source to new string ret
    return ret;
}

void reSizeStr(char** strToChange, int newSize) {
    *strToChange = realloc(*strToChange, newSize);
}

char* strMemCpy(char* source, int size) {
    char* ret = malloc(size + 1); //allocate memory for string
    memcpy(ret, source, size); //copy block of memory
    ret[size] = '\0'; //add '\0' at end of string to make it a true cstring
    return ret;
}

void getNumStr(char* source, char delimiter, int* numStr, int* maxStrSize) {
    *numStr = 1; //set to 1, both clearing old value or init var if not init
    *maxStrSize = 0; //hold value of len of longest str in group
    int counter = 0; //counter for how many characters in string
    char* current = source; //pointer to current char in source string
    while (*current != '\0') { //while not at end of source string
        counter++;
        if (*current == delimiter) {
            (*numStr)++; //increase value by 1
            //if current string is longer than previous max string
            if (*maxStrSize < counter) {
                //update value (counter-1 as we dont count the delimiter)
                *maxStrSize = counter-1;
            }
            counter = 0; //clear counter for next string
        }
        current++; //move to next char in source string (element)
    }
}


int sharesFromList(char* strToTest, char* charList, int sizeList) {
    int i;
    //iterate throught each char in strToTest
    for (i = 0; i < strlen(strToTest); i++) {
        int j;
        //for each char in strToTest check with each element in charlist
        for (j = 0; j < sizeList; j++) {
            if (strToTest[i] == charList[j]) { //if any match return
                return i; //return index of first match
            }
        }
    }
    return -1; //return -1 if no matches in entire string
}

int sharesFromStr(char* strToTest, char* otherStr) {
    int i;
    //iterate throught each char in strToTest
    for (i = 0; i < strlen(strToTest); i++) {
        int j;
        //for each char in strToTest check with each char in otherStr
        for (j = 0; j < strlen(otherStr); j++) {
            if (strToTest[i] == otherStr[j]) { //if any match return
                return i; //return index of first match
            }
        }
    }
    return -1; //return -1 if no matches in entire string
}

bool isInList(char charToTest, char* charList, int sizeList) {
    int i;
    for (i = 0; i < sizeList; i++) { //iterate over each char in charList
        if (charToTest == charList[i]) { //if charToTest is in charList:
            return true;
        }
    }
    return false;
}

bool isInStr(char charToTest, char* string) {
    int i;
    for (i = 0; i < strlen(string); i++) { //iterate over each char in string
        if (charToTest == string[i]) { //if charToTest is in charList:
            return true;
        }
    }
    return false;
}
//right now only checks delimiters by char needs to work when delim is in consectutive order
int* strSplitInt(char* source, char* delimiters) {
    char* current = source; //pointer to source
    int size = 2;
    int* ret = malloc(2 * sizeof(*ret)); //min. mem alloc for 1 substring
    int counter = 0; //counter for how many characters in substring
    int indexS = 0; //current index into source string
    int indexR = 1; //current index into ret
    while (*current != '\0') { //while not at end of source string
        counter++;
        if (isInStr(*current, delimiters)) {
            size += 2;
            *current = '\0'; //replace old char (delimiter) with null char (\0)

            //increase size of array
            int* tmpPtr = realloc(ret, size * sizeof(*ret));
            if (tmpPtr == NULL) { //check if allocation successful
                //realloc failed:
                return NULL;
            }
            //successful realloc:
            ret = tmpPtr;

            ret[indexR++] = counter-1; //add length of string to ret
            ret[indexR++] = indexS; //add index of delimiter to ret
            counter = 0; //reset counter
        }
        indexS++; //increment index
        current++; //move to next char in source string (element)
    }
    ret[indexR] = counter; //add last substring's length to ret
    ret [0] = size; //add the size of the ret to the first entry

    return ret;
}

char** strSplitPtr(char* source, char* delimiters, int* numSubStr) {
    //mutate source and get indices into source for substrings
    int* splitIndices = strSplitInt(source, delimiters);
    *numSubStr = splitIndices[0] / 2; //get number of substrings
    char** ret = malloc(*numSubStr * sizeof(*ret)); //alloc memory
    int i;
    int j = 1;
    ret[0] = source; //point first entry into ret to first substring
    for (i = 2; i < splitIndices[0]; i += 2) {
        ret[j] = &source[splitIndices[i]+1]; //all other substrings
        j++;
    }
    free(splitIndices);
    return ret;
}

void parse(char** array, char* source, char* delimiters, int* numStr) {
    char* current = source;
    array[0] = current; //set first string ptr to source
    *numStr = 1; //init / reset numStr
    while (*current != '\0') {
        if (isInStr(*current, delimiters)) { //if char is a delim
            *current = '\0';
            if (!isInStr(*(current + 1), delimiters) && *(current + 1) != '\0') {//next char is NOT a delim

                //change size of array
                //*array = realloc(*array, ((*numStr) + 2) * sizeof(*array));

                array[(*numStr)++] = ++current;
            }
            else {//next char still delim or is last char in string
                current++;
            }
        }
        else { //char is in the middle of a string
            current++;
        }
    }
    array[*numStr] = NULL; //set the last pointer to NULL to mark end of array
}
