#ifndef __OTHER_H__
#define __OTHER_H__

/*�ŶӶ��庯��*/
extern void shut_nt();
extern void keyscan();
extern void Init_interruptor();
extern void StartModule();
extern void DisplayDistance(void);
extern void Avoid();
extern void delayms(unsigned int z);
extern void Delay10us(unsigned char i);
extern unsigned char read_cmd(void);
extern void write_data(unsigned char wdlcd);
extern void write_cmd(unsigned char wclcd,unsigned char busyc);
extern void Init_LCD(void);
extern void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
extern void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
extern void forward();
extern void backward();
extern void stop();
extern void left_rapidly();

/*��������*/
extern unsigned char pwm_left_val;  	//����ռ�ձ�ֵȡֵ��Χ0-170,0���
extern unsigned char pwm_right_val;  	//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170,0���
extern unsigned char pwm_t;				//����
extern unsigned int  time;				//����ʱ��
extern unsigned long S;					//����
extern bit flag;						//����������Χ��־λ
extern unsigned char code range[];  	//LCD1602��ʾ��ʽ
extern unsigned char code ASCII[13];
extern unsigned char code table[]; 	 
extern unsigned char code table1[];   
extern unsigned char disbuff[4];      	//��ʾ���뻺��	

#endif
