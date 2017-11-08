/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 14:44:37 by amathias          #+#    #+#             */
/*   Updated: 2017/11/08 18:12:05 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	display_header_info(t_env *e)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(e->addr->ai_family,
			&((struct sockaddr_in *)e->addr->ai_addr)->sin_addr, ip, sizeof(ip));

	printf("PING %s (%s) %lu(%lu) bytes of data.\n", e->hostname, ip,
			sizeof(t_rpacket) - sizeof(struct icmp), sizeof(t_rpacket));
}

void	display_response(t_env *e, int bytes_receive, int seq, double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(e->addr->ai_family,
			&((struct sockaddr_in *)e->addr->ai_addr)->sin_addr, ip, sizeof(ip));
	if (ft_strcmp(ip, e->hostname) == 0 || e->flag.numeric_out)
	{
		printf("%u bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
				bytes_receive, ip, seq, e->flag.ttl, duration);
	}
	else
	{
		printf("%u bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n",
				bytes_receive, e->hostname, ip, seq, e->flag.ttl, duration);
	}
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
	printf("\n--- %s ping statistics ---\n", e->hostname);
	printf("%u packets transmitted, %u received, %.0f%% packet loss, time %.0fms\n",
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
