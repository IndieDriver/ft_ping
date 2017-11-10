/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 10:21:03 by amathias          #+#    #+#             */
/*   Updated: 2017/11/10 14:42:34 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help()
{
	printf("./ft_ping <destination>\n"
		"Flags:\n"
		"  -h: print help\n"
		"  -v: verbose mode\n"
		"  -n: numeric mode\n"
		"  -t ttl: set ttl\n"
		"  -c count: set count\n"
		"  -W timeout: set packet timeout in second\n"
		);
	exit(0);
}

void	get_number(int *flag, int index, char *arg, char **argv)
{
	if (arg[1] == '\0' && argv[index + 1] != NULL)
	{
		*flag = ft_atoi(argv[index + 1]);
	}
	else
	{
		fprintf(stderr, "flag -%c need a numeric\n", *arg);
		exit(0);
	}
}

void	get_opt(t_env *e, int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc) {
		int next = 0;
		switch (argv[i][0]) {
			case '-':
				while (*++argv[i])
				{
					switch (*argv[i]) {
						case 'v' : e->flag.verbose = 1; break;
						case 'h' : print_help(); break;
						case 'n' : e->flag.numeric_out = 1; break;
						case 't' : get_number(&e->flag.ttl,
										i, argv[i], argv);
								   next = 1;
								   break;
						case 'c' : get_number(&e->flag.counter,
										i, argv[i], argv);
								   next = 1;
								   break;
						case 'W' : get_number(&e->flag.timeout,
										i, argv[i], argv);
								   next = 1;
								   break;
						default :
							printf ("unrecognized flag %c, ignored.\n",*argv[i]);
					}
					if (next)
						break;
				}
				if (next)
					i++;
				break;
			default:
				if (e->hostname == NULL)
					e->hostname = ft_strdup(argv[i]);
				else
				{
					fprintf(stderr, "./ping <destination>\n");
					exit(1);
				}
		}
	}
	if (e->hostname == NULL)
	{
		fprintf(stderr, "./ping <destination>\n");
		exit(1);
	}
	if (e->flag.counter != -1 && e->flag.counter < 0)
	{
		e->flag.counter = -1;
	}
	if (e->flag.timeout <= 0)
	{
		e->flag.timeout = 0;
	}
}
