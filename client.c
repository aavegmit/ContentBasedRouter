#include "headers.h"
#include "fileIO.h"

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
    off_t fileSize = FILE_SIZE*1024*1024;
    struct sockaddr sa;
    int sock, ethhdr_len;
    //uint16_t len ;
    off_t len;
    unsigned char buffer[100] ;
    struct frame header ;
    FILE *fp;
    struct ifreq ethreq;
    long int counter = 0;

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

    /*fp = fopen(argv[2], "rb") ;
      if(fp == NULL)
      printf("No such file exists\n") ;*/
    loadFileToMMap((unsigned char *)argv[2]);

    header.type = 0x4e ;

    BindRawSocketToInterface(argv[1], sock, ETH_P_ALL);
    CreateEthernetHeader(&header, SRC_ETHER_ADDR, DST_ETHER_ADDR, ETH_P_ALL);
    ethhdr_len = sizeof(struct sniff_ethernet);

    //while(!feof(fp)){
    while((counter*50)<fileSize){
        //len = fread(header.buf, 1, 50, fp) ;
        len = (fileSize)-(counter*50);
        if(len<50){
            memcpy(header.buf, &fileMap[counter*50] , len);
            header.len = len;
        }
        else{
            memcpy(header.buf, &fileMap[counter*50] , FRAME_LEN-4);
            header.len = 50;
        }

        //header.len = len ;


        //printf("Sending %d bytes\n", len) ;
        /*for(int i = 0 ; i < len ; ++i)
          printf("%02x-", header.buf[i]) ;
          printf("\n") ;*/

        /* Writing ethernet header first */
        //memcpy(buffer, packet_header, ethhdr_len);
        //memcpy(&buffer[ethhdr_len], &header, FRAME_LEN);

        if(write(sock,&header,ethhdr_len+FRAME_LEN) < 0){
            perror("sendto");
        }
        //usleep(0) ;
        for(int i = 0;i<500;i++);

        counter++;
        //printf("Packet sent: %ld\n", counter++);
        //memset(buffer, '\0', 100);

        //	if(write(sock,buffer,len ) < 0){
        //	    perror("sendto");
        //	}
    }
    printf("Finish sending data...\n");
}
