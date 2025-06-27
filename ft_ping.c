#include "ft_ping.h"

static int open_socket();
static void start_ping(const char *destination, int sockfd);

s_flags global_flag = {
	.verbose_mode = FALSE,
	.time_to_live = 64
};


int main(int ac, char *argv[]){
	program_check(ac, argv);

	int sockfd = open_socket();
	start_ping(global_flag.destination, sockfd);
}


static int open_socket(){
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sockfd < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &global_flag.time_to_live, sizeof(global_flag.time_to_live)) < 0){
		perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
	}

	return (sockfd);
}

static void start_ping(const char *destination, int sockfd){
	struct sockaddr_in dest_addr;
	struct icmp packet;
	
    memset(&packet, 0, sizeof(packet));
    memset(&dest_addr, 0, sizeof(dest_addr));
    resolve_address(destination, &dest_addr);
	dest_addr.sin_family = AF_INET;

	packet.icmp_type = ICMP_ECHO;
    packet.icmp_code = 0;
    packet.icmp_id = getpid();

	char recv_buffer[1024];
    int seq = 0;

	if(global_flag.verbose_mode)
		flag_verbose(global_flag.destination);
	printf("PING %s (%s): %d data bytes\n", destination, destination, PACKET_SIZE);

	while (TRUE){
		seq++;
		packet.icmp_seq = seq;
		packet.icmp_cksum = 0;
		packet.icmp_cksum = calculate_checksum(&packet, sizeof(packet));
		
		long start_time = get_current_time();
		socklen_t addr_len = sizeof(dest_addr);

		if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, addr_len) == 0){
			perror("sendto");
			continue;
		}
		
        if (recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&dest_addr, &addr_len) <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                if (global_flag.verbose_mode)
                    printf("[VERBOSE] ICMP request timeout for sequence %d\n", seq);
                printf("Request timeout for icmp_seq %d\n", seq);
            }
            else
                perror("recvfrom");
            continue;
        }

        struct iphdr *ip_header = (struct iphdr *)recv_buffer;
        int ttl = ip_header->ttl;

        long end_time = get_current_time();
		printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
			PACKET_SIZE, destination, inet_ntoa(dest_addr.sin_addr), seq, ttl, (end_time - start_time) / 1000.0);
		usleep(1000000);
	}
}


