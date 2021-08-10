#include "NetworkConf.h"
#include <string.h>


NetworkConf *NetworkConf::self = nullptr;
NetworkConf *NetworkConf::Instance()
{
    if (!self) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new NetworkConf;
        }
    }
    return self;
}


NetworkConf::NetworkConf()
{
	int iRoute = 0;
    comFunc comfunc;
    if (0 == wifi_open())
	{
		bWifiModular = true;
		memset(&s_wifiInfo, 0, sizeof(s_wifiInfo));
		string sWifiName, sWifiMac, sPwd,sAddr, sMask, sGateway, sDns;
        int iAdhoc,iAuthMode, iSecMode, iAuto;
        sWifiName = comfunc.qstringTostd(s_netInfo.wifiSsid);
        sWifiMac = comfunc.qstringTostd(s_netInfo.wifiMac);
        sPwd = comfunc.qstringTostd(s_netInfo.wifiPwd);
        sAddr = comfunc.qstringTostd(s_netInfo.sIp);
        sMask = comfunc.qstringTostd(s_netInfo.sMask);
        sGateway = comfunc.qstringTostd(s_netInfo.sGateWay);
        sDns = comfunc.qstringTostd(s_netInfo.sDns);
        iAuto = s_netInfo.autoIp;
        iAdhoc = s_netInfo.adhoc;
        iAuthMode = s_netInfo.wifiAuthMode;
        iSecMode = s_netInfo.secMode;
        if(1)
        {
            printf("[%s] %d netDBOpt.getWifiNetDb ok\n", __FUNCTION__, __LINE__);
            if(sWifiMac.length() >= 0)
			{
				strcpy(s_wifiInfo.s_ab_info.essid, sWifiName.c_str());
				strcpy(s_wifiInfo.s_ab_info.bssid, sWifiMac.c_str());
				s_wifiInfo.s_ab_info.adhoc = iAdhoc;
				s_wifiInfo.s_ab_info.authmode = iAuthMode;
				s_wifiInfo.s_ab_info.secmode = iSecMode;
				strcpy(s_wifiInfo.cPwd, sPwd.c_str());
				strcpy(s_wifiInfo.cAddr, sAddr.c_str());
				strcpy(s_wifiInfo.cMask, sMask.c_str());
				strcpy(s_wifiInfo.cGateway, sGateway.c_str());
				strcpy(s_wifiInfo.cDns, sDns.c_str());
				s_wifiInfo.iAuto = iAuto;
				s_wifiInfo.t_new = time(NULL);
				mapWifi[s_wifiInfo.s_ab_info.bssid] = s_wifiInfo;
			}
		}
	}
	else {
		bWifiModular = false;
	}

    if (0 == wnet_init(NULL,0))
	{
		bWirelessModular = true;
		memset(&s_wnetInfo, 0, sizeof(s_wnetInfo));
		int iAuth=2;
		string sApn, sUser, sPassword;
        sApn = comfunc.qstringTostd(s_netInfo.APN);
        sUser = comfunc.qstringTostd(s_netInfo.user);
        sPassword = comfunc.qstringTostd(s_netInfo.pwd);
        if(1)
		{
			printf("[%s] %d netDBOpt.getMobileNetDb ok sApn[%s], sUser[%s], sPassword[%s], iAuth[%d]\n", __FUNCTION__, __LINE__, sApn.c_str(), sUser.c_str(), sPassword.c_str(), iAuth);
			strcpy(s_wnetInfo.cApn , sApn.c_str());
			strcpy(s_wnetInfo.cUser , sUser.c_str());
			strcpy(s_wnetInfo.cPassword , sPassword.c_str());
			s_wnetInfo.auth = (WNET_AUTH)iAuth;
		}
	}
	else {
		bWirelessModular = false;
	}

    {
        memset(&s_ethInfo, 0, sizeof(s_ethInfo));
        string sAddr, sMask, sGateway, sDns;
        int iAuto;
        sAddr = comfunc.qstringTostd(s_netInfo.sIp);
        sMask = comfunc.qstringTostd(s_netInfo.sMask);
        sGateway = comfunc.qstringTostd(s_netInfo.sGateWay);
        sDns = comfunc.qstringTostd(s_netInfo.sDns);
        iAuto = s_netInfo.autoIp;
        if(1)
        {
            strcpy(s_ethInfo.cAddr,sAddr.c_str());
            strcpy(s_ethInfo.cMask, sMask.c_str());
            strcpy(s_ethInfo.cGateway, sGateway.c_str());
            strcpy(s_ethInfo.cDns, sDns.c_str());
            s_ethInfo.iAuto = iAuto;
        }
    }


#if 0
    netSet_route((NET_ROUTE)iRoute);
    netRoute = (NET_ROUTE)iRoute;
	printf("[%s] %d start ThAutoConf\n", __FUNCTION__, __LINE__);
    bRunAutoConf = true;
    ptrThAutoConf = make_shared<thread>(&NetworkConf::ThAutoConfPro, this);
#else

    netSet_route((NET_ROUTE)s_netInfo.netMode);
    netRoute = (NET_ROUTE)s_netInfo.netMode;

    printf("netRoute=%d\n",netRoute);
    if (NET_ROUTE_ETH == netRoute)
    {
        networkConnect(NET_ROUTE_ETH);
    }
    else if (bWirelessModular && NET_ROUTE_WL == netRoute) {
        networkConnect(NET_ROUTE_WL);
    }
    else if (bWifiModular && NET_ROUTE_WIFI == netRoute) {
        networkConnect(NET_ROUTE_WIFI);
    }

#endif
}

