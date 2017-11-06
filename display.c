/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 14:44:37 by amathias          #+#    #+#             */
/*   Updated: 2017/11/06 21:17:42 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	display_header_info(t_env *env)
{
	printf("PING %s (%s): %lu(%lu) bytes of data\n", env->hostname, env->hostname,
			sizeof(t_rpacket) - sizeof(struct icmp), sizeof(t_rpacket));
}

void	display_response(t_env *e, int bytes_receive, int seq, double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(e->addr->ai_family,
			&((struct sockaddr_in *)e->addr->ai_addr)->sin_addr, ip, sizeof(ip));
	printf("%u bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
			bytes_receive, ip, seq, 64, duration);
}

void	display_footer(t_env *e)
{
	double			lost;
	double			average;
	double			mdev;
	struct timeval	current_time;

	lost = e->sent != 0
		? 100 - ((e->received * 100) / e->sent) : 0;
	gettimeofday(&current_time, NULL);
	printf("-- %s ping statistics ---\n", e->hostname);
	printf("%u packets transmitted, %u received, %.2f%% packet loss, time %.0fms\n",
			e->sent, e->received, lost,
			get_time_elapsed(&e->start_time, &current_time));

	average = e->sent != 0 ? e->sum / e->sent : 0;
	mdev = (e->sent - (average * average)) != 0
		? sqrtf(e->sum_square / (e->sent - (average * average))) : 0;
	if (e->received > 0)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
				e->ping_min, average, e->ping_max, mdev);

	exit(0);
}
