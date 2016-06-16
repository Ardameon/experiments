#ifndef NETDEVICE_H
#define NETDEVICE_H

#include <stdint.h>

#define ETH_ALEN 6
#define ETH_HLEN 14
#define INET_ALEN 4

/* If the packet's ethernet header is aligned,
   this is the number of bytes by which the packet must be shifted
   in order for its IP header to be aligned on 32-bit boundary. */
#define APP_IP_ALIGN 2

#define NETDEV_UP_FLAG_SET IFF_UP|IFF_BROADCAST|IFF_RUNNING|IFF_MULTICAST
#define NETDEV_UP_FLAG     IFF_UP

int rt_add_gw(uint32_t gw, int allow_existing);
int rt_del_gw(int allow_nonexisting);
int if_set_ip(const char *if_name, uint32_t addr, uint32_t mask, uint32_t bcast);
int if_del_ip(const char *if_name);
//int if_get_mac_ip(const char *if_name, unsigned char * mac, int mac_size, unsigned char * ip, int ip_size);
//int if_get_mac_ip(const char *if_name, unsigned char * mac, int mac_size, unsigned char * ip, int ip_size, unsigned char *netmask);
int if_get_mac(const char *if_name, unsigned char *mac, int mac_size);
int if_set_mac(const char *if_name, unsigned char *mac, int mac_size);
int if_set_mtu(const char *if_name, int mtu);
int if_set_flag(const char *if_name, short flag);
int if_clear_flag(const char *if_name, short flag);
int if_set_arp(const char *if_name, uint32_t ipaddr, unsigned char *mac);

int if_add_vlan(const char *if_name, uint16_t vid);
int if_del_vlan(const char *if_name, uint16_t vid);
int if_set_ip_vlan(const char *if_name, uint16_t vid, uint32_t addr, uint32_t mask, uint32_t bcast);
int if_del_ip_vlan(const char *if_name, uint16_t vid);
int if_get_mac_vlan(const char *if_name, uint16_t vid, unsigned char *mac, int mac_size);
int if_set_mac_vlan(const char *if_name, uint16_t vid, unsigned char *mac, int mac_size);
int if_set_mtu_vlan(const char *if_name, uint16_t vid, int mtu);
int if_set_flag_vlan(const char *if_name, uint16_t vid, short flag);
int if_clear_flag_vlan(const char *if_name, uint16_t vid, short flag);
int if_set_arp_vlan(const char *if_name, uint16_t vid, uint32_t ipaddr, unsigned char *mac);
int if_set_ip_vlan_index(const char *if_name, uint16_t vid, int index, uint32_t addr, uint32_t mask, uint32_t bcast);
int if_del_ip_vlan_index(const char *if_name, uint16_t vid, int index);

int if_str2mac(const char *src, unsigned char *mac, size_t size);

#if 0
/* These two configure the ethernet management interface (MGMT_IF_NAME),
   but access its configuration via dsaif device. */
int dsaif_set_mgmt_vid(uint16_t vid);
int dsaif_del_mgmt_vid(void);

int dsaif_setup_cpu_codes(void);
int dsaif_enable_ingress_filtering(uint8_t dev_id, uint8_t port, int enable);
//int dsaif_set_port_vlans(uint8_t port, uint32_t vlans[]);
int dsaif_set_vlan_members(uint8_t dev_id, uint16_t vid, uint32_t *ports, uint8_t size);
int dsaif_set_pvid(uint8_t dev_id, uint8_t port, uint16_t vid);
int dsaif_set_device_id(uint8_t dev_id);
int dsaif_set_ports_stp_state(uint8_t stg_id, uint8_t stp_state, uint8_t dev_id, uint8_t *ports, uint8_t nports);
int dsaif_set_cpu_code_action(uint32_t id, int to_app);
int dsaif_set_uplink_vidx(uint16_t vidx);
int dsaif_set_shelf_src_id(uint8_t src_id);
#endif

#endif /* NETDEVICE_H */