NetworkConf::~NetworkConf()
{
#ifndef WIN32
	wifi_close();
	wnet_deinit();
#endif
	bRunAutoConf = false;
	if (ptrThAutoConf && ptrThAutoConf->joinable())
	{
		ptrThAutoConf->join();
	}
}
#ifndef WIN32
bool NetworkConf::wifiScan(ap_info* ap, int& iCount)
{
	
	printf("[%s] %d start wifiScan \n", __FUNCTION__, __LINE__);
	ap_info apTemp[20] = { 0 };
	int iCountTemp = sizeof(apTemp)/sizeof(ap_info);
	int iCountT = iCount;
	iCount = 0;
	if (bWifiModular&&0 == wifi_scan(apTemp, &iCountTemp))
	{
		lock_guard<mutex> lk(mtAutoConf);
		if (strlen(s_wifiInfo.s_ab_info.bssid) > 0)
		{
			memcpy(ap, &s_wifiInfo.s_ab_info, sizeof(ap_info));
			iCount++;
		}

		for (int i = 0; i < iCountTemp; i++)
		{
			if (iCount < iCountT)
			{
				if (0 == strcmp(s_wifiInfo.s_ab_info.bssid, apTemp[i].bssid)&&strlen(s_wifiInfo.s_ab_info.bssid)== strlen(apTemp[i].bssid))
				{
					memcpy(ap, &apTemp[i], sizeof(ap_info));
					
				}
				else {
					memcpy(ap + iCount, &apTemp[i], sizeof(ap_info));
					iCount++;
				}		
			}
			if (mapWifi.find(apTemp[i].bssid) != mapWifi.end())
			{
				mapWifi[apTemp[i].bssid].t_new = time(NULL);
				memcpy(&mapWifi[apTemp[i].bssid].s_ab_info, &apTemp[i], sizeof(ap_info));
			}
			else {
				S_WIFIINFO wifiInfo;
				memset(&wifiInfo, 0, sizeof(wifiInfo));
				wifiInfo.t_new = time(NULL);
				memcpy(&wifiInfo.s_ab_info, &apTemp[i], sizeof(ap_info));
				mapWifi[apTemp[i].bssid] = wifiInfo;
			}
		}
		printf("[%s] %d wifiScan iCount[%d] ap[0].bssid[%s] ok\n", __FUNCTION__, __LINE__, iCount, ap[0].bssid);
		return true;
	}else {
		printf("[%s] %d wifiScan fail\n", __FUNCTION__, __LINE__);
		return false;
	}
}

