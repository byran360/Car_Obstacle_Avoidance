#include<reg52.h>
#include "other.h"
#include "QXA51.h"
#include <intrins.h>

extern unsigned char pwm_left_val;  //����ռ�ձ�ֵȡֵ��Χ0-170,0���
extern unsigned char pwm_right_val ;  //�ҵ��ռ�ձ�ֵȡֵ��Χ0-170,0���
extern unsigned char pwm_t;				//����
extern unsigned int  time;				//����ʱ��
extern unsigned long S;				//����
extern bit flag;			//����������Χ��־λ
extern unsigned char code range[];  		 //LCD1602��ʾ��ʽ
extern unsigned char code ASCII[13];
extern unsigned char code table[]; 	 
extern unsigned char code table1[];   
extern unsigned char disbuff[4];      //��ʾ���뻺��	 		 

/*��ʱ*/
/*���뼶��ʱ*/
void delayms(unsigned int z)		
{
	unsigned int x, y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

/*10us��ʱ����, ����������ģ��ʱʹ��*/
void Delay10us(unsigned char i)    	
{ 
	unsigned char j; 
	for(; i > 0; i--)
		for(j = 10; j > 0; j--)
			_nop_();                //һ��ʱ������1.085us
}


/*�رչ����������*/
void shut_nt()
{
    dula = 1;  	  
    P0 = 0X00;		//������������������͵�ƽ��ȫ��Ϩ��
    dula = 0;
} 

/*�ж�S2�Ƿ񱻰���*/
void keyscan()
{
	for(;;)					//��ѭ��
		if(key_s2 == 0)		// ʵʱ���S2�����Ƿ񱻰���
		{
			delayms(5); 	//����
			if(key_s2 == 0)	//�ٴμ��S2�Ƿ񱻰���
			{
				while(!key_s2);		//���ּ��
				beep = 0;			//ʹ����Դ������
				delayms(100);		//100������ʱ
				beep = 1;			//�ر���Դ������
				break;				//�˳�FOR��ѭ��
			}
		}	
}

/*��ʼ���ж�*/
void Init_interruptor()
{
	TMOD |= 0x20;		//��ʱ��1����ģʽ2,8λ�Զ���װ��T1���ڲ���PWM
	TMOD |= 0x01;		//��ʱ��0����ģ��1,16λ��ʱģʽ��T0�ò�ECH0���峤��
	TH1 = 220; 
	TL1 = 220; 		//T1װ��ֵ
	TH0	= 0;
    TL0	= 0;		//T0װ��ֵ,16λ��ʱ�������ڼ�¼ECHO�ߵ�ƽʱ��         
    ET1	= 1;		//����T1�ж�
	ET0 = 1;		//����T0�ж�
	TR1 = 1;		//����T1
	EA  = 1;		//�������ж�
}

/*������ģ�麯��*/
/*����������ģ��*/
void  StartModule() 		         
{
	TX=1;			                 //����һ��Trigģ��
	Delay10us(2);					 //�ߵ�ƽ20us
	TX=0;
}

/*���㳬����������벢��ʾ*/
void DisplayDistance(void)
{
	time = TH0*256 + TL0;			 //TH0����8λ,����TL0
	TH0 = 0;						     
	TL0 = 0;
	S = (float)(time*1.085)*0.17;     //���뷶ΧS��λΪmm,��Ϊ������
	if((S >= 7000) || flag == 1) 	 //����������Χ
	{	 
		flag=0;
		DisplayListChar(0,1,table1);		//1602��ʾ����table1��"Out of range!"
	}
	else
	{
		disbuff[0] = S/1000; 			//������ֵǧλ
		disbuff[1] = S%1000/100;		//������ֵ��λ
		disbuff[2] = S%100/10;			//������ֵʮλ
		disbuff[3] = S%10; 				//������ֵ��λ
	    DisplayListChar(0,1, table); 					//��ʾ: range:000.0cm
	    DisplayOneChar( 6,1, ASCII[disbuff[0]]); 		//��ʾǧλ
	    DisplayOneChar( 7,1, ASCII[disbuff[1]]);		//��ʾ��λ
	    DisplayOneChar( 8,1, ASCII[disbuff[2]]);	    //��ʾʮλ
        DisplayOneChar( 9,1, ASCII[10]);				//��ʾС����.	
	    DisplayOneChar(10,1, ASCII[disbuff[3]]);	    //��ʾ��λ        xxxxmm = xxx.xcm	 
	}
}


/*LCD1602Һ��������*/
/*��BUSY״̬*/
unsigned char read_cmd(void)
{
	LCD_data = 0xFF; 			
	LCD_RS = 0;					 //�����
	Delay10us(1); 
	LCD_RW = 1;				   	 //��
	Delay10us(1); 
	do{
	LCD_EN = 0;					
	Delay10us(1); 			     //�������ٶ�̫�߿���������С����ʱ
	LCD_EN = 0;
	Delay10us(1); 
	LCD_EN = 1;					 //ʹ��
	Delay10us(1); 
	}while(LCD_data & BUSY);     //���æ��־
	return LCD_data;
}

/*д����*/
void write_data(unsigned char wdlcd) 
{
	read_cmd(); 		 		//���æ
	LCD_data = wdlcd;
	LCD_RS = 1;	   				//���ݶ�
	Delay10us(1); 
	LCD_RW = 0;			   		//д
	Delay10us(1); 
	LCD_EN = 0; 				//ʹ��
	Delay10us(1); 				//�������ٶ�̫�߿���������С����ʱ
	LCD_EN = 0;				    
	Delay10us(1); 
	LCD_EN = 1;
	Delay10us(1); 
}

/*дָ��*/
void write_cmd(unsigned char wclcd,unsigned char busyc) //busyCΪ0ʱ����æ���
{
	if(busyc)	read_cmd(); 	//������Ҫ���æ
	LCD_data = wclcd;
	LCD_RS = 0;				   	 //�����
	Delay10us(1); 
	LCD_RW = 0;				   	 //д
	Delay10us(1); 
	LCD_EN = 0;					 //ʹ��
	Delay10us(1); 				 //�������ٶ�̫�߿���������С����ʱ
	LCD_EN = 0;
	Delay10us(1); 
	LCD_EN = 1;
	Delay10us(1); 
}

/*LCD��ʼ��*/
void Init_LCD(void) 
{
	LCD_data = 0;
	write_cmd(0x38,0);   	//������ʾģʽ����, �����æ�ź�
	delayms(5); 
	write_cmd(0x38,0);
	delayms(5); 
	write_cmd(0x38,0);
	delayms(5); 
	write_cmd(0x38,1); 		//��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	write_cmd(0x08,1); 		//�ر���ʾ
	write_cmd(0x01,1); 		//��ʾ����
	write_cmd(0x06,1); 		//��ʾ����ƶ�����
	write_cmd(0x0c,1); 		//��ʾ�����������
}

/*��ָ��λ����ʾһ���ַ�*/
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x01;			//YΪ����
	X &= 0x0F; 			//һ��ֻ����ʾ0F��ASCII
	X |= 0x80;
	if(Y)	X |= 0x40; 	//Y=1,���ڶ���ʱ��ַ
	write_cmd(X,1);		//��������
	write_data(DData);  //������
}

