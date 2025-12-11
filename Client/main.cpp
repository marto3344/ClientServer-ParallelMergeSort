#include<iostream>
#include<vector>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOCAL_HOST "127.0.0.1"
#define SERVER_PORT 3000
#define BUFF_SIZE 1024
using namespace std;

int main()
{

    int sockfd = 0;
    char buff[BUFF_SIZE];
    sockaddr_in serv_addr;
    ssize_t bytes;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr<<"\n Error : Could not create socket \n";
		return 1;
	}
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, LOCAL_HOST, &serv_addr.sin_addr)<=0)
	{
		cerr<<"\n inet_pton error occured!\n";
		return -1;
	}

    if (connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr<<"\n Error connecting to the server!\n";
        return -1;
    }

    while (1)
    {
        bytes = read(0, buff, sizeof(buff));
        if(bytes <= 0)
            break;
        
        write(sockfd,buff,bytes);
        sleep(1);
    }
    return 0;
    // int threadsCount, size;
    // cout<<"Enter number of threads:\n";
    // cin>>threadsCount;
    // cout<<"Enter array size:\n";
    // cin>>size;
    // vector<double> arr(size, 0);
    // int a;
    // for (size_t i = 0; i < size; i++)
    // {
    //     cin>>a;
    //     arr[i]= a;
    // }
    
}   