bool NetworkConf::wifiConnect(const string& sWifiName, const string& sWifiMac, const string& sPwd, int timeout)
{
	if (!bWifiModular)
	{
		printf("[%s] %d: bWifiModular[%d] \n", __FUNCTION__, __LINE__, bWifiModular);
		return false;
	}
	//检测Wifi是否已登录
	WIFI_STATUS status;
    int iRssi,ret;
    char cEssid[128] = { 0 }, cBssid[128] = { 0 };
	if (0 == wifi_check(&status, cEssid, cBssid, &iRssi))
	{
		if (WIFI_STATUS_IDLE != status)
		{
            ret = wifi_disconnect();
            printf("wifi disconnect=%d\n",ret);
		}
	}
    printf("wifi status = %d\n",status);
#if 1
	std::unique_lock<std::mutex> lk(mtAutoConf);
	map<string, S_WIFIINFO>::iterator it = mapWifi.find(sWifiMac);
	printf("[%s] %d sWifiName[%s]  sWifiMac[%s] mapWifi.size[%d] \n", __FUNCTION__, __LINE__, sWifiName.c_str(), sWifiMac.c_str(), mapWifi.size());
	if (it != mapWifi.end())
	{
        printf("[%s] %d sWifiName[%s]  sWifiMac[%s] sWifiPwd[%s],start wifi_connect \n", __FUNCTION__, __LINE__, sWifiName.c_str(), sWifiMac.c_str(),sPwd.c_str());

        ap_info ap = it->second.s_ab_info;
		const char* pPwd = (0 == sPwd.length() ? NULL : sPwd.c_str());
        int ret=wifi_connect(&ap, (char*)pPwd, timeout);
        printf("essid=%s:authmode=%d:bssid=%s\n",ap.essid,ap.authmode,ap.bssid);
        printf("bWifiModular=%d:connect ret=%d\n",bWifiModular,ret);
    //	if (bWifiModular && 0 == wifi_connect(&ap, (char*)pPwd, timeout))
        if (bWifiModular && 0 == ret)
		{
			bool bChWifi = (0 != strcmp(s_wifiInfo.s_ab_info.bssid, sWifiMac.c_str()));//更改另一个wifi连接直接自动连接
			memset(&s_wifiInfo.s_ab_info, 0, sizeof(ap_info));
			memcpy(&s_wifiInfo.s_ab_info, &mapWifi[sWifiMac].s_ab_info, sizeof(ap_info));
			memset(mapWifi[sWifiMac].cPwd, 0, sizeof(mapWifi[sWifiMac].cPwd));
			strcpy(mapWifi[sWifiMac].cPwd, sPwd.c_str());
			memset(s_wifiInfo.cPwd, 0, sizeof(s_wifiInfo.cPwd));
			strcpy(s_wifiInfo.cPwd, sPwd.c_str());
        //	netDBOpt.setWifiNetDb(s_wifiInfo.s_ab_info.essid, s_wifiInfo.s_ab_info.bssid, s_wifiInfo.s_ab_info.adhoc, s_wifiInfo.s_ab_info.authmode, s_wifiInfo.s_ab_info.secmode, s_wifiInfo.cPwd, s_wifiInfo.cAddr, s_wifiInfo.cMask, s_wifiInfo.cGateway, s_wifiInfo.cDns, s_wifiInfo.iAuto);
			lk.unlock();
			if (1 == s_wifiInfo.iAuto|| bChWifi|| 0==strlen(s_wifiInfo.cAddr))
			{
				wifiSetAutoConfig();
			}
			else {
				if (strlen(s_wifiInfo.cAddr) > 0)
				{
					wifiSetConfig(s_wifiInfo.cAddr, s_wifiInfo.cMask, s_wifiInfo.cGateway, s_wifiInfo.cDns);
				}
			}
			printf("[%s] %d sWifiName[%s]  sWifiMac[%s] wifi_connect ok\n", __FUNCTION__, __LINE__, sWifiName.c_str(), sWifiMac.c_str());
			return true;	
		}
	}
#endif
	return false;
}

