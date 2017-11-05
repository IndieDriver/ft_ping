/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:50:28 by amathias          #+#    #+#             */
/*   Updated: 2017/11/05 19:20:32 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

# include <stdint.h>
# include <unistd.h>
# include "libft/libft.h"

# define XV(err,res,str) (x_void(err,res,str,__FILE__))
# define X(err,res,str) (x_int(err,res,str,__FILE__))

typedef struct		s_icmp
{
	uint8_t			type;
	uint8_t			code;
	uint16_t		checksum;
	uint32_t		header_data;
}					t_icmp;

typedef struct				s_env
{
	char					*hostname;
	int						socket;
	struct addrinfo			*addr;
}							t_env;

int							x_int(int err, int res, char *str, char *file);
void						*x_void(void *err, void *res, char *str, char *file);

#endif
