#ifndef __LIB_MQTT_H__
#define __LIB_MQTT_H__

enum MQTT_QOS{
	ONECE_MAX=0,
	ONECE_LATEST,
	ONECE_ONLY
	};//消息质量 最多一次，最少一次，只有一次

typedef  void**  MQTT_HANDLE;
typedef void (*MSG_RECIVE_CALLBACL)(char* topic, char* msg,int msglen);


#ifdef __cplusplus
extern "C"
{
#endif
/*
函数说明:
	初始化
参数:
	@parame[in]:mqtt客户端ID 
	@parame[in]:代理服务器地址ip:port 或者tcp://ip:port
	@parame[in]:用户名，没有验证可以为NULL
	@parame[in]:密码，没有验证可以为NULL
	@parame[in]:接受订阅信息回调函数
返回值:
	success mqtt句柄
	faile null
*/
MQTT_HANDLE mqtt_init(char *client_id,char* addres,char* username,char *password,MSG_RECIVE_CALLBACL recive_msg_callback);
/*
函数说明:
	订阅
参数:
	@parame[in]:mqtt句柄
	@parame[in]:发布的主题
	@parame[in]:消息质量 MQTT_QOS
返回值:
	success > 0
	faile <0
*/
int mqtt_subscribe(MQTT_HANDLE handle,char* topic,int qos);
/*
函数说明:
	发布
参数:
	@parame[in]:mqtt句柄
	@parame[in]:发布的主题
	@parame[in]:发布的荷载
	@parame[in]:荷载长度
	@parame[in]:消息质量 MQTT_QOS
返回值:
	success > 0
	faile <0
*/
int mqtt_publish(MQTT_HANDLE handle,char* topic,char *msg,int msg_len,int qos);
/*
函数说明:
	销毁库
参数:
	@parame[in]:mqtt句柄
返回值:
	null
*/
void mqtt_destroy(MQTT_HANDLE handle);
#ifdef __cplusplus
}
#endif
#endif

