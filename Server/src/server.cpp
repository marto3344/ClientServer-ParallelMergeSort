#include<iostream>
#include <vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include <netinet/in.h>
#include <cstring>
#include "server.h"
#include"mergesort.h"
#include <arpa/inet.h>

Server::Server() : server_fd(-1), epoll_fd(-1), tpool(TPOOL_SIZE) {}

Server::~Server()
{
    if(server_fd != -1)
    {
        close(server_fd);
    }
    if(epoll_fd != -1)
    {
        close(epoll_fd);
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

int Server::SetClientTimeout(int client_fd, int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("[Server] Failed to set recv timeout");
        return -1;
    }
    if (setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("[Server] Failed to set send timeout");
        return -1;
    }
    return 0;
}

void Server::HandleClient(int client_fd)
{
    size_t arrsize, n_threads;
    ssize_t bytes;

    bytes = read(client_fd,&n_threads, sizeof(n_threads));
    std::cout<<"Recieved number of threads!";
    if(bytes <= 0)
    {
        perror("[Worker][HandleClient] Can't read the number of threads!");
        return;
    }
    bytes = read(client_fd, &arrsize, sizeof(arrsize));
    if(bytes <= 0)
    {
        perror("[Worker][HandleClient] Can't read the size of the array!");
        return;
    }

    std::vector<int> arr (arrsize);
    if(RecieveArray(arr, client_fd) == -1)
    {
        close(client_fd);
        return;
    }
    ConvertToHostByte(arr);
    ParallelMergeSort(arr,n_threads);
    ConvertToNetworkByte(arr);
    if (SendArray(arr, client_fd) == -1)
    {
        close(client_fd);
    }
}

int Server::RecieveArray(std::vector<int> &arr, int client_fd)
{
    size_t total_bytes, bytes_received;
    total_bytes = arr.size()*sizeof(int);

    bytes_received = recv(client_fd, reinterpret_cast<char *>(arr.data()), total_bytes, MSG_WAITALL);
    if (bytes_received != total_bytes)
    {
        std::cerr << "[Worker][RecieveArray] Error! Expected " << total_bytes 
        << " bytes but received only " << bytes_received << '\n';
        return -1;
    }
    return 0;
}

int Server::SendArray(std::vector<int> &arr, int client_fd)
{
    size_t total_bytes, bytes_sent;
    total_bytes = arr.size()*sizeof(int);
    bytes_sent = 0;
    const char* arr_ptr = reinterpret_cast<const char *>(arr.data());
    while (bytes_sent < total_bytes)
    {
        ssize_t bytes = send(
            client_fd,
            arr_ptr + bytes_sent,
            total_bytes - bytes_sent,
            MSG_NOSIGNAL
        ); 
        if(bytes > 0)
            bytes_sent += static_cast<size_t>(bytes);
        else if(bytes == -1 && errno != EINTR)
        {
            perror("[Worker][SendArray] Fatal error!");
            return -1;
        }
    } 
    return 0;
}

void Server::ConvertToHostByte(std::vector<int> &arr)
{
     for (int& val : arr) 
        val = ntohl(val);
}

void Server::ConvertToNetworkByte(std::vector<int> &arr)
{
    for(int& val : arr)
        val = htonl(val);
}

int Server::Initialize()
{
    server_fd = CreateSocket();
    if (server_fd == -1) {
            return -1;
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("[Server] Epoll create error");
        return -1;
    }
    using namespace std;
    cout<<"╔════════════════════════════════════════════════════════╗\n";
    cout<<"║               PARALLEL MERGE SORT SERVER               ║\n" ;
    cout<<"╚════════════════════════════════════════════════════════╝\n";
    cout<<"Server started successfully on port:" << PORT <<'\n';
    return 0;
}

void Server::Run()
{
    if(server_fd == -1)
    {
        std::cerr<<"[Server] Invlaid socket file descriptor!. Please use Initialize() before Run()!\n";
        return;
    }
    if(epoll_fd == -1)
    {
        std::cerr<<"[Server] Invlaid epoll file descriptor!. Please use Initialize() before Run()!\n";
        return;
    }

    const int MAX_EVENTS = 5;
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&event) == -1)
    {
        perror("[Server] Epoll registration failed!");
        return;
    }
    epoll_event events_arr [MAX_EVENTS];
    while (1)
    {
        int n = epoll_wait(epoll_fd,events_arr, MAX_EVENTS, -1);
        if(n == -1)
        {
            perror("[Server] Epoll wait failiure!");
            return;
        }
        for (int i = 0; i < n; i++)
        {
            if(events_arr[i].data.fd == server_fd)
            {
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
                if(client_fd == -1)
                {
                    perror("[Server] Accept error!");
                    continue;
                }
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                int client_port = ntohs(client_addr.sin_port);
                std::cout << "[Server] New connection from " << client_ip 
                          << ":" << client_port 
                          << " (fd: " << client_fd << ")" <<'\n';
                if(SetClientTimeout(client_fd, 30) == -1)
                {
                    continue;
                }
                tpool.Enqueue([this, client_fd]{HandleClient(client_fd);});    
            }
        }  
    }
    
}
