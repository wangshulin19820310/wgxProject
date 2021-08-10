#pragma once
#include<string>
/**********自定义结构体**********/
//二代身份证结构体
typedef struct S_IdentityData
{
	char id[64];            //身份证号或外国人居住证号(外国人居住证)
	char name[128];        	//姓名 
	char gender[16];        //性别
	char nation[128];  		//民族或国籍(外国人居住证)     
	char birthday[32];     	//出生日期
	char card_type[32];     //卡类型
	char address[128];       //住址
	char grant_dept[128];     //发证机关
	char bmpFile[128];        //身份证照片(bmp格式)
}*PS_IdentityData;
//人脸识别算法
typedef enum E_Face_Algorithm {
	FACE_KS ,//旷世算法
	FACE_YTLF,  //云天励飞算法
}E_Face_Algorithm;
typedef enum E_User_State {
	USER_STATE_REAL_UNREGISTERED=1,//未注册
	USER_STATE_REAL_REGISTERED,  //已注册
}E_UserState;
//人脸识别信息结构体
typedef struct S_UserInfo {
	uint64_t nId;   //识别到人脸对应的id
	E_User_State state;
	float fScore; //识别相似分数
	int iMask;//-1 无法确定,0没带口罩，1带口罩
	int fmt;//视频格式
	int width;//宽
	int height;//高 
	int x;    //x,y,w,h对应视频人脸位置
	int y; 
	int w; 
	int h;
	void* ptr;//现场视频数据
}PS_UserInfo;
//图像数据格式类型
typedef enum E_ImgDataType {
	ImgJpg, //jpg格式
	ImgPng,  //png格式
}E_ImgDataType;

/**********定义回调**********/
//测温回调，fValue，-1：则错误，0，测到的温度小于35.5,>=35.5则是人体温度；sErr错误信息,pObj 调用者的对象
using FuncTempCallback = void(*)(float fValue, std::string sErr, void* pObj);
//韦根卡号回调，sCardNo,为卡号；sErr错误信息,pObj 调用者的对象
using FuncCardNoCb = void(*)(std::string sCardNo, std::string sErr, void* pObj);
//二代身份信息回调，pObj 调用者的对象
using FuncIdentityDataCb = void(*)(S_IdentityData s_IdentityData, void* pObj);
//非接读卡器回调，ucUID为UID数据,iLen为数据长度，pObj 调用者的对象
using FuncCardReaderDataCb = void(*)(unsigned char* ucUID,int iLen, void* pObj);
//人脸框信息回调，left，top,right，bottom是人脸框位置，都为0，则没人脸。
using FuncFaceBoxCb=void(*)(int left, int top, int right, int bottom, void* pObj);
//人脸识别信息回调。
using FuncUserInfoCb = void(*)(S_UserInfo* pUserInfo, void* pObj);

//接收数据回调
using FuncServerRecvdCb = int(*)(long lNet, const char* data, int len, void* pObj);

//客户端连接成功回调
using FuncClientConnectedCb= void(*)(long lNet, const char* pAddr, int nPort, void* pObj);

//客户端连接断开回调
using FuncClientCloseCb = void(*)(long lNet, void* pObj);
