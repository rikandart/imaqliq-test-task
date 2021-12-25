// posix
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
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

class Server {
	int _defSock = INVALID_SOCKET;
	int _accSock = INVALID_SOCKET;
	std::string _fn = "";
	bool _binded = false;
public:
	Server(const unsigned short port);
	~Server();
	void setFileName(const std::string& fileName);
	bool listenAndSave();
	void stop();
	const std::string& fileName() const{return _fn;}
	inline bool binded() const {return _binded;};
};

class TerminationHandler{
	static Server* _srv;
public:
	static void setServer(Server* srv);
	static void sighupHandler(int signum);
	static void sigtermHandler(int signum);
	static void sigintHandler(int signum);
};