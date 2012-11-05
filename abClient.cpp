#include "common.h"
#include "abClient.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




STATUS abClient::ExtractCmd(){ 
    STATUS status=0;  
    cmd.name[0]='\0';
    cmd.tel[0]='\0';
    cmd.addr[0]='\0';
    if (cmdline=="add")   
        cmd.cmdcode=1;  
    else if (cmdline=="search")     
        cmd.cmdcode=2;   
    else if (cmdline=="remove")     
        cmd.cmdcode=3;   
    else if (cmdline=="!quit")   
        cmd.cmdcode=0; 
    else if (cmdline=="!help")    
        cmd.cmdcode=4; 
    else{    
        cmd.cmdcode=-1;    
        status=CLI_ERROR;  
    }    
    return status;
}


STATUS abClient::InitialSocket(char* server_ip,char* server_port){
    STATUS status=0;	
    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {  
        printf("create socket error: %s(errno:%d)\n)",strerror(errno),errno);  
        exit(0);  
    }  

    memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(atoi(server_port));  

    if( inet_pton(AF_INET,server_ip,&server_addr.sin_addr) <=0 ) {  
        printf("inet_pton error for %s\n",server_ip);  
    }

    /*
    struct addrinfo hints, *servinfo, *p;

    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;00:00

    rv=getaddrinfo(socketaddr,PORT,&hints,&servinfo);

    for(p=servinfo;p!=NULL;p=p->ai_next){
    if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
    continue;
    }
    if(connect(sockfd,p->ai_addr,p->ai_addrlen) 																																																																																																																																																																																															 ==-1){
    close(sockfd);
    continue;
    }
    break;
    }
    freeaddrinfo(servinfo);*/
    return status;

}
STATUS abClient::SendCmd(){

    char buf[MAXDATASIZE];
    memset(buf,0,MAXDATASIZE);  
    Encode(buf,cmd);

    InitialSocket(server_ip,server_port);
    if( connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0) {  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  

    printf("send message to server: \n"); 


    if( send( sockfd,buf,MAXDATASIZE,0 ) < 0 ) {  
        printf("send message error\n");  
        exit(0);  
    }  


    return 1;
}

STATUS abClient::Encode(char* buf, struct command cmd){
    STATUS status=0;
    buf[0]=cmd.cmdcode;
    buf[2]=cmd.type;
    strcpy(&buf[4],cmd.name);
    strcpy(&buf[5+MAXNAME],cmd.tel);
    strcpy(&buf[6+MAXNAME+MAXMOBILE],cmd.addr);


    return status;
}
/*STATUS abClient::Decode(char* buf, int &leftnum){
STATUS status=0;
char type=buf[0];


if(type==0){
leftnum=(int)buf[2];
}
if(type==1){
strcpy(name,&buf[4]);
strcpy(tel,&buf[MAXNAME+4]);
strcpy(addr,&buf[MAXNAME+MAXMOBILE+4]);
}
return status;
}*/

STATUS abClient::ExecuteCmd(){  
    STATUS status=0; 
    switch (cmd.cmdcode)    {
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


STATUS abClient::ExecuteAdd(){    
    STATUS status=0;    
    string n,t,a;    
    std::cout<<"name: ";    
    std::cin>>n;  
    std::cout<<"mobile: ";   
    std::cin>>t;  
    std::cout<<"address: ";   
    std::cin>>a;  
    if(n.size()>MAXNAME || t.size()>MAXMOBILE || a.size()>MAXADDRESS)
        return ERROR_SIZE;
    if(t.size()!=10)
        return ERROR_SIZE;

    cmd.type=0;
    strcpy(cmd.name,n.c_str());
    strcpy(cmd.tel,t.c_str());
    strcpy(cmd.addr,a.c_str());

    //status=abbook->Add(n,t,a);  
    return status;
}


STATUS abClient::ExecuteSearch(){   
    STATUS status=0;   
    string tmp;   

    std::cout<<"by (name|mobile|address): "; 
    std::cin>>tmp;    
    if (tmp=="name")    
        cmd.type=1;    
    else if (tmp=="mobile")    
        cmd.type=2;   
    else if (tmp=="address")     
        cmd.type=3;    
    else 
        return ERROR_FORMAT; 


    switch (cmd.type)    { 
    case 1:   
        std::cout<<"name: ";      
        std::cin>>tmp; 
        if(tmp.size()>MAXNAME )
            return ERROR_SIZE;
        else
            strcpy(cmd.name,tmp.c_str());
        break;  
    case 2:  
        std::cout<<"mobile: ";    
        std::cin>>tmp;  
        if(tmp.size()>MAXMOBILE )
            return ERROR_SIZE;
        else
            strcpy(cmd.tel,tmp.c_str());
        break;  
    case 3:   
        std::cout<<"address: ";   
        std::cin>>tmp; 
        if(tmp.size()>MAXADDRESS )
            return ERROR_SIZE;
        else
            strcpy(cmd.addr,tmp.c_str());
        break;   
    default:    
        tmp="";   
    }


    //status=abbook->Search(tmp,type);  
    return status;
}
STATUS abClient::ExecuteRemove(){  
    STATUS status=0;  
    string tmp;  

    std::cout<<"by (name|mobile|address): ";  
    std::cin>>tmp;   
    if (tmp=="name")   
        cmd.type=1;  
    else if (tmp=="mobile")   
        cmd.type=2;  
    else if (tmp=="address")   
        cmd.type=3;   
    else 
        return ERROR_FORMAT;  


    switch (cmd.type)    { 
    case 1:   
        std::cout<<"name: ";      
        std::cin>>tmp; 
        if(tmp.size()>MAXNAME )
            return ERROR_SIZE;
        else
            strcpy(cmd.name,tmp.c_str());
        break;  
    case 2:  
        std::cout<<"mobile: ";    
        std::cin>>tmp;  
        if(tmp.size()>MAXMOBILE )
            return ERROR_SIZE;
        else
            strcpy(cmd.tel,tmp.c_str());
        break;  
    case 3:   
        std::cout<<"address: ";   
        std::cin>>tmp; 
        if(tmp.size()>MAXADDRESS )
            return ERROR_SIZE;
        else
            strcpy(cmd.addr,tmp.c_str());
        break;   
    default:    
        tmp="";   
    }

    //status=abbook->Remove(tmp,type);    
    return status;
}   


STATUS abClient::RecMsg(){

    char buf[MAXDATASIZE];
    memset(buf,0,MAXDATASIZE); 
    int numbytes,cnt;
    cnt=0;
    numbytes=recv(sockfd,buf,20,0);
    char code=buf[0];
    int *tmp=(int*)&buf[4];
    cnt=*tmp;
    cout<<cnt<<" entries"<<endl;

    if(code==2)
        for(int i=0;i<cnt;++i){
            memset(buf,0,MAXDATASIZE); 
            if((numbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1)
                printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
            // cout<<numbytes;
            Print(buf);
        }
        close(sockfd);
}


void abClient::Print(char* buf){
    printf("\nname: %s \nmobile: %s \naddress: %s\n",(char*)&buf[4],(char*)&buf[MAXNAME+5],(char*)&buf[MAXNAME+MAXMOBILE+6]);

    // cout<<endl<<"name: "<<(char*)&buf[4]<<endl<<"mobile: "<<(char*)&buf[MAXNAME+1]<<endl<<"address: "<<(char*)&buf[MAXNAME+MAXMOBILE+2]<<endl;


}


void abClient::Run(){   
    STATUS status=0;    


    while(1)    {    
        std::cout<<"ab> ";      
        std::cin>>cmdline;    
        status=ExtractCmd();    
        if(status!=0){          
            ErrorProcess(status);      
            continue;     
        }      
        if(cmd.cmdcode==0) break;   
        status=ExecuteCmd();   
        if(status!=0){     
            ErrorProcess(status);     
            continue;      
        } 
        status=SendCmd();
        status=RecMsg();
    }   
    std::cout<<"Bye!"<<endl;
}
void abClient::ErrorProcess(STATUS status){   
    switch(status){   
    case INTERNAL_ERROR:  
    case CLI_ERROR:    
        cout<<"Warning: command is wrong. "<<endl;   
        PrintHelp();     
        break;  
    case ERROR_SIZE:
        cout<<"Note: name is no longer than "<<MAXNAME<<endl;
        cout<<"tel must as long as "<<MAXMOBILE<<endl;
        cout<<"address is no longer than "<<MAXADDRESS<<endl;
        break;
    case BOOK_ERROR:  
    case BOOK_EMPTY:   
        cout<<"Book is empty. "<<endl;     
        break;  
    case BOOK_ADD_ERROR:     
        cout<<"Error: add operation cannot be executed."<<endl;        break;  
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

void abClient::PrintHelp(){   
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

