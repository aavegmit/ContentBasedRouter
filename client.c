#include "headers.h"

int main(int argc, char **argv){

    char* interface = argv[1];
    struct sockaddr sa;
    int sock;
    uint16_t len ;
    unsigned char buffer[50] ;
    struct frame header ;
    FILE *fp;

    if(argc != 3)
	printf("Wrong number of arguments\n") ;

    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) { 
	printf("when opening socket in PAListener");
    }

    fp = fopen(argv[2], "rb") ;
    if(fp == NULL)
	printf("No such file exists\n") ;

    header.type = 0x4e ;
    
    strcpy(sa.sa_data,interface) ;
    while(!feof(fp)){
	len = fread(buffer, 1, 50, fp) ;
	header.len = len ;

//	if(sendto(sock,&header,sizeof(struct frame) ,0,&sa,sizeof(sa)) < 0){
//	    perror("sendto");
//	}

	if(sendto(sock,buffer,len ,0,&sa,sizeof(sa)) < 0){
	    perror("sendto");
	}
    }

}
