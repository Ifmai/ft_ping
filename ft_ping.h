#ifndef FT_PING_H

	#define FT_PING_H

	#define TRUE 1
	#define FALSE 0
	#define NO_FLAG 2
	#define PACKET_SIZE 64


	typedef struct t_flags
	{
		int verbose_mode;   // -v
		int time_to_live;   // --ttl
		char * destination;

	} s_flags;

	extern s_flags global_flag;
	
	#include <stdio.h>
	#include <errno.h>
	#include <ctype.h>
	#include <netdb.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <netinet/ip_icmp.h>

	long    		get_current_time();
	void 			program_check(int ac, char *argv[]);
	unsigned short  calculate_checksum(void *b, int len);
	void 			resolve_address(const char *destination, struct sockaddr_in *dest_addr);

#endif