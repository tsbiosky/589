#include <iostream>
#include <string>
#include <stdio.h>
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
int status=0;
//#include "../include/logger.h"
//#include "../include/Client.h"
//#include "../include/Server.h"
using namespace std;
//Client side

//Server side
//Chat Application
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



//common  commends
void command_AUTHOR(void) {
    cout<<"author"<<endl;
}
void command_IP(void) {
    cout<<"IP"<<endl;
}
void command_PORT(void) {
    cout<<"port"<<endl;
}
void command_LIST(void) {
    //command_LIST(client_list);
}
//common commends
void command_LOGIN(const std::string &server_ip, const std::string &server_port) {
    cout<<"login"<<endl;
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

    } else if (v.size() == 1 && v[0] == "LIST") {
        command_LIST();

    } else if (v.size() == 1 && v[0] == "STATISTICS") {
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

    } else if (v.size() == 1 && v[0] == "LIST" && status == 1) {
        command_LIST();

    } else if (v.size() == 3 && v[0] == "LOGIN" && status == 0) {
        command_LOGIN(v[1], v[2]);

    } else if (v.size() == 1 && v[0] == "REFRESH" && status == 1) {
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
    while (true) {
        string commend_str;
        getline(cin,commend_str);
        select_commend_client(commend_str);
        //cout<<commend[0]<<endl;
    }
}
void TCPserver (int port){

    while(true){
        string commend_str;
        getline(cin,commend_str);
        select_commend_server(commend_str);
    }
}
int main(int argc, char *argv[])
{
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
}
