#include "headers.h"

char *outFile ;

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

void CreateEthernetHeader(struct frame *header, char *src_mac, char *dst_mac, int protocol)
{
    
    //ethernet_header = (struct sniff_ethernet *)malloc(sizeof(struct sniff_ethernet));

    /* copy the Src mac addr */
    memcpy(header->ether_shost, (void *)ether_aton(src_mac), 6);

    /* copy the Dst mac addr */
    memcpy(header->ether_dhost, (void *)ether_aton(dst_mac), 6);

    /* copy the protocol */
    header->ether_type = htons(protocol);
}




int main(int argc, char **argv){

    char* interface = argv[1];
    unsigned char *packet_header;

    struct sockaddr sa;
    int sock, ethhdr_len;
    uint16_t len ;
    unsigned char buffer[100] ;
    struct frame header ;
    FILE *fp;
    struct ifreq ethreq;
    long int counter = 0;

    if(argc != 6){
        printf("Wrong number of arguments\n") ;
	printf("Usage:\n\t./node interface <input file> <own mac> <router mac> <output file>\n\n") ;
	exit(0) ;
    }

    outFile = argv[5] ;

    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) { 
        printf("when opening socket in PAListener");
    }

    fp = fopen(argv[2], "rb") ;
    if(fp == NULL)
        printf("No such file exists\n") ;

    header.type = 0x4e ;

    BindRawSocketToInterface(argv[1], sock, ETH_P_ALL);
    CreateEthernetHeader(&header, argv[3], argv[4], ETH_P_ALL);
    ethhdr_len = sizeof(struct sniff_ethernet);
    pthread_t interfaceSniffer;
    int res ;
    res = pthread_create(&interfaceSniffer, NULL, sniffer, (void *)sock); 
    if( res != 0){
	fprintf(stderr, "TCP read thread creation failed\n") ;
	exit(EXIT_FAILURE) ;
    }

    printf("hello\n") ;
    int wait ;
    scanf("Enter when ready %d", &wait) ;

    while(!feof(fp)){
	if (fscanf(fp, "%s" , header.buf) == EOF) 
	    break ;
	printf("Sent %s\n", header.buf) ;

        if(write(sock,&header,ethhdr_len+FRAME_LEN) < 0){
            perror("sendto");
        }
        usleep(100) ;
    }
    pthread_join(interfaceSniffer, NULL) ;
}

void *sniffer(void *arg){
    int sock = (long)arg ;
    struct frame *header ;
    int length ;
    unsigned char *buffer ;
    int ethhdr_len = sizeof(struct sniff_ethernet);
    buffer = (unsigned char*)malloc(FRAME_LEN+ethhdr_len); 
    FILE *fp ;
    int counter = 0 ;
    fp = fopen(outFile, "w") ;
    if(fp == NULL)
	printf("Error in file opening\n") ;

    while(1){
	length = read(sock, buffer, FRAME_LEN + ethhdr_len);

	if(length != FRAME_LEN+ethhdr_len)
	    continue ;

	header = (struct frame *)(buffer);
	if(header->type != 0x4e)
	    continue ;
	++counter ;
	printf("Reeived %d\r", counter) ;

	fprintf(fp, "%s ", header->buf) ;
	fflush(fp) ;
	
    }
}
