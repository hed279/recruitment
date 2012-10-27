#ifndef ABCLI_H
#define ABCLI_H

#include "book.h"



class abCLI{  //CLI interface
  private:
    string cmdline; //store last cmdline
    int cmdcode;//0-quit,1-add,2-search,3-remove,4-help
    Book* abbook; //Address Book
    STATUS ExecuteAdd(); 
    STATUS ExecuteSearch();
    STATUS ExecuteRemove();
    STATUS ExtractCmd(); //Extract command code from cmdline
    STATUS ExecuteCmd(); //Execute command
    void ErrorProcess(STATUS status); //Error process function
    void PrintHelp(); //Print help text

  public:
    abCLI(){abbook=NULL;}

    void Run(); //Main loop 

};


#endif
