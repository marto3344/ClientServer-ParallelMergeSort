#pragma once
#include "threadpool.h"

class Server
{    
    public:
        static const int PORT = 3000;
        static const int TPOOL_SIZE = 4;
        int Initialize();
        void Run();
        Server();
        ~Server();
    private:
        int server_fd;
        int epoll_fd;
        ThreadPool tpool;
        int CreateSocket();
        int SetReuseAddr(int sockfd);
        int SetClientTimeout(int client_fd, int seconds);
        void HandleClient(int client_fd);
        int  RecieveArray(std::vector<int>& arr, int client_fd);
        int SendArray(std::vector<int>& arr, int client_fd);
        void ConvertToHostByte(std:: vector<int>& arr);
        void ConvertToNetworkByte(std::vector<int>& arr);
};



