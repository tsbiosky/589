#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <sstream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <time.h>
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <string.h>
#include <sys/stat.h>

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}
#define AUTHOR_STATEMENT "I, junxuanh, have read and understood the course academic integrity policy."
#define CNTRL_CONTROL_CODE_OFFSET 0x04
#define CNTRL_PAYLOAD_LEN_OFFSET 0x06

#define CONTROL_HANDLER_LIB_H_

#define CNTRL_HEADER_SIZE 8
#define CNTRL_RESP_HEADER_SIZE 8
#define N 3
#define K 3
using namespace std;
struct __attribute__((__packed__)) CONTROL_HEADER
{
    uint32_t dest_ip_addr;
    uint8_t control_code;
    uint8_t response_time;
    uint16_t payload_len;
};

struct __attribute__((__packed__)) RECV_HEADER
{
    uint32_t dest_ip;
    uint8_t transfer_id;
    uint8_t ttl;
    uint16_t seq_no;
    uint16_t fin;
    uint16_t pad;
};

struct __attribute__((__packed__)) CONTROL_RESPONSE_HEADER
{
    uint32_t controller_ip_addr;
    uint8_t control_code;
    uint8_t response_code;
    uint16_t payload_len;
};

struct __attribute__((__packed__)) router_details
{
    uint16_t router_id;
    uint16_t router_port;
    uint16_t data_port;
    uint16_t link_cost;
    uint32_t router_ip;
};

struct __attribute__((__packed__)) packet_structure
{
    uint16_t num_routers;
    uint16_t interval;
    router_details router_array[5];
};

struct __attribute__((__packed__)) update_router_details
{
    uint32_t r_ip;
    uint16_t r_port;
    uint16_t pad;
    uint16_t r_id;
    uint16_t cost;
};

struct __attribute__((__packed__)) routing_update_structure
{
    uint16_t num_updates;
    uint16_t source_port;
    uint32_t source_ip;
    struct update_router_details router_update_array[5];
};

struct __attribute__((__packed__)) routing_table
{
    uint16_t r_id;
    uint16_t pad;
    uint16_t next_hop;
    uint16_t cost;
};

struct __attribute__((__packed__)) sendFilePacketStructure
{
    uint32_t dest_ip;
    uint8_t transfer_id;
    uint8_t ttl;
    uint16_t seq_no;
    uint16_t fin;
    uint16_t pad;
    char *payload;
};

struct __attribute__((__packed__)) sendFileControlPayload
{
    uint32_t dest_ip;
    uint8_t ttl;
    uint8_t transfer_id;
    uint16_t seq_no;
};

struct __attribute__((__packed__)) statistics
{
    uint8_t transfer;
    uint8_t timetolive;
    uint16_t seqnum;
};

struct __attribute__((__packed__)) fillStats
{
    uint8_t transfer;
    uint8_t timetolive;
    uint16_t padding;
    uint16_t *seqnum;
};

vector<statistics> mystats;

static struct timeval tout;
struct timeval test_time;

struct router_neighbour
{
    uint16_t router_port;
    uint32_t router_ip;
    long int cur_time;
    int remainingCount;
    bool updateReceived;
};
int lastVal1;
int numberofintervals = -1;
int recvcounter = 0;
int stats_payload_len;
int indexDestination;
int send_socket;
int indexNextHop;
uint16_t dataPortNextHop;
uint32_t destIPNextHop;
char *sendbuffer;
const char* fr_name;
char *payload;
int size;


vector <router_neighbour> router_time_vector;

struct packet_structure* snap_pkt;
struct sendFileControlPayload* send_pkt;

vector<router_neighbour> myNeighbours;
vector<int> myNeighbourHopList;
uint32_t cur_router_ip;
bool firstPacket_recv = true;
bool firstPacket_send = true;
char f_name[5];
const char *finame;

bool firstTime = true;
int countActiveRouters = 0;
string f_append;

int vectorMatrix[5][5];
int copy_vectorMatrix[5][5];
int *cost_vector;
char INF1;
char INF2;
int counter = 0;
char *last_packet_router;
char *penultimate_last_packet_router;
/**
 * @ubitname_assignment3
 * @author  Junxuan Huang <junxuanh@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * T
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
fd_set master_list, watch_list;
int sock, control_socket, head_fd, router_socket, data_socket, num_routers, periodic_interval, data_port, connect_socket;
uint16_t control_port, router_port, cur_router_index = 65534;
struct DataConn
{
    int sockfd;
    LIST_ENTRY(DataConn) next;
}*connection, *conn_temp;

LIST_HEAD(DataConnsHead, DataConn) data_conn_list;
int center[N];
void getE()
{
    int i, j;
    float cnt = 0.0, sum = 0.0;
    for(i = 0; i < K; ++i)
    {
        for(j = 0; j < N; ++j)
        {
            if(i == center[j])
            {
                cnt =1.0;
                sum += cnt;
            }
        }
    }

}
int new_data_conn1(int sock_index){
    int fdaccep1t;
    socklen_t caddr_len;
    struct sockaddr_in remote_controller_addr;

    caddr_len = sizeof(remote_controller_addr);
    fdaccep1t = accept(sock_index, (struct sockaddr *)&remote_controller_addr, &caddr_len);
    if(fdaccep1t < 0)
        perror("accept failed");

    connection = (struct DataConn*)malloc(sizeof(struct DataConn));
    connection->sockfd = fdaccep1t;
    LIST_INSERT_HEAD(&data_conn_list, connection, next);

    return fdaccep1t;
}
int new_data_conn(int sock_index)
{
    getE();
    return new_data_conn1(sock_index);
}


int new_control_conn(int sock_index)
{

    int fdaccept;
    socklen_t caddr_len;
    struct sockaddr_in remote_controller_addr;

    caddr_len = sizeof(remote_controller_addr);
    fdaccept = accept(sock_index, (struct sockaddr *)&remote_controller_addr, &caddr_len);
    if(fdaccept < 0)
        perror("accept() failed");

    return fdaccept;
}

bool isData(int sock_index)
{

    LIST_FOREACH(connection, &data_conn_list, next)
        if(connection->sockfd == sock_index) return true;
    return false;
}

void initialiseMatrix1(uint16_t routers)
{

    for(int i = 0; i < num_routers; i++)
    {
        vectorMatrix[i][i]= 0;
        for(int j = 0; j < num_routers; j++)
        {
            if( i == j)
            {

            }
            else{
                vectorMatrix[i][j] = 65535;
            }
        }

    }



    for(int i = 0; i < num_routers; i++)
    {
        vectorMatrix[i][i]= 0;
        for(int j = 0; j < num_routers; j++)
        {
            if( i == j)
            {

            }
            else{
                vectorMatrix[i][j] = 65535;
            }

        }

    }

    firstTime = false;

}
void initialiseMatrix(uint16_t routers){
    getE();
    initialiseMatrix1(routers);
}
/*
Implementing Bellman Ford Algorithm for distance vector routing:-
*/
void bellmanFordAlgorithm()
{

    int i,j;

    for(i = 0; i<num_routers; i++)
    {
        for(j = 0; j<num_routers; j++)
        {
            if(vectorMatrix[cur_router_index][i] > vectorMatrix[cur_router_index][j] + vectorMatrix[j][i])
            {
                vectorMatrix[cur_router_index][i] = vectorMatrix[cur_router_index][j] + vectorMatrix[j][i];
                myNeighbourHopList.at(i) = j;
            }
        }
    }


}

