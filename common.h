#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
//#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;


//ERROR_CODE
#define INTERNAL_ERROR 1
#define CLI_ERROR 2
#define ERROR_SIZE 3
#define ERROR_FORMAT 4

#define BOOK_ERROR 10
#define BOOK_ADD_ERROR 11
#define BOOK_SEARCH_ERROR 12
#define BOOK_REMOVE_ERROR 13
#define BOOK_FILE_ERROR 14
#define BOOK_ATTRIBUTE_ERROR 15
#define BOOK_EMPTY 21
//Store file name
#define FILENAME "stored_book.log"

#define MAXNAME 20
#define MAXMOBILE 10
#define MAXADDRESS 200
#define MAXDATASIZE 300

typedef int STATUS;

#define PORT 8888
#define BACKLOG 10
struct command{
    char cmdcode;
    char type;
    char name[MAXNAME+1];
    char tel[MAXMOBILE+1];
    char addr[MAXADDRESS+1];
};


#endif
