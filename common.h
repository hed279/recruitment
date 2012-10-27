#ifndef COMMON_H
#define COMMON_H

#include <string>
//#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;


//ERROR_CODE
#define INTERNAL_ERROR 1
#define CLI_ERROR 2
#define BOOK_ERROR 10
#define BOOK_ADD_ERROR 11
#define BOOK_SEARCH_ERROR 12
#define BOOK_REMOVE_ERROR 13
#define BOOK_FILE_ERROR 14
#define BOOK_ATTRIBUTE_ERROR 15
#define BOOK_EMPTY 21

//Store file name
#define FILENAME "stored_book.log"

typedef int STATUS;

#endif
