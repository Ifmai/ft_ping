#include "ft_ping.h"

static void flag_help();
static int is_valid_domain_or_ip(char * const domain);
static int is_valid_flag(const char *target);

void program_check(int ac, char* argv[]){
	if(ac < 2){
		fprintf(stderr,"ft_ping: usage -> <flag> <destination>\n");
		exit (EXIT_FAILURE);
	}
	else {
		int i = 1;
		int valid_flag = NO_FLAG;
		int valid_destination = FALSE;
	
		while(argv[i]){
			if(argv[i][0] == '-')
				valid_flag = is_valid_flag(argv[i]);
			else
				valid_destination = is_valid_domain_or_ip(argv[i]);
			i++;
		}
		if (valid_destination == FALSE){
			fprintf(stderr, "ft_ping: IP address is not valid\n");
			exit (EXIT_FAILURE);
		}
		else if(valid_flag == FALSE){
			fprintf(stderr, "ft_ping: Flag is not valid\n");
			exit (EXIT_FAILURE);
		}
	}
}

long    get_current_time(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void resolve_address(const char *destination, struct sockaddr_in *dest_addr) {
    if (inet_pton(AF_INET, destination, &dest_addr->sin_addr) > 0)
        return;
    else
    {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(destination, NULL, &hints, &res) == 0)
        {
            struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
            memcpy(dest_addr, addr, sizeof(struct sockaddr_in));
            freeaddrinfo(res);
            return;
        }
        else
        {
            perror("DNS resolution failed");
            exit(EXIT_FAILURE);
        }
    }
}

unsigned short  calculate_checksum(void *b, int len){
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

static int is_valid_domain_or_ip(char * const domain){
    struct sockaddr_in sa;
	if (inet_pton(AF_INET, domain, &(sa.sin_addr)) == 1){
		global_flag.destination = domain;
        return TRUE;
	}
	
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(domain, NULL, &hints, &res) == 0){
		freeaddrinfo(res);
		global_flag.destination = domain;
		return (TRUE);
	}

	return (FALSE);
}

static int is_valid_flag(const char *target){
	int valid = TRUE;
	if (strcmp(target, "-v") == 0)
		global_flag.verbose_mode = TRUE;
	else if (strcmp(target, "-?") == 0 || strcmp(target, "-h") == 0)
		flag_help();
	else
		valid = FALSE;
	return (valid);
}

static void flag_help(){
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

