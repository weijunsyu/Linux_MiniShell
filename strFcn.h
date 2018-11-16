#ifndef STRFCN_H
#define STRFCN_H

#include <stdbool.h>

/* Takes in some string and correctly allocates memory to create some copy of
said string. Returns dynamic memory. */
extern char* allocStr(char* source);

/* Given some dynamic string strToChange resize it given newSize. */
extern void reSizeStr(char** strToChange, int newSize);

/* Copies the values of size bytes from the location pointed to by source to
a new cstring pointed to by the return value. Returns dynamic memory. */
extern char* strMemCpy(char* source, int size);

/* Finds the number of strings found from source given some char delimiter
and stores that value in numStr as well as the size of the longest string of
which is stored in maxStrSize. */
extern void getNumStr(char* source, char delimiter, int* numStr,
                      int* maxStrSize);

/* Checks if cstring strToTest contains any of the characters listed in
charList (where sizeList is the size of the list) and returns the index where
the first match is found in strToTest or -1 if no match. */
extern int sharesFromList(char* strToTest, char* charList, int sizeList);

/* Checks if cstring strToTest shares any characters with otherStr. Returns
the index where the first match is found in strToTest or -1 if no match. */
extern int sharesFromStr(char* strToTest, char* otherStr);

/* Checks if the char charToTest is an element of charList (where sizeList
is the size of the list) returning true if yes, false otherwise. */
extern bool isInList(char charToTest, char* charList, int sizeList);

/* Checks if the char charToTest is found in the cstring string. Return true
if yes, false otherwise. */
extern bool isInStr(char charToTest, char* string);

/* Split a string given some delimiter list delimiters (if any char in
delimiters is matched, source string will be split there) and return an array
containing the indices of where each delimiter character is within source
strings as well as the length of each substring. The first entry in the array
is the size of the array with each proceeding entry being of the format: length
of substring then index of delimiter in source string with the last substring
NOT having an entry for location of delimiter. Then modifies source string
such that all instances of delimiter are changed to the '\0' char. If function
fails allocating memory, will return NULL. (Number of substrings is half the
size of array.) Returns dynamic memory.
    eg. source: "This is-a test_string." <- in mem last char (after '.') is '\0'
        delimiters: ' _-'
        strSplitInt(source, delimieters);
        return: 10, 4, 4, 2, 7, 1, 9, 4, 14, 7
        source: "This\0is\0a\0test\0string.\0" <- last \0 was not added
                                                  but already existed prior */
extern int* strSplitInt(char* source, char* delimiters);

/* Split a string given some delimiter list delimiters and return an array of
pointers to each substring within source string. Modifies source string such
that all instances of delimiter are changed to the '\0' char. If function fails
allocating memory, will return NULL. numSubStr is modified to the number of
substrings (size of return array). Returns dynamic memory.
NOTE: Pointers in return array point to locations in source string, modifying
source string after the fact will affect the returned array. */
extern char** strSplitPtr(char* source, char* delimiters, int* numSubStr);

/* Parse over source string splitting it up given delimiters and store
arguments in array of which is NULL terminating. Store size of useful array in
numStr (number of non-NULL args). Mutates source string turning all instances
of delimiters into null char (\0). */
extern void parse(char** array, char* source, char* delimiters, int* numStr);

#endif
