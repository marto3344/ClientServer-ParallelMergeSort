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
        ThreadPool tpool;
        int CreateSocket();
        int SetReuseAddr(int sockfd);
        void HandleClient(int client_fd);
};



