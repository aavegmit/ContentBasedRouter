#include <pthread.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <netinet/if_ether.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#define FRAME_LEN 3

struct sniff_ethernet {
    u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
    u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
    u_short ether_type;                     /* IP? ARP? RARP? etc */
};

struct frame {
    u_char type ;	// 0x4e - packet type for content based routing
    uint16_t len ;
};
