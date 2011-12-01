/***********************************************************
 SixXSd - The Daemon of SixXS
 by Jeroen Massar <jeroen@sixxs.net>
 (C) Copyright SixXS 2000-2011 All Rights Reserved
************************************************************
 $Author: $
 $Id: $
 $Date: $
***********************************************************/

#ifndef CONFIG_H
#define CONFIG_H "SixXSd's Configuration Handling"

#include "sixxsd.h"

enum sixxsd_sockets
{
	SIXXSD_SOCK_TUNTAP = 0,						/* In/out for native packets */
	SIXXSD_SOCK_PROTO41,						/* In for proto-41 packets */
	SIXXSD_SOCK_AYIYA,						/* In for AYIYA packets */
	SIXXSD_SOCK_HB,							/* In for Heartbeat control packets */
	SIXXSD_SOCK_MAX
};

struct sixxsd_socket
{
	SOCKET			socket;					/* The socket */

	uint16_t		type;					/* SIXXSD_SOCK_<...> */
	uint16_t		af;					/* Address Family that we are listening on */
	uint16_t		proto;					/* Protocol "" */
	uint16_t		port;					/* Port "" */
};

struct sixxsd_packet
{
	struct sixxsd_socket	*sock;					/* Which socket it came in on */

	IPADDRESS		src;					/* Source of the packet */
	uint16_t		port;					/* Source port of the packet */
	uint16_t		length;					/* Length of the packet */

	uint8_t			buf[1532];				/* The packet */
};

/* Our configuration structure */
struct sixxsd_configuration
{
	/* Settings */
	char			pop_name[128];				/* The name of this installation */
	IPADDRESS		pop_ipv4, pop_ipv6;			/* Used for ICMP Source Address and listen */

	char			pop_ipv4_asc[NI_MAXHOST],		/* Textual representation of IPv4 address */
				pop_ipv6_asc[NI_MAXHOST];		/* ""                        IPv6 "" */
	uint8_t			__padding[6];

	IPADDRESS		cli_acl[32];				/* ACLs for the CLI */

	BOOL			daemonize;				/* Daemonized? */
	uint64_t		debugging;				/* Are we debugging? */
	volatile BOOL		running;				/* Running? */
	BOOL			opened_syslog;				/* Syslog gets opened at first reference */
	uint64_t		starttime;				/* When we got started */

	/* Verbosity levels */
	uint64_t		verbose;				/* Verbose
									 *           0 = disabled           (NOTICE and up),
									 *           1 = verbose            (INFO and up),
									 *           2 = more verbose       (DEBUG and up),
									 *           3 = very verbose
									 *           4 = extremely verbose
									 */
	BOOL			verbose_sixxsd;
	BOOL			verbose_common;
	BOOL			verbose_config;
	BOOL			verbose_prefix;
	BOOL			verbose_thread;
	BOOL			verbose_ayiya;

	/* Threads */
	rwl			rwl_threads;				/* RWL protecting the threads */
	struct sixxsd_thread	*threads;				/* All the threads */

	/* Pinger */
	mutex			mutex_pinger;				/* Avoid pinging & fetching results at the same time */

	/* Incoming/Outgoing sockets */
	struct sixxsd_socket	sockets[16];				/* In/out sockets */

	/* Outgoing socket */
	SOCKET			tuntap;					/* Tun/tap (copied from above) */
	SOCKET			rawsocket_ipv4;				/* Need to do raw packets as otherwise we need to do a bind per send... */

	struct sixxsd_stats	stats_total, stats_uplink;		/* Statistics Total + Uplink */

	/* The Tunnels */
	struct sixxsd_tunnels	tunnels;				/* We can have 1 tunnel prefix per PoP */

	/* The Subnets */
	struct sixxsd_subnets	subnets[16];				/* The subnets (16 /40's per PoP max) */
	uint64_t		subnets_hi;				/* The highest in-use subnet */
};

/* Our global configuration */
extern struct sixxsd_configuration *g_conf;

/* Generic Configuration functions */
int	cfg_init(struct sixxsd_context *ctx, uint32_t verbose);
VOID	cfg_exit(VOID);

#endif /* CONFIG_H */
