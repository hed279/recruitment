


#ifndef ABCLIENT_H
#define ABCLIENT_H
#include "book.h"
#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <errno.h>  


class abClient{  //CLI interface 
private:    
    string cmdline; //store last cmdline    
    int cmdcode;//0-quit,1-add,2-search,3-remove,4-help    
    char * socketaddr;
    int sockfd;
    char* server_ip;
    char* server_port;
    struct sockaddr_in server_addr;  
    struct command cmd;

    //char buf[MAXDATASIZE];

    STATUS ExtractCmd(); //Extract command code from cmdline    
    void ResultProcess();
    STATUS ExecuteAdd();     
    STATUS ExecuteSearch();    
    STATUS ExecuteRemove();   
    STATUS ExecuteCmd(); //Execute command    
    void ErrorProcess(STATUS status); //Error process function    
    void PrintHelp(); //Print help text
    STATUS InitialSocket(char* server_ip,char* server_port);
    STATUS SendCmd();
    STATUS RecMsg();
    STATUS Encode(char* buf, struct command cmd);
    STATUS Decode(char* buf){}
    void Print(char* buf);
public:    
    abClient(){}
    abClient(char* ip,char* port){
        // socketaddr=server_ip;
        server_ip=ip;
        server_port=port;

    }
    ~abClient(){
        //close(sockfd);
    }
    void Run(); //Main loop 
};

#endif

