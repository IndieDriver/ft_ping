/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:49:46 by amathias          #+#    #+#             */
/*   Updated: 2017/11/05 19:58:22 by amathias         ###   ########.fr       */
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
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	s = getaddrinfo(NULL, addr, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;
		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
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
	int		sock;

	if (addr == NULL)
		return ;
	sock = X(-1, socket(PF_INET, SOCK_RAW, IPPROTO_ICMP), "socket");
	e->socket = sock;

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
	ping_host(&e, e.hostname);

	printf("%d|%d|%d|%d\n", e.addr->ai_flags, e.addr->ai_family,
			e.addr->ai_socktype, e.addr->ai_protocol);
	return 0;
}