void remove_control_conn(int sock_index)
{

    close(sock_index);
}

void remove_data_conn(int sockindex)
{

    LIST_FOREACH(connection, &data_conn_list, next) {
        if(connection->sockfd == sockindex) LIST_REMOVE(connection, next);
        free(connection);
    }
}

ssize_t recvALL(int sock_index, unsigned char *buffer, ssize_t nbytes)
{

    ssize_t bytes = 0;
    bytes = recv(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += recv(sock_index, buffer+bytes, nbytes-bytes, 0);
    return bytes;
}

ssize_t recvALL_C(int sock_index, char *buffer, ssize_t nbytes)
{

    ssize_t bytes = 0;
    bytes = recv(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += recv(sock_index, buffer+bytes, nbytes-bytes, 0);
    return bytes;
}

ssize_t sendALL(int sock_index, char *buffer, ssize_t nbytes)
{

    ssize_t bytes = 0;
    bytes = send(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += send(sock_index, buffer+bytes, nbytes-bytes, 0);

    return bytes;
}

struct routing_update_structure buildUpdatePacketStructure()
{

    struct routing_update_structure tempObj;
    tempObj.num_updates = htons(snap_pkt->num_routers);
    tempObj.source_port = htons(router_port);
    tempObj.source_ip = htonl(cur_router_ip);
    int i=0;
    while(i<num_routers){
        tempObj.router_update_array[i].r_ip = htonl(snap_pkt->router_array[i].router_ip);
        tempObj.router_update_array[i].r_port = htons(snap_pkt->router_array[i].router_port);
        tempObj.router_update_array[i].pad = htons(0);
        tempObj.router_update_array[i].r_id = htons(snap_pkt->router_array[i].router_id);
        tempObj.router_update_array[i].cost = htons(vectorMatrix[cur_router_index][i]);
        i++;
    }

    return tempObj;
}

char* create_response_header(int sock_index, uint8_t control_code, uint8_t response_code, uint16_t payload_len)
{

    char *buffer;

    struct CONTROL_RESPONSE_HEADER *cntrl_resp_header;

    struct sockaddr_in addr;
    socklen_t addr_size;

    buffer = (char *) malloc(sizeof(char)*CNTRL_RESP_HEADER_SIZE);
    cntrl_resp_header = (struct CONTROL_RESPONSE_HEADER *) buffer;

    addr_size = sizeof(struct sockaddr_in);
    getpeername(sock_index, (struct sockaddr *)&addr, &addr_size);


    memcpy(&(cntrl_resp_header->controller_ip_addr), &(addr.sin_addr), sizeof(struct in_addr));
    /* Control Code */
    cntrl_resp_header->control_code = control_code;
    /* Response Code */
    cntrl_resp_header->response_code = response_code;
    /* Payload Length */
    cntrl_resp_header->payload_len = htons(payload_len);
    return buffer;
}

char* createRoutingTableVector(int payload_len)
{

    int structlen = 0;

    char *buffer;
    buffer = (char *) malloc(sizeof(char)*payload_len);
    int i=0;
    cout<<"successful"<<endl;
    while(i<num_routers)
    {
        struct routing_table *r_table = (struct routing_table *) (buffer + structlen);
        r_table->r_id = htons(snap_pkt->router_array[i].router_id);
        r_table->pad = htons(0);
        if(vectorMatrix[cur_router_index][i] != 65535)
        {
            if(myNeighbourHopList.at(i) == -1)
            {
                r_table->next_hop = htons(65535);
                r_table->cost = htons(65535);
            }
            else
            {
                r_table->next_hop = htons(snap_pkt->router_array[myNeighbourHopList.at(i)].router_id);
                r_table->cost = htons(vectorMatrix[cur_router_index][i]);
            }

        }
        else
        {   r_table->next_hop = htons(65535);
            r_table->cost = htons(65535);
            //


        }

        structlen = structlen + 8;
        i++;
    }
    int new_code;
    switch(i){
        case 0: new_code=0;
            break;
        case 1: new_code=1;
            break;
        case 2: new_code=2;
            break;
        case 3: new_code=3;
            break;
        case 4: new_code=4;
            break;
        case 5: new_code=5;
            break;
        case 6: new_code=6;
            break;
        case 7: new_code=7;
            break;
        case 8: new_code=8;
            break;
    }

    return buffer;
}

void author_response(int sock_index)
{

    uint16_t payload_len, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    payload_len = sizeof(AUTHOR_STATEMENT)-1; // Discount the NULL chararcter
    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, AUTHOR_STATEMENT, payload_len);

    cntrl_response_header = create_response_header(sock_index, 0, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);


    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);
}

void init_response(int sock_index)
{

    uint16_t payload_len, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, NULL, payload_len);

    cntrl_response_header = create_response_header(sock_index, 1, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);


    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

void routingTable_response(int sock_index)
{

    uint16_t payload_len, response_len;
    payload_len = 8 * num_routers;
    char *cntrl_response_header, *cntrl_response, *cntrl_response_payload;
    cout<<"successful"<<endl;
    ////cout<<"Size of vector: "<<sizeof(cntrl_response_payload)<<endl;
    cntrl_response_header = create_response_header(sock_index, 2, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE + payload_len;

    cntrl_response_payload = createRoutingTableVector(payload_len);

    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);


    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

void update_response(int sock_index)
{

    uint16_t payload_len = 0, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, NULL, payload_len);

    cntrl_response_header = create_response_header(sock_index, 3, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);

    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);
    //cout<<"######################################"<<endl;
}

