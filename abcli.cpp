#include "common.h"
#include "abcli.h"

STATUS abCLI::ExecuteAdd(){
    STATUS status=0;
    string n,t,a;
    std::cout<<"name: ";
    std::cin>>n;
    std::cout<<"mobile: ";
    std::cin>>t;
    std::cout<<"address: ";
    std::cin>>a;
    status=abbook->Add(n,t,a);
    return status;
}
STATUS abCLI::ExecuteSearch(){
    STATUS status=0;
    string tmp;
    int type;
    std::cout<<"by (name|mobile|address): ";
    std::cin>>tmp;
    if (tmp=="name")
        type=1;
    else if (tmp=="mobile")
        type=2;
    else if (tmp=="address")
        type=3;
    else type=0;
    switch (type)
    {
    case 1:
        std::cout<<"name: ";
        std::cin>>tmp;
        break;
    case 2:
        std::cout<<"mobile: ";
        std::cin>>tmp;
        break;
    case 3:
        std::cout<<"address: ";
        std::cin>>tmp;
        break;
    default:
        tmp="";
    }
    
    status=abbook->Search(tmp,type);
    
    return status;
}
STATUS abCLI::ExecuteRemove(){
    STATUS status=0;
    string tmp;
    int type;
    std::cout<<"by (name|mobile|address): ";
    std::cin>>tmp;
    if (tmp=="name")
        type=1;
    else if (tmp=="mobile")
        type=2;
    else if (tmp=="address")
        type=3;
    else type=0;
    switch (type)
    {
    case 1:
        std::cout<<"name: ";
        std::cin>>tmp;
        break;
    case 2:
        std::cout<<"mobile: ";
        std::cin>>tmp;
        break;
    case 3:
        std::cout<<"address: ";
        std::cin>>tmp;
        break;
    default:
        tmp="";
    }
    
    
    status=abbook->Remove(tmp,type);
    
    return status;
}
    
STATUS abCLI::ExtractCmd(){
    STATUS status=0;
    if (cmdline=="add")
        cmdcode=1;
    else if (cmdline=="search")
        cmdcode=2;
    else if (cmdline=="remove")
        cmdcode=3;
    else if (cmdline=="!quit")
        cmdcode=0;
    else if (cmdline=="!help")
        cmdcode=4;
    else{
        cmdcode=-1;
        status=CLI_ERROR;
    }
    return status;
}

STATUS abCLI::ExecuteCmd(){
    STATUS status=0;
    switch (cmdcode)
    {
    case 1:
        status=ExecuteAdd();
        break;
    case 2:
        status=ExecuteSearch();
        break;
    case 3:
        status=ExecuteRemove();
        break;
    case 4:
        PrintHelp();
        break;
    default:
        status=CLI_ERROR;
    }
    return status;
}


/*
STATUS abCLI::OpenPersistFile(){
    STATUS status=0;
    logfile.open(FILENAME);
    if(!logfile)
        status=1;
    return status;
}*/



//STATUS SavePersistFile();

    
void abCLI::Run(){
    STATUS status=0;
    //status=OpenPersistFile();
    ifstream ifile(FILENAME);
    if(ifile)
        abbook=new Book(ifile);
    else
        abbook=new Book;
    ifile.close();

    while(1)
    {
        std::cout<<"ab> ";
        std::cin>>cmdline;

        status=ExtractCmd();
        if(status!=0){
            ErrorProcess(status);
            continue;
        }
        if(cmdcode==0) break;
        status=ExecuteCmd();
        if(status!=0){
            ErrorProcess(status);
            continue;
        }

    }
    ofstream ofile(FILENAME);
    //status=OpenPersistFile();
    if(ofile){
        status=abbook->BookStore(ofile);
        if (status!=0) 
            ErrorProcess(status);
        ofile.close();
    }
    else
        ErrorProcess(BOOK_FILE_ERROR);

    std::cout<<"Bye!"<<endl;
}

void abCLI::ErrorProcess(STATUS status){
    switch(status){
    case INTERNAL_ERROR:
    case CLI_ERROR:
        cout<<"Warning: command is wrong. "<<endl;
        PrintHelp();
        break;
    case BOOK_ERROR:
    case BOOK_EMPTY:
        cout<<"Book is empty. "<<endl;
        break;
    case BOOK_ADD_ERROR:
        cout<<"Error: add operation cannot be executed."<<endl;
        break;
    case BOOK_SEARCH_ERROR:
        cout<<"Error: search operation cannot be executed."<<endl;
        break;
    case BOOK_REMOVE_ERROR:
        cout<<"Error: remove operation cannot be executed."<<endl;
        break;
    case BOOK_FILE_ERROR:
        cout<<"Error: log file cannot be open or file name is wrong."<<endl;
        break;
    case BOOK_ATTRIBUTE_ERROR:
        cout<<"Error: please notice the format of input."<<endl;
        PrintHelp();
        break;
    default:
        break;
    }

}

void abCLI::PrintHelp(){
    cout<<endl<<"--------------------HELP: CLI: ab---------------------"<<endl;
    cout<<"A command line application developed in C++ programming language."<<endl;
    cout<<"This application manages addresses by adding, searching and removing address entries."<<endl<<endl;
    cout<<"Data structure is like below"<<endl;
    cout<<"name:"<<endl;
    cout<<"mobile:"<<endl;
    cout<<"address:"<<endl<<endl;
    cout<<"Notice: mobile name must be a integer with size larger than 10."<<endl;
    cout<<"Notice: name and home address cannot be blank. "<<endl;
    cout<<"Command accepted: <add>, <search>, <remove>."<<endl;
    cout<<"If you want to quit, use <!quit> command."<<endl;
    cout<<"If you want to view the text again, use <!help> command."<<endl;
    cout<<"Address Book will be stored in log file with filename:<stored_book.log>."<<endl;
    cout<<"------------------------------------------------------------"<<endl;

}