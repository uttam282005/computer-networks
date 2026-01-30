struct addrinfo {
int ai_flags;// AI_PASSIVE, AI_CANONNAME, etc.
int ai_family;// AF_INET, AF_INET6, AF_UNSPEC
int ai_socktype;// SOCK_STREAM, SOCK_DGRAM
int ai_protocol;// use 0 for "any"
size_t ai_addrlen;// size of ai_addr in bytes
struct sockaddr *ai_addr;
char
// struct sockaddr_in or _in6
*ai_canonname; // full canonical hostname
struct addrinfo *ai_next;
// linked list, next node
};

struct sockaddr {
unsigned shortsa_family;// address family, AF_xxx
char sa_data[14];// 14 bytes of protocol address
};

// (IPv4 only--see struct sockaddr_in6 for IPv6)
struct sockaddr_in {
short int
sin_family;
// Address family, AF_INET
unsigned short int sin_port;
// Port number
struct in_addrsin_addr;
// Internet address
unsigned charsin_zero[8]; // Same size as struct sockaddr
}
