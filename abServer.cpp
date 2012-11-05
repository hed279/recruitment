#include "common.h"
#include "abServer.h"





/*void sigchld_handler(int s){
while(waitpid(-1,NULL,WNOHANG)>0);
}
*/
abServer::abServer(){

    InitialSocket();

}


/*STATUS abServer::RecMsg(){

while( 1 ) {   
socklen_t sin_size = sizeof(struct sockaddr_in);  
if(( accept_fd = accept(sockfd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
{  
//throw "Accept error!";  
continue;  
}  
printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  

//if( !fork() ) {  
char buffer[MAXSIZE];  
memset(buffer,0,MAXSIZE);  
if( ( read(accept_fd,buffer,MAXSIZE)) < 0 ) {  
// throw("Read() error!");  
} else {  
Decode(buffer);
printf("Received message: %s\n",buffer);  
break;  
}  
// exit(0);                  
//}                  
close(accept_fd);      
}  
return 0;  


}
//STATUS abServer::SendMsg(){}
*/
struct result abServer::ExecuteAdd(struct command cmd){    
    struct result res; 
    string n,t,a;
    n=cmd.name;
    t=cmd.tel;
    a=cmd.addr;
    res=abbook->Add(n,t,a);  
    return res;
}
struct result  abServer::ExecuteSearch(struct command cmd){   
    struct result res;
    string tmp;
    switch (cmd.type)    { 
    case 1:   
        tmp=cmd.name;
        break;  
    case 2:  
        tmp=cmd.tel;  
        break;  
    case 3:   
        tmp=cmd.addr;   
        break;   
    default:    
        tmp="";   
    }       
    res=abbook->Search(tmp,cmd.type);  
    return res;
}
struct result abServer::ExecuteRemove(struct command cmd){  
    struct result res;
    string tmp;
    switch (cmd.type)    { 
    case 1:   
        tmp=cmd.name;
        break;  
    case 2:  
        tmp=cmd.tel;  
        break;  
    case 3:   
        tmp=cmd.addr;   
        break;   
    default:    
        tmp="";   
    }          
    res=abbook->Remove(tmp,cmd.type);    
    return res;}   
struct result abServer::ExecuteCmd(struct command cmd){  
    struct result res; 
    switch (cmd.cmdcode)    {
    case 1:     
        res=ExecuteAdd(cmd);   
        res.cmdcode=1;
        break; 
    case 2:    
        res=ExecuteSearch(cmd);    
        res.cmdcode=2;
        break;   
    case 3:   
        res=ExecuteRemove(cmd);   
        res.cmdcode=3;
        break;  
    default:     
        break;    
    }    
    return res;
}

STATUS abServer::InitialSocket(){
    STATUS status=0;	

    if(( sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){  
        //throw "socket() failed";  
    }  
    memset(&tcpserver,0,sizeof(tcpserver));  
    tcpserver.sin_family = AF_INET;  
    tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);  
    tcpserver.sin_port = htons(PORT);  
    /*
    struct addrinfo hints, *servinfo, *p;

    int rv;
    int yes=1;

    char s[INET6_ADDRSTRLEN];

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    rv=getaddrinfo(socketaddr,PORT,&hints,&servinfo);
    */


    if(bind(sockfd,(sockaddr*) &tcpserver,sizeof(tcpserver)) < 0 ){
        close(sockfd);
    }

    if( listen(sockfd,10) < 0 ) {  
        printf( "listen() failed");  
    }  



    return status;


}
void abServer::Run(){   
    STATUS status=0;    
    struct command cmd;
    struct result res;
    // struct sigaction sa;

    ifstream ifile(FILENAME);   
    if(ifile)       
        abbook=new Book(ifile);  
    else      
        abbook=new Book;   
    ifile.close(); 

    /*
    if (listen(sockfd,BACKLOG)==-1)
    return;

    sa.sa_handler=sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD,&sa,NULL)==-1)
    return;

    */
    int accept_fd;

    while(1)    {    
        /*accept*/
        socklen_t sin_size = sizeof(struct sockaddr_in);  
        if(( accept_fd = accept(sockfd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
        {  
            //throw "Accept error!";  
            continue;  
        }  
        printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  

        char buffer[MAXDATASIZE], buffer_back[20];  
        memset(buffer,0,MAXDATASIZE);  
        if(  recv(accept_fd,buffer,MAXDATASIZE-1,0) < 0 ) {  
            // throw("Read() error!");  
            printf("recv error!");
        }
        else { 

            printf("Received msg.\n"); 
            cmd=Decode(buffer);
            res=ExecuteCmd(cmd);
            Encode(buffer_back,res);
            printf("Ready to send msg.\n ");
            send(accept_fd,buffer_back,20,0);
            if(res.cmdcode==2){
                Entry* ptr=res.head;
                for(int i=0;i<res.num;i++){
                    memset(buffer,0,MAXDATASIZE); 
                    ptr->Encode(buffer);
                    ptr=ptr->next;
                    //printf("%s,%s,%s",&buffer[4],&buffer[25],&buffer[36]);
                    send(accept_fd,buffer,MAXDATASIZE,0);
                }
            }
            printf("Send message done. \n");  
            //    break;  
        }  



        if(res.cmdcode!=2){
            ofstream ofile(FILENAME);    //status=OpenPersistFile();   
            if(ofile){      
                status=abbook->BookStore(ofile);  
                if (status!=0)     
                    ErrorProcess(status);  
                ofile.close();   
            }   
            else     
                ErrorProcess(BOOK_FILE_ERROR); 

            close(accept_fd);   
        }   

    }
    close(sockfd);


    std::cout<<"Bye!"<<endl;
}
void abServer::ErrorProcess(STATUS status){   
    switch(status){   
    case INTERNAL_ERROR:  
    case CLI_ERROR:    
        cout<<"Warning: command is wrong. "<<endl;   
        //PrintHelp();     
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
        //PrintHelp();     

        break; 
    default:     
        break;   
    }
}

void abServer::Encode(char* buf, struct result res){
    buf[0]=res.cmdcode;
    int *tmp=(int*)&buf[4];
    *tmp=res.num;


}
struct command abServer::Decode(char* buf){
    struct command cmd;
    cmd.cmdcode=buf[0];
    cmd.type=buf[2];
    strcpy(cmd.name,&buf[4]);
    strcpy(cmd.tel,&buf[25]);
    strcpy(cmd.addr,&buf[36]);
    return cmd;
}