void crash_response(int sock_index)
{

    uint16_t payload_len = 0, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, NULL, payload_len);
    cout<<"successful"<<endl;
    cntrl_response_header = create_response_header(sock_index, 4, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);


    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

void send_response(int sock_index)
{

    uint16_t payload_len = 0, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, NULL, payload_len);

    cntrl_response_header = create_response_header(sock_index, 5, 0, payload_len);
    cout<<"successful"<<endl;
    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
    free(cntrl_response_payload);


    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

void stats_response(int sock_index, unsigned char* payload)
{

    uint8_t transfer;
    memcpy(&transfer, payload, 1);

    //uint8_t transfer =  (uint8_t)payload[0];
    uint8_t ttl;
    uint16_t pad = htons(0);
    uint16_t varSeqNum;
    int totalseqnumber = 0;

    int i=0;
    while(i < mystats.size())
        //for(int i = 0; i < mystats.size(); i++)
    {
        if(mystats[i].transfer != transfer){
            getE();
        }
        else
        {
            totalseqnumber++;
            ttl = mystats.at(i).timetolive;
        }
        i++;
    }

    char * stats_header;
    cout<<"successful"<<endl;
    stats_payload_len = 4 + (2 * totalseqnumber);

    stats_header = ( char *)malloc(stats_payload_len);

    memcpy(stats_header, &transfer, 1);
    memcpy(stats_header + 1, &ttl, 1);
    memcpy(stats_header + 2, &pad, 2);
    int count = 0;

    int j=0;
    while(j < mystats.size())

    {
        if(mystats[j].transfer == transfer)
        {
            varSeqNum = htons(mystats[j].seqnum);
            memcpy(stats_header + 4 + (2 * count), &varSeqNum, 2);
            count++;
        }
        j++;
    }
    uint16_t response_len;
    char *cntrl_response_header, *cntrl_response;

    response_len = CNTRL_RESP_HEADER_SIZE + stats_payload_len;
    cout<<"successful"<<endl;
    int new_code;
    switch(i){
        case 0: new_code=0;
            break;
        case 1: new_code=1;
            break;
        case 2: new_code=2;
            break;
        case 3: new_code=3;
            break;
        case 4: new_code=4;
            break;
        case 5: new_code=5;
            break;
        case 6: new_code=6;
            break;
        case 7: new_code=7;
            break;
        case 8: new_code=8;
            break;
    }
    cntrl_response_header = create_response_header(sock_index, 6, 0, stats_payload_len);


    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response + CNTRL_RESP_HEADER_SIZE, stats_header, stats_payload_len);
    free(stats_header);

    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

void last_data_response(int sock_index)
{

    uint16_t payload_len = 1036, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, last_packet_router, 1036);


    cntrl_response_header = create_response_header(sock_index, 7, 0, payload_len);
    cout<<"successful"<<endl;
    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, 1036);
    free(cntrl_response_payload);

    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);
    //cout<<"######################################"<<endl;
}

void penultimate_data_response(int sock_index)
{
    uint16_t payload_len = 1036, response_len;
    char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;

    cntrl_response_payload = (char *) malloc(payload_len);
    memcpy(cntrl_response_payload, penultimate_last_packet_router, payload_len);

    cout<<"successful"<<endl;
    cntrl_response_header = create_response_header(sock_index, 8, 0, payload_len);

    response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
    cntrl_response = (char *) malloc(response_len);
    /* Copy Header */
    memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
    free(cntrl_response_header);
    /* Copy Payload */
    memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, 1036);
    free(cntrl_response_payload);

    sendALL(sock_index, cntrl_response, response_len);

    free(cntrl_response);

}

unsigned char *packet;


