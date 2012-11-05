


#ifndef ABSERVER_H
#define ABSERVER_H
#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include "book.h"



class abServer{  //CLI interface 
private:    

    char *socketaddr;
    int sockfd;
    sockaddr_in tcpserver;  
    sockaddr_in remote_addr;  

    Book* abbook; //Address Book    

    //struct command cmd;

    struct result ExecuteAdd(struct command cmd);     
    struct result ExecuteSearch(struct command cmd);    
    struct result ExecuteRemove(struct command cmd);   
    struct result ExecuteCmd(struct command cmd); //Execute command    

    STATUS InitialSocket();
    // STATUS SendCmd();
    // STATUS RecMsg();
    void Encode(char* buf, struct result res);
    struct command Decode(char* buf);

    void ErrorProcess(STATUS status);
public:    
    abServer();
    abServer(char* input){
        socketaddr=input;

    }
    ~abServer(){
        delete abbook;
    }
    void Run(); //Main loop 
};

#endif


