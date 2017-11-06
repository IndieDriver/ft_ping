/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 14:44:37 by amathias          #+#    #+#             */
/*   Updated: 2017/11/06 16:15:25 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	display_header_info(t_env *env)
{
	printf("PING %s (%s): %lu(%lu) bytes of data\n", env->hostname, env->hostname,
			sizeof(t_packet), sizeof(t_rpacket));
	printf("%ld\n", sizeof(struct icmp));
	/*
	   printf("sizeof packet %ld\n", sizeof(t_packet));
	   printf("sizeof rpacket %ld\n", sizeof(t_rpacket));
	   printf("sizeof iphdr %ld\n", sizeof(struct iphdr));
	   printf("sizeof icmphdr %ld\n", sizeof(struct icmphdr));
	   printf("sizeof packet + iphdr %ld\n", sizeof(struct iphdr) + sizeof(t_packet)); */
}

void	display_response(t_env *e, int bytes_receive, int seq, double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(e->addr->ai_family,
			&((struct sockaddr_in *)e->addr->ai_addr)->sin_addr, ip, sizeof(ip));
	printf("%u bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
			bytes_receive, ip, seq, 64, duration);
}