/*��ָ��λ����ʾһ���ַ�*/
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
	unsigned char ListLength;
	ListLength = 0;
	Y &= 0x01;
	X &= 0x0F; 											//����X������15��Y������1
	while(DData[ListLength] > 0x19) 					//�������ִ�β���˳�
	{
		if (X <= 0x0F) 									//X����ӦС��0x0F
		{
			DisplayOneChar(X, Y, DData[ListLength]);	//��ʾ�����ַ�
			ListLength++;
			X++;
		}
	}
}


/*���ģ�麯��*/
/*С��ǰ��*/
void forward()
{
	left_motor_go; 		//����ǰ��
	right_motor_go; 	//�ҵ��ǰ��
}
/*PWM����ʹ��С������*/
void backward()
{
	left_motor_back; 	//��������
	right_motor_back; 	//�ҵ������	
}
/*С����ת*/
void left_run()
{
	left_motor_stop; 	//����ֹͣ
	right_motor_go; 	//�ҵ��ǰ��	
}
/*С��ֹͣ*/
void stop()
{
	right_motor_stop;	//�ҵ��ֹͣ
	left_motor_stop; 	//����ֹͣ	
}
/*PWM����ʹ��С��������ת*/
void left_rapidly()
{
	left_motor_back;
	right_motor_go;	
}

 /*����������*/
void Avoid()
{
	if(S < 1200)			//��1.2m�����ϰ�����
	{
		unsigned char pwm_left_val  = 125;  	//����ռ�ձ�ֵȡֵ��Χ0-170,0���
		unsigned char pwm_right_val = 135;      //�ҵ��ռ�ձ�ֵȡֵ��Χ0-170,0���
		if(S < 800)			//��0.8m�����ϰ���������
		{
			pwm_left_val  = 135;	
			pwm_right_val = 145;	
			forward();				//ǰ��
		}
		if(S < 600)		  			//��0.6m�����ϰ���ת
		{
			pwm_left_val  = 180;	
			pwm_right_val = 120;	
			left_run();				//��ת
			delayms(100);			//ʵ����С��ת
		}
		if(S < 400 || left_led2 == 0 && right_led2 == 0) 		//С�����ϰ������,���˵�ͷ,��ת
		{
			beep = 0;				//ʹ�ܷ�����
			pwm_left_val  = 150;	
			pwm_right_val = 160;	
			stop();					//ͣ��
			delayms(150);			//ʱ��Խ��,ֹͣʱ��Խ��	
			backward();				//����
			delayms(100);			//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ��,���˾���ԽԶ
			beep = 1;				//�رշ�����
			do{
				pwm_left_val  = 135;	
				pwm_right_val = 145;	
				left_rapidly();		//������ת
				delayms(100);		//ʱ��Խ��,ת��Ƕ�Խ��
				stop();				//ͣ��
				delayms(150);		//ʱ��Խ��,ֹͣʱ��Խ��
				StartModule();		//����ģ����,�ٴ��жϾ���
				while(!RX);			//��RX(ECHO�źŻ���)Ϊ��ʱ�ȴ�
				TR0=1;			    //��������
				while(RX);			//��RXΪ1�������ȴ�
				TR0=0;				//�رռ���
				DisplayDistance();	//������ʾ����
			}while(S < 280);		//�ж�ǰ���ϰ������		
		}
		if(left_led2 == 1 && right_led2 == 0)
		{
	 		stop();	
			delayms(150);
			backward();				//����
			delayms(150);			//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ��,���˾���ԽԶ
			pwm_left_val  = 180;	
			pwm_right_val = 120;	
			left_run();				//��ת
			delayms(100);			//ʵ����С��ת
		}
		if(left_led2 == 0 && right_led2 == 1)
		{
			stop();	
			delayms(150);
			backward();				//����
			delayms(150);			//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ��,���˾���ԽԶ
			pwm_left_val  = 140;	
			pwm_right_val = 150;	
			left_rapidly();			//��ת
			delayms(100);			//ʵ����ת	
		}
		if(left_led2 == 0 && right_led2 == 0)
		{
			do{
				pwm_left_val  = 135;	
				pwm_right_val = 145;	
				left_rapidly();		//������ת
				delayms(100);		//ʱ��Խ��,ת��Ƕ�Խ��
				stop();				//ͣ��
				delayms(150);		//ʱ��Խ��,ֹͣʱ��Խ��
				StartModule();		//����ģ����,�ٴ��жϾ���
				while(!RX);			//��RX(ECHO�źŻ���)Ϊ��ʱ�ȴ�
				TR0=1;			    //��������
				while(RX);			//��RXΪ1�������ȴ�
				TR0=0;				//�رռ���
				DisplayDistance();	//������ʾ����
			}while(S < 280);		//�ж�ǰ���ϰ������
		}
		else	forward();				//ǰ��	
	}
	if(left_led2 == 1 && right_led2 == 0)
	{
	 	stop();	
		delayms(150);
		backward();				//����
		delayms(100);			//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ��,���˾���ԽԶ
		pwm_left_val  = 180;	
		pwm_right_val = 120;	
		left_run();				//��ת
		delayms(100);			//ʵ����С��ת
	}
	if(left_led2 == 0 && right_led2 == 1)
	{
		stop();	
		delayms(150);
		backward();				//����
		delayms(100);			//���˵�ʱ��Ӱ����˵ľ��롣����ʱ��Խ��,���˾���ԽԶ
		pwm_left_val  = 140;	
		pwm_right_val = 150;	
		left_rapidly();			//��ת
		delayms(100);			//ʵ����ת
	}
	else
	{
		unsigned char pwm_left_val  = 120;  	
		unsigned char pwm_right_val = 110;    
		forward();				//ǰ��
	}
}