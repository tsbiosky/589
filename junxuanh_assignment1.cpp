#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include "../include/logger.h"
#include "../include/global.h"
#include <arpa/inet.h>
#define ubit "junxuanh"
#define EMPTY_STRING ""
//#include <WinSock2.h>
int status=0;
using namespace std;
//Client side
fd_set read_fd, all_fd;
int server_sockfd;
int max_fd;
vector<vector<string> > client_list;
//Server side
//Chat Application
void print_SUCCESS(std::string command) {
	cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
}

void print_ERROR_END(std::string command) {
	cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
	cse4589_print_and_log("[%s:END]\n", command.c_str());
}

void print_END(std::string command) {
	cse4589_print_and_log("[%s:END]\n", command.c_str());
}

int str2int(const std::string &s) {
	int d;
	std::istringstream ss(s);
	ss.imbue(std::locale::classic());
	ss >> d;
	return d;
}
string int2str(int d) {
	std::stringstream ss;
	ss << d;
	return ss.str();
}
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));
}

bool send_buff(int sock_fd, const char *buf, unsigned long size) {
	unsigned long total = 0;
	unsigned long bytes_remaining = size;
	ssize_t n = 0;
	std::cout << "send " << size << " bytes: " << buf << std::endl;

	while (total < size) {
		n = send(sock_fd, buf + total, bytes_remaining, 0);
		if (n == -1) { break; }
		total += n;
		bytes_remaining -= n;
	}

	return n != -1;
}
bool send_string(int sock_fd, const std::string &data) {
	if (data.size() == 0) {
		return false;
	}
	return send_buff(sock_fd, data.c_str(), data.length());
}
bool bind_listen_on(int *sock_fd, const char *port) {
	struct addrinfo *result, *temp;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;;

	int getaddrinfo_result;
	int reuse = 1;
	if ((getaddrinfo_result = getaddrinfo(NULL, port, &hints, &result)) != 0) {
//            DEBUG_LOG("getaddrinfo: " << gai_strerror(getaddrinfo_result));
		return false;
	}

	for (temp = result; temp != NULL; temp = temp->ai_next) {
		*sock_fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (*sock_fd == -1)
			continue;

		if (setsockopt(*sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
			std::cout << "Cannot setsockopt() on: " << *sock_fd << " errono:";
			close(*sock_fd);
			continue;
		}


		close(*sock_fd);
	}
	freeaddrinfo(result);

	if (temp == NULL) {
		std::cout << "Cannot find a socket to bind to; errno:" ;
		return false;
	}

	if (listen(*sock_fd, 10) == -1) {
//            DEBUG_LOG("Cannot listen on: " << *sock_fd << " errno: " << strerror(errno));
		return false;
	}
	return true;
}
bool connect_to(int *sock_fd, const char *ip, const char *port, int ai_socktype) {
	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = ai_socktype;

	addrinfo *result;
	int status = getaddrinfo(ip, port, &hints, &result);
	if (status != 0)
		return false;

	addrinfo *tmp;
	int yes = 1;
	for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
		*sock_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (*sock_fd == -1)
			continue;

		if (setsockopt(*sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			close(*sock_fd);
			continue;
		}

		if (connect(*sock_fd, tmp->ai_addr, tmp->ai_addrlen) == -1) {
			close(*sock_fd);
			continue;
		}

		break;
	}
	freeaddrinfo(result);

	if (tmp == NULL)
		return false;

	return true;
}
string get_ip(const sockaddr_storage &addr) {
	if (addr.ss_family == AF_INET) { // IPv4
		char ip[INET_ADDRSTRLEN];
		return std::string(inet_ntop(addr.ss_family,
									 &(((sockaddr_in *) &addr)->sin_addr),
									 ip, INET_ADDRSTRLEN));
	} else {                          // IPv6
		char ip[INET6_ADDRSTRLEN];
		return std::string(inet_ntop(addr.ss_family,
									 &(((sockaddr_in6 *) &addr)->sin6_addr),
									 ip, INET6_ADDRSTRLEN));
	}
}
string get_external_ip(void) {

	int sock_fd = 0;
	if (connect_to(&sock_fd, "8.8.8.8", "53", SOCK_DGRAM)) {

		sockaddr_storage in;
		socklen_t in_len = sizeof(in);
		if (getsockname(sock_fd, (sockaddr *) &in, &in_len) == -1) {
			return EMPTY_STRING;
		}

		if (sock_fd != 0) {
			close(sock_fd);
		}

		return get_ip(in);
	}

	return EMPTY_STRING;
}


//common  commends
void command_AUTHOR(void) {
	std::string command = "AUTHOR";
	print_SUCCESS(command);
	cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n",ubit);
	print_END(command);
}
void command_IP(void) {
	//cout<<"IP"<<endl;
	std::string command = "IP";
	string ip =get_external_ip();
	if (ip != EMPTY_STRING) {
		print_SUCCESS(command);
		cse4589_print_and_log("IP:%s\n", ip.c_str());
		printf("IP:%s\n", ip.c_str());
		print_END(command);
	} else {
		print_ERROR_END(command);
	}
}
void command_PORT(void) {
	cout<<"port"<<endl;
}
void command_LIST(vector<vector<string> > &client_list) {
	//command_LIST(client_list);
	std::string command="LIST";
	cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
	for (int i = 0; i < client_list.size(); i++){

		cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i+1, client_list[i][0].c_str(), client_list[i][1].c_str(), str2int(client_list[i][2]));


	}
	cse4589_print_and_log("[%s:END]\n", command.c_str());
}
//common commends
char buff_login[100];
char* command_LOGIN(const std::string &server_ip, const std::string &server_port,int port,vector<vector<string> > &client_list) {
	std::string command = "LOGIN";
	buff_login[0]='\0';
	sockaddr_in addrSrv;
	bzero((char*)&addrSrv, sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	inet_pton(AF_INET, server_ip.c_str(), &(addrSrv.sin_addr));
	//addrSrv.sin_addr.s_addr = inet_addr(server_ip);
	int clientSd = socket(AF_INET, SOCK_STREAM, 0);
	//创建套接字
	bind(clientSd,(struct sockaddr *)&addrSrv,sizeof(struct sockaddr));
	//bind_listen_on(&clientSd,server_port.c_str());
	//向服务器发出连接请求
	//char buff[100];
	if(connect_to(&clientSd, server_ip.c_str(), server_port.c_str(), SOCK_STREAM)==false){
		printf("Connect failed\n");
		print_ERROR_END(command);
		return NULL;
	}
	/*if(connect(clientSd, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == -1){
		printf("Connect failed\n");
		print_ERROR_END(command);
		return;
	}*/else
	{   cout<<"connect successful"<<endl;
		//接收数据
		send_string(clientSd, int2str(port));
		print_SUCCESS(command);
		print_END(command);
		//while (FD_ISSET(clientSd, &read_fd)){
			recv(clientSd,buff_login, sizeof(buff_login), 0);
			//cse4589_print_and_log("%s",buff);
			//cout<<buff<<endl;
			//string temp(buff);
			return buff_login;
		//}
		//printf("%s\n", buff);
	}

}

void command_REFRESH(void) {
	cout<<"refresh"<<endl;

}

void command_SEND(const std::string &client_ip, const std::string &msg) {
	std::string command = "SEND";


	cout<<command<<endl;
}

void command_BROADCAST(const std::string &msg) {
	std::string command = "BROADCAST";

	cout<<command<<endl;
}

void command_BLOCK(const std::string &client_ip) {
	std::string command = "BLOCK";
	cout<<command<<endl;

}

void command_UNBLOCK(const std::string &client_ip) {
	std::string command = "UNBLOCK";

	cout<<command<<endl;
}

void command_LOGOUT(void) {
	std::string command = "LOGOUT";
	cout<<command<<endl;
}

void command_EXIT(void) {
	std::string command = "EXIT";

	cout<<command<<endl;
}
//client commends

//server commends
void command_STATISTICS(void) {
	std::string command = "STATISTICS";

	cout<<command<<endl;
}
void command_BLOCKED(const std::string &ip) {
	std::string command = "BLOCKED";

	cout<<command<<endl;
}
//server commends
void select_commend_server(string commend_str){
	std::vector <string> v;
	SplitString(commend_str, v, " ");
	if (v.size() == 1 && v[0] == "AUTHOR") {
		command_AUTHOR();

	} else if (v.size() == 1 && v[0] == "IP") {
		command_IP();

	} else if (v.size() == 1 && v[0] == "PORT") {
		command_PORT();

	}  else if (v.size() == 1 && v[0] == "STATISTICS") {
		command_STATISTICS();

	} else if (v.size() == 2 && v[0] == "BLOCKED") {
		command_BLOCKED(v[1]);

	} else {

		std::cout << "Command Error!" << std::endl;
	}
}
void select_commend_client(string commend_str) {
	std::vector <string> v;
	SplitString(commend_str, v, " ");
	if (v.size() == 1 && v[0] == "AUTHOR") {
		command_AUTHOR();

	} else if (v.size() == 1 && v[0] == "IP") {
		command_IP();

	} else if (v.size() == 1 && v[0] == "PORT") {
		command_PORT();

	}  else if (v.size() == 1 && v[0] == "REFRESH" && status == 1) {
		command_REFRESH();

	} else if (v.size() >= 3 && v[0] == "SEND" && status == 1) {
		//v = StringHandler::split_part(s, ' ', 3);
		command_SEND(v[1], v[2]);

	} else if (v.size() >= 2 && v[0] == "BROADCAST" && status == 1) {
		//v = StringHandler::split_part(s, ' ', 2);
		command_BROADCAST(v[1]);

	} else if (v.size() == 2 && v[0] == "BLOCK" && status == 1) {
		command_BLOCK(v[1]);

	} else if (v.size() == 2 && v[0] == "UNBLOCK" && status == 1) {
		command_UNBLOCK(v[1]);

	} else if (v.size() == 1 && v[0] == "LOGOUT" && status == 1) {
		command_LOGOUT();

	} else if (v.size() == 1 && v[0] == "EXIT") {
		command_EXIT();

	} else {
		std::cout << "Command Error!" << std::endl;
	}
}

void TCPclient (int port) {
	vector<vector <string> > useless;
	char *client_list;
	while (true) {
		string commend_str;

		char *p1;
		getline(cin,commend_str);
		std::vector <string> v;

		SplitString(commend_str, v, " ");
		if(commend_str=="PORT"){
			print_SUCCESS(commend_str);
			cse4589_print_and_log("PORT:%d\n", port);
			print_END(commend_str);
		}
		else if (v.size() == 3 && v[0] == "LOGIN" && status == 0) {
			//string temp;

			client_list=command_LOGIN(v[1], v[2],port,useless);
			cout<<client_list<<endl;
			//std::vector <string> v;
			//vector<string> vv;
			cout<<"1"<<endl;
			//v=strtok(temp,"?");
			/*
			for(int i=0;i<v.size();i++) {
				//cout<<v[i]<<endl;
				cout<<"2"<<endl;
				SplitString(v[i], vv, " ");
				for(int j=0;j<vv.size();j++){
					cout<<vv[j]<<endl;
				}
				client_list.push_back(vv);
				char * cstr = new char [vv[1].length()+1];
				strcpy (cstr, vv[1].c_str());
				//printf("%-5d\n",vv[0][0]);
				//const char t[] = vv[1].data();
				//char *p=vv[1].data();
				printf("%s\n",cstr);
				//printf("%-5d\n",str2int(vv[2]));
				//printf("%-5d%-35s%-20s%-8d\n",)
			}*/

		}else if(commend_str=="LIST"){
			p1=strtok(client_list,"?");
			int i=0;


			while(p1!=NULL){
				i++;
				char *p2;
				char temp[]="";
				strcpy(temp,p1);
				p2=strtok(temp," ");
				char *hostname=p2;
				p2=strtok(NULL," ");
				char *ip=p2;
				p2=strtok(NULL," ");
				int port=atoi(p2);
				cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i, hostname, ip, port);
				p1=strtok(NULL,"?");
			}
		}
		else {

			select_commend_client(commend_str);
		}
		//cout<<commend[0]<<endl;
	}
}
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void TCPserver (int port){
	const char *PORT=int2str(port).c_str();
	fd_set master;    // master file descriptor list
	fd_set read_fds;  // temp file descriptor list for select()
	int fdmax;        // maximum file descriptor number

	int listener;     // listening socket descriptor
	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	fd_set all_fd;
	//char buf[256];    // buffer for client data
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int i, j, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);

	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p = ai; p != NULL; p = p->ai_next) {
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) {
			continue;
		}

		// lose the pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		bind(listener, p->ai_addr, p->ai_addrlen);
		/*
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}*/

		break;
	}

	// if we got here, it means we didn't get bound
	if (p == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai); // all done with this

	// listen
	if (listen(listener, 10) == -1) {
		perror("listen");
		exit(3);
	}

	// add the listener to the master set
	FD_SET(listener, &master);

	// keep track of the biggest file descriptor
	fdmax = listener; // so far, it's this one
	printf("/server: waiting for connections...\n");
	// main loop
	ssize_t bytes_read_count;
	char buffer[100];
	FD_SET(STDIN_FILENO, &master);
	string commend_str;
	for(;;) {
		read_fds = master; // copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}
		//if (FD_ISSET(STDIN_FILENO, &read_fds)) {


		//}

		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) {

			if (FD_ISSET(i, &read_fds)) {// we got one!!

				if (i == listener) {
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener,
								   (struct sockaddr *)&remoteaddr,
								   &addrlen);

					if (newfd == -1) {
						perror("accept");
					} else {
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) {    // keep track of the max
							fdmax = newfd;
						}
						printf("selectserver: new connection from %s on "
									   "socket %d\n",
							   inet_ntop(remoteaddr.ss_family,
										 get_in_addr((struct sockaddr*)&remoteaddr),
										 remoteIP, INET6_ADDRSTRLEN),
							   newfd);
						char host[NI_MAXHOST], service[NI_MAXSERV];
						getnameinfo((struct sockaddr *) &remoteaddr,
									addrlen, host, NI_MAXHOST,
									service, NI_MAXSERV, NI_NUMERICSERV);
						std::string ip = get_ip(remoteaddr);
						std::string hostname = host;
						recv(newfd, buffer, sizeof buffer, 0);
						//int port = ((sockaddr_in *) &remoteaddr)->sin_port;
						int port=str2int(buffer);
						vector<string> temp;
						temp.push_back(hostname);
						temp.push_back(ip);
						temp.push_back(int2str(port));
						temp.push_back(int2str(newfd));
						client_list.push_back(temp);
						cout<<ip<<host<<port<<endl;

					}//SENT LIST
					for (int i = 0; i < client_list.size(); i++){
						int child_fd = str2int(client_list[i][3]);
						string send_str="?";
						for (int j=0;j<client_list.size();j++){
							send_str=send_str+client_list[j][0]+" "+client_list[j][1]+" "+client_list[j][2]+" ?";
						}
						send_string(child_fd,send_str);
					}
				}

				/*else {
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							printf("selectserver: socket %d hung up\n", i);
						} else {
							perror("recv");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
					} else {
						// we got some data from a client
						for(j = 0; j <= fdmax; j++) {
							// send to everyone!
							if (FD_ISSET(j, &master)) {
								// except the listener and ourselves
								if (j != listener && j != i) {
									if (send(j, buf, nbytes, 0) == -1) {
										perror("send");
									}
								}
							}
						}
					}
				}// END handle data from client
				*/  //do later
			} // END got new input

			for (int i = 0; i < client_list.size(); i++){
				int child_fd = str2int(client_list[i][3]);
				if (FD_ISSET(child_fd, &read_fd)){
					memset(&buffer, 0, sizeof buffer);
					if (recv(child_fd, buffer, sizeof buffer, 0) == 0) { // Check if closing
						//client_list.exit(i);
						close(child_fd);
						std::cout << "CLOSE" << std::endl;

					} else {  // handle events
						std::cout << "client " << i << " " << std::endl;
						std::cout << "receive: " << std::string(buffer) << std::endl;
						std::string command_str = buffer;
						//select_event(client_list.client_vector[i].ip, command_str);
					}
				}
			}

		} // END looping through file descriptors
		cout<<"input command"<<endl;
		getline(cin,commend_str);
		if(commend_str=="PORT"){
			print_SUCCESS(commend_str);
			cse4589_print_and_log("PORT:%d\n", port);
			print_END(commend_str);
		}else if(commend_str=="IP"){
			select_commend_server(commend_str);
		} else if(commend_str=="AUTHOR"){
			select_commend_server(commend_str);
		}else{
			int noting;
			//store command and go next
		}
		if(commend_str=="LIST"){
			command_LIST(client_list);
		}else{
			cout<<"command error"<<endl;
		}
		fflush(STDIN_FILENO);
	} // END for(;;)--and you thought it would never end!


}


int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);

	/* Clear LOGFILE*/
    fclose(fopen(LOGFILE, "w"));

	/*Start Here*/
	if(argc!=3) {
		cerr << "Need valid s/c and port number arguments\n" << endl; exit(0);
	}
	//char *serverIp = argv[1];
	int port = atoi(argv[2]);
	char s[] = "s";
	char c[] = "c";

	if(strcmp (argv[1],c) == 0) {
		TCPclient(port);
	}
	if(strcmp (argv[1],s) == 0){
		TCPserver(port);
	}else{
		cerr << "Invalid c/s argument\n" << endl; exit(0);
	}
	return 0;
	return 0;
}
