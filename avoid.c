#include <reg52.h> 				//51头文件
#include <intrins.h>   			//包含nop等系统函数
#include "..\CONFIG\QXA51.h"	//QX-A51智能小车配置文件
#include "..\CONFIG\other.h"	//自己编写程序的文件

unsigned char pwm_left_val  = 120;  	//左电机占空比值取值范围0-170,0最快
unsigned char pwm_right_val = 110;      //右电机占空比值取值范围0-170,0最快
unsigned char pwm_t;					//周期
unsigned int  time = 0;					//传输时间
unsigned long S = 0;					//距离
bit flag = 0;							//超出测量范围标志位
unsigned char code range[]   = "DISTANCE";  			 //LCD1602显示格式
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]   = "range:000.0cm"; 	 
unsigned char code table1[]  = "Out of range!";   
unsigned char disbuff[4] = {0,0,0,0};   				  //显示距离缓存	  

/*定时器0中断*/
void timer0() interrupt 1		     //T0中断用来计数器溢出,超过测距范围
{
	flag=1;							 //中断溢出标志			 
}

/*定时器1中断输出PWM信号*/
void timer1() interrupt 3
{
	pwm_t++;					//计数器
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)	//左电机占空比	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)	//右电机占空比
		EN2 = 1;			 
}

void main()
{
	shut_nt();		//关数码管
	Init_LCD(); 	//LCD初始化
	delayms(5);		//延时
	DisplayListChar(0,0,range);		//1602第一行显示range数组内容
	DisplayListChar(0,1,table);		//1602第二行显示table数组内容
	keyscan();				//等待按下S2启动小车
	delayms(1000);			//延时1秒
	Init_interruptor();	 	//初始化定时器
	while(1)
	{
		 StartModule();			//启动模块测距

		 while(!RX);			//当RX（ECHO信号回响）为零时等待
		 TR0=1;			   		//开启计数
		 while(RX);				//当RX为1计数并等待
		 TR0=0;					//关闭计数

	     DisplayDistance();		//计算显示距离
		 Avoid();				//避障
		 delayms(65);			//测试周期不低于60MS		  
	}
}