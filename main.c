/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:49:46 by amathias          #+#    #+#             */
/*   Updated: 2017/11/06 17:33:35 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_sockaddr(t_env *e, const char *addr)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	s = getaddrinfo(addr, NULL, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
		{
			close(sfd);
			break;
		}
		close(sfd);
	}
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	e->addr = result;
}

double	get_time_elapsed(struct timeval *t1, struct timeval *t2)
{

	double time_ms1 = (t1->tv_sec) * 1000 + (t1->tv_usec) / 1000;
	double time_ms2 = (t2->tv_sec) * 1000 + (t2->tv_usec) / 1000;


	return (time_ms2 - time_ms1);
}

uint16_t	swap_byte16_t(uint16_t val)
{
	return ((val << 8) | (val >> 8));
}

void	ping_connect(t_env *e)
{
	e->socket = X(-1, socket(PF_INET, SOCK_RAW, IPPROTO_ICMP), "socket");
	//X(-1, setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)), "setsockopt");
}

void	ping_send(t_env *e)
{
	t_packet		packet;

	ft_memset(&packet, 0, sizeof(t_packet));
	packet.icmp.icmp_type = (uint8_t)ICMP_ECHO;
	packet.icmp.icmp_code = (uint8_t)0;

	packet.icmp.icmp_id = swap_byte16_t((uint16_t)getpid());
	packet.icmp.icmp_seq = swap_byte16_t((uint16_t)++e->counter);

	//gettimeofday (&packet.payload.send_at, NULL);
	for (int i = 0; i < 36; i++)
		packet.data[i] = 10 + i;
	packet.icmp.icmp_cksum = checksum((uint16_t*)&packet, sizeof(t_packet));
	X(-1, sendto(e->socket, &packet, sizeof(t_packet), 0,
				e->addr->ai_addr, e->addr->ai_addrlen), "sendto");
	hexdump(&packet, sizeof(t_packet));
}

void	ping_receive(t_env *e)
{
	t_rpacket		received;
	struct msghdr	msg_header;
	struct iovec	iovec;
	char			control[64];
	int				byte_recv;

	ft_memset(&msg_header, 0, sizeof(struct msghdr));
	ft_memset(&iovec, 0, sizeof(struct iovec));
	ft_memset(&received, 0, sizeof(t_rpacket));
	iovec.iov_base = &received;
	iovec.iov_len = sizeof(t_rpacket);
	msg_header.msg_name = e->addr->ai_addr;
	msg_header.msg_namelen = e->addr->ai_addrlen;
	msg_header.msg_iov = &iovec;
	msg_header.msg_iovlen = 1;
	msg_header.msg_control = &control;
	msg_header.msg_controllen = sizeof(char) * 64;
	msg_header.msg_flags = 0;
	if ((byte_recv = recvmsg(e->socket, &msg_header, 0)))
	{
		char * temp = msg_header.msg_iov[0].iov_base;//The obtained data
		temp[byte_recv] = '\0';//Add at the end of the end for data
		printf("\n");
		hexdump(&received.icmp, 64);
		printf("received pid: %d|%d\n", getpid(), received.icmp.icmp_id);
		printf("sequence: %hu\n", received.icmp.icmp_seq);
		double elap = 0.0f;
		//gettimeofday (&received.payload.send_at, NULL);
		//double elap = get_time_elapsed(&packet.payload.send_at,
		//				&received.payload.send_at);
		//printf("elap: %f\n", elap);
		display_response(e, byte_recv, received.icmp.icmp_seq, (double)elap);
	} else {
		printf("not recvmsg\n");
	}
}

void	ping_host(t_env *e)
{

	ping_connect(e);
	ping_send(e);
	ping_receive(e);
}

int main(int argc, char *argv[])
{
	t_env e;

	ft_memset(&e, 0, sizeof(t_env));
	get_opt(&e, argc, argv);
	printf("flag.help: %d\nflag.verbose:%d\n", e.flag.help, e.flag.verbose);
	if (getuid() != 0)
	{
		fprintf(stderr, "Command need to be run as root\n");
		exit(1);
	}
	get_sockaddr(&e, e.hostname);
	display_header_info(&e);
	ping_host(&e);
	freeaddrinfo(e.addr);
	return 0;
}