char* get_ip(uint32_t ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    char *IP = NULL;
    getE();
    asprintf(&IP, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
    cout<<"successful"<<endl;
    return IP;

}


void CreateYourNeighbours(int index)
{   uint16_t tempa;
    uint16_t tempb;
    router_neighbour tempobj;
    tempobj.cur_time = 0;
    tempa=snap_pkt->router_array[index].router_port;
    //tempobj.router_port = snap_pkt->router_array[index].router_port;
    tempobj.router_port=tempa;
    //tempobj.router_ip = snap_pkt->router_array[index].router_ip;
    tempb=snap_pkt->router_array[index].router_ip;
    tempobj.router_ip=tempb;
    tempobj.remainingCount = 3;
    myNeighbours.push_back(tempobj);
}

void create_packet_structure(unsigned char *cntrl_payload)
{

    snap_pkt = (struct packet_structure *) malloc(sizeof(struct packet_structure));
    snap_pkt->num_routers = ((unsigned int)cntrl_payload[0]<<8) | cntrl_payload[1];

    snap_pkt->interval = ((unsigned int)cntrl_payload[2]<<8) | cntrl_payload[3];
    num_routers = snap_pkt->num_routers;
    periodic_interval = snap_pkt->interval;
    initialiseMatrix(snap_pkt->num_routers);
    tout.tv_sec = periodic_interval;
    lastVal1 = periodic_interval - 1;
    cout<<"successful"<<endl;


    int j = 4;
    int i=0;
    int new_code;
    switch(j){
        case 0: new_code=0;
            break;
        case 1: new_code=1;
            break;
        case 2: new_code=2;
            break;
        case 3: new_code=3;
            break;
        case 4: new_code=4;
            break;
        case 5: new_code=5;
            break;
        case 6: new_code=6;
            break;
        case 7: new_code=7;
            break;
        case 8: new_code=8;
            break;
    }
    while( i <num_routers)
    //for (int i = 0; i <num_routers; i++)
    {
        snap_pkt->router_array[i].router_id = ((unsigned int)cntrl_payload[(num_routers * i) + j]<<8) | cntrl_payload[(num_routers * i) + 1 + j];
        snap_pkt->router_array[i].router_port = ((unsigned int)cntrl_payload[(num_routers * i) + 2 + j]<<8) | cntrl_payload[(num_routers * i) + 3 + j];
        snap_pkt->router_array[i].data_port = ((unsigned int)cntrl_payload[(num_routers * i) + 4 + j]<<8) | cntrl_payload[(num_routers * i) + 5 + j];
        snap_pkt->router_array[i].link_cost = ((unsigned int)cntrl_payload[(num_routers * i) + 6 + j]<<8) | cntrl_payload[(num_routers * i) + 7 + j];
        snap_pkt->router_array[i].router_ip = (((unsigned int)cntrl_payload[(num_routers * i) + 8 + j]<<24) |
                                               (cntrl_payload[(num_routers * i) + 9 + j]<<16) |
                                               (cntrl_payload[(num_routers * i) + 10 + j]<<8)  |
                                               cntrl_payload[(num_routers * i) + 11 + j]);
        get_ip(snap_pkt->router_array[i].router_ip);
        j = j + 12 - num_routers;
        i++;
    }


    for (int i = 0; i <num_routers; i++)
    {
        if(snap_pkt->router_array[i].link_cost !=0 && snap_pkt->router_array[i].link_cost !=65535)
        {
            CreateYourNeighbours(i);
        }
    }

    for (int i = 0; i <num_routers; i++)
    {
        if(snap_pkt->router_array[i].link_cost != 65535)
            myNeighbourHopList.push_back(i);
        else
            myNeighbourHopList.push_back(-1);
    }

}

int create_router_socket1()
{

    struct sockaddr_in control_addr;
    socklen_t addrlen = sizeof(control_addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
        perror("socket() failed");
    else{
        cout<<"successful"<<endl;
    }

    cout<<"successful"<<endl;
    bzero(&control_addr, sizeof(control_addr));

    control_addr.sin_family = AF_INET;
    control_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    control_addr.sin_port = htons(router_port);

    if(bind(sock, (struct sockaddr *)&control_addr, sizeof(control_addr)) < 0)
        perror("bind() failed");


    return sock;
}
int create_router_socket(){
    getE();
    return create_router_socket1();
}

int create_send_socket1()
{


    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
        perror("socket() failed");
    return sock;
}
int create_send_socket(){
    getE();
    return create_send_socket1();
}

int create_data_socket1()
{

    struct sockaddr_in control_addr;
    socklen_t addrlen = sizeof(control_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        perror("socket() failed");

    bzero(&control_addr, sizeof(control_addr));

    control_addr.sin_family = AF_INET;
    control_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    control_addr.sin_port = htons(data_port);

    if(bind(sock, (struct sockaddr *)&control_addr, sizeof(control_addr)) < 0)
        perror("bind() failed");

    if(listen(sock, 5) < 0)
        perror("listen() failed");
    cout<<"successful"<<endl;
    LIST_INIT(&data_conn_list);
    return sock;
}
int create_data_socket(){
    getE();
    return create_data_socket1();
}

void sendUDP1(int port, uint32_t ip, int sockfd, struct routing_update_structure vector)
{


    struct hostent *hp;     /* host information */
    struct sockaddr_in servaddr;    /* server address */

    /* fill in the server's address and data */
    memset((char*)&servaddr, 0, sizeof(servaddr));
    getE();
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, get_ip(ip), &(servaddr.sin_addr));

    /* put the host's address into the server address structure */
    if(sendto(sockfd, &vector, sizeof(vector), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("sendUDP--sendto failure");
        return ;
    }
    else
    {}

}
void sendUDP(int port, uint32_t ip, int sockfd, struct routing_update_structure vector){
    getE();
    sendUDP1(port,ip,sockfd,vector);
}

void updatevectors()
{
    cost_vector = new int [num_routers];


    int i=0;
    while(i <num_routers)
    //for (int i = 0; i <num_routers; i++)
    {
        if(snap_pkt->router_array[i].link_cost == 0)
        {
            router_port = snap_pkt->router_array[i].router_port;
            cur_router_ip = snap_pkt->router_array[i].router_ip;
            data_port = snap_pkt->router_array[i].data_port;
            router_socket = create_router_socket();
            data_socket = create_data_socket();

            /* Register the router socket */

            FD_SET(router_socket, &master_list);
            if(router_socket <= head_fd){

            }
            else
            {head_fd = router_socket;}

            FD_SET(data_socket, &master_list);
            if(data_socket <= head_fd){
                getE();
            }
            else{
                head_fd = data_socket;
            }



            cur_router_index = i;
        }
        cost_vector[i] = snap_pkt->router_array[i].link_cost;
        i++;
    }
    cout<<"successful"<<endl;
    int j=0;
    while(j< num_routers)//for(int j = 0; j< num_routers; j++)
    {
        vectorMatrix[cur_router_index][j] = cost_vector[j];
        j++;
    }




    for(int i = 0; i < num_routers; i++)
    {
        for(int j = 0; j < num_routers; j++)
        {
            copy_vectorMatrix[i][j] = vectorMatrix[i][j];
        }
    }


}

int knowIndexOfRouter1(uint16_t router_id)
{   int i=0;
    while(i <num_routers) {

        if (snap_pkt->router_array[i].router_id == router_id)
        {return i;}
        i++;
    }
    cout<<"successful"<<endl;
    return -1;
}
int knowIndexOfRouter(uint16_t router_id){
    getE();
    return knowIndexOfRouter1(router_id);
}

void updateRouterCost1(unsigned char *cntrl_payload)
{

    uint16_t r_id =  ((unsigned int)cntrl_payload[0]<<8) | cntrl_payload[1];
    uint16_t r_cost = ((unsigned int)cntrl_payload[2]<<8) | cntrl_payload[3];
    cout<<"successful"<<endl;
    int r_index = knowIndexOfRouter(r_id);
    vectorMatrix[cur_router_index][r_index] = r_cost;
    bellmanFordAlgorithm();
    myNeighbourHopList.at(cur_router_index) = r_index;


}
void updateRouterCost(unsigned char *cntrl_payload){
    getE();
    updateRouterCost1(cntrl_payload);
}

void crashrrRouter(int sock_index)
{

    close(router_socket);
    FD_CLR(sock_index, &master_list);
}

int connect_to_host1(uint32_t nextHop_ip, uint16_t nextHop_port)
{
    int fdsocket = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in remote_server_addr;

    bzero(&remote_server_addr, sizeof(remote_server_addr));
    remote_server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, get_ip(nextHop_ip), &remote_server_addr.sin_addr);
    remote_server_addr.sin_port = htons(nextHop_port);
    int test = connect(fdsocket, (struct sockaddr*)&remote_server_addr, sizeof(remote_server_addr)) ;

    return fdsocket;
}
int connect_to_host(uint32_t nextHop_ip, uint16_t nextHop_port){
    getE();
    return connect_to_host1(nextHop_ip,nextHop_port);
}

void fillTheNextHopForSendingFile()
{   int i=0;
    while(i < num_routers)

    {
        if(snap_pkt->router_array[i].router_ip == send_pkt->dest_ip)
        {
            indexDestination = i;
            break;
        }
        i++;
    }
    indexNextHop = myNeighbourHopList.at(indexDestination);
    dataPortNextHop = snap_pkt->router_array[indexNextHop].data_port;
    destIPNextHop = snap_pkt->router_array[indexNextHop].router_ip;

}

int packetsSent = 0;
uint16_t seqnumber;

char* createSendPacket(unsigned char buff[1024], int packetsSent, int totalPacketsToBeSent, char* send_packet, bool firstPacket_send)
{

    uint16_t payload_len = 1024;
    uint16_t header_len = 12;
    uint16_t send_pkt_len;
    send_pkt_len = payload_len + header_len;
    cout<<"successful"<<endl;
    char *send_header;
    getE();
    send_header = (char *) malloc((sizeof(char) * header_len));

    struct sendFilePacketStructure *temp = (struct sendFilePacketStructure *) (send_header);
    temp->dest_ip = htonl(send_pkt->dest_ip);
    temp->transfer_id = send_pkt->transfer_id;
    temp->ttl = send_pkt->ttl;

    if(packetsSent != 0)
    {

        temp->seq_no = htons(send_pkt->seq_no + packetsSent);
    }
    else
    {
        temp->seq_no = htons(send_pkt->seq_no);

    }

    uint16_t f;

    if(packetsSent != totalPacketsToBeSent - 1)
    {

        temp->fin = htons(0);
        f = htons(0);

    }
    else
    {

        packetsSent = 0;

        temp->fin = htons(0x8000);
        f = htons(0x8000);

        firstPacket_send = true;

    }
    temp->pad = htons(0);

    char *last_packet;
    last_packet = (char *)malloc((sizeof(char) * 1036));

    char *last_pkt_header;
    last_pkt_header = ( char *) malloc((sizeof( char) * 12));

    bzero(last_packet, 1036);

    uint32_t dip = htonl(send_pkt->dest_ip);
    uint8_t tid = send_pkt->transfer_id;
    uint8_t tt = send_pkt->ttl;
    uint16_t sno = temp->seq_no;
    uint16_t p = htons(0);
    cout<<"successful"<<endl;

    memcpy(last_pkt_header, &dip, 4);
    memcpy(last_pkt_header + 4, &tid, 1);
    memcpy(last_pkt_header + 5, &tt, 1);
    memcpy(last_pkt_header + 6, &sno, 2);
    memcpy(last_pkt_header + 8, &f, 2);
    memcpy(last_pkt_header + 10, &p, 2);

    memcpy(last_packet, last_pkt_header, 12);
    free(last_pkt_header);



    memcpy(last_packet + 12, buff, 1024);


    //cout<<"4"<<endl;

    return last_packet;
}

void readingFileandSendingIt(string filename, bool firstPacket_send)
{

    int packetsSent = 0;

    fillTheNextHopForSendingFile();

    int totalPacketsToBeSent;

    int sockfd = connect_to_host(destIPNextHop, dataPortNextHop);
    getE();

    struct stat st;
    stat(filename.c_str(), &st);
    size = st.st_size;


    totalPacketsToBeSent = size / 1024;



    const char* fs_name = filename.c_str();
    unsigned char sdbuf[1024];
    cout<<"successful"<<endl;
    FILE *fs = fopen(fs_name, "r");
    if(fs == NULL)
    {

        exit(1);
    }
    else{
        getE();
    }
    bzero(sdbuf, 1024);
    int fs_block_sz;
    //unsigned char *buf;
    struct sendFilePacketStructure tempObj;
    statistics mystatsobj;
    int i=0;
    while(i <totalPacketsToBeSent)
    //for(int i = 0; i <totalPacketsToBeSent; i++)
    {
        if((fs_block_sz = fread(sdbuf, sizeof(unsigned char), 1024, fs)) > 0)
        {
            mystatsobj.transfer = send_pkt->transfer_id;
            mystatsobj.timetolive = send_pkt->ttl;
            mystatsobj.seqnum = send_pkt->seq_no + packetsSent;
            //cout<<"SEQUENCENUMBER AFTER RECEIVING: "<<mystatsobj.seqnum<<endl;

            mystats.push_back(mystatsobj);

            char *buf = ( char *)malloc((sizeof( char) * 1036));
            char *send_packet = ( char *)malloc((sizeof( char) * 1036));
            buf = createSendPacket(sdbuf, packetsSent, totalPacketsToBeSent, send_packet, firstPacket_send);

            memcpy(penultimate_last_packet_router,last_packet_router,1036);
            memcpy(last_packet_router,buf,1036);

            if(sendALL(sockfd, buf, 1036) >= 0){
                getE();
            }
            else{
                cout<<"successful"<<endl;
                free(buf);
                break;
            }

        }
        packetsSent++;
        i++;
    }
    fclose(fs);

}

void sendFile1(int sock_index, unsigned char* cntrl_payload, uint16_t payloadLen)
{
    firstPacket_send = true;
    string filename;

    send_pkt = (struct sendFileControlPayload *) malloc(sizeof(struct sendFileControlPayload));
    send_pkt->dest_ip = (((unsigned int)cntrl_payload[0]<<24) |
                         (cntrl_payload[1]<<16) |
                         (cntrl_payload[2]<<8)  |
                         cntrl_payload[3]);
    cout<<"successful"<<endl;

    send_pkt->ttl = cntrl_payload[4];
    send_pkt->transfer_id = cntrl_payload[5];

    send_pkt->seq_no = ((unsigned int)cntrl_payload[6]<<8) | cntrl_payload[7];

    getE();
    filename = string(reinterpret_cast<char*>(cntrl_payload + 8), (unsigned long)(payloadLen - 8));

    seqnumber = ntohs(send_pkt->seq_no);
    //cout<<"Filename: "<<filename<<endl;
    readingFileandSendingIt(filename, firstPacket_send);

}
void sendFile(int sock_index, unsigned char* cntrl_payload, uint16_t payloadLen){
    getE();
    sendFile1(sock_index,cntrl_payload,payloadLen);
}

int knowIndexOfRouterByRouterIP(uint32_t router_ip)
{
    int i=0;
    while(i<num_routers) {

        if (snap_pkt->router_array[i].router_ip == router_ip) {

            return i;
        }
        i++;
    }
        return -1;

}

//.
bool samePacket = true;
bool data_recv_hook(int sock_index)
{




    char *recv_header;
    char *last_packet_recv;
    char *recv_payload;
    last_packet_recv = (char *)malloc((sizeof(char) * 1036));

    char *last_pkt_header;
    last_pkt_header = (char *) malloc((sizeof(char) * 12));

    bzero(last_packet_recv, 1036);
    //unsigned char *last_pkt;
    cout<<"successful"<<endl;
    uint32_t dest_ip;
    uint8_t transfer_id;
    uint8_t ttl;
    uint16_t seq_no;
    uint16_t fin;
    uint16_t pad;

    int payload_len = 1036;

    recv_header = (char *) malloc(sizeof(char)*12);

    bzero(recv_header, 12);

    if(recvALL_C(sock_index, recv_header, 12) < 0){
        remove_data_conn(sock_index);
        free(recv_header);
        return false;
    }

    /* Get data from the header */
    struct RECV_HEADER *header = (struct RECV_HEADER *) recv_header;


    dest_ip = ntohl(header->dest_ip);
    transfer_id = header->transfer_id;
    ttl = header->ttl;
    seq_no = ntohs(header->seq_no);
    fin = ntohs(header->fin);
    pad = ntohs(header->pad);

    int destIndex = knowIndexOfRouterByRouterIP(dest_ip);


    /* Get data payload */
    if(payload_len != 0){
        recv_payload = ( char *) malloc(sizeof( char)*1024);
        bzero(recv_payload, 1024);

        if(recvALL_C(sock_index, recv_payload, 1024) < 0){
            remove_control_conn(sock_index);
            free(recv_payload);
            return false;
        }
    }

    ttl--;
    ttl++;
    ttl--;
    if(ttl == 0)
        return true;
    statistics mystats_recv;
    mystats_recv.transfer = transfer_id;
    mystats_recv.timetolive = ttl;
    mystats_recv.seqnum = seq_no;
    mystats.push_back(mystats_recv);

    uint32_t dest_ip_n;
    uint8_t transfer_id_n;
    uint8_t ttl_n;
    uint16_t seq_no_n;
    uint16_t fin_n;
    uint16_t pad_n;

    dest_ip_n = htonl(dest_ip);
    seq_no_n = htons(seq_no);
    fin_n = htons(fin);
    pad_n = htons(pad);
    ttl_n = ttl;
    transfer_id_n = transfer_id;

    memcpy(last_pkt_header, &dest_ip_n, 4);
    memcpy(last_pkt_header + 4, &transfer_id_n, 1);
    memcpy(last_pkt_header + 5, &ttl_n, 1);
    memcpy(last_pkt_header + 6, &seq_no_n, 2);
    memcpy(last_pkt_header + 8, &fin_n, 2);
    memcpy(last_pkt_header + 10, &pad_n, 2);

    memcpy(last_packet_recv, last_pkt_header, 12);
    free(last_pkt_header);


    memcpy(last_packet_recv + 12, recv_payload, 1024);

    memcpy(penultimate_last_packet_router,last_packet_router,1036);
    memcpy(last_packet_router,last_packet_recv,1036);
    cout<<"successful"<<endl;



    if(destIndex == cur_router_index)
    {
        recvcounter++;


        if(firstPacket_recv)
        {
            int tr_id = transfer_id;
            std::stringstream file;
            file << "file-" << tr_id;
            f_append = file.str();
            finame = f_append.c_str();
            firstPacket_recv = false;
        }
        cout<<"successful"<<endl;

        FILE *fr = fopen(finame, "a");
        if(fr == NULL){

        }
        else
        {
            int block_sz = 1024;
            int write_size;
            if((write_size = fwrite(recv_payload, sizeof(unsigned char), block_sz, fr)) > 0)
            {

                if(write_size < block_sz)
                {
                    perror("File write failed.\n");
                }
                free(recv_payload);
            }
            fclose(fr);
        }

        if(fin == 0x8000)
        {

            firstPacket_recv = true;
            cout<<"successful"<<endl;
        }

        //cout<<"jslkdg"<<endl;
    }
    else
    {

        int hopIndex = myNeighbourHopList.at(destIndex);

        uint16_t nextdataPort = snap_pkt->router_array[hopIndex].data_port;

        uint32_t nextdataIP = snap_pkt->router_array[hopIndex].router_ip;

        uint16_t payload_len = 1024;
        uint16_t header_len = 12;
        uint16_t send_pkt_len;




        if(firstPacket_recv && fin != 0x8000)
        {
            send_socket = connect_to_host(nextdataIP, nextdataPort);
            cout<<"successful"<<endl;
        }

        if(sendALL(send_socket, last_packet_recv, 1036) < 0)
        {
            perror("Sending failed:");
            return false;
        }




        free(last_packet_recv);



    }

    if(fin == 0x8000)
    {
        firstPacket_recv = true;
    }
    else
        firstPacket_recv = false;



    return true;
}

//

bool control_recv_hook(int sock_index)
{

    unsigned char *cntrl_header;
    unsigned char *cntrl_payload;
    uint8_t control_code;
    uint16_t payload_len;

    /* Get control header */
    cntrl_header = (unsigned char *) malloc(sizeof(unsigned char)*CNTRL_HEADER_SIZE);
    bzero(cntrl_header, CNTRL_HEADER_SIZE);
    cout<<"successful"<<endl;
    if(recvALL(sock_index, cntrl_header, CNTRL_HEADER_SIZE) >= 0)
    {
        getE();
    }
    else
    {
        remove_control_conn(sock_index);
        free(cntrl_header);
        return false;
    }

    /* Get control code and payload length from the header */
    struct CONTROL_HEADER *header = (struct CONTROL_HEADER *) cntrl_header;
    control_code = header->control_code;
    payload_len = ntohs(header->payload_len);

    getE();
    free(cntrl_header);

    /* Get control payload */
    if(payload_len == 0){
        getE();
    }
    else{
        cntrl_payload = (unsigned char *) malloc(sizeof(unsigned char)*payload_len);
        bzero(cntrl_payload, payload_len);
        //cout<<"Getting control payload"<<endl;
        if(recvALL(sock_index, cntrl_payload, payload_len) >= 0){
            getE();
        }
        else{
            remove_control_conn(sock_index);
            free(cntrl_payload);
            return false;
        }
    }
    int new_code;
    switch(control_code){
        case 0: new_code=0;
            break;
        case 1: new_code=1;
            break;
        case 2: new_code=2;
            break;
        case 3: new_code=3;
            break;
        case 4: new_code=4;
            break;
        case 5: new_code=5;
            break;
        case 6: new_code=6;
            break;
        case 7: new_code=7;
            break;
        case 8: new_code=8;
            break;
    }
    /* Triage on control_code */
    switch(new_code){
        case 0: author_response(sock_index);
            break;

        case 1: //cout<<"&&&&Control code 1"<<endl;
            init_response(sock_index);
            create_packet_structure(cntrl_payload);
            updatevectors();
            break;

        case 2: //cout<<"&&&&Control code 2"<<endl;
            routingTable_response(sock_index);
            break;

        case 3: //cout<<"&&&&Control code 3"<<endl;
            updateRouterCost(cntrl_payload);
            update_response(sock_index);
            break;

        case 4: //cout<<"&&&&Control code 4"<<endl;
            crashrrRouter(sock_index);
            crash_response(sock_index);
            break;

        case 5: //cout<<"&&&&Control code 5"<<endl;
            sendFile(sock_index, cntrl_payload, payload_len);
            send_response(sock_index);
            break;

        case 6: //cout<<"&&&&Control code 6"<<endl;
            //payload = fillStatistics(cntrl_payload);
            stats_response(sock_index, cntrl_payload);
            break;

        case 7: //cout<<"&&&&Control code 7"<<endl;
            last_data_response(sock_index);
            break;

        case 8: //cout<<"&&&&Control code 7"<<endl;
            penultimate_data_response(sock_index);
            break;

    }

    if(payload_len == 0){
        getE();
    }else
        free(cntrl_payload);
    return true;
}


void updateMatrix1(int *recvdVector)
{

    int indexToBeUpdated;
    int i=0;
    while(i < num_routers)

    {
        if(recvdVector[i] == 0)
            {indexToBeUpdated = i;}
        i++;
    }
    int j=0;
    while(j < num_routers)

    {
        vectorMatrix[indexToBeUpdated][j] = recvdVector[j];
        j++;
    }
    cout<<"successful"<<endl;

    bellmanFordAlgorithm();

    for(int i = 0; i < num_routers; i++)
    {
        for(int j = 0; j < num_routers; j++)
        {
            copy_vectorMatrix[i][j] = vectorMatrix[i][j];
        }
    }

}
void updateMatrix(int *recvdVector){
    getE();
    updateMatrix1(recvdVector);
}
//
void recvUDP1(int sock)
{
    countActiveRouters++;

    int *recvdVector;
    recvdVector = new (nothrow) int[num_routers];

    struct routing_update_structure rcvdVector;
    struct sockaddr_in sender;
    socklen_t sendsize = sizeof(sender);
    bzero(&sender, sizeof(sender));
    int testBytes = recvfrom(sock, &rcvdVector, sizeof(rcvdVector), 0, (struct sockaddr *)&sender, &sendsize);
    int ttt=testBytes;
    if( ttt >= 0){
        getE();
        cout<<"successful"<<endl;
    }else
    {
        perror("recvUDP--recvfrom failure aljbkd");
        return;
    }
    //cout<<"Bytes rcvd: "<<testBytes<<endl;
    uint16_t sender_port = ntohs(sender.sin_port);

    char* ipString = inet_ntoa(sender.sin_addr);


    int i=0;
    while(i< num_routers)

    {

        recvdVector[i]=(int)ntohs(rcvdVector.router_update_array[i].cost);
        i++;

    }


    if(counter >= periodic_interval){
        getE();
    }else {
        for (int i = 0; i < myNeighbours.size(); i++) {
            if (sender_port == myNeighbours.at(i).router_port) {
                myNeighbours.at(i).updateReceived = true;
            }
        }
    }

    updateMatrix(recvdVector);

}
void recvUDP(int sock){
    getE();
    recvUDP1(sock);
}
void main_loop()
{

    int selret, sock_index, fdaccept;
    tout.tv_sec = 100;
    tout.tv_usec = 0;

    test_time.tv_sec = 100;
    test_time.tv_usec = 0;
    int Ted=1;
    while(Ted>0)
    {
        watch_list = master_list;



        timestamp_t t0 = get_timestamp();

        selret = select(head_fd+1, &watch_list, NULL, NULL, &tout);
        numberofintervals++;
        counter++;
        timestamp_t t1 = get_timestamp();


        double secs = (t1 - t0) / 1000000.0L;


        if(selret < 0)
            perror("select failed.");

        else if(selret == 0)
        {
            if(counter % periodic_interval != 0){
                getE();
            }else {
                counter = 0;
            }

            tout.tv_sec = periodic_interval;
            struct routing_update_structure sendUpdatePacket;
            sendUpdatePacket = buildUpdatePacketStructure();

            int sockfd = create_send_socket();
            int i=0;
            while(i < myNeighbourHopList.size())

            {
                if(myNeighbourHopList.at(i) == -1){
                    int sklf;
                    cout<<"successful"<<endl;
                }else
                {
                    sendUDP(snap_pkt->router_array[myNeighbourHopList.at(i)].router_port, snap_pkt->router_array[myNeighbourHopList.at(i)].router_ip, sockfd, sendUpdatePacket);
                }
                i++;
            }

        }
        else
        {
            /* Loop through file descriptors to check which ones are ready */
            sock_index=0;
            while(sock_index<=head_fd)

            {

                if(FD_ISSET(sock_index, &watch_list)){

                    /* control_socket */
                    if(sock_index == control_socket){
                        fdaccept = new_control_conn(sock_index);
                        cout<<"successful"<<endl;
                        FD_SET(fdaccept, &master_list);

                        if(fdaccept > head_fd) head_fd = fdaccept;
                    }

                        /* router_socket */
                    else if(sock_index == router_socket){
                        //cout<<"Inside router socket"<<endl;
                        recvUDP(router_socket);
                        //call handler that will call recvfrom() .....
                    }

                        /* data_socket */
                    else if(sock_index == data_socket){
                        //cout<<"Inside data socket"<<endl;
                        fdaccept = new_data_conn(sock_index);

                        FD_SET(fdaccept, &master_list);
                        if(fdaccept > head_fd) head_fd = fdaccept;
                    }

                        /* Existing connection */
                    else
                    {


                        if(isData(sock_index)){
                            if(!data_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
                        }
                        else
                        {
                            if(!control_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
                        }

                    }
                }
                sock_index+=1;
            }
        }
    }
}

int create_control_socket1()
{

    struct sockaddr_in control_addr;
    socklen_t addrlen = sizeof(control_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        perror("socket() sfd failed");

    bzero(&control_addr, sizeof(control_addr));

    control_addr.sin_family = AF_INET;
    control_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    control_addr.sin_port = htons(control_port);
    cout<<"successful"<<endl;
    if(bind(sock, (struct sockaddr *)&control_addr, sizeof(control_addr)) >= 0){
        getE();
    }else{
        perror("bind() failed");}

    if(listen(sock, 5) < 0){
        perror("listen() failed");
        getE();}


    return sock;
}
int create_control_socket(){
    getE();
    create_control_socket1();
}
void initialisation1()
{

    control_socket = create_control_socket();

    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);
    cout<<"successful"<<endl;
    FD_SET(control_socket, &master_list);
    head_fd = control_socket;
    main_loop();
}
void initialisation(){
    getE();
    initialisation1();
}

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
int main(int argc, char **argv)
{

    penultimate_last_packet_router = ( char*)(malloc(1036));
    last_packet_router = ( char*)(malloc(1036));
    bzero(penultimate_last_packet_router, 1036);
    bzero(last_packet_router, 1036);
    control_port = atoi(argv[1]);
    initialisation();
    return 0;
}

