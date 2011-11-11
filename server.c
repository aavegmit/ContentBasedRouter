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

    unsigned char* buffer, content[100] ;
    int length, s, ethhdr_len;
    struct ifreq ethreq;
    struct frame *header ;
    struct sniff_ethernet *ethernet_header;
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
    ethhdr_len = sizeof(struct sniff_ethernet);
    buffer = (unsigned char*)malloc(FRAME_LEN+ethhdr_len); 
    length = 0; 
    BindRawSocketToInterface(argv[1], s, ETH_P_ALL);

    while(1){

//	length = recvfrom(s, buffer, FRAME_LEN, 0, NULL, NULL);
/*    length = read(s, &ethernet_header, sizeof(struct ethhdr));
    if(length!=14){
        continue;
        printf("Incorrect header\n");
    }*/
	length = read(s, buffer, FRAME_LEN + ethhdr_len);

	if(length != FRAME_LEN+ethhdr_len)
	    continue ;

    ethernet_header = (struct sniff_ethernet *)buffer ;
	header = (struct frame *)(buffer + ethhdr_len );
	printf("type %02x len %d\n", header->type, header->len) ;
	if(header->type != 0x4e)
	    continue ;

//	length = read(s, content, header->len);
	for(int i = 0 ; i < header->len ; ++i)
	    printf("%02x-", header->buf[i]) ;
	printf("\n") ;
	fwrite(header->buf, 1, header->len, fp) ;
	fflush(fp) ;
    }
}
