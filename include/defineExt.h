#pragma once
#include<string>
/**********�Զ���ṹ��**********/
//�������֤�ṹ��
typedef struct S_IdentityData
{
	char id[64];            //���֤�Ż�����˾�ס֤��(����˾�ס֤)
	char name[128];        	//���� 
	char gender[16];        //�Ա�
	char nation[128];  		//��������(����˾�ס֤)     
	char birthday[32];     	//��������
	char card_type[32];     //������
	char address[128];       //סַ
	char grant_dept[128];     //��֤����
	char bmpFile[128];        //���֤��Ƭ(bmp��ʽ)
}*PS_IdentityData;
//����ʶ���㷨
typedef enum E_Face_Algorithm {
	FACE_KS ,//�����㷨
	FACE_YTLF,  //���������㷨
}E_Face_Algorithm;
typedef enum E_User_State {
	USER_STATE_REAL_UNREGISTERED=1,//δע��
	USER_STATE_REAL_REGISTERED,  //��ע��
}E_UserState;
//����ʶ����Ϣ�ṹ��
typedef struct S_UserInfo {
	uint64_t nId;   //ʶ��������Ӧ��id
	E_User_State state;
	float fScore; //ʶ�����Ʒ���
	int iMask;//-1 �޷�ȷ��,0û�����֣�1������
	int fmt;//��Ƶ��ʽ
	int width;//��
	int height;//�� 
	int x;    //x,y,w,h��Ӧ��Ƶ����λ��
	int y; 
	int w; 
	int h;
	void* ptr;//�ֳ���Ƶ����
}PS_UserInfo;
//ͼ�����ݸ�ʽ����
typedef enum E_ImgDataType {
	ImgJpg, //jpg��ʽ
	ImgPng,  //png��ʽ
}E_ImgDataType;

/**********����ص�**********/
//���»ص���fValue��-1�������0���⵽���¶�С��35.5,>=35.5���������¶ȣ�sErr������Ϣ,pObj �����ߵĶ���
using FuncTempCallback = void(*)(float fValue, std::string sErr, void* pObj);
//Τ�����Żص���sCardNo,Ϊ���ţ�sErr������Ϣ,pObj �����ߵĶ���
using FuncCardNoCb = void(*)(std::string sCardNo, std::string sErr, void* pObj);
//���������Ϣ�ص���pObj �����ߵĶ���
using FuncIdentityDataCb = void(*)(S_IdentityData s_IdentityData, void* pObj);
//�ǽӶ������ص���ucUIDΪUID����,iLenΪ���ݳ��ȣ�pObj �����ߵĶ���
using FuncCardReaderDataCb = void(*)(unsigned char* ucUID,int iLen, void* pObj);
//��������Ϣ�ص���left��top,right��bottom��������λ�ã���Ϊ0����û������
using FuncFaceBoxCb=void(*)(int left, int top, int right, int bottom, void* pObj);
//����ʶ����Ϣ�ص���
using FuncUserInfoCb = void(*)(S_UserInfo* pUserInfo, void* pObj);

//�������ݻص�
using FuncServerRecvdCb = int(*)(long lNet, const char* data, int len, void* pObj);

//�ͻ������ӳɹ��ص�
using FuncClientConnectedCb= void(*)(long lNet, const char* pAddr, int nPort, void* pObj);

//�ͻ������ӶϿ��ص�
using FuncClientCloseCb = void(*)(long lNet, void* pObj);
