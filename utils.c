#include "ft_ping.h"

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


