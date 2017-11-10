/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:49:46 by amathias          #+#    #+#             */
/*   Updated: 2017/11/10 17:04:35 by amathias         ###   ########.fr       */
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
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	s = getaddrinfo(addr, NULL, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "ping: unknow host %s\n", addr);
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;
		close(sfd);
		break ;
	}
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	e->addr = result;
}

void	ping_connect(t_env *e)
{
	int res;
	e->socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (e->socket == -1)
	{
		fprintf(stderr, "Connection failure\n");
		exit(EXIT_FAILURE);
	}
	res = setsockopt(e->socket, IPPROTO_IP, IP_TTL, &e->flag.ttl,
			sizeof(e->flag.ttl));
	if (res == -1)
	{
		fprintf(stderr, "Invalid TTL %d\n", e->flag.ttl);
		exit(1);
	}
}

void	ping_send(t_env *e, struct timeval *send_time, uint16_t sequence, int ttl)
{
	t_packet		packet;
	int				res;

	ft_memset(&packet, 0, sizeof(t_packet));
	packet.icmp.icmp_type = (uint8_t)ICMP_ECHO;
	packet.icmp.icmp_code = (uint8_t)0;
	packet.icmp.icmp_id = e->pid_be;
	packet.icmp.icmp_seq = swap_byte16_t((uint16_t)sequence);
	for (int i = 0; i < 36; i++)
		packet.data[i] = 10 + i;
	packet.icmp.icmp_cksum = checksum((uint16_t*)&packet, sizeof(t_packet));
	if (send_time != NULL)
		gettimeofday (send_time, NULL);
	res = setsockopt(e->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	if (res == -1)
	{
		fprintf(stderr, "Invalid TTL %d\n", ttl);
		exit(1);
	}
	res = sendto(e->socket, &packet, sizeof(t_packet), 0,
			e->addr->ai_addr, e->addr->ai_addrlen);
	e->sent++;
}

int		ping_receive(t_env *e, struct timeval send_time, int sequence)
{
	struct sockaddr_storage	sender;
	struct timeval			received_time;
	t_rpacket				received;
	struct msghdr			msg_header;
	struct iovec			iovec;
	char					control[64];
	int						byte_recv;
	double					time_elapsed;

	ft_memset(&msg_header, 0, sizeof(struct msghdr));
	ft_memset(&iovec, 0, sizeof(struct iovec));
	ft_memset(&received, 0, sizeof(t_rpacket));
	iovec.iov_base = &received;
	iovec.iov_len = sizeof(t_rpacket);
	msg_header.msg_name = &sender;
	msg_header.msg_namelen = sizeof(struct sockaddr_storage);
	msg_header.msg_iov = &iovec;
	msg_header.msg_iovlen = 1;
	msg_header.msg_control = &control;
	msg_header.msg_controllen = sizeof(char) * 64;
	msg_header.msg_flags = 0;
	if (e->has_timeout)
	{
		if (e->flag.verbose)
		{
			ping_send(e, NULL, sequence, 1);
			e->sent--;
			alarm(1);
			e->has_timeout = 0;
			return (0);
		}
		else
		{
			e->has_timeout = 0;
			alarm(0);
			return (1);
		}
	}
	if ((byte_recv = recvmsg(e->socket, &msg_header, MSG_DONTWAIT)) > 0)
	{
		if (received.icmp.icmp_id == e->pid_be)
		{
			if (received.icmp.icmp_type == ICMP_ECHO)
				return (0);
			if (!is_same_host(msg_header.msg_name, (struct sockaddr_in*)e->addr->ai_addr))
				return (0);
			e->received++;
			gettimeofday (&received_time, NULL);
			alarm(0);
			ft_sleep(1);
			time_elapsed = get_time_elapsed(&send_time, &received_time);
			e->sum += time_elapsed;
			e->sum_square += time_elapsed * time_elapsed;
			e->ping_min = MIN(e->ping_min, time_elapsed);
			e->ping_max = MAX(e->ping_max, time_elapsed);
			if (e->flag.verbose)
			{
				display_verbose(e, byte_recv - sizeof(struct iphdr),
				received.icmp.icmp_type,
				received.icmp.icmp_code);
			}
			else
			{
				display_response(e, byte_recv - sizeof(struct iphdr),
					swap_byte16_t(received.icmp.icmp_seq),
					received.ipheader.ttl,
					time_elapsed);

			}
			return (1);
		}
		else if (received.icmp.icmp_type != ICMP_ECHOREPLY)
		{
			if (e->flag.verbose)
			{
				display_verbose(e, byte_recv - sizeof(struct iphdr),
				received.icmp.icmp_type,
				received.icmp.icmp_code);
			}
			alarm(0);
			ft_sleep(1);
			return (1);
		}
	}
	return (0);
}

void	ping_host(t_env *e)
{
	int sequence;
	struct timeval send_time;

	sequence = 0;
	gettimeofday (&e->start_time, NULL);
	ping_connect(e);
	while (1)
	{
		if (e->flag.counter != -1 && sequence >= e->flag.counter)
		{
			break ;
		}
		sequence++;
		ping_send(e, &send_time, (uint16_t)sequence, e->flag.ttl);
		alarm(e->flag.timeout);
		while (!ping_receive(e, send_time, sequence))
			;
	}
}

int main(int argc, char *argv[])
{
	ft_memset(&g_env, 0, sizeof(t_env));
	g_env.ping_min = 1000.0;
	g_env.flag.ttl = 64;
	g_env.flag.counter = -1;
	g_env.flag.timeout = 1;
	if (getuid() != 0)
	{
		fprintf(stderr, "Command need to be run as root\n");
		exit(1);
	}
	get_opt(&g_env, argc, argv);
	get_sockaddr(&g_env, g_env.hostname);
	g_env.pid_be = swap_byte16_t(getpid());
	display_header_info(&g_env);
	signal(SIGALRM, sig_handler);
	signal(SIGINT, sig_handler);
	ping_host(&g_env);
	display_footer(&g_env);
	return 0;
}
