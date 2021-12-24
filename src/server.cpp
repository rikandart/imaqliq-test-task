#include "server.h"
Server::Server(const unsigned short port){
    _defSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_defSock == INVALID_SOCKET){
        return;
    }
    sockaddr_in si;
    si.sin_family = AF_INET;
    si.sin_addr.s_addr = inet_addr("127.0.0.1");
    si.sin_port = htons(port);
    _binded = bind(_defSock, (struct sockaddr*)&si, sizeof(struct sockaddr_in)) != SOCKET_ERROR;
}

// остановка сервера
void Server::stop(){
    close(_accSock);
    shutdown(_accSock, SHUT_RDWR);
    close(_defSock);
    shutdown(_defSock, SHUT_RDWR);
}

Server::~Server(){}

void Server::setFileName(const std::string& fileName){
    _fn = fileName;
}


// слушаем порт и сохраняем
// для повторного считывания в файл необходим перезапуск
bool Server::listenAndSave(){
#ifdef UNIT_TEST
    return _save("test string");
#endif
    if(listen(_defSock, 1) == SOCKET_ERROR) return false;
    _accSock = accept(_defSock, nullptr, NULL);
    if(_accSock == INVALID_SOCKET) return false;
    char buf[MAX_BUFFER];
    ssize_t received = recv(_accSock, buf, MAX_BUFFER, 0);
    if(received != -1 && received > 0){
        buf[received] = '\0';
        if(_save(buf)){
            std::string reply = "Succesfully written to " + _fn;
            int sent = send(_accSock, reply.c_str(), reply.length(), 0);
            if(sent != reply.length()) return false;
        }
    } else {
        errno = EINVAL;
        return false;
    }
    close(_accSock);
    shutdown(_accSock, SHUT_RDWR);
    return true;
}
bool Server::_save(const char* data){
    std::ofstream os(_fn.c_str());
    if(!os.is_open()){
        errno = EIO;
        return false;
    }
    os << data;
    os.close();
    return true;
}

#ifndef UNIT_TEST

// TerminationHandler - класс для правильного обращения с сигналами
Server* TerminationHandler::_srv;

void TerminationHandler::setServer(Server* srv){
    _srv = srv;
}

void TerminationHandler::sighupHandler(int signum){
    _srv->stop();
    exit(0);
}

void TerminationHandler::sigtermHandler(int signum){
    _srv->stop();
    exit(0);
}

void TerminationHandler::sigintHandler(int signum){
    _srv->stop();
    exit(0);
}

static void forkOff(){
    pid_t pid;

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    umask(0);
}

int main(int argc, char** argv){
    int iPort = -1;
    unsigned short port = 0;
    std::string filename;
    if(argc == 2 && !strcmp(argv[1], "--help") || argc == 1){
        std::cout << "server -p <port> -f <filename>" << std::endl;
        return 0;
    }
    bool fileIsPresent = false, portIsPresent = false;
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-p")){
            try{
                iPort = std::stoi(argv[++i]);
                if (iPort > (int)USHRT_MAX){
                    throw std::invalid_argument("wrong port");
                }
                port = static_cast<unsigned short>(iPort);
                portIsPresent = true;
            } catch(const std::invalid_argument& e){
                std::cout << "Wrong port assignment" << std::endl;
                return -1;
            }
        } else if(!strcmp(argv[i], "-f")){
            filename = argv[++i];
            fileIsPresent = true;
        } else {
            std::cout << "Unknown flag '" << argv[i] << "'" << std::endl;
            std::cout << "See 'server --help' as guide" << std::endl;
            return -1;
        }
    }
    if(!portIsPresent){
        std::cout << "Port is not present! Use '-p <port>' flag to specify it" << std::endl;
        return -1;
    }
    if(!fileIsPresent){
        std::cout << "File is not present! Use '-f <filename>' flag to specify it" << std::endl;
        return -1;
    }
    Server srv(port);
    if(!srv.binded()){
        std::cout << "Error bind to port " << port << std::endl;
        return -1;
    }
    forkOff();
    TerminationHandler::setServer(&srv);
    signal(SIGTERM, TerminationHandler::sigtermHandler);
    signal(SIGINT, TerminationHandler::sigintHandler);
    signal(SIGHUP, TerminationHandler::sighupHandler);
    srv.setFileName(filename);
    while(true){
        if(!srv.listenAndSave()){
            std::cout << "listening error errno: " << errno << std::endl;
        }
    }
    srv.stop();
    return 0;
}
#endif