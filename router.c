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
    int sock[4] ;
    FILE *fp ;
    struct frame header[4] ;

    for(int i = 0 ; i < 4 ; ++i){
	sock[i] = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock[i] == -1) { 
	    printf("when opening socket in PAListener");
	}
    }

//    fp = fopen(argv[1], "r") ;
//    if(fp == NULL)
//        printf("No such file exists\n") ;


    // Bind interfaces
    BindRawSocketToInterface(INTERFACE_1, sock[0], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_2, sock[1], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_3, sock[2], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_4, sock[3], ETH_P_ALL);

    // Create headers
//    CreateEthernetHeader(&header[0], RTR_ETHER_1, NODE0_ETHER, ETH_P_ALL);
//    CreateEthernetHeader(&header[1], RTR_ETHER_2, NODE1_ETHER, ETH_P_ALL);
//    CreateEthernetHeader(&header[2], RTR_ETHER_3, NODE2_ETHER, ETH_P_ALL);
//    CreateEthernetHeader(&header[3], RTR_ETHER_4, NODE3_ETHER, ETH_P_ALL);

//    for(int i = 0 ; i < 4 ; ++i)
//	header[i].type = 0x4e ;
//
//    char color[10] ;
//    while(!feof(fp)){
//	fscanf(fp, "%s" , color) ;
//	printf("%s-", color) ;
//    }

    while(1){

    }


}
