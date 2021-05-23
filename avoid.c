#include <reg52.h> 				//51ͷ�ļ�
#include <intrins.h>   			//����nop��ϵͳ����
#include "..\CONFIG\QXA51.h"	//QX-A51����С�������ļ�
#include "..\CONFIG\other.h"	//�Լ���д������ļ�

unsigned char pwm_left_val  = 120;  	//����ռ�ձ�ֵȡֵ��Χ0-170,0���
unsigned char pwm_right_val = 110;      //�ҵ��ռ�ձ�ֵȡֵ��Χ0-170,0���
unsigned char pwm_t;					//����
unsigned int  time = 0;					//����ʱ��
unsigned long S = 0;					//����
bit flag = 0;							//����������Χ��־λ
unsigned char code range[]   = "DISTANCE";  			 //LCD1602��ʾ��ʽ
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]   = "range:000.0cm"; 	 
unsigned char code table1[]  = "Out of range!";   
unsigned char disbuff[4] = {0,0,0,0};   				  //��ʾ���뻺��	  

/*��ʱ��0�ж�*/
void timer0() interrupt 1		     //T0�ж��������������,������෶Χ
{
	flag=1;							 //�ж������־			 
}

/*��ʱ��1�ж����PWM�ź�*/
void timer1() interrupt 3
{
	pwm_t++;					//������
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)	//����ռ�ձ�	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)	//�ҵ��ռ�ձ�
		EN2 = 1;			 
}

void main()
{
	shut_nt();		//�������
	Init_LCD(); 	//LCD��ʼ��
	delayms(5);		//��ʱ
	DisplayListChar(0,0,range);		//1602��һ����ʾrange��������
	DisplayListChar(0,1,table);		//1602�ڶ�����ʾtable��������
	keyscan();				//�ȴ�����S2����С��
	delayms(1000);			//��ʱ1��
	Init_interruptor();	 	//��ʼ����ʱ��
	while(1)
	{
		 StartModule();			//����ģ����

		 while(!RX);			//��RX��ECHO�źŻ��죩Ϊ��ʱ�ȴ�
		 TR0=1;			   		//��������
		 while(RX);				//��RXΪ1�������ȴ�
		 TR0=0;					//�رռ���

	     DisplayDistance();		//������ʾ����
		 Avoid();				//����
		 delayms(65);			//�������ڲ�����60MS		  
	}
}