#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int connectToServer(const char* addr, int port)
{
    int sockfd = 0;
    sockaddr_in serv_addr;
    ssize_t bytes;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr<<"\n Error : Could not create socket \n";
		return 1;
	}
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, addr, &serv_addr.sin_addr)<=0)
	{
		cerr<<"\n inet_pton error occured!\n";
		return -1;
	}

    if (connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr<<"\n Error connecting to the server!\n";
        return -1;
    }
    return sockfd;
}

int sendData(int sockfd, const void* data, size_t dataSize)
{
    const char * ptr = static_cast<const char*>(data);
    size_t bytes_sent = 0;
    while (bytes_sent < dataSize)
    {
        ssize_t bytes = send(sockfd, ptr + bytes_sent, dataSize - bytes_sent, 0); 
        if (bytes == -1) {
            perror("[Client] Send error");
            return -1;
        }
        bytes_sent += bytes;   
    }
    return 0;
}

int sendAllInfo(int sockfd, size_t n_threads, const vector<int>& arr)
{
    if(sendData(sockfd,&n_threads, sizeof(n_threads))  == -1)
    {
        cerr<<"[Client] Failed to send number of threads!";
        return -1;
    }
    const size_t vecsize = arr.size();
    if(sendData(sockfd,&vecsize, sizeof(vecsize))  == -1)
    {
        cerr<<"[Client] Failed to send size of the array";
        return -1;
    }
    cout<<"Sending array!";
    if(sendData(sockfd, arr.data(), arr.size()*sizeof(int))  == -1)
    {
        cerr<<"[Client] Failed to send the array";
        return -1;
    }
    return 0;
}

int ReceiveSortedArr(int sockfd, std::vector<int>& data, size_t arr_size) 
{
    size_t total_bytes = arr_size * sizeof(int);
    ssize_t bytes_received = recv(
        sockfd, 
        reinterpret_cast<char*>(data.data()), 
        total_bytes, 
        MSG_WAITALL
    );
    if (bytes_received != total_bytes) 
    {
        std::cerr << "[Client] Error! Expected " << total_bytes 
                  << " bytes but received " << bytes_received << " (or error).\n";
        return -1;
    }
    return 0;
}

void inputFromBinFile(const string& fname, vector<int>& arr)
{
    ifstream infile(fname, std::ios::binary | std::ios::ate);
    if (!infile) 
    {
        cerr<<"Error opening the file\n";
        return;
    }
    std::streamsize file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    size_t array_size = file_size / sizeof(int);
    arr.resize(array_size);
    infile.read(reinterpret_cast<char*>(arr.data()), file_size);
}

int main()
{
    const char * addr = "127.0.0.1";
    int port =  3000;
    size_t nThreads;
    cout<<"Enter the number of threads!\n";
    cin>>nThreads;
    int arr_input_option = -1;
    cout<<"How you want to input the array?\n";
    cout<<"Press 0 to input from binary file \n";
    cout<<"Press 1 to input from the standart input\n";
    cin>>arr_input_option;
    
    vector<int> arr;
    if(arr_input_option == 0)
    {   
        string fname;
        cout<<"Enter binary file name:\n";
        cin>>fname;
        inputFromBinFile(fname, arr);
    }
    else if (arr_input_option == 1)
    {
        
    }
    else
    {
        cerr<<"Invalid input option!\n";
        return 1;
    }
    int sockfd = connectToServer(addr, port);
    if(sockfd == -1)
    {
        return 1;
    }
    sendAllInfo(sockfd,nThreads, arr);
    cout<<"Info sent successfully!";  
    ReceiveSortedArr(sockfd,arr, arr.size());  
    for (auto val : arr)
        cout<<val<<' ';
}   
