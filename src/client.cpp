#include "client.h"

Client::Client(const unsigned short port){
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_sock == INVALID_SOCKET){
        return;
    }
    sockaddr_in si;
    si.sin_family = AF_INET;
    si.sin_addr.s_addr = inet_addr("127.0.0.1");
    si.sin_port = htons(port);
    _connected = connect(_sock, (struct sockaddr*)&si, sizeof(struct sockaddr_in)) != SOCKET_ERROR;
}

Client::~Client(){}

void Client::stop(){
    close(_sock);
    shutdown(_sock, SHUT_RDWR);
}

void Client::setFileName(const std::string& fileName){
    _fn = fileName;
}

// чтение файла и отправка на сервер
bool Client::_send(){
    std::ifstream is(_fn.c_str());
    if(!is.is_open()){
        errno = EIO;
        return false;
    }
    size_t size = is.tellg();
    if (size > MAX_BUFFER){
        errno = EFBIG;
        return false;
    }
    std::stringstream sstr;
    sstr << is.rdbuf();
    std::string data = sstr.str();
    std::cout << "Sending '" << data << "' from " << _fn << std::endl;
    int sent = send(_sock, data.c_str(), data.length(), 0);
    return sent == data.length();
}

// отправка на сервер и получение ответа
bool Client::sendAndListen(){
    if(!_send()) return false;
    char buf[MAX_BUFFER];
    int len = read(_sock, buf, MAX_BUFFER);
    buf[len] = '\0';
    std::cout << "TCP Daemon response:\n" << buf << std::endl;
    stop();
    return true;
}

#ifndef UNIT_TEST
// TerminationHandler - класс для правильного обращения с сигналами
Client* TerminationHandler::_cli;

void TerminationHandler::setClient(Client* cli){
    _cli = cli;
}

void TerminationHandler::sighupHandler(int signum){
    _cli->stop();
    exit(0);
}

void TerminationHandler::sigtermHandler(int signum){
    _cli->stop();
    exit(0);
}

void TerminationHandler::sigintHandler(int signum){
    _cli->stop();
    exit(0);
}

int main(int argc, char** argv){
    int iPort = -1;
    unsigned short port = 0;
    std::string filename;
    if(argc == 2 && !strcmp(argv[1], "--help") || argc == 1){
        std::cout << "client -p <port> -f <filename>" << std::endl;
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
            std::cout << "See 'client --help' as guide" << std::endl;
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
    Client cli(port);
    if(!cli.connected()){
        std::cout << "Error connect to port " << port << std::endl;
        return -1;
    }
    TerminationHandler::setClient(&cli);
    signal(SIGTERM, TerminationHandler::sigtermHandler);
    signal(SIGINT, TerminationHandler::sigintHandler);
    signal(SIGHUP, TerminationHandler::sighupHandler);
    cli.setFileName(filename);
    if(!cli.sendAndListen()){
        std::cout << "send error errno: " << errno << std::endl;
    }
    return 0;
}
#endif