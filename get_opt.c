/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 10:21:03 by amathias          #+#    #+#             */
/*   Updated: 2017/11/06 12:28:02 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

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
						case 'h' : e->flag.help = 1; break;
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