bool NetworkConf::wifiSetConfig(const string& sAddr, const string& sMask, const string& sGateway, const string& sDns)
{
	NET_ROUTE route = netRoute;
	net_dhcp_stop();
	netSet_route(NET_ROUTE_WIFI);
	if (0 == net_set_config(sAddr.c_str(), sMask.c_str(), sGateway.c_str(), sDns.c_str()))
	{
		lock_guard<mutex> lk(mtAutoConf);
		t_opt = time(NULL);
		memset(s_wifiInfo.cAddr, 0, sizeof(s_wifiInfo.cAddr));
		strcpy(s_wifiInfo.cAddr, sAddr.c_str());
		memset(s_wifiInfo.cMask, 0, sizeof(s_wifiInfo.cMask));
		strcpy(s_wifiInfo.cMask, sMask.c_str());
		memset(s_wifiInfo.cGateway, 0, sizeof(s_wifiInfo.cGateway));
		strcpy(s_wifiInfo.cGateway, sGateway.c_str());
		memset(s_wifiInfo.cDns, 0, sizeof(s_wifiInfo.cDns));
		strcpy(s_wifiInfo.cDns, sDns.c_str());
		s_wifiInfo.iAuto = 0;
    //	netDBOpt.setWifiNetDb(s_wifiInfo.s_ab_info.essid, s_wifiInfo.s_ab_info.bssid, s_wifiInfo.s_ab_info.adhoc, s_wifiInfo.s_ab_info.authmode, s_wifiInfo.s_ab_info.secmode, s_wifiInfo.cPwd, s_wifiInfo.cAddr, s_wifiInfo.cMask, s_wifiInfo.cGateway, s_wifiInfo.cDns, s_wifiInfo.iAuto);
		return true;
	}
	netSet_route(route);
	return false;
}

bool NetworkConf::wifiSetAutoConfig()
{
	NET_ROUTE route = netRoute;
	net_dhcp_stop();
	netSet_route(NET_ROUTE_WIFI);
	if (0 == net_dhcp_start())
	{
		WIFI_STATUS status; 
		string sWifiName, sWifiMac;
		int rssi;
		if (wifiCheck(status, sWifiName, sWifiMac, rssi))
		{
			lock_guard<mutex> lk(mtAutoConf);
			t_opt = time(NULL);
			s_wifiInfo.iAuto = 1;
        //	netDBOpt.setWifiNetDb(s_wifiInfo.s_ab_info.essid, s_wifiInfo.s_ab_info.bssid, s_wifiInfo.s_ab_info.adhoc, s_wifiInfo.s_ab_info.authmode, s_wifiInfo.s_ab_info.secmode, s_wifiInfo.cPwd, s_wifiInfo.cAddr, s_wifiInfo.cMask, s_wifiInfo.cGateway, s_wifiInfo.cDns, s_wifiInfo.iAuto);
			return true;
		}
		
	}
	netSet_route(route);
	return false;
}


void NetworkConf::wifiDisconnect()
{
	wifi_disconnect();
	networkConnect(NET_ROUTE_ETH);
}

bool NetworkConf::wifiCheck(WIFI_STATUS& status, string& sWifiName, string& sWifiMac, int& rssi)
{
	char cWifiName[128] = { 0 };
	char cWifiMac[128] = { 0 };
	if (0 == wifi_check(&status, cWifiName, cWifiMac, &rssi))
	{
		sWifiName = cWifiName;
		sWifiMac = cWifiMac;
		
		return true;
	}
	return false;
}

bool NetworkConf::wifiGetIsAuto(bool& bAuto)
{
	bAuto = (1 == s_wifiInfo.iAuto);
	return true;
}

bool NetworkConf::wnetLogin(const string& sApn, const string& sUser, const string& sPassword, WNET_AUTH auth, int timeout)
{
	const char* pApn = (0 == sApn.length() ? NULL : sApn.c_str());
	const char* pUser = (0 == sUser.length() ? NULL : sUser.c_str());
	const char* pPassword = (0 == sPassword.length() ? NULL : sPassword.c_str());
	if (!bWirelessModular)
	{
		printf("[%s] %d: bWirelessModular[%d] \n", __FUNCTION__, __LINE__, bWirelessModular);
		return false;
	}
	//检测4G是否已登录
	WNET_STATUS status;
	if (0 == wnet_check(&status))
	{
		if (WNET_STATUS_IDLE != status && WNET_STATUS_INITED != status)
		{
			wnet_logout();
		}
	}
	int  iRet = wnet_login(pApn, pUser, pPassword, auth, timeout);
	if (0 == iRet)
	{
		lock_guard<mutex> lk(mtAutoConf);
		memset(&s_wnetInfo, 0, sizeof(s_wnetInfo));
		strcpy(s_wnetInfo.cApn, sApn.c_str());
		strcpy(s_wnetInfo.cUser, sUser.c_str());
		strcpy(s_wnetInfo.cPassword, sPassword.c_str());
		s_wnetInfo.auth = auth;
		t_opt = time(NULL);
		netSet_route(NET_ROUTE_WL);
    //	netDBOpt.setMobileNetDb(sApn, sUser, sPassword, auth);
		return true;
	}
	printf("[%s] %d: iRet[%d] = wnet_login('%s','%s','%s',%d,%d) \n", __FUNCTION__, __LINE__, iRet, sApn.c_str(), sUser.c_str(), sPassword.c_str(), auth, timeout);
	return false;
}

