#include<iostream>
#include <vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
//#include "merge-sort.h"


#define PORT 3000
#define BUFF_SIZE 1024

void handleclient()
{
    
}

void start()
{

}

int main(){
    sockaddr_in serv_addr;
    int listenfd = 0, connfd = 0;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[BUFF_SIZE];
    ssize_t bytes;

    if(listenfd == -1)
    {
        std::cerr<<"Cannot create socket!\n";
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

    if (bind(listenfd,(sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr<<"Error binding!";
        return - 1;
    }
    if (listen(listenfd, 10) < 0)
    {
        std::cerr<<"Listen error!";
        return -1;
    };
    std::cout<<"Started listening on port: "<<PORT<<std::endl;
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    while(1)
	{
        
		bytes = read(connfd, buffer, sizeof(buffer) - 1);
        if(bytes > 0)
        {
            buffer[bytes] ='\0';
            std::cout<<"Recieved from client:\n";
            std::cout<<buffer;
        }
        else if (bytes == 0)
        {
            std::cout<<"CLient closed connection!\n";
            break;
        }
        else{
            std::cerr<<"Error!\n";
            break;
        }
		sleep(1);
	}
    close(connfd);
}