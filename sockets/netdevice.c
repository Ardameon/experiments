#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_vlan.h>
#include <linux/sockios.h>
#include <fcntl.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "netdevice.h"

static int skfd = -1;
static int skfd6 = -1;

#define log_perror(msg) printf("%s: %s failed (%s)\n", __FUNCTION__, (msg), strerror(errno));

#define log_dev_perror(dev, msg) \
	printf("%s: device %s: %s failed (%s)\n", \
	__FUNCTION__, (dev), (msg), strerror(errno));

int init_socket(void)
{
	if (skfd < 0)
		if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			log_perror("socket");
			return -1;
		}
	
	return 0;
}

int init_socket6(void)
{
	if (skfd6 < 0)
		if ((skfd6 = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
			log_perror("socket6");
			return -1;
		}

	return 0;
}

int rt_add_gw(uint32_t gw, int allow_existing)
{
	struct rtentry rt;
	struct sockaddr_in *sin;

	if (init_socket()) return -1;

	memset(&rt, 0, sizeof(rt));
	rt.rt_flags = RTF_UP | RTF_GATEWAY;

	sin = ((struct sockaddr_in *) &rt.rt_gateway);
	sin->sin_addr.s_addr = gw;
	sin->sin_family = AF_INET;

	sin = ((struct sockaddr_in *) &rt.rt_dst);
	sin->sin_addr.s_addr = 0;
	sin->sin_family = AF_INET;

	sin = ((struct sockaddr_in *) &rt.rt_genmask);
	sin->sin_addr.s_addr = 0;
	sin->sin_family = AF_INET;

	errno = 0;
	if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
		if (allow_existing && errno == EEXIST)
			return 0;
		log_perror("SIOCADDRT");
		return -1;
	}
	
	return 0;
}

int rt_del_gw(int allow_nonexisting)
{
	struct rtentry rt;
	struct sockaddr_in *sin;

	if (init_socket()) return -1;

	memset(&rt, 0, sizeof(rt));
	rt.rt_flags = RTF_UP;

	sin = ((struct sockaddr_in *) &rt.rt_dst);
	sin->sin_addr.s_addr = 0;
	sin->sin_family = AF_INET;

	sin = ((struct sockaddr_in *) &rt.rt_genmask);
	sin->sin_addr.s_addr = 0;
	sin->sin_family = AF_INET;

	errno = 0;
	if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
		if (allow_nonexisting && errno == ESRCH)
			return 0;
		log_perror("SIOCDELRT");
		return -1;
	}
	
	return 0;
}

static int set_ip_using(int sock, const char *if_name, int c, uint32_t ip)
{
	struct ifreq ifr;
	struct sockaddr_in sin;

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	memset(&sin, 0, sizeof(struct sockaddr));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip;
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
	if (ioctl(sock, c, &ifr) < 0)
		return -1;
	return 0;
}

int if_set_ip(const char *if_name, uint32_t addr, uint32_t mask, uint32_t bcast)
{
	if (init_socket()) return -1;

	if (set_ip_using(skfd, if_name, SIOCSIFADDR, addr)) {
		log_dev_perror(if_name, "SIOCSIFADDR");
		return -1;
	}
		
	if (!addr) return 0;

	if (mask) {
		if (set_ip_using(skfd, if_name, SIOCSIFNETMASK, mask)) {
			log_dev_perror(if_name, "SIOCSIFNETMASK");
			return -1;
		}
	}

	if (bcast) {
		if (set_ip_using(skfd, if_name, SIOCSIFBRDADDR, bcast)) {
			log_dev_perror(if_name, "SIOCSIFBRDADDR");
			return -1;
		}
	}
	
	return 0;
}

int if_del_ip(const char *if_name)
{
	if (init_socket()) return -1;
	
	if (set_ip_using(skfd, if_name, SIOCSIFADDR, 0)) {
		log_dev_perror(if_name, "SIOCSIFADDR");
		return -1;
	}
	
	return 0;
}

int if_get_mac(const char *if_name, unsigned char *mac, int mac_size)
{
	struct ifreq ifr;

	if (init_socket()) return -1;

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCGIFHWADDR");
		return -1;
	}

	memcpy(mac, (unsigned char *) &(ifr.ifr_hwaddr.sa_data), mac_size);
	return 0;
}

int if_set_mac(const char *if_name, unsigned char *mac, int mac_size)
{
	struct ifreq ifr;

	if (init_socket()) return -1;

	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	memcpy((unsigned char *) &(ifr.ifr_hwaddr.sa_data),
	       mac,
	       mac_size);
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	if (ioctl(skfd, SIOCSIFHWADDR, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCSIFHWADDR");
		return -1;
	}
	
	return 0;
}

int if_set_mtu(const char *if_name, int mtu)
{
	struct ifreq ifr;

	if (init_socket()) return -1;

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	ifr.ifr_mtu = mtu;
	if (ioctl(skfd, SIOCSIFMTU, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCSIFMTU");
		return -1;
	}

	return 0;
}

int if_set_flag(const char *if_name, short flag)
{
	struct ifreq ifr;

	if (init_socket()) return -1;

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCGIFFLAGS");
		return -1;
	}

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	ifr.ifr_flags |= flag;
	if (ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCSIFFLAGS");
		return -1;
	}

	return 0;
}

int if_clear_flag(const char *if_name, short flag)
{
	struct ifreq ifr;

	if (init_socket()) return -1;

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCGIFFLAGS");
		return -1;
	}

	strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
	ifr.ifr_flags &= ~flag;
	if (ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) {
		log_dev_perror(if_name, "SIOCSIFFLAGS");
		return -1;
	}

	return 0;
}