void NetworkConf::wnetLogout()
{
	wnet_logout();
	networkConnect(NET_ROUTE_ETH);
}

//无线网络信号
bool NetworkConf::wnetGet_signal(int& signal)
{
	if (0 == wnet_get_signal(&signal))
	{
		return true;
	}
	else {
		return false;
	}
}

//无线网络状态
bool NetworkConf::wnetCheck(WNET_STATUS& status)
{
	if (0 == wnet_check(&status))
	{
		return true;
	}
	else {
		return false;
	}
}

//设置有线网络信息
bool NetworkConf::netSetConfig(const string& sAddr, const string& sMask, const string& sGateway, const string& sDns)
{
	NET_ROUTE route = netRoute;
	net_dhcp_stop();
	netSet_route(NET_ROUTE_ETH);
	if (0 == net_set_config(sAddr.c_str(), sMask.c_str(), sGateway.c_str(), sDns.c_str()))
	{
		lock_guard<mutex> lk(mtAutoConf);
		t_opt = time(NULL);
		memset(&s_ethInfo, 0, sizeof(s_ethInfo));
		strcpy(s_ethInfo.cAddr, sAddr.c_str());
		strcpy(s_ethInfo.cMask, sMask.c_str());
		strcpy(s_ethInfo.cGateway, sGateway.c_str());
		strcpy(s_ethInfo.cDns, sDns.c_str());
		s_ethInfo.iAuto = 0;
		
    //	netDBOpt.setEtherNetDb(s_ethInfo.cAddr, s_ethInfo.cMask, s_ethInfo.cGateway, s_ethInfo.cDns, s_ethInfo.iAuto);
		return true;
	}
	netSet_route(route);
	return false;
}

bool NetworkConf::netSetAutoConfig()
{
	NET_ROUTE route = netRoute;
	net_dhcp_stop();
	netSet_route(NET_ROUTE_ETH);
	if (0 == net_dhcp_start())
	{
		lock_guard<mutex> lk(mtAutoConf);
		t_opt = time(NULL);
		s_ethInfo.iAuto = 1;
//		netDBOpt.setEtherNetDb(s_ethInfo.cAddr, s_ethInfo.cMask, s_ethInfo.cGateway, s_ethInfo.cDns, s_ethInfo.iAuto);
		return true;
	}
	netSet_route(route);
	return false;
}
//获取有线网络信息
bool NetworkConf::netGet_configFromRoute(NET_ROUTE net_route, string& sAddr, string& sMask, string& sGateway, string& sDns)
{
	if (NET_ROUTE_MAX == net_route) net_route = netRoute;
	char cAddr[64] = { 0 }, cMask[64] = { 0 },cGateway[64] = { 0 }, cDns[64] = { 0 };
	if (0 == net_get_config(net_route,cAddr, cMask, cGateway, cDns))
	{
		sAddr = cAddr;
		sMask = cMask;
		sGateway = cGateway;
		sDns = cDns;
		return true;
	}
	return false;
}

