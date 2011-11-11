#include "headers.h"

int BindRawSocketToInterface(char *device, int rawsock, int protocol)
{

    struct sockaddr_ll sll;
    struct ifreq ifr;

    bzero(&sll, sizeof(sll));
    bzero(&ifr, sizeof(ifr));

    /* First Get the Interface Index  */


    strncpy((char *)ifr.ifr_name, device, IFNAMSIZ);
    if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)
    {
	printf("Error getting Interface index !\n");
	exit(-1);
    }

    /* Bind our raw socket to this interface */

    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(protocol); 


    if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
    {
	perror("Error binding raw socket to interface\n");
	exit(-1);
    }

    return 1;

}




int main(int argc, char **argv){

    char* interface = argv[1];
    struct sockaddr sa;
    int sock;
    uint16_t len ;
    unsigned char buffer[50] ;
    struct frame header ;
    FILE *fp;
    struct ifreq ethreq;

    if(argc != 3)
	printf("Wrong number of arguments\n") ;

    //    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    //    if (sock == -1) { 
    //	printf("when opening socket in PAListener");
    //    }

    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) { 
	printf("when opening socket in PAListener");
    }

    fp = fopen(argv[2], "rb") ;
    if(fp == NULL)
	printf("No such file exists\n") ;

    header.type = 0x4e ;

    BindRawSocketToInterface(argv[1], sock, ETH_P_ALL);

//    strcpy(sa.sa_data,interface) ;
    while(!feof(fp)){
	len = fread(header.buf, 1, 50, fp) ;
	header.len = len ;

	printf("Sending %d bytes\n", len) ;
	for(int i = 0 ; i < len ; ++i)
	    printf("%02x-", header.buf[i]) ;
	printf("\n") ;

	if(write(sock,&header,FRAME_LEN) < 0){
	    perror("sendto");
	}
	sleep(1) ;

//	if(write(sock,buffer,len ) < 0){
//	    perror("sendto");
//	}
    }
}
