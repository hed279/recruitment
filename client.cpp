#include "common.h"
#include "abClient.h"
#include "book.h"

int main(int argc, char *argv[]){
    if(argc!=2){
        cout<<"command must like: abclient IP"<<endl;
        return -1;
    }
    abClient client(argv[1],"8888");
    client.Run();
    return 1;
}


