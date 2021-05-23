#ifndef __OTHER_H__
#define __OTHER_H__

/*团队定义函数*/
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

/*变量申明*/
extern unsigned char pwm_left_val;  	//左电机占空比值取值范围0-170,0最快
extern unsigned char pwm_right_val;  	//右电机占空比值取值范围0-170,0最快
extern unsigned char pwm_t;				//周期
extern unsigned int  time;				//传输时间
extern unsigned long S;					//距离
extern bit flag;						//超出测量范围标志位
extern unsigned char code range[];  	//LCD1602显示格式
extern unsigned char code ASCII[13];
extern unsigned char code table[]; 	 
extern unsigned char code table1[];   
extern unsigned char disbuff[4];      	//显示距离缓存	

#endif
