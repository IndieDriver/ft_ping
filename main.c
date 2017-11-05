/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:49:46 by amathias          #+#    #+#             */
/*   Updated: 2017/11/05 21:13:40 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	get_sockaddr(t_env *e, const char *addr)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
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

void	ping_host(t_env *e, const char *addr)
{
	struct icmphdr header;
	int		sock;

	if (addr == NULL)
		return ;
	sock = X(-1, socket(PF_INET, SOCK_RAW, IPPROTO_ICMP), "socket");
	e->socket = sock;
	memset(&header, 0, sizeof(struct icmphdr));
	header.type = ICMP_ECHO;
	header.code = 0;
	header.checksum = checksum((uint16_t*)&header, sizeof(struct icmphdr));
	//printf("%s\n", e->addr->ai_canonname);
	X(-1, sendto(e->socket, &header, sizeof(struct icmphdr), 0,
		e->addr->ai_addr, e->addr->ai_addrlen), "sendto");
}

void	get_opt(t_env *e, int ac, char **av)
{
	e->hostname = ac == 2 ? ft_strdup(av[1]) : NULL;
	if (ac != 2)
	{
		fprintf(stderr, "./ping <destination>\n");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	t_env e;

	get_opt(&e, argc, argv);
	if (getuid() != 0)
	{
		fprintf(stderr, "Command need to be run as root\n");
		exit(1);
	}
	get_sockaddr(&e, e.hostname);
	ping_host(&e, e.hostname);
	freeaddrinfo(e.addr);
	return 0;
}
