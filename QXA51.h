#include<reg52.h>
#ifndef __QXA51_H__
#define __QXA51_H__

/*电机驱动模块*/
sbit IN1 = P1^2; //为1:左电机反转
sbit IN2 = P1^3; //为1:左电机正转
sbit IN3 = P1^6; //为1:右电机正转
sbit IN4 = P1^7; //为1:右电机反转
sbit EN1 = P1^4; //为1:左电机使能
sbit EN2 = P1^5; //为1:右电机使能
/*循迹*/
sbit left_led1 = P3^3;	//左寻迹信号 为0:没有识别到黑线 为1:识别到黑线
sbit right_led1 = P3^2;	//右寻迹信号 为0:没有识别到黑线 为1:识别到黑线
sbit left_led2 = P3^4;	//左避障信号 为0:识别障碍物 	为1:没有识别到障碍物
sbit right_led2 = P3^5;	//右避障信号 为0:识别障碍物 	为1:没有识别到障碍物
/*数码管模块*/
sbit dula = P2^6;			//数码管段选
sbit wela = P2^7;			//数码管位选
/*超声波模块*/
sbit RX = P2^0;				//ECHO超声波模块回响端
sbit TX = P2^1;				//TRIG超声波模块触发端
/*1602液晶屏模块*/
sbit LCD_RW = P3^6;     //LCD读写选择端, 0为写, 1为读
sbit LCD_RS = P3^5;		//LCD数据、命令选择端, 0为命令, 1为数据
sbit LCD_EN = P3^4;		//LCD使能端
/*蜂鸣器模块*/
sbit beep = P2^3;		//蜂鸣器
/*独立按键定义*/
sbit key_s2 = P3^0;
sbit key_s3 = P3^1;

/*电机*/
#define left_motor_en		EN1 = 1				//左电机使能
#define right_motor_en		EN2 = 1				//右电机使能
#define left_motor_stop		IN1 = 0, IN2 = 0	//左电机停止
#define right_motor_stop	IN3 = 0, IN4 = 0	//右电机停止
#define left_motor_go		IN1 = 0, IN2 = 1	//左电机正传
#define left_motor_back		IN1 = 1, IN2 = 0	//左电机反转
#define right_motor_go		IN3 = 1, IN4 = 0	//右电机正传
#define right_motor_back	IN3 = 0, IN4 = 1	//右电机反转
/*超声波*/
#define LCD_data  P0    //液晶屏数据口
#define BUSY 0x80       //用于检测LCD的BF, BF=0,随时可接受外部指令和数据

#endif