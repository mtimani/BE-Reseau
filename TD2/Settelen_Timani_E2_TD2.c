/* Travail réalisé par :
 SETTELN Antoine et 
 TIMANI Mario*/

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

int mic_tcp_accept(int socket, mic_tcp_sock_addr *addr) {
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    if(mode==SERVER && mysocket.fd==socket){

        //WAIT FOR SYN
        mysocket.state = WAIT_FOR_SYN;
        SYN.payload.size= 2*sizeof(short)+2*sizeof(int)+3*sizeof(char);
        SYN.payload.data=malloc(SYN.payload.size);

        //Reception SYN
        unsigned long timer1=1;
        while(IP_recv(SYN, &mysocket_distant.addr,timer1)==-1){};

        // Construire SYNACK
        mic_tcp_pdu SYNACK={{mysocket.addr.port, mysocket_distant.addr.port, num_seq, 0,1,1,0},{"",0}};

        while(1){
            //Activation timer
            unsigned long timer=1;

            //Envoi SYNACK
            IP_send(SYNACK, mysocket_distant.addr);

            //WAIT FOR ACK
            mysocket.state = WAIT_FOR_ACK;
            ACK.payload.size= 2*sizeof(short)+2*sizeof(int)+3*sizeof(char);
            ACK.payload.data=malloc(ACK.payload.size);

            if (IP_recv(ACK,&mysocket_distant.addr,timer)==-1) {
                //Expiration timer ou non reception ACK
                printf("ACK non reçu\n") ;
            }
            else {
                //Reception ACK
                ACK.hd=get_mic_tcp_header(ACK.payload);
                if(ACK.hd.ack==1){
                    //Connected
                    mysocket.state = CONNECTED;
                    return 0;
                }
            }
        }
    }
    else{
        return -1;
    }
}

int mic_tcp_connect (int socket, mic_tcp_sock_addr addr) {
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    if(mode==CLIENT && mysocket.fd==socket){

        // Construire SYN={{//Header},{//payload}};
        mic_tcp_pdu SYN={{mysocket.addr.port, mysocket_distant.addr.port, num_seq, 0,1,0,0},{"",0}};

        while(1){
            // Activation timer
            unsigned long timer=1;

            // Envoi SYN
            IP_send(SYN, mysocket_distant.addr);

            // WAIT for SYNACK
            mysocket.state = WAIT_FOR_SYNACK;

            SYNACK.payload.size= 2*sizeof(short)+2*sizeof(int)+3*sizeof(char);
            SYNACK.payload.data=malloc(SYNACK.payload.size);

            if (IP_recv(SYNACK,&mysocket_distant.addr,timer)==-1) {
                //Expiration timer ou non reception SYNACK
                printf("SYNACK non reçu\n") ;
            }
            else {
                // Reception SYNACK
                SYNACK.hd=get_mic_tcp_header(SYNACK.payload);
                if(SYNACK.hd.syn==1 && SYNACK.hd.ack==1){
                    // Construire ACK
                    mic_tcp_pdu ACK={{mysocket.addr.port, mysocket_distant.addr.port, num_seq,0,0,1,0},{"",0}};

                    //Envoi ACK
                    IP_send(ACK, mysocket_distant.addr);

                    //Connected
                    mysocket.state = CONNECTED;
                    return 0;
                }
            }
        }
    }
    else{
        return -1;
    }
}
