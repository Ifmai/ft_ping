#include "ft_ping.h"

void flag_verbose(char *domain){
    
    struct addrinfo hints, *res, *ai;
    int sock4 = -1, sock6 = -1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    int ret = getaddrinfo(domain, NULL, &hints, &res);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(ret));
        exit (EXIT_FAILURE);
    }

    for (ai = res; ai != NULL; ai = ai->ai_next) {
        if (ai->ai_family == AF_INET) {
            sock4 = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        } else if (ai->ai_family == AF_INET6) {
            sock6 = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
        }
    }

    printf("ping: sock4.fd: %d (socktype: SOCK_RAW), sock6.fd: %d (socktype: SOCK_RAW), hints.ai_family: AF_UNSPEC\n\n", sock4, sock6);
    printf("ai->ai_family: AF_INET, ai->ai_canonname: '%s'\n", domain);

    if (sock4 != -1) 
        close(sock4);
    if (sock6 != -1) 
    close(sock6);
    freeaddrinfo(res);
}

void flag_help(){
	printf(
		"Usage\n"
		"  ping [options] <destination>\n\n"

		"Options:\n"
		"  <destination>      dns name or ip address\n"
		"  -a                 use audible ping\n"
		"  -A                 use adaptive ping\n"
		"  -B                 sticky source address\n"
		"  -c <count>         stop after <count> replies\n"
		"  -C                 call connect() syscall on socket creation\n"
		"  -D                 print timestamps\n"
		"  -d                 use SO_DEBUG socket option\n"
		"  -e <identifier>    define identifier for ping session, default is random for\n"
		"                     SOCK_RAW and kernel defined for SOCK_DGRAM\n"
		"                     Imply using SOCK_RAW (for IPv4 only for identifier 0)\n"
		"  -f                 flood ping\n"
		"  -h                 print help and exit\n"
		"  -I <interface>     either interface name or address\n"
		"  -i <interval>      seconds between sending each packet\n"
		"  -L                 suppress loopback of multicast packets\n"
		"  -l <preload>       send <preload> number of packages while waiting replies\n"
		"  -m <mark>          tag the packets going out\n"
		"  -M <pmtud opt>     define mtu discovery, can be one of <do|dont|want>\n"
		"  -n                 no dns name resolution\n"
		"  -O                 report outstanding replies\n"
		"  -p <pattern>       contents of padding byte\n"
		"  -q                 quiet output\n"
		"  -Q <tclass>        use quality of service <tclass> bits\n"
		"  -s <size>          use <size> as number of data bytes to be sent\n"
		"  -S <size>          use <size> as SO_SNDBUF socket option value\n"
		"  -t <ttl>           define time to live\n"
		"  -U                 print user-to-user latency\n"
		"  -v                 verbose output\n"
		"  -V                 print version and exit\n"
		"  -w <deadline>      reply wait <deadline> in seconds\n"
		"  -W <timeout>       time to wait for response\n\n"

		"IPv4 options:\n"
		"  -4                 use IPv4\n"
		"  -b                 allow pinging broadcast\n"
		"  -R                 record route\n"
		"  -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>\n\n"

		"IPv6 options:\n"
		"  -6                 use IPv6\n"
		"  -F <flowlabel>     define flow label, default is random\n"
		"  -N <nodeinfo opt>  use icmp6 node info query, try <help> as argument\n\n"

		"For more details see ping(8).\n"
	);
	exit (EXIT_SUCCESS);
}
