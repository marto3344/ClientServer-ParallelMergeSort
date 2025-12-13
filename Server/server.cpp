#include<iostream>
#include <vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "server.hpp"

Server::Server() : server_fd(-1), tpool(TPOOL_SIZE) {}

Server::~Server()
{
    if(server_fd != -1)
    {
        close(server_fd);
    }
}

int Server::CreateSocket()
{
   sockaddr_in serv_addr;
   int sockfd = -1;
   sockfd = socket(AF_INET,SOCK_STREAM, 0);
   if (sockfd == -1)
   {
      perror("[Server] Failed to create a socket!");
      return -1;
   }
   
   if(SetReuseAddr(sockfd) == -1)
   {
      perror("[Server] Warning: Failed to set SO_REUSEADDR");
   }


   memset(&serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("[Server] Failed to bind!");
        close(sockfd);
        return -1;
    }
    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("[SERVER] Failed to listen!");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int Server::SetReuseAddr(int sockfd)
{
    int opt = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return res;
}

int Server::Initialize()
{
    server_fd = CreateSocket();
    if (server_fd == -1) {
            return -1;
    }

    using namespace std;
    cout<<"\n╔════════════════════════════════════════════════════════╗\n";
    cout<<"║     PARALLEL MERGE SORT SERVER                       ║\n" ;
    cout<<"╚════════════════════════════════════════════════════════╝\n";
    cout<<"Server started successfully on port:" << PORT <<'\n';    
    cout<<"Worker threads count: "<<TPOOL_SIZE;
    return 0;
}

void Server::Run()
{
    if(server_fd == -1)
    {
        std::cerr<<"[Server] Invlaid socket file descriptor!. Please use Initialize() before Run()!\n";
        return;
    }

    while (1)
    {
        //TODO: Selector logic
    }
    
}
