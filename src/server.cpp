// posix
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <signal.h>

// stl
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <limits.h>
#include <iostream>

void sighupHandler(){

}

void sigtermHandler(int signum){
    std::cout << "sigterm " << signum << std::endl;
    exit(0);
}

#ifndef UNIT_TEST
int main(){
    signal(SIGINT, sigtermHandler);
    sleep(10);
    std::cout << "im server" << std::endl;
    return 0;
}
#endif