#include<reg52.h>
#include "other.h"
#include "QXA51.h"
#include <intrins.h>

extern unsigned char pwm_left_val;  //左电机占空比值取值范围0-170,0最快
extern unsigned char pwm_right_val ;  //右电机占空比值取值范围0-170,0最快
extern unsigned char pwm_t;				//周期
extern unsigned int  time;				//传输时间
extern unsigned long S;				//距离
extern bit flag;			//超出测量范围标志位
extern unsigned char code range[];  		 //LCD1602显示格式
extern unsigned char code ASCII[13];
extern unsigned char code table[]; 	 
extern unsigned char code table1[];   
extern unsigned char disbuff[4];      //显示距离缓存	 		 

/*延时*/
/*毫秒级延时*/
void delayms(unsigned int z)		
{
	unsigned int x, y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

/*10us延时函数, 启动超声波模块时使用*/
void Delay10us(unsigned char i)    	
{ 
	unsigned char j; 
	for(; i > 0; i--)
		for(j = 10; j > 0; j--)
			_nop_();                //一个时钟周期1.085us
}


/*关闭共阴极数码管*/
void shut_nt()
{
    dula = 1;  	  
    P0 = 0X00;		//共阴极数码管阳极给低电平，全部熄灭
    dula = 0;
} 

/*判断S2是否被按下*/
void keyscan()
{
	for(;;)					//死循环
		if(key_s2 == 0)		// 实时检测S2按键是否被按下
		{
			delayms(5); 	//消抖
			if(key_s2 == 0)	//再次检测S2是否被按下
			{
				while(!key_s2);		//松手检测
				beep = 0;			//使能有源蜂鸣器
				delayms(100);		//100毫秒延时
				beep = 1;			//关闭有源蜂鸣器
				break;				//退出FOR死循环
			}
		}	
}

/*初始化中断*/
void Init_interruptor()
{
	TMOD |= 0x20;		//定时器1工作模式2,8位自动重装。T1用于产生PWM
	TMOD |= 0x01;		//定时器0工作模块1,16位定时模式。T0用测ECH0脉冲长度
	TH1 = 220; 
	TL1 = 220; 		//T1装初值
	TH0	= 0;
    TL0	= 0;		//T0装初值,16位定时计数用于记录ECHO高电平时间         
    ET1	= 1;		//允许T1中断
	ET0 = 1;		//允许T0中断
	TR1 = 1;		//启动T1
	EA  = 1;		//启动总中断
}

/*超声波模块函数*/
/*启动超声波模块*/
void  StartModule() 		         
{
	TX=1;			                 //启动一次Trig模块
	Delay10us(2);					 //高电平20us
	TX=0;
}

/*计算超声波所测距离并显示*/
void DisplayDistance(void)
{
	time = TH0*256 + TL0;			 //TH0左移8位,加上TL0
	TH0 = 0;						     
	TL0 = 0;
	S = (float)(time*1.085)*0.17;     //距离范围S单位为mm,且为浮点型
	if((S >= 7000) || flag == 1) 	 //超出测量范围
	{	 
		flag=0;
		DisplayListChar(0,1,table1);		//1602显示数组table1："Out of range!"
	}
	else
	{
		disbuff[0] = S/1000; 			//距离数值千位
		disbuff[1] = S%1000/100;		//距离数值百位
		disbuff[2] = S%100/10;			//距离数值十位
		disbuff[3] = S%10; 				//距离数值个位
	    DisplayListChar(0,1, table); 					//显示: range:000.0cm
	    DisplayOneChar( 6,1, ASCII[disbuff[0]]); 		//显示千位
	    DisplayOneChar( 7,1, ASCII[disbuff[1]]);		//显示百位
	    DisplayOneChar( 8,1, ASCII[disbuff[2]]);	    //显示十位
        DisplayOneChar( 9,1, ASCII[10]);				//显示小数点.	
	    DisplayOneChar(10,1, ASCII[disbuff[3]]);	    //显示个位        xxxxmm = xxx.xcm	 
	}
}


/*LCD1602液晶屏函数*/
/*读BUSY状态*/
unsigned char read_cmd(void)
{
	LCD_data = 0xFF; 			
	LCD_RS = 0;					 //命令端
	Delay10us(1); 
	LCD_RW = 1;				   	 //读
	Delay10us(1); 
	do{
	LCD_EN = 0;					
	Delay10us(1); 			     //若晶振速度太高可以在这后加小的延时
	LCD_EN = 0;
	Delay10us(1); 
	LCD_EN = 1;					 //使能
	Delay10us(1); 
	}while(LCD_data & BUSY);     //检测忙标志
	return LCD_data;
}

/*写数据*/
void write_data(unsigned char wdlcd) 
{
	read_cmd(); 		 		//检测忙
	LCD_data = wdlcd;
	LCD_RS = 1;	   				//数据端
	Delay10us(1); 
	LCD_RW = 0;			   		//写
	Delay10us(1); 
	LCD_EN = 0; 				//使能
	Delay10us(1); 				//若晶振速度太高可以在这后加小的延时
	LCD_EN = 0;				    
	Delay10us(1); 
	LCD_EN = 1;
	Delay10us(1); 
}

/*写指令*/
void write_cmd(unsigned char wclcd,unsigned char busyc) //busyC为0时忽略忙检测
{
	if(busyc)	read_cmd(); 	//根据需要检测忙
	LCD_data = wclcd;
	LCD_RS = 0;				   	 //命令端
	Delay10us(1); 
	LCD_RW = 0;				   	 //写
	Delay10us(1); 
	LCD_EN = 0;					 //使能
	Delay10us(1); 				 //若晶振速度太高可以在这后加小的延时
	LCD_EN = 0;
	Delay10us(1); 
	LCD_EN = 1;
	Delay10us(1); 
}

/*LCD初始化*/
void Init_LCD(void) 
{
	LCD_data = 0;
	write_cmd(0x38,0);   	//三次显示模式设置, 不检测忙信号
	delayms(5); 
	write_cmd(0x38,0);
	delayms(5); 
	write_cmd(0x38,0);
	delayms(5); 
	write_cmd(0x38,1); 		//显示模式设置,开始要求每次检测忙信号
	write_cmd(0x08,1); 		//关闭显示
	write_cmd(0x01,1); 		//显示清屏
	write_cmd(0x06,1); 		//显示光标移动设置
	write_cmd(0x0c,1); 		//显示开及光标设置
}

/*按指定位置显示一个字符*/
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x01;			//Y为行数
	X &= 0x0F; 			//一行只能显示0F个ASCII
	X |= 0x80;
	if(Y)	X |= 0x40; 	//Y=1,到第二行时地址
	write_cmd(X,1);		//发命令字
	write_data(DData);  //发数据
}

/*按指定位置显示一串字符*/
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
	unsigned char ListLength;
	ListLength = 0;
	Y &= 0x01;
	X &= 0x0F; 											//限制X不大于15，Y不大于1
	while(DData[ListLength] > 0x19) 					//若到达字串尾则退出
	{
		if (X <= 0x0F) 									//X坐标应小于0x0F
		{
			DisplayOneChar(X, Y, DData[ListLength]);	//显示单个字符
			ListLength++;
			X++;
		}
	}
}


/*电机模块函数*/
/*小车前进*/
void forward()
{
	left_motor_go; 		//左电机前进
	right_motor_go; 	//右电机前进
}
/*PWM控制使能小车后退*/
void backward()
{
	left_motor_back; 	//左电机后退
	right_motor_back; 	//右电机后退	
}
/*小车左转*/
void left_run()
{
	left_motor_stop; 	//左电机停止
	right_motor_go; 	//右电机前进	
}
/*小车停止*/
void stop()
{
	right_motor_stop;	//右电机停止
	left_motor_stop; 	//左电机停止	
}
/*PWM控制使能小车高速左转*/
void left_rapidly()
{
	left_motor_back;
	right_motor_go;	
}

 /*超声波避障*/
void Avoid()
{
	if(S < 1200)			//在1.2m处有障碍降速
	{
		unsigned char pwm_left_val  = 125;  	//左电机占空比值取值范围0-170,0最快
		unsigned char pwm_right_val = 135;      //右电机占空比值取值范围0-170,0最快
		if(S < 800)			//在0.8m处有障碍继续降速
		{
			pwm_left_val  = 135;	
			pwm_right_val = 145;	
			forward();				//前进
		}
		if(S < 600)		  			//在0.6m处有障碍左转
		{
			pwm_left_val  = 180;	
			pwm_right_val = 120;	
			left_run();				//左转
			delayms(100);			//实现左小弯转
		}
		if(S < 400 || left_led2 == 0 && right_led2 == 0) 		//小车理障碍物过近,后退掉头,左急转
		{
			beep = 0;				//使能蜂鸣器
			pwm_left_val  = 150;	
			pwm_right_val = 160;	
			stop();					//停车
			delayms(150);			//时间越长,停止时间越长	
			backward();				//后退
			delayms(100);			//后退的时间影响后退的距离。后退时间越长,后退距离越远
			beep = 1;				//关闭蜂鸣器
			do{
				pwm_left_val  = 135;	
				pwm_right_val = 145;	
				left_rapidly();		//高速左转
				delayms(100);		//时间越长,转向角度越大
				stop();				//停车
				delayms(150);		//时间越长,停止时间越长
				StartModule();		//启动模块测距,再次判断距离
				while(!RX);			//当RX(ECHO信号回响)为零时等待
				TR0=1;			    //开启计数
				while(RX);			//当RX为1计数并等待
				TR0=0;				//关闭计数
				DisplayDistance();	//计算显示距离
			}while(S < 280);		//判断前面障碍物距离		
		}
		if(left_led2 == 1 && right_led2 == 0)
		{
	 		stop();	
			delayms(150);
			backward();				//后退
			delayms(150);			//后退的时间影响后退的距离。后退时间越长,后退距离越远
			pwm_left_val  = 180;	
			pwm_right_val = 120;	
			left_run();				//左转
			delayms(100);			//实现左小弯转
		}
		if(left_led2 == 0 && right_led2 == 1)
		{
			stop();	
			delayms(150);
			backward();				//后退
			delayms(150);			//后退的时间影响后退的距离。后退时间越长,后退距离越远
			pwm_left_val  = 140;	
			pwm_right_val = 150;	
			left_rapidly();			//左转
			delayms(100);			//实现左急转	
		}
		if(left_led2 == 0 && right_led2 == 0)
		{
			do{
				pwm_left_val  = 135;	
				pwm_right_val = 145;	
				left_rapidly();		//高速左转
				delayms(100);		//时间越长,转向角度越大
				stop();				//停车
				delayms(150);		//时间越长,停止时间越长
				StartModule();		//启动模块测距,再次判断距离
				while(!RX);			//当RX(ECHO信号回响)为零时等待
				TR0=1;			    //开启计数
				while(RX);			//当RX为1计数并等待
				TR0=0;				//关闭计数
				DisplayDistance();	//计算显示距离
			}while(S < 280);		//判断前面障碍物距离
		}
		else	forward();				//前进	
	}
	if(left_led2 == 1 && right_led2 == 0)
	{
	 	stop();	
		delayms(150);
		backward();				//后退
		delayms(100);			//后退的时间影响后退的距离。后退时间越长,后退距离越远
		pwm_left_val  = 180;	
		pwm_right_val = 120;	
		left_run();				//左转
		delayms(100);			//实现左小弯转
	}
	if(left_led2 == 0 && right_led2 == 1)
	{
		stop();	
		delayms(150);
		backward();				//后退
		delayms(100);			//后退的时间影响后退的距离。后退时间越长,后退距离越远
		pwm_left_val  = 140;	
		pwm_right_val = 150;	
		left_rapidly();			//左转
		delayms(100);			//实现左急转
	}
	else
	{
		unsigned char pwm_left_val  = 120;  	
		unsigned char pwm_right_val = 110;    
		forward();				//前进
	}
}