/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 17:32:03 by amathias          #+#    #+#             */
/*   Updated: 2017/11/05 17:35:11 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_ping.h"

int		x_int(int err, int res, char *str, char *file)
{
	if (res == err)
	{
		printf("%s error(%s): %s\n", str, file, strerror(errno));
		exit(1);
	}
	return (res);
}

void	*x_void(void *err, void *res, char *str, char *file)
{
	if (res == err)
	{
		printf("%s error(%s): %s\n", str, file, strerror(errno));
		exit(1);
	}
	return (res);
}
