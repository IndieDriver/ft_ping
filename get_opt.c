/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 10:21:03 by amathias          #+#    #+#             */
/*   Updated: 2017/11/08 16:30:43 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help()
{
	printf("./ft_ping <destination>\n"
		"Flags:\n"
		"  -h: print help\n"
		"  -v: verbose mode\n");
	exit(0);
}

void	get_opt(t_env *e, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc) {
		switch (argv[i][0]) {
			case '-':
				while (*++argv[i])
				{
					switch (*argv[i]) {
						case 'v' : e->flag.verbose = 1; break;
						case 'h' : print_help(); break;
						default :
							printf ("Bad switch %c, ignored.\n",*argv[i]);
					}
				}
				break;
			default:
				e->hostname = ft_strdup(argv[i]);
		}
	}
	if (e->hostname == NULL)
	{
		fprintf(stderr, "./ping <destination>\n");
		exit(1);
	}
}
