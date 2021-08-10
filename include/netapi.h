#ifndef __NETAPI_H__
#define __NETAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- wifi ---------------------------*/
typedef enum {
    WIFI_STATUS_IDLE=0,
    WIFI_STATUS_INITED,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
} WIFI_STATUS;

typedef enum {
	WIFI_AUTH_NONE_OPEN = 1,
	WIFI_AUTH_NONE_WEP,
	WIFI_AUTH_WEP_SHARED,
	WIFI_AUTH_IEEE8021X,
	WIFI_AUTH_WPA_PSK,
	WIFI_AUTH_WPA_EAP,
	WIFI_AUTH_WPA_WPA2_PSK,
	WIFI_AUTH_WPA_WPA2_EAP,
	WIFI_AUTH_WPA2_PSK,
	WIFI_AUTH_WPA2_EAP
} WIFI_AUTH_MODE;

#define WIFI_SEC_NONE	    0x00
#define WIIF_SEC_WEP64	    0x01	
#define WIFI_SEC_WEP128	    0x02
#define WIFI_SEC_WEPX	    0x04
#define WIFI_SEC_CCMP	    0x10
#define WIFI_SEC_TKIP	    0x20

typedef struct {
    char essid[33];     /* AP name */
    char bssid[32];     
    int rssi;           /* Signal value */
    int adhoc;
    int authmode;       /* Authentication modes */
    int secmode;        /* Encryption mode, NONE,WEP,TKIP,CCMP */
} ap_info;

/* return code definition */

#define WIFI_MODULE_BASE            1000
#define WIFI_RET_OK                 0
#define WIFI_RET_ERR_NO_INIT        -(1 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_DIS_LINK       -(2 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_DOWN           -(3 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_PARA           -(4 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_TIMEOUT        -(5 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_MEM            -(6 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_BUSY           -(7 + WIFI_MODULE_BASE)
#define WIFI_RET_ERR_SYS            -(8 + WIFI_MODULE_BASE)

/* wifi module */
int wifi_open(void);
int wifi_close(void);
int wifi_scan(ap_info *ap, int *count);
int wifi_connect(const ap_info *ap, char *pwd, int timeout);
int wifi_disconnect(void);
int wifi_check(WIFI_STATUS *status, char *essid, char *bssid, int *rssi);

/* ----------------------- wnet -------------------------------*/
typedef enum {
    WNET_AUTH_NONE=0,
    WNET_AUTH_PAP,
    WNET_AUTH_CHAP,
    WNET_AUTH_CHAP_V2
} WNET_AUTH;

typedef enum {
    WNET_STATUS_IDLE=0,
    WNET_STATUS_INITED,
    WNET_STATUS_REGISTERING,
    WNET_STATUS_REGISTERED,
    WNET_STATUS_UNREGISTERING,
} WNET_STATUS;

/* return code */
#define WNET_RET_BASE           3000
#define WNET_RET_OK             0
#define WNET_RET_ERR_PARA       -(WNET_RET_BASE + 1)
#define WNET_RET_ERR_STATUS     -(WNET_RET_BASE + 2)
#define WNET_RET_ERR_SYS        -(WNET_RET_BASE + 3)
#define WNET_RET_ERR_NO_DEV     -(WNET_RET_BASE + 4)
#define WNET_RET_ERR_NO_MEM     -(WNET_RET_BASE + 5)
#define WNET_RET_ERR_TIMEOUT    -(WNET_RET_BASE + 6)
#define WNET_RET_ERR_NOCARD     -(WNET_RET_BASE + 7)

/* wireless module */
int wnet_init(const char *sim_pin, int reset);
int wnet_get_signal(int *signal);
int wnet_login(const char *apn, const char *user, const char *password,
                WNET_AUTH auth, int timeout);
int wnet_check(WNET_STATUS *status);
int wnet_logout(void);
int wnet_deinit(void);

/*------------------------- net ---------------------------------*/
enum NET_ROUTE {
    NET_ROUTE_ETH=0,
    NET_ROUTE_WL,
    NET_ROUTE_WIFI,
    NET_ROUTE_MAX
};

/* return code definition */
#define NET_RET_BASE                3000
#define NET_RET_OK                  0
#define NET_RET_ERR_ROUTE           -(NET_RET_BASE + 1)
#define NET_RET_ERR_SYS             -(NET_RET_BASE + 2)
#define NET_RET_ERR_IF              -(NET_RET_BASE + 3)
#define NET_RET_ERR_ACCESS          -(NET_RET_BASE + 4)
#define NET_RET_ERR_MEM             -(NET_RET_BASE + 5)
#define NET_RET_ERR_TIMEOUT         -(NET_RET_BASE + 6)
#define NET_RET_ERR_PARA            -(NET_RET_BASE + 7)
#define NET_RET_ERR_DNS             -(NET_RET_BASE + 8)
#define NET_RET_ERR_SESSION         -(NET_RET_BASE + 9)
#define NET_RET_ERR_DOING           -(NET_RET_BASE + 10)
#define NET_RET_ERR_STATUS          -(NET_RET_BASE + 11)
#define NET_RET_ERR_DHCP_DISCOVER   -(NET_RET_BASE + 12)
#define NET_RET_ERR_DHCP_OFFER      -(NET_RET_BASE + 13)
#define NET_RET_ERR_NOT_SUPPORT     -(NET_RET_BASE + 14)

/* network interface */
int net_ping(const char *addr, int timeout);
int net_dns(const char *name, char *addr, int timeout);
int net_set_route(NET_ROUTE net_route);
int net_get_route(NET_ROUTE *net_route);
int net_set_config(const char *addr, const char *mask,
                    const char *gateway, const char *dns);
int net_get_mac(NET_ROUTE net_route, unsigned char *mac);
int net_get_config(NET_ROUTE net_route, char *addr, char *mask, char *gateway, char *dns);
int net_dhcp_start();
int net_dhcp_check();
int net_dhcp_stop();
int net_add_arp(const char *addr, const unsigned char mac[6]);

#define NETAPI_LIB_VERSION "LIB_VERSION_1.00"
int xosal_net_get_version(char *ver);

#ifdef __cplusplus
}
#endif

#endif
