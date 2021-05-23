#include<reg52.h>
#ifndef __QXA51_H__
#define __QXA51_H__

/*�������ģ��*/
sbit IN1 = P1^2; //Ϊ1:������ת
sbit IN2 = P1^3; //Ϊ1:������ת
sbit IN3 = P1^6; //Ϊ1:�ҵ����ת
sbit IN4 = P1^7; //Ϊ1:�ҵ����ת
sbit EN1 = P1^4; //Ϊ1:����ʹ��
sbit EN2 = P1^5; //Ϊ1:�ҵ��ʹ��
/*ѭ��*/
sbit left_led1 = P3^3;	//��Ѱ���ź� Ϊ0:û��ʶ�𵽺��� Ϊ1:ʶ�𵽺���
sbit right_led1 = P3^2;	//��Ѱ���ź� Ϊ0:û��ʶ�𵽺��� Ϊ1:ʶ�𵽺���
sbit left_led2 = P3^4;	//������ź� Ϊ0:ʶ���ϰ��� 	Ϊ1:û��ʶ���ϰ���
sbit right_led2 = P3^5;	//�ұ����ź� Ϊ0:ʶ���ϰ��� 	Ϊ1:û��ʶ���ϰ���
/*�����ģ��*/
sbit dula = P2^6;			//����ܶ�ѡ
sbit wela = P2^7;			//�����λѡ
/*������ģ��*/
sbit RX = P2^0;				//ECHO������ģ������
sbit TX = P2^1;				//TRIG������ģ�鴥����
/*1602Һ����ģ��*/
sbit LCD_RW = P3^6;     //LCD��дѡ���, 0Ϊд, 1Ϊ��
sbit LCD_RS = P3^5;		//LCD���ݡ�����ѡ���, 0Ϊ����, 1Ϊ����
sbit LCD_EN = P3^4;		//LCDʹ�ܶ�
/*������ģ��*/
sbit beep = P2^3;		//������
/*������������*/
sbit key_s2 = P3^0;
sbit key_s3 = P3^1;

/*���*/
#define left_motor_en		EN1 = 1				//����ʹ��
#define right_motor_en		EN2 = 1				//�ҵ��ʹ��
#define left_motor_stop		IN1 = 0, IN2 = 0	//����ֹͣ
#define right_motor_stop	IN3 = 0, IN4 = 0	//�ҵ��ֹͣ
#define left_motor_go		IN1 = 0, IN2 = 1	//��������
#define left_motor_back		IN1 = 1, IN2 = 0	//������ת
#define right_motor_go		IN3 = 1, IN4 = 0	//�ҵ������
#define right_motor_back	IN3 = 0, IN4 = 1	//�ҵ����ת
/*������*/
#define LCD_data  P0    //Һ�������ݿ�
#define BUSY 0x80       //���ڼ��LCD��BF, BF=0,��ʱ�ɽ����ⲿָ�������

#endif