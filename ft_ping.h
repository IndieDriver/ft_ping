/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amathias </var/spool/mail/amathias>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 16:50:28 by amathias          #+#    #+#             */
/*   Updated: 2017/11/06 17:28:35 by amathias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

# include <stdint.h>
# include <unistd.h>
#include <sys/time.h>

# include "libft/libft.h"

# define XV(err,res,str) (x_void(err,res,str,__FILE__))
# define X(err,res,str) (x_int(err,res,str,__FILE__))

typedef struct				s_env
{
	char					*hostname;
	int						socket;
	struct addrinfo			*addr;
	struct {
		int					help;
		int					verbose;
	}	flag;
	uint16_t				counter;
}							t_env;

typedef struct				s_packet
{
	struct icmp				icmp;
	uint8_t					data[36];
	/*
	union
	{
		uint8_t				data[36];
		struct {
			struct timeval	send_at;
			uint8_t			padding[40];
		};

	} payload; */
}							t_packet;

typedef struct				s_rpacket
{
	struct iphdr			ipheader;
	struct icmp				icmp;
	uint8_t					data[36];
	/*
	union
	{
		uint8_t				data[56];
		struct {
			struct timeval	send_at;
			uint8_t			padding[40];
		};

	} payload; */
}							t_rpacket;

uint16_t					checksum(uint16_t *data, int length);

int							x_int(int err, int res, char *str, char *file);
void						*x_void(void *err, void *res, char *str, char *file);
void						hexdump(void *mem, unsigned int len);

void						get_opt(t_env *e, int argc, char **argv);

void						display_header_info(t_env *env);
void						display_response(t_env *e, int bytes_receive,
								int seq, double duration);


#endif