int if_set_arp(const char *if_name, uint32_t ipaddr, unsigned char *mac) 
{ 
	struct arpreq ar; 
	uint32_t	mask = 0xffffffff;

	if (init_socket()) return -1;

	memset(&ar, 0, sizeof(ar)); 

	strncpy( ar.arp_dev, if_name, sizeof(ar.arp_dev));
	((struct sockaddr_in*) &ar.arp_pa)->sin_addr.s_addr = ipaddr;
//	memcpy( &ar.arp_pa.sa_data, &pa->sa_data, sizeof(struct sockaddr));
	ar.arp_pa.sa_family = AF_INET; 
	memcpy( &ar.arp_netmask.sa_data, &mask, INET_ALEN); 
	
	
	memcpy( &ar.arp_ha.sa_data, mac, ETH_ALEN); 
	ar.arp_flags = ATF_PERM;

	if( ioctl( skfd, SIOCSARP, (char *) &ar) < 0) 
	{ 
		log_dev_perror(if_name, "SIOCSARP");
		return -1;
	}

	return 0;
}


static const char conf_file_name[] = "/proc/net/vlan/config";
int if_add_vlan(const char *if_name, uint16_t vid)
{
	int ret;
	struct vlan_ioctl_args ifr;

	if (init_socket()) return -1;

	/* not need to close, it's done automatically (busybox say) */
	if (open(conf_file_name, O_RDONLY) < 0)
		return -1;

	memset(&ifr, 0, sizeof(ifr));

	ifr.cmd = ADD_VLAN_CMD;
	ifr.u.VID = vid;
	memcpy(ifr.device1, if_name, strlen(if_name)); 
	
	if( (ret = ioctl( skfd, SIOCSIFVLAN, (char *) &ifr)) < 0) 
	{ 
		log_dev_perror(if_name, "SIOCSIFVLAN");
		return -1;
	}

	return 0;
}

int if_del_vlan(const char *if_name, uint16_t vid)
{
	struct vlan_ioctl_args ifr;

	if (init_socket()) return -1;

	/* not need to close, it's done automatically (busybox say) */
	if (open(conf_file_name, O_RDONLY) < 0)
		return -1;

	memset(&ifr, 0, sizeof(ifr));

	ifr.cmd = DEL_VLAN_CMD;
	sprintf(ifr.device1, "%s.%d", if_name, vid);

	if( ioctl( skfd, SIOCSIFVLAN, (char *) &ifr) < 0) 
	{ 
		log_dev_perror(if_name, "SIOCSIFVLAN");
		return -1;
	}

	return 0;
}

int if_set_ip_vlan(const char *if_name, uint16_t vid, uint32_t addr, uint32_t mask, uint32_t bcast)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_set_ip(if_vlan_name, addr, mask, bcast);
}

int if_del_ip_vlan(const char *if_name, uint16_t vid)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_del_ip(if_vlan_name);
}

int if_get_mac_vlan(const char *if_name, uint16_t vid, unsigned char *mac, int mac_size)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_get_mac(if_vlan_name, mac, mac_size);
}

int if_set_mac_vlan(const char *if_name, uint16_t vid, unsigned char *mac, int mac_size)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_set_mac(if_vlan_name, mac, mac_size);
}

int if_set_mtu_vlan(const char *if_name, uint16_t vid, int mtu)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_set_mtu(if_vlan_name, mtu);
}

int if_set_flag_vlan(const char *if_name, uint16_t vid, short flag)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_set_flag(if_vlan_name, flag);
}

int if_clear_flag_vlan(const char *if_name, uint16_t vid, short flag)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_clear_flag(if_vlan_name, flag);
}

int if_set_arp_vlan(const char *if_name, uint16_t vid, uint32_t ipaddr, unsigned char *mac) 
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d", if_name, vid);

	return if_set_arp(if_vlan_name, ipaddr, mac);
}

int if_set_ip_vlan_index(const char *if_name, uint16_t vid, int index, uint32_t addr, uint32_t mask, uint32_t bcast)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d:%d", if_name, vid, index);

	return if_set_ip(if_vlan_name, addr, mask, bcast);
}

int if_del_ip_vlan_index(const char *if_name, uint16_t vid, int index)
{
	char if_vlan_name[24];
	sprintf(if_vlan_name, "%s.%d:%d", if_name, vid, index);

	return if_del_ip(if_vlan_name);
}

/* MAC_ADDR_STRLEN = strlen("00:11:22:33:44:55") */
#define MAC_ADDR_STRLEN	17

int if_str2mac(const char *src, unsigned char *mac, size_t size)
{
	int i = 0;
	int rc = -1;

	if (size < 6)
		goto out_err;

	if (!src)
		goto out_err;

	if (strlen(src) != MAC_ADDR_STRLEN)
		goto out_err;

	memset(mac, 0, size);
	for (i = 0; i < 6; i++, mac++)
		if (1 != sscanf(&src[i * 3], "%02X", mac))
			goto out_err;
	rc = 0;
out_err:
	return rc;
}
