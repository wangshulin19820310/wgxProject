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
	
	/**********����**********/
	//����������ת(0,90,180,270,Ĭ��0[����])�����Ļ�Ҫ����Ϊ270
	static void setRotationAngle(int iAngle);
	//����������ʾλ��(Ĭ��[0,0,800,1280])
	static void setDisplayRect(int x, int y, int w, int h);
	//ץ������ͼ��Jpg��png
	static bool captureToPicture(std::string sFile, E_ImgDataType eImgType);
	//ץ���������������
	static bool captureToData(std::vector<unsigned char>& vecData, E_ImgDataType eImgType= ImgJpg);
	/**********��������ʶ��ӿ�**********/
	//�����㷨
	static void setFace_Algorithm(E_Face_Algorithm eAlgorithm);
	//�������������㷨��֤����
	static void setYtlfLicenseConfig(std::string sServerIp, std::string  sServerPort, std::string sApiKey, std::string sApiSecret);
	//�����������ʶ����Ϣ�ص�
	static void setRegisterFaceCB(FuncFaceBoxCb cbFaceBox, FuncUserInfoCb cbUserInfo, void* pObj);
	//������������������Ϣ
	static bool loadFeatureInfo(const void* pDetect, int iLen, uint64_t nFaceId, std::string& sError);
	//����ɾ������������Ϣ
	static bool removeFeatureInfo(uint64_t nFaceId, std::string& sError);
	//�������ʶ��
	static void clearDetect();
	//����ͼƬ�ļ���ȡ����ֵ
	static bool extractFeature(const char* sPicPath, void* pDetect, int& iLen, std::string& sError);
	//����ͼƬ������ȡ����ֵ
	static bool extractFeatureFormData(std::vector<unsigned char> vecData, void* pDetect, int& iLen, std::string& sError);
	//ʶ����Ƿ��Ѵ��ڸ�����ֵ
	static bool isExistsFeature(const void* pDetect, int iLen, uint64_t& nFaceId, std::string& sError);
	//����ʶ��ֵ,ϵͳĬ��71�������÷�Χ(65--100)֮��
	static bool setRecogThreshold(int iScore);
	//��ȡʶ��ֵ
	static bool getRecogThreshold(int& iScore);
	//��������Ƶ֡����ΪJpg��png
	static bool convertToPicture(const S_UserInfo* pUserInfo, std::string sFile, E_ImgDataType eImgType,bool bOnlyFace=true);
	//��������Ƶ֡����Ϊ������(jpg��png)
	static bool convertToData(const S_UserInfo* pUserInfo, std::vector<unsigned char>& vecData, E_ImgDataType eImgType = ImgJpg, bool bOnlyFace = true);

	/**********�������֤��ǽӶ�����ˢ���ӿ�**********/
	//���ö���֤���߻�����ˢ����Ϣ�ص�
	static bool setIdentityCB(FuncIdentityDataCb cb,bool bOnLine,void* pObj);
	//���÷ǽӶ�����ˢ����Ϣ�ص�
	static bool setCardReaderCB(FuncCardReaderDataCb cb, void* pObj);

	/**********Τ���ӿ�**********/
	//����Τ�����Żص�
	static void setCardNoCB(FuncCardNoCb cb, void* pObj);


	/**********���½ӿ�**********/
	//�����¶Ȼص�
	static void setTempCB(FuncTempCallback cb, void* pObj);
	//�����¶Ȳ���
	static void setTempOffset(float fTemp);


	/**********�ƹ���ƽӿ�**********/
	 //���ò��������ֵlevel(0--255)��
	static bool setLightBrightness(int level, std::string& sError);
	//��ȡ��ǰ�趨�Ĳ���Ƶ�����ֵlevel(0--255)��
	static bool getLightBrightness(int& level, std::string& sError);
	//ʵʱ�Ļ�����������ֵvalue(0--255)��
	static bool getLightSensorValue(int& value, std::string& sError);
	//�򿪻�رպ����߹�flag 0���رգ�1:��
	static bool setIrLight(int flag, std::string& sError);


	/**********�����ӿ�**********/
	//��������(wav�ļ�)
	static void playAudio(std::string sAudioFile);
	//ֹͣ����(wav�ļ�)
	static void stopAudio();
	//�Ƿ����ڲ���
	static bool isPlaying();

	/**********��Ƶ�ӿ�**********���ڿ��ͻ��ʱ�޷�������Ƶ/
	//��Ƶ��ʾλ�ã�x,y,w,h��Ϊ0ʱȫ������,Ĭ��ȫ����
	/*static void setVideoDisplayRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	//���ò���Դ�б�
	static bool setDataSource(std::vector<std::string> vecFilePath);
	//��ʼ����
	static bool startPlay();
	//ֹͣ����
	static bool stopPlay();
	//��ͣ����
	static bool pausePlay();*/

	/**********բ���ӿ�**********/
	//��ѯ�̵�����Ŀ
	static bool getRelayCount(int& iRelayCount);
	//��ѯ�Ŵ���Ŀ
	static bool getDoorCount(int& iDoorCount);
	//���Ƽ̵���,iId:�̵�����ţ�1,2,3���� iEnable:1,��򿪣�0��رգ�sError ���ش�����Ϣ
	static bool gateOpt(int iId, int iEnable,std::string& sError);
	//��ѯ�Ŵ�״̬,iId:�̵�����ţ�1,2,3���� iValue:�Ŵ�״̬��sError ���ش�����Ϣ
	static bool getDoorState(int iId, int& iValue, std::string& sError);
	//�����Ƿ��Զ��ӳٹ��ţ�iDelayΪ��ʱʱ��ms��bAutoClose�Ƿ��趨ʱ���رգ�Ĭ�ϲ��Զ��ر�
	static bool setDelayClose(int iDelay, bool bAutoClose);
	//��ȡ�Ƿ��Զ���ʱ����
	static bool getDelayClose(int& iDelay, bool& bAutoClose);


	/**********�����ӿ�**********/
	 //����ʱ��(����ʱ��������/usr/share/zoneinfoĿ¼��,��/usr/share/zoneinfo/Asia/Shanghai��Asia/Shanghai)
	static bool setZone(const std::string& sZone);
	//��ȡʱ��
	static bool getZone(std::string& sZone);
	//У��ϵͳʱ��
	static bool setTime(int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec);
	//��ȡϵͳʱ��
	static bool getTime(int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int& iSec, int& iWeek);
	//��ȡϵͳ����
	static bool getVolume(int& iVolume);
	//����ϵͳ����(0--255)
	static bool setVolume(int iVolume);
	//�Ƿ����־��ӡ��Ϣ
	static void setLogPrint(bool bOn = true);
};
 /*tcp����ӿ�*/
 class EXTERNALLIB_API TcpServerApi
 {
 public:
	 /*
	nProt	�����������˿�
	nTimeOut	�ͻ������ӳ�ʱʱ��(����)��Ĭ��120000(120��)
	*/
	 bool TCPServerInit(FuncClientConnectedCb CbConnected, FuncServerRecvdCb CbRecvd, FuncClientCloseCb CbClose, int nProt, int nTimeOut= 120000, void* pObj=NULL);
	 //�������� 
	 bool TCPServerSend(long lNet, char* buffer, int nlen);
 public:
	 TcpServerApi();
	 virtual ~TcpServerApi();
 private:
	 std::shared_ptr<LibTcpServer> ptrTcpServer = nullptr;

 };

