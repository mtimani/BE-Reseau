#include <mictcp.h>
#include <api/mictcp_core.h>

mic_tcp_sock socket;
int nextfd = 0;

int mic_tcp_socket(start_mode sm){
    socket.fd = nextfd++;
    socket.protocol.state = IDLE;

    if(initialize_components(sm) == -1){
        return -1;
    }

    return socket.fd;
}

int mic_tcp_bind(int socket, mic_tcp_sock_addr addr){
    if (mysocket.fd == socket){
        memcpy((char *)&mysocket.addr, (char*)&addr, sizeof(mic_tcp_sock_addr));
        return 0;
    }
    else return -1;
}

int sn = 0;
int mic_tcp_send(int fd, char *mesg, int mesg_size){
    mic_tcp_PDU p;

    p.header.source_port = 9000;
    p.header.dest_port = 2000;
    p.header.seq_num = sn++;
    p.header.ack_num = 0;
    p.header.syn = 0;
    p.header.ack = 0;
    p.header.fin = 0;

    p.payload.data = mesg;
    p.payload.size = mesg_size;

    int t = IP_send(p,addr_sock_dest);
    int r = IP_recv(ack,timout);
    if(timeout){
        //Reboucler sur IP_send
    }
    else if (ack.seq_num != (sn-1)){
        //Reboucler sur IP_send
    }
    return t;
}

int mic_tcp_recv(int fd, char *mesg, int max_mesg_size){
    int nb_octets_lus;
    mic_tcp_payload pdu;

    pdu.data = mesg;
    pdu.size = max_mesg_size;
    if (mysock.fd == socket && mysock.state == CONNECTED){
        mysock.state = WAIT_FOR_PDU;
        nb_octets_lus = app_buffer_get(pdu);
        mysock.state = CONNECTED;
        return nb_octets_lus;
    }
    else{
        return -1;
    }
}

void process_recieved_PDU(mic_tcp_PDU pdu, mic_tcp_sock_addr addr){
    mic_tcp_pdu ack;
    ack.header.ack = 1;
    ack.header.ack_num = p.header.seq_num;
    ack.payload.size = 0;
    IP_send(ack,addr_sock_source);
    app_buffer_put(pdu.payload);
}

