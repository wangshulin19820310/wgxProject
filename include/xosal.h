#ifndef __XOSAL_H__
#define __XOSAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define XOSAL_LIB_VERSION "LIB_VERSION_1.00"

typedef		unsigned short	    uint16;
typedef		unsigned int	    uint32;
typedef		unsigned char	    uint8;

#ifndef NULL 
#define NULL (void *)0
#endif

enum {COM0, COM1, COM2, COM3, COM4, USB0, USB1, USB2};
enum {BOARD_ID_R19S=0xB1, BOARD_ID_R19N, BOARD_ID_R19P};

/* private interface */
void print_hex(char *tip, unsigned char *in_data, int in_len);
void debug_on();
void debug_off();

/* public interface */
int xosal_set_relay(int enable);
int xosal_get_relay(int *value);

int xosal_get_radar_signal(int *value);

int xosal_tty_open(int channel, const char *attr);
void xosal_tty_close(int channel);
int xosal_tty_send(int channel, const uint8 *in_data, int size);
int xosal_tty_reset(int channel);
int xosal_tty_receive(int channel, uint8 *out_data, int size, unsigned int timeoutMs);

int xosal_set_locale(char *zone_name);
int xosal_set_time(int *time);
int xosal_get_time(int *time);
int xosal_set_backlight(int level);
int xosal_set_alarm(int day, int hour, int min, int sec);
int xosal_set_poweroff(int day, int hour, int min, int sec);
int xosal_set_volume(int level);
int xosal_get_volume(int *level);
int xosal_read_sn(char *sn);
int xosal_get_meminfo(char *info);
int xosal_get_flashinfo(char *info);

unsigned char asc2hex(unsigned char *data_in);
int xosal_get_bom_id(int *board_id);

/* XOSAL RETURN CODE DEFINITION */
#define XOSAL_RET_BASE          (0xA000)
#define XOSAL_RET_OK            0
#define XOSAL_RET_ERR_PARA      -(XOSAL_RET_BASE + 1)
#define XOSAL_RET_ERR_OPEN_DEV  -(XOSAL_RET_BASE + 2)
#define XOSAL_RET_ERR_IOCTL_DEV -(XOSAL_RET_BASE + 3)
#define XOSAL_RET_ERR_SYS       -(XOSAL_RET_BASE + 4)

#ifdef __cplusplus
}
#endif

#endif /* __XOSAL_H__ */