bool NetworkConf::netGet_Mac(NET_ROUTE net_route, string& sMac)
{
	if (NET_ROUTE_MAX == net_route) net_route = netRoute;
	char cMac[64] = { 0 }, cMacTemp[64] = { 0 };
	if (0 == net_get_mac(net_route,(unsigned char*)cMacTemp))
	{
		sprintf(cMac, "%02x-%02x-%02x-%02x-%02x-%02x", cMacTemp[0], cMacTemp[1], cMacTemp[2], cMacTemp[3], cMacTemp[4], cMacTemp[5]);
		sMac = cMac;
		printf("[%s] %d NetworkConf::netGet_Mac cMac[%s] ok \n", __FUNCTION__, __LINE__, cMac);
		return true;
	}
	return false;
}

//获取 dhcp 服务状态
bool NetworkConf::netDhcp_check()
{
	if (0 == net_dhcp_check())
	{
		return true;
	}
	else {
		return false;
	}
}

bool NetworkConf::netGetIsAuto(bool& bAuto)
{
	bAuto = (1 == s_ethInfo.iAuto);
	return true;
}

void NetworkConf::ThAutoConfPro()
{
	bool bStartPro = true;
	std::this_thread::sleep_for(chrono::seconds(4));
	while (bRunAutoConf)
	{
        printf("func=%s,line=%d\n",__func__,__LINE__);
		if (bStartPro)
		{
			bStartPro = false;
			if (NET_ROUTE_ETH == netRoute)
			{
				networkConnect(NET_ROUTE_ETH);
			}
			else if (bWirelessModular && NET_ROUTE_WL == netRoute) {
				networkConnect(NET_ROUTE_WL);
			}
			else if (bWifiModular && NET_ROUTE_WIFI == netRoute) {
				networkConnect(NET_ROUTE_WIFI);
			}
		}

		time_t t_now=time(0);
		if (difftime(t_now, t_opt) > 30)
		{
			string sAddr = "", sMask = "", sGateway = "", sDns = "";
			bool bRes = netGet_configFromRoute(netRoute, sAddr, sMask, sGateway, sDns);
			//printf("[%s] %d netGet_config netRoute[%d], sAddr[%s], sMask[%s], sGateway[%s], sDns[%s], bRes[%d]\n", __FUNCTION__, __LINE__, netRoute, sAddr.c_str(), sMask.c_str(), sGateway.c_str(), sDns.c_str(), bRes);
			if (!bRes || (bRes && 0 == sAddr.length()))
			{
				if (NET_ROUTE_ETH != netRoute )
				{
                    printf("func=%s,line=%d\n",__func__,__LINE__);
					networkConnect(NET_ROUTE_ETH);
				}
				else if (bWirelessModular && NET_ROUTE_WL != netRoute) {
                    printf("func=%s,line=%d\n",__func__,__LINE__);
					networkConnect(NET_ROUTE_WL);
				}
				else if (bWifiModular && NET_ROUTE_WIFI != netRoute ) {
                    printf("func=%s,line=%d\n",__func__,__LINE__);
					networkConnect(NET_ROUTE_WIFI);
				}
			}
		}
		std::unique_lock<std::mutex> lk(mtAutoConf);
		auto it = mapWifi.begin();
		while(it != mapWifi.end())
		{
			if (0 != it->first.compare(s_wifiInfo.s_ab_info.bssid)) 
			{
				if (difftime(t_now, it->second.t_new) > 3600 * 12)
				{
					mapWifi.erase(it++);
				}
				else {
					++it;
				}
			}
			else {
				++it;
			}
		}
		cvAutoConf.wait_for(lk, chrono::milliseconds(3000));
	}
}



void NetworkConf::timeCheckNet()
{
        printf("func=%s,line=%d\n",__func__,__LINE__);
        string sAddr = "", sMask = "", sGateway = "", sDns = "";
        bool bRes = netGet_configFromRoute(netRoute, sAddr, sMask, sGateway, sDns);
        printf("[%s] %d netGet_config netRoute[%d], sAddr[%s], sMask[%s], sGateway[%s], sDns[%s], bRes[%d]\n", __FUNCTION__, __LINE__, netRoute, sAddr.c_str(), sMask.c_str(), sGateway.c_str(), sDns.c_str(), bRes);
        if(0 != sAddr.length())
        {
            netIpAddr = sAddr;
        }
        else
        {
            netIpAddr.clear();
        }
        if (!bRes || (bRes && 0 == sAddr.length()))
        {
            if (NET_ROUTE_ETH == netRoute )
            {
                printf("func=%s,line=%d\n",__func__,__LINE__);
                networkConnect(NET_ROUTE_ETH);
            }
            else if (bWirelessModular && NET_ROUTE_WL == netRoute) {
                printf("func=%s,line=%d\n",__func__,__LINE__);
                networkConnect(NET_ROUTE_WL);
            }
            else if (bWifiModular && NET_ROUTE_WIFI == netRoute ) {
                printf("func=%s,line=%d\n",__func__,__LINE__);
                networkConnect(NET_ROUTE_WIFI);
            }
        }
}



void NetworkConf::networkConnect(NET_ROUTE net_route)
{
	NET_ROUTE route = netRoute;
	if (NET_ROUTE_ETH == net_route)
	{
		if (1 == s_ethInfo.iAuto)
		{
			printf("[%s] %d netDhcp_start()\n", __FUNCTION__, __LINE__);
			if (!netSetAutoConfig())
			{
				netSet_route(route);
			}

		}
		else {
			if (strlen(s_ethInfo.cAddr) > 0 && strlen(s_ethInfo.cMask) > 0 && strlen(s_ethInfo.cGateway) > 0 && strlen(s_ethInfo.cDns)>0)
			{
				printf("[%s] %d netSet_config\n", __FUNCTION__, __LINE__);
				if (!netSetConfig(s_ethInfo.cAddr, s_ethInfo.cMask, s_ethInfo.cGateway, s_ethInfo.cDns))
				{
					netSet_route(route);
				}
			}
		}
	}
	else if (NET_ROUTE_WL == net_route&& bWirelessModular) {
		printf("[%s] %d start wnetLogin\n", __FUNCTION__, __LINE__);
		if (wnetLogin(s_wnetInfo.cApn, s_wnetInfo.cUser, s_wnetInfo.cPassword, s_wnetInfo.auth))
		{
			printf("[%s] %d wnetLogin ok\n", __FUNCTION__, __LINE__);
		}
		else {
			netSet_route(route);
		}
	}
	else if (NET_ROUTE_WIFI == net_route && bWifiModular) {
		printf("[%s] %d start wifiConnect\n", __FUNCTION__, __LINE__);
#if 1
		if (wifiConnect(s_wifiInfo.s_ab_info.essid, s_wifiInfo.s_ab_info.bssid, s_wifiInfo.cPwd))
		{
			printf("[%s] %d wifiConnect ok\n", __FUNCTION__, __LINE__);
		}
		else {
			printf("[%s] %d wifiConnect fail\n", __FUNCTION__, __LINE__);
			return;
		}

		if (1 != s_wifiInfo.iAuto&&strlen(s_wifiInfo.cAddr)>0)
		{
			if (!wifiSetConfig(s_wifiInfo.cAddr, s_wifiInfo.cMask, s_wifiInfo.cGateway, s_wifiInfo.cDns))
			{
				netSet_route(route);
			}
		}
		else {
			if (!wifiSetAutoConfig())
			{
				netSet_route(route);
			}
		}
#endif
	}
}

//设置路由
bool NetworkConf::netSet_route(NET_ROUTE net_route)
{
	if (0 == net_set_route(net_route))
	{
		netRoute = net_route;
    //	netDBOpt.setRouteNetDb(netRoute);
		return true;
	}
	else {
		return false;
	}
}


//获取路由
bool NetworkConf::netGet_route(NET_ROUTE& net_route)
{

	if (0 == net_get_route(&net_route))
	{
		netRoute = net_route;
		printf("[%s] %d NetworkConf::netGet_route netRoute[%d] ok \n", __FUNCTION__, __LINE__, net_route);
		return true;
	}
	else {
		printf("[%s] %d NetworkConf::netGet_route faile \n", __FUNCTION__, __LINE__);
		return false;
	}
}

void NetworkConf::isExistWifiAndWnet(bool& bWifi, bool& bWent)
{
	bWifi = bWifiModular;
	bWent = bWirelessModular;
}

bool NetworkConf::netPing(string sAddr, int timeout)
{
	return 0 == net_ping(sAddr.c_str(), timeout);
}

string NetworkConf::netIp()
{
    return netIpAddr;
}

#endif
