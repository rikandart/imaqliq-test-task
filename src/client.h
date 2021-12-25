// posix
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

// stl
#include <string>
#include <limits.h>
#include <fstream>
#include <iostream>

// plain c
#include <cstring>
#include <cstdlib>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define MAX_BUFFER 8192

class Client{
    int _sock = INVALID_SOCKET;
    bool _connected = false;
    std::string _fn = "";
    bool _send();
public:
    Client(const unsigned short port);
    ~Client();
    bool sendAndListen();
    bool connected() const {return _connected;}
    const std::string& fileName() const{return _fn;}
    void stop();
    void setFileName(const std::string& fileName);
};

class TerminationHandler{
    static Client* _cli;
public:
    static void setClient(Client* cli);
    static void sighupHandler(int signum);
    static void sigtermHandler(int signum);
    static void sigintHandler(int signum);
};