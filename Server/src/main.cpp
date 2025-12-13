#include<iostream>
#include "server.h"

int main(){
    Server server;
    if(server.Initialize() != -1)
        server.Run();
}