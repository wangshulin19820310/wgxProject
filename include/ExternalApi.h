#pragma once
#include<memory>
#include<vector>
#include"defineExt.h"
#ifdef WIN32
#ifdef EXTERNALLIB_API_EXPORTS
#define EXTERNALLIB_API __declspec(dllexport)
#else
#define EXTERNALLIB_API __declspec(dllimport)
#endif
#else
#define EXTERNALLIB_API
#endif
class LibTcpServer;
 class EXTERNALLIB_API ExternalApi
{
public:
	
	/**********摄像**********/
	//设置摄像旋转(0,90,180,270,默认0[竖屏])横屏的话要设置为270
	static void setRotationAngle(int iAngle);
	//设置摄像显示位置(默认[0,0,800,1280])
	static void setDisplayRect(int x, int y, int w, int h);
	//抓拍摄像图像Jpg或png
	static bool captureToPicture(std::string sFile, E_ImgDataType eImgType);
	//抓拍摄像二进制数据
	static bool captureToData(std::vector<unsigned char>& vecData, E_ImgDataType eImgType= ImgJpg);
	/**********旷世人脸识别接口**********/
	//设置算法
	static void setFace_Algorithm(E_Face_Algorithm eAlgorithm);
	//设置云天励飞算法认证配置
	static void setYtlfLicenseConfig(std::string sServerIp, std::string  sServerPort, std::string sApiKey, std::string sApiSecret);
	//设置人脸框和识别信息回调
	static void setRegisterFaceCB(FuncFaceBoxCb cbFaceBox, FuncUserInfoCb cbUserInfo, void* pObj);
	//单个载入人脸体征信息
	static bool loadFeatureInfo(const void* pDetect, int iLen, uint64_t nFaceId, std::string& sError);
	//单个删除人脸体征信息
	static bool removeFeatureInfo(uint64_t nFaceId, std::string& sError);
	//清空人脸识别
	static void clearDetect();
	//人脸图片文件提取特征值
	static bool extractFeature(const char* sPicPath, void* pDetect, int& iLen, std::string& sError);
	//人脸图片数据提取特征值
	static bool extractFeatureFormData(std::vector<unsigned char> vecData, void* pDetect, int& iLen, std::string& sError);
	//识别库是否已存在该特征值
	static bool isExistsFeature(const void* pDetect, int iLen, uint64_t& nFaceId, std::string& sError);
	//设置识别阀值,系统默认71，可设置范围(65--100)之间
	static bool setRecogThreshold(int iScore);
	//获取识别阀值
	static bool getRecogThreshold(int& iScore);
	//将人脸视频帧保存为Jpg或png
	static bool convertToPicture(const S_UserInfo* pUserInfo, std::string sFile, E_ImgDataType eImgType,bool bOnlyFace=true);
	//将人脸视频帧保存为二进制(jpg或png)
	static bool convertToData(const S_UserInfo* pUserInfo, std::vector<unsigned char>& vecData, E_ImgDataType eImgType = ImgJpg, bool bOnlyFace = true);

	/**********二代身份证或非接读卡器刷卡接口**********/
	//设置二代证离线或在线刷卡信息回调
	static bool setIdentityCB(FuncIdentityDataCb cb,bool bOnLine,void* pObj);
	//设置非接读卡器刷卡信息回调
	static bool setCardReaderCB(FuncCardReaderDataCb cb, void* pObj);

	/**********韦根接口**********/
	//设置韦根卡号回调
	static void setCardNoCB(FuncCardNoCb cb, void* pObj);


	/**********体温接口**********/
	//设置温度回调
	static void setTempCB(FuncTempCallback cb, void* pObj);
	//设置温度补偿
	static void setTempOffset(float fTemp);


	/**********灯光控制接口**********/
	 //设置补光灯亮度值level(0--255)。
	static bool setLightBrightness(int level, std::string& sError);
	//获取当前设定的补光灯的亮度值level(0--255)。
	static bool getLightBrightness(int& level, std::string& sError);
	//实时的环境光照亮度值value(0--255)。
	static bool getLightSensorValue(int& value, std::string& sError);
	//打开或关闭红外线光flag 0：关闭，1:打开
	static bool setIrLight(int flag, std::string& sError);


	/**********声音接口**********/
	//播放声音(wav文件)
	static void playAudio(std::string sAudioFile);
	//停止播放(wav文件)
	static void stopAudio();
	//是否正在播放
	static bool isPlaying();

	/**********视频接口**********由于库冲突暂时无法播放视频/
	//视频显示位置（x,y,w,h都为0时全屏播放,默认全屏）
	/*static void setVideoDisplayRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	//设置播放源列表
	static bool setDataSource(std::vector<std::string> vecFilePath);
	//开始播放
	static bool startPlay();
	//停止播放
	static bool stopPlay();
	//暂停播放
	static bool pausePlay();*/

	/**********闸机接口**********/
	//查询继电器数目
	static bool getRelayCount(int& iRelayCount);
	//查询门磁数目
	static bool getDoorCount(int& iDoorCount);
	//控制继电器,iId:继电器编号（1,2,3）； iEnable:1,则打开，0则关闭；sError 返回错误信息
	static bool gateOpt(int iId, int iEnable,std::string& sError);
	//查询门磁状态,iId:继电器编号（1,2,3）； iValue:门磁状态；sError 返回错误信息
	static bool getDoorState(int iId, int& iValue, std::string& sError);
	//设置是否自动延迟关门，iDelay为延时时间ms，bAutoClose是否到设定时间后关闭，默认不自动关闭
	static bool setDelayClose(int iDelay, bool bAutoClose);
	//获取是否自动延时关门
	static bool getDelayClose(int& iDelay, bool& bAutoClose);


	/**********其他接口**********/
	 //设置时区(所有时区参数在/usr/share/zoneinfo目录下,如/usr/share/zoneinfo/Asia/Shanghai则Asia/Shanghai)
	static bool setZone(const std::string& sZone);
	//获取时区
	static bool getZone(std::string& sZone);
	//校正系统时间
	static bool setTime(int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec);
	//获取系统时间
	static bool getTime(int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int& iSec, int& iWeek);
	//获取系统音量
	static bool getVolume(int& iVolume);
	//设置系统音量(0--255)
	static bool setVolume(int iVolume);
	//是否打开日志打印信息
	static void setLogPrint(bool bOn = true);
};
 /*tcp服务接口*/
 class EXTERNALLIB_API TcpServerApi
 {
 public:
	 /*
	nProt	服务器监听端口
	nTimeOut	客户端连接超时时间(毫秒)，默认120000(120秒)
	*/
	 bool TCPServerInit(FuncClientConnectedCb CbConnected, FuncServerRecvdCb CbRecvd, FuncClientCloseCb CbClose, int nProt, int nTimeOut= 120000, void* pObj=NULL);
	 //发送数据 
	 bool TCPServerSend(long lNet, char* buffer, int nlen);
 public:
	 TcpServerApi();
	 virtual ~TcpServerApi();
 private:
	 std::shared_ptr<LibTcpServer> ptrTcpServer = nullptr;

 };

