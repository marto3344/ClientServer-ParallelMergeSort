#include<iostream>
#include "server.hpp"

int main(){
    Server server;
    if(server.Initialize() != -1)
        server.Run();
}