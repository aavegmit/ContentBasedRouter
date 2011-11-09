#include "headers.h"


int main(int argc, char **argv){

    unsigned char* buffer, content[100] ;
    int length, s;
    struct ifreq ethreq;
    struct frame *header ;
    FILE *fp ;

    s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (s == -1) { 
	printf("when opening socket in PAListener");
	return NULL;
    }

    fp = fopen(argv[2], "wb") ;
    if(fp == NULL){
	printf("Error opening the file to write\n") ;
    }

    /* set to promiscuous mode */
    strcpy(ethreq.ifr_name,argv[1]);
    ioctl(s, SIOCGIFFLAGS, &ethreq);
    ethreq.ifr_flags |= IFF_PROMISC;
    ioctl(s, SIOCSIFFLAGS, &ethreq);

    /*Buffer for ethernet frame*/
    buffer = (unsigned char*)malloc(ETH_FRAME_LEN); 
    length = 0; 

    while(1){

	length = recvfrom(s, buffer, ETH_FRAME_LEN, 0, NULL, NULL);


	if(length != FRAME_LEN)
	    continue ;

	header = (struct frame *)buffer ;
	printf("type %02x len %02x\n", header->type, length) ;
	if(header->type != 0x4e)
	    continue ;

	length = recvfrom(s, content, header->len, 0, NULL, NULL);
	fwrite(content, 1, header->len, fp) ;

    }

}
