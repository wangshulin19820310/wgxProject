#pragma once
#include<map>
#include<string>
#include<thread>
#include<memory>
#include<condition_variable>
#include<mutex>
#include<chrono>
#include <time.h>

#include "netapi.h"
#include "include.h"
#include "comFunc/comfunc.h"

using namespace std;
struct S_WIFIINFO {
	ap_info s_ab_info;
	char cPwd[128];
	char cAddr[128];
	char cMask[128];
	char cGateway[128];
	char cDns[128];
	int iAuto;//0手动或1自动
	time_t t_new;
};

struct S_WNETINFO {
	char cApn[128];
	char cUser[128];
	char cPassword[128];
	WNET_AUTH auth;
};

struct S_ETHINFO {
	char cAddr[128];
	char cMask[128];
	char cGateway[128];
	char cDns[128];
	int iAuto;//0手动或1自动
};

/*网络配置类*/
class NetworkConf
{
public:
	NetworkConf();
	virtual ~NetworkConf();

	//扫描wifi
	bool wifiScan(ap_info* ap, int& iCount);
	//wifi物理连接
	bool wifiConnect(const string& sWifiName, const string& sWifiMac, const string& sPwd,int timeout=30000);
	//设置wifi固定网络信息
	bool wifiSetConfig(const string& sAddr, const string& sMask, const string& sGateway, const string& sDns);
	//设置wifi自动网络信息
	bool wifiSetAutoConfig();
	//断开wifi
	void wifiDisconnect();
	//wifi状态
	bool wifiCheck(WIFI_STATUS& status, string& sWifiName, string& sWifiMac, int& rssi);
	//获取wifi 目前手动或自动；
	bool wifiGetIsAuto(bool& bAuto);

	//无线网络登录
	bool wnetLogin(const string& sApn, const string& sUser, const string& sPassword,WNET_AUTH auth= WNET_AUTH_CHAP,int timeout = 30000);
	//断开无线网络
	void wnetLogout();
	//无线网络信号
	bool wnetGet_signal(int& signal);
	//无线网络状态
	bool wnetCheck(WNET_STATUS& status);
	//设置有线固定网络信息
	bool netSetConfig(const string& sAddr, const string& sMask, const string& sGateway, const string& sDns);
	//设置有线自动网络信息
	bool netSetAutoConfig();

	//获取不同的网络信息（包括wifi或有线网络）0(NET_ROUTE_ETH):有线网络;1(NET_ROUTE_WL):无线网络;2(NET_ROUTE_WIFI):wifi网络;3(NET_ROUTE_MAX)当前路由
	bool netGet_configFromRoute(NET_ROUTE net_route,string& sAddr,string& sMask, string& sGateway, string& sDns);

	//获取不同的网络Mac（包括wifi或有线网络）0(NET_ROUTE_ETH):有线网络;1(NET_ROUTE_WL):无线网络;2(NET_ROUTE_WIFI):wifi网络;3(NET_ROUTE_MAX)当前路由
	bool netGet_Mac(NET_ROUTE net_route,string& sMac);
	
	//有线网络目前手动或自动；
	bool  netGetIsAuto(bool& bAuto);

	//获取当前路由
	bool netGet_route(NET_ROUTE& net_route);

	//是否存在wifi和4G 模块
	void isExistWifiAndWnet(bool& bWifi, bool& bWent);
	//设置路由
	bool netSet_route(NET_ROUTE net_route);
	//ping网络是否通
	bool netPing(string sAddr, int timeout=2000);

    static NetworkConf *Instance();

    void timeCheckNet();

    bool bWifiModular = false;//有wifi网络模块
    bool bWirelessModular=false;//有无线网络模块
    string netIpAddr;
    string netIp();


private:
	//获取 dhcp 服务状态。 
	bool netDhcp_check();

	void networkConnect(NET_ROUTE net_route);
	
	void ThAutoConfPro();
    void ThAutoConfPro_new();

private:

	time_t t_opt;

	NET_ROUTE netRoute;      //0(NET_ROUTE_ETH):有线网络;1(NET_ROUTE_WL):无线网络;2(NET_ROUTE_WIFI):wifi网络;
	S_WIFIINFO s_wifiInfo;
	S_WNETINFO s_wnetInfo;
	S_ETHINFO s_ethInfo;
	map<string, S_WIFIINFO> mapWifi;

	shared_ptr<thread> ptrThAutoConf;
	mutex mtAutoConf;
	condition_variable cvAutoConf;
	bool bRunAutoConf;    

    static NetworkConf *self;

};
