//base
int at_test(int argc, char *argv[], char *op[]);
int at_show_version(int argc, char *argv[], char *op[]);		//ver
int at_go_cli(int argc, char *argv[], char *op[]);				//cli
int at_return_def(int argc, char *argv[], char *op[]);			//rdef
int at_rst_module(int argc, char *argv[], char *op[]);			//rst
#ifndef CONFIG_USB_CDC_ACM_DEV
int at_ur_config(int argc, char *argv[], char *op[]);			//ur
#endif
int at_params_save(int argc, char *argv[], char *op[]);
int at_debug_print(int argc, char *argv[], char *op[]);

//Wi-Fi
int at_wifi_mode(int argc, char *argv[], char *op[]);			//wmode
int at_wifi_scan(int argc, char *argv[], char *op[]);			//wscan
int at_wifi_phy(int argc, char *argv[], char *op[]);			//wphy
int at_wifi_txpwr(int argc, char *argv[], char *op[]);			//wtxpwr
int at_wifi_staup(int argc, char *argv[], char *op[]);			//wstaup
int at_wifi_stadn(int argc, char *argv[], char *op[]);			//wstadn
int at_wifi_autoconn(int argc, char *argv[], char *op[]);		//wautoconn
int at_wifi_reconn(int argc, char *argv[], char *op[]);			//wreconn
int at_wifi_omcfgon(int argc, char *argv[], char *op[]);		//womcfgon
int at_wifi_omcfgoff(int argc, char *argv[], char *op[]);		//womcfgoff
int at_wifi_apup(int argc, char *argv[], char *op[]);			//wapup
int at_wifi_apdn(int argc, char *argv[], char *op[]);			//wapdn
int at_wifi_link(int argc, char *argv[], char *op[]);			//wlink
int at_wifi_sta(int argc, char *argv[], char *op[]);			//wsta
#if 0
int at_wifi_wps(int argc, char *argv[], char *op[]);			//wps
int at_wifi_mdns(int argc, char *argv[], char *op[]);			//mdns
#endif
int at_wifi_sleep(int argc, char *argv[], char *op[]);			//wsleep

//net
int at_net_dhcp(int argc, char *argv[], char *op[]);			//dhcp
int at_net_dhcps(int argc, char *argv[], char *op[]);			//dhcps

int at_net_ip(int argc, char *argv[], char *op[]);				//ip
int at_net_ping(int argc, char *argv[], char *op[]);			//ping
int at_net_dns(int argc, char *argv[], char *op[]);				//dns
int at_net_host(int argc, char *argv[], char *op[]);			//host
int at_net_mac(int argc, char *argv[], char *op[]);
int at_socket_TCPServer(int argc, char *argv[], char *op[]);
int at_trans_savelink(int argc, char *argv[], char *op[]);

//socket
int at_socket_info(int argc, char *argv[], char *op[]);			//sinfo
int at_socket_add(int argc, char *argv[], char *op[]);			//sadd
int at_socket_del(int argc, char *argv[], char *op[]);			//sdel
int at_socket_send(int argc, char *argv[], char *op[]);			//ssend
int at_trans_mode(int argc, char *argv[], char *op[]);			//tmode
int at_update(int argc, char *argv[], char *op[]);	


