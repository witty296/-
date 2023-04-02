
#include <reg52.h>//库函数
#include <dht11.h> //库函数//如果 实物 打开
#include <adc0832.h> //AD转换库函数
#include "eeprom52.h"

#define uchar unsigned char//宏定意
#define uint  unsigned int//宏定意
#define ulong unsigned long	//宏定意
bit s=1;
#define NULL 0
#define RsBuf_N			150

unsigned char xdata RsBuf[RsBuf_N];	//定义串口接收数据缓冲区
uchar RsPoint;

sbit rs=P2^5;//LCD 1602 引脚定意		
sbit rd=P2^6;//LCD 1602 引脚定意		
sbit lcden=P2^7;//LCD 1602 引脚定意
sbit menu=P2^0;	//主功能键	
sbit add=P2^1;//加键		
sbit dec=P2^2;// 减键
sbit open=P2^3;// 手动 开
sbit close=P2^4;// 手动 关	 

sbit IR=P3^6; //红外感 应
sbit BEEP=P3^4;// 蜂鸣器报警
#define MOTORSTEP P1 			//宏定义，定义P1口为步进电机驱动端口

uchar kval,con;// 各种变量标记
uchar second;//时钟变量
bit dsflj,opensw,closesw,befl,menusw,lofl,zhenzhuan,fanzhuan,keysw;	
uint adc0,adc1;
uchar dwbrig,kwbrig,uphum,uptemp,upsmo; // 下限 亮度 上限湿度// 上限温度

uchar   U8T_data_H,U8RH_data_H;

char dianhuahaoma[12]; //接收号码（只可以用移动或者联通的号码，插在模块上的卡也必须是移动或者联通，就是不能用电信）

uchar code somebody[]		="8BF76CE8610FFF0C67094EBA8FDB5165FF01";	//请注意！有人进入！（发送短信内容的编码）

bit jishi=1;
bit ZD_SD_MODE=0;
bit flag_celiang=0;

#define Imax 14000    //此处为晶振为11.0592时的取值, 
#define Imin 8000    //如用其它频率的晶振时,
#define Inum1 1450    //要改变相应的取值。
#define Inum2 700 
#define Inum3 3000 

uchar f=0;
uchar Im[4]={0x00,0x00,0x00,0x00}; // 遥控码存储空间
uchar show[2]={0,0};  // 变量申明
ulong m,Tc;	  // 变量申明
uchar IrOK;		// 变量申明

void delay(uint z);// 开窗时间变量

void write_rvalue(); // 显示函数
//**************以下是各种字幕提示语数组************************
uchar code logo1[]= "Welcome to use  ";
uchar code logo2[]= "Window   system ";
uchar code logo3[]= "S:    H:  %     ";
uchar code logo4[]= "L:    T:  C     ";
uchar code logo5[]= "Window Opening  ";
uchar code logo6[]= "Window Closing  ";
uchar code logo7[]= "L-CLOSE:   LUX  ";
uchar code logo8[]= "H-Hum :    %    ";
uchar code logo9[]= "Please press +/-";
uchar code logo10[]="Please wait...  ";
uchar code logo11[]="H-Tem :    C    ";
uchar code logo15[]="H-Smoke:        ";
uchar code logo16[]="L-OPEN :   LUX  ";

uchar code logo17[]=" Sending SMS    ";

void delaymoto() 					//步进电机每一步间延迟函数
{  	
	uint y=200;
	while(y--);
}

uchar code FFW[]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09}; //步进电机驱动值数组
uchar code REV[]={0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01}; //步进电机驱动值数组

//uchar num1 [4] =    {0x00,0x00,0x00,0x00,};				
//uchar num2 [4] =    {0x00,0x00,0x00,0x00,};			 
//uchar num3 [4] =    {0x00,0x00,0x00,0x00,};		   
uchar num4 [4] =    {0x00,0x00,0x00,0x00,};	// AD转换变量组组
//uchar num5 [4] =    {0x00,0x00,0x00,0x00,};	// AD转换变量组组

/******************把数据保存到单片机内部eeprom中******************/
void write_eeprom()
{
	SectorErase(0x2000);
	byte_write(0x2000, uptemp);
	byte_write(0x2001, uphum);
	byte_write(0x2002, dwbrig);
	byte_write(0x2003, kwbrig);
	byte_write(0x2004, upsmo);

	byte_write(0x2005, dianhuahaoma[0]);
	byte_write(0x2006, dianhuahaoma[1]);
	byte_write(0x2007, dianhuahaoma[2]);
	byte_write(0x2008, dianhuahaoma[3]);
	byte_write(0x2009, dianhuahaoma[4]);
	byte_write(0x200a, dianhuahaoma[5]);
	byte_write(0x200b, dianhuahaoma[6]);
	byte_write(0x200c, dianhuahaoma[7]);
	byte_write(0x200d, dianhuahaoma[8]);
	byte_write(0x200e, dianhuahaoma[9]);
	byte_write(0x200f, dianhuahaoma[10]);

	byte_write(0x2060, a_a);	
}

/******************把数据从单片机内部eeprom中读出来*****************/
void read_eeprom()
{
	uptemp= byte_read(0x2000);
	uphum= byte_read(0x2001);
	dwbrig= byte_read(0x2002);
	kwbrig= byte_read(0x2003);
	upsmo= byte_read(0x2004);

	dianhuahaoma[0]=byte_read(0x2005);
	dianhuahaoma[1]=byte_read(0x2006);
	dianhuahaoma[2]=byte_read(0x2007);
	dianhuahaoma[3]=byte_read(0x2008);
	dianhuahaoma[4]=byte_read(0x2009);
	dianhuahaoma[5]=byte_read(0x200a);
	dianhuahaoma[6]=byte_read(0x200b);
	dianhuahaoma[7]=byte_read(0x200c);
	dianhuahaoma[8]=byte_read(0x200d);
	dianhuahaoma[9]=byte_read(0x200e);
	dianhuahaoma[10]=byte_read(0x200f);

	a_a      = byte_read(0x2060);
}

/**************开机自检eeprom初始化*****************/
void init_eeprom() 
{
	read_eeprom();		//先读
	if(a_a != 1)		//新的单片机初始单片机内问eeprom
	{
		a_a = 1;
  		uptemp=33; //温度上限
	  	uphum=60;	 //湿度上限
	  	dwbrig=30;//光控下限
	  	kwbrig=180;	//开窗光线
	  	upsmo=85;	//烟雾上限值

		dianhuahaoma[0]=0;
		dianhuahaoma[1]=0;
		dianhuahaoma[2]=0;
		dianhuahaoma[3]=0;
		dianhuahaoma[4]=0;
		dianhuahaoma[5]=0;
		dianhuahaoma[6]=0;
		dianhuahaoma[7]=0;
		dianhuahaoma[8]=0;
		dianhuahaoma[9]=0;
		dianhuahaoma[10]=0;

		write_eeprom();	   //保存数据
	}	
}

void delay(uint z)//延时函数
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void delay_1602(uint z)//延时函数
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=10;y>0;y--);
}

void SETP_MOTOR_FFW()	 //反转
{
 	uint i=0;
	for(i=0;i<8;i++) //8步一个循环
	{
		MOTORSTEP=FFW[i]; //取值赋给P1驱动口
		delaymoto(); 
	}
 }

void SETP_MOTOR_REV()  //正转
{
    uint i=0;
	for(i=0;i<8;i++) //8步一个循环
	{
		MOTORSTEP=REV[i]; //取值赋给P1驱动口
		delaymoto(); 
	}
}

void write_com(uchar com) //LCD 1602 写指令
{
	rs=0;
	lcden=0;
	P0=com;
	delay_1602(5);
	lcden=1;
	delay_1602(5);
	lcden=0;	
}

void write_date(uchar date)//LCD 1602 写数据
{
	rs=1;
	lcden=0;
	P0=date;
	delay_1602(5);
	lcden=1;
	delay_1602(5);
	lcden=0;	
}

void write_string(uchar a_b,uchar *p)
{ 
	if(a_b!=0)
	write_com(a_b);
	while(*p!='\0')
	{
		write_date(*p);
		p++;
	}
}

void init1602()//LCD 1602 初始化
{
	uchar num;
	rd=0;
	lcden=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
	for(num=0;num<16;num++)
	{
		write_date(logo1[num]);//开机字幕
	}
	write_com(0x80+0x40);
	for(num=0;num<16;num++)
	{
		write_date(logo2[num]);	//开机字幕
	}
}

void keyscn()//按键扫描函数
{
	uchar num; //局部变量
	if(kval==0)	  //没有进入设置状态时
	{
		if(add==0||(IrOK==1&&Im[2]==0x15))		//加按键是手动自动切换按键
		{
			delay(20);
			if(add==0||(IrOK==1&&Im[2]==0x15))
			{
				BEEP=0;
				delay(50);
				BEEP=1;
				Im[2]=0x00;
				ZD_SD_MODE=!ZD_SD_MODE;
				write_com(0x80+0x40+13);
				if(ZD_SD_MODE==0)		 //等于0是自动
				{
					write_date('Z');
					write_date('D');
					write_date(' ');	
				}				
				else
				{
					write_date('S');
					write_date('D');
					write_date(' ');	
				}
				while(add==0);
			}
		}
		if(dec==0||(IrOK==1&&Im[2]==0x0d))
		{
			delay(20);
			if(dec==0||(IrOK==1&&Im[2]==0x0d))
			{
				BEEP=0;
				delay(50);
				BEEP=1;
				Im[2]=0x00;
				befl=0;			 //关闭报警
				BEEP=1;	
				while(dec==0);
			}
		}
	}
	if(ZD_SD_MODE==1)	//手动模式
	{
		if(keysw==1)	   //允许手动开关窗
		{
			if(closesw==1)	   //关闭状态时
			{
				if(open==0||(IrOK==1&&Im[2]==0x09))	 // 手动开窗按键按下
				{				    
				    delay(20);
					if(open==0||(IrOK==1&&Im[2]==0x09))
					{
						BEEP=0;
						delay(50);
						BEEP=1;
						write_com(0x80);
						for(num=0;num<16;num++)//显示开窗提示语
						{
							write_date(logo5[num]);
						}
						write_com(0x80+0x40);
						for(num=0;num<16;num++)//显示提示语
						{
							write_date(logo10[num]);
						}
						Im[2]=0x00;
						
						zhenzhuan=0;// 正转 开启
						fanzhuan=1;	 // 反转关闭
						second=0;// 秒清零 步进电机 会停
						jishi=1;
						keysw=0;  //禁止手动开关窗
						menusw=0; //手动开始运行，清零
						while(!open);
					} 	 
				}
				if(adc0>upsmo)
				{
					befl=1;  
					BEEP=0;
					delay(50);
					BEEP=1;
					write_com(0x80);
					for(num=0;num<16;num++)//显示开窗提示语
					{
						write_date(logo5[num]);
					}
					write_com(0x80+0x40);
					for(num=0;num<16;num++)//显示提示语
					{
						write_date(logo10[num]);
					}
					Im[2]=0x00;
					
					zhenzhuan=0;// 正转 开启
					fanzhuan=1;	 // 反转关闭
					second=0;// 秒清零 步进电机 会停
					jishi=1;
					keysw=0;  //禁止手动开关窗
					menusw=0; //手动开始运行，清零
				}
			}
			if(opensw==1)//如果窗户是打开的
			{
			 	if(close==0||(IrOK==1&&Im[2]==0x07))//按下手动关
				{
				    delay(20);
					if(close==0||(IrOK==1&&Im[2]==0x07))
					{
						BEEP=0;
						delay(50);
						BEEP=1;
						write_com(0x80);
						for(num=0;num<16;num++)//显示提示语
						{
							write_date(logo6[num]);
						}
					 	write_com(0x80+0x40);
						for(num=0;num<16;num++)
						{
							write_date(logo10[num]);//显示提示语
						}
						Im[2]=0x00;
						zhenzhuan=1;  //正转关
						fanzhuan=0;	// 反转 开启
						second=0;  	// 定时清零
						jishi=1;
						keysw=0; // 禁止手动开关窗
						menusw=0;	//手动开始运行，清零
						while(!close);
					}	 
				} 
				if(adc0>upsmo)
				befl=1; 	
			}
		}
	}

	if(menusw==1)//如果窗户运行完毕
	{
 	 	if(menu==0||(IrOK==1&&Im[2]==0x47))// 如果主功能键按下
		{
		    delay(20);	//延时消抖动
			if(menu==0||(IrOK==1&&Im[2]==0x47))
			{
				BEEP=0;
				delay(50);
				BEEP=1;
				Im[2]=0x00;
				dsflj=0;// 关主页显示
				kval++; // 按键变量++  设置不同的值
				keysw=0;//不允许手动 开窗  和关窗
				befl=0;
				lofl=1;	//字幕只需要载入一次
				write_eeprom();			   //保存数据
				while(!menu);//松手检测
			}
		} 

//*************************************************************************8
	
		
	 //**************************以下 亮度会上设定  和上面小时一样
	 //只是就量不一样 不作注释*******************************************
	 //**************
	
	
	  //********************************设定环境参数************************************
	  	if(kval==1)
	 	{	
//		   	write_com(0x0d);
		   	num4[4]=dwbrig;
		   	write_rvalue();
		  	if(lofl==1)
		  	{
			 	write_com(0x80);
				for(num=0;num<16;num++)
				{
					write_date(logo7[num]);
				}
			
				write_com(0x80+0x40);
				for(num=0;num<16;num++)
				{
					write_date(logo9[num]);
					lofl=0;
				}
			}
		    if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
			   	delay(20);	    
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
					 dwbrig++;
					 num4[4]=dwbrig;
					 write_rvalue();
			         if(dwbrig>=kwbrig)
			         {dwbrig=kwbrig-1;}
				 }
			} 
		
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20); 
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
					dwbrig--;
					num4[4]=dwbrig;
					write_rvalue();	
			        if(dwbrig<1)
			        {dwbrig=0;}
				}
			} 
		}
	//*********************************************************************
	
	
	   //********************************设定环境参数************************************
	 
	  	else if(kval==2)
	 	{
			
//			write_com(0x0d);
			num4[4]=kwbrig;
			write_rvalue();
			if(lofl==1)
			{
			 	write_com(0x80);
				for(num=0;num<16;num++)
				{
					write_date(logo16[num]);
				}
				write_com(0x80+0x40);
				for(num=0;num<16;num++)
				{
					write_date(logo9[num]);
					lofl=0;
				}
			}
		    if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
					kwbrig++;
					num4[4]=kwbrig;
					write_rvalue();
	
			        if(kwbrig>=255)
			        {kwbrig=255;}
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
				delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
					kwbrig--;
				 	num4[4]=kwbrig;
					write_rvalue();
			        if(kwbrig<=dwbrig)
			        {kwbrig=dwbrig+1;}
				}
			} 
		}
	
	 //**************************以下 湿度会上设定  和上面小时一样
	 //只是就量不一样 不作注释*******************************************
	 //**************		
	 	else if(kval==3)
	 	{
			num4[4]=uphum;
			write_rvalue();
			if(lofl==1)
		  	{
				write_com(0x80);
				for(num=0;num<16;num++)
				{
					write_date(logo8[num]);
				}
				write_com(0x80+0x40);
				for(num=0;num<16;num++)
				{
					write_date(logo9[num]);
					lofl=0;
				}
			}
		    if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
			  	delay(20); 
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
					uphum++;
					num4[4]=uphum;
					write_rvalue();
		         	if(uphum>=95)
		          	{uphum=95;}
				 }
		 	}
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
					uphum--;
					num4[4]=uphum;
					write_rvalue();
			        if(uphum<=5)
			        {uphum=5;}
				 }
			} 
	 	}
	 //**************************以下 温度度会上设定  和上面小时一样
	 //只是就量不一样 不作注释*******************************************
	 //**************				
	 	else if(kval==4)
	 	{
			num4[4]=uptemp;
		 	write_rvalue();
			if(lofl==1)
			{
			 	write_com(0x80);
			  	for(num=0;num<16;num++)
				{
					write_date(logo11[num]);
				}
				write_com(0x80+0x40);
				for(num=0;num<16;num++)
				{
					write_date(logo9[num]);
					lofl=0;
				}
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);			    
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
					uptemp++;
					num4[4]=uptemp;
					write_rvalue();
			        if(uptemp>=60)
			        {uptemp=0;}
				 }
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
				delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
					uptemp--;
					num4[4]=uptemp;
					write_rvalue();
			        if(uptemp<=0)
			        {uptemp=0;}
				}
			} 
		}
	
	 //**************************以下烟雾度会上设定  和上面小时一样
	 //只是就量不一样 不作注释*******************************************
	 //**************				
	 	else if(kval==5)
		{
		   	num4[4]=upsmo;
		   	write_rvalue();
		  	if(lofl==1)
		  	{
		 		write_com(0x80);
				for(num=0;num<16;num++)
				{
					write_date(logo15[num]);
				}
			   	write_com(0x80+0x40);
				for(num=0;num<16;num++)
				{
					write_date(logo9[num]);
					lofl=0;
				}
			}
		    if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	upsmo++;
				 	num4[4]=upsmo;
				 	write_rvalue();
		         	if(upsmo>=255)
		          	{upsmo=0;}
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	upsmo--;
				 	num4[4]=upsmo;
				 	write_rvalue();
		         	if(uptemp<=0)
		          	{upsmo=0;}
				}
			} 
	 	}
		else if(kval==6)
		{
			if(lofl==1)
			{
				write_string(0x80,"Phone number:   ");
				write_string(0xc0,"                ");
				write_com(0x80+0x40+5);	   //选中液晶屏上的第二行第十一列
				write_date('0'+dianhuahaoma[0]);
				write_date('0'+dianhuahaoma[1]);
				write_date('0'+dianhuahaoma[2]);
				write_date('0'+dianhuahaoma[3]);
				write_date('0'+dianhuahaoma[4]);
				write_date('0'+dianhuahaoma[5]);
				write_date('0'+dianhuahaoma[6]);
				write_date('0'+dianhuahaoma[7]);
				write_date('0'+dianhuahaoma[8]);
				write_date('0'+dianhuahaoma[9]);
				write_date('0'+dianhuahaoma[10]);
				write_com(0x0f);//闪烁
				write_com(0x80+0x40+5);//闪烁位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[0]++;
					if(dianhuahaoma[0]>9) 
					dianhuahaoma[0]=0;
					write_com(0x80+0x40+5);
					write_date('0'+dianhuahaoma[0]);
					write_com(0x80+0x40+5);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[0]--;
					if(dianhuahaoma[0]<0) 
					dianhuahaoma[0]=9;
					write_com(0x80+0x40+5);
					write_date('0'+dianhuahaoma[0]);
					write_com(0x80+0x40+5);
				}
			}
		}
		else if(kval==7)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+6);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[1]++;
					if(dianhuahaoma[1]>9) 
					dianhuahaoma[1]=0;
					write_com(0x80+0x40+6);
					write_date('0'+dianhuahaoma[1]);
					write_com(0x80+0x40+6);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[1]--;
					if(dianhuahaoma[1]<0) 
					dianhuahaoma[1]=9;
					write_com(0x80+0x40+6);
					write_date('0'+dianhuahaoma[1]);
					write_com(0x80+0x40+6);
				}
			}
		}
			
		else if(kval==8)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+7);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[2]++;
					if(dianhuahaoma[2]>9) 
					dianhuahaoma[2]=0;
					write_com(0x80+0x40+7);
					write_date('0'+dianhuahaoma[2]);
					write_com(0x80+0x40+7);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[2]--;
					if(dianhuahaoma[2]<0) 
					dianhuahaoma[2]=9;
					write_com(0x80+0x40+7);
					write_date('0'+dianhuahaoma[2]);
					write_com(0x80+0x40+7);
				}
			}
		}
		else if(kval==9)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+8);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[3]++;
					if(dianhuahaoma[3]>9) 
					dianhuahaoma[3]=0;
					write_com(0x80+0x40+8);
					write_date('0'+dianhuahaoma[3]);
					write_com(0x80+0x40+8);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[3]--;
					if(dianhuahaoma[3]<0) 
					dianhuahaoma[3]=9;
					write_com(0x80+0x40+8);
					write_date('0'+dianhuahaoma[3]);
					write_com(0x80+0x40+8);
				}
			}
		}
		else if(kval==10)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+9);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[4]++;
					if(dianhuahaoma[4]>9) 
					dianhuahaoma[4]=0;
					write_com(0x80+0x40+9);
					write_date('0'+dianhuahaoma[4]);
					write_com(0x80+0x40+9);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[4]--;
					if(dianhuahaoma[4]<0) 
					dianhuahaoma[4]=9;
					write_com(0x80+0x40+9);
					write_date('0'+dianhuahaoma[4]);
					write_com(0x80+0x40+9);
				}
			}
		}
		else if(kval==11)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+10);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[5]++;
					if(dianhuahaoma[5]>9) 
					dianhuahaoma[5]=0;
					write_com(0x80+0x40+10);
					write_date('0'+dianhuahaoma[5]);
					write_com(0x80+0x40+10);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[5]--;
					if(dianhuahaoma[5]<0) 
					dianhuahaoma[5]=9;
					write_com(0x80+0x40+10);
					write_date('0'+dianhuahaoma[5]);
					write_com(0x80+0x40+10);
				}
			}
		}
		else if(kval==12)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+11);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[6]++;
					if(dianhuahaoma[6]>9) 
					dianhuahaoma[6]=0;
					write_com(0x80+0x40+11);
					write_date('0'+dianhuahaoma[6]);
					write_com(0x80+0x40+11);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[6]--;
					if(dianhuahaoma[6]<0) 
					dianhuahaoma[6]=9;
					write_com(0x80+0x40+11);
					write_date('0'+dianhuahaoma[6]);
					write_com(0x80+0x40+11);
				}
			}
		}
		else if(kval==13)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+12);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[7]++;
					if(dianhuahaoma[7]>9) 
					dianhuahaoma[7]=0;
					write_com(0x80+0x40+12);
					write_date('0'+dianhuahaoma[7]);
					write_com(0x80+0x40+12);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[7]--;
					if(dianhuahaoma[7]<0) 
					dianhuahaoma[7]=9;
					write_com(0x80+0x40+12);
					write_date('0'+dianhuahaoma[7]);
					write_com(0x80+0x40+12);
				}
			}
		}
		else if(kval==14)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+13);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[8]++;
					if(dianhuahaoma[8]>9) 
					dianhuahaoma[8]=0;
					write_com(0x80+0x40+13);
					write_date('0'+dianhuahaoma[8]);
					write_com(0x80+0x40+13);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[8]--;
					if(dianhuahaoma[8]<0) 
					dianhuahaoma[8]=9;
					write_com(0x80+0x40+13);
					write_date('0'+dianhuahaoma[8]);
					write_com(0x80+0x40+13);
				}
			}
		}
		else if(kval==15)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+14);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[9]++;
					if(dianhuahaoma[9]>9) 
					dianhuahaoma[9]=0;
					write_com(0x80+0x40+14);
					write_date('0'+dianhuahaoma[9]);
					write_com(0x80+0x40+14);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[9]--;
					if(dianhuahaoma[9]<0) 
					dianhuahaoma[9]=9;
					write_com(0x80+0x40+14);
					write_date('0'+dianhuahaoma[9]);
					write_com(0x80+0x40+14);
				}
			}
		}
		else if(kval==16)
		{
			if(lofl==1)
			{
				write_com(0x80+0x40+15);//位置
				lofl=0;
			}
			if(add==0||(IrOK==1&&Im[2]==0x40))	
			{
				delay(20);
				if(add==0||(IrOK==1&&Im[2]==0x40))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!add);						                
				 	dianhuahaoma[10]++;
					if(dianhuahaoma[10]>9) 
					dianhuahaoma[10]=0;
					write_com(0x80+0x40+15);
					write_date('0'+dianhuahaoma[10]);
					write_com(0x80+0x40+15);
				}
			} 
			if(dec==0||(IrOK==1&&Im[2]==0x19))	
			{
			   	delay(20);
				if(dec==0||(IrOK==1&&Im[2]==0x19))
				{
					BEEP=0;
					delay(50);
					BEEP=1;
					Im[2]=0x00;
					while(!dec);						                
				 	dianhuahaoma[10]--;
					if(dianhuahaoma[10]<0) 
					dianhuahaoma[10]=9;
					write_com(0x80+0x40+15);
					write_date('0'+dianhuahaoma[10]);
					write_com(0x80+0x40+15);
				}
			}
		}
	
	 //*****退出设定*********		
		if(kval>=17)
		{
			write_com(0x0c);
			kval=0;		   //清零退出设置状态
			dsflj=1;	   //打开主页显示并读取传感器数据
//			lofl=1;
			keysw=1;  //允许手动开关窗
			if(lofl==1)
			{
			 	write_com(0x80);
			 	for(num=0;num<16;num++)
				{
					write_date(logo4[num]);//字幕显示
				}
				write_com(0x80+0x40);
				for(num=0;num<16;num++)//字幕显示
				{
					write_date(logo3[num]);
				}
				if(closesw==1)
				{
					write_com(0x80+13);				
					write_date('O');
					write_date('F');
					write_date('F');
				}
				else
				{
					write_com(0x80+13);				
					write_date('O');
					write_date('N');
					write_date(' ');
				}
				if(ZD_SD_MODE==0)
				{
					write_com(0x80+0x40+13);				
					write_date('Z');
					write_date('D');
					write_date(' ');	
				}
				else
				{
					write_com(0x80+0x40+13);				
					write_date('S');
					write_date('D');
					write_date(' ');
				}
			  	lofl=0;
			}
	  	}
 	}
}
	
//步进电机函数
void setmoto()
{
	char num;
   	if(zhenzhuan==1)	 //正转关窗
	{
		dsflj=0;		//正在开关窗时关闭主界面显示和传感器数据读取
		SETP_MOTOR_REV();	 
		if(second>=5)
		{
			jishi=0;
			BEEP=0;
			zhenzhuan=0;
			P1=0X00;
			dsflj=1;	 //窗子运行完成后打开主界面显示和传感器数据读取
			opensw=0;	  //打开
			closesw=1;
			keysw=1;	  //允许手动开关窗
		   	menusw=1;	 //运行完毕置1
	
//		  	zhenfl=1;
//			fanfl=0;

			write_com(0x80);
			for(num=0;num<13;num++)
			{						   //字幕显示
				write_date(logo4[num]);
			}
			write_com(0x80+0x40);	//字幕显示
			for(num=0;num<13;num++)
			{
				write_date(logo3[num]);
			}
			write_com(0x80+13);				
			write_date('O');
			write_date('F');
			write_date('F');
			if(ZD_SD_MODE==0)
			{
				write_com(0x80+0x40+13);				
				write_date('Z');
				write_date('D');
				write_date(' ');	
			}
			else
			{
				write_com(0x80+0x40+13);				
				write_date('S');
				write_date('D');
				write_date(' ');
			}
			BEEP=1;
		}
	}		
    if(fanzhuan==1)  //反转开窗
	{
		dsflj=0;	  //正在开关窗时关闭主界面显示和传感器数据读取
		SETP_MOTOR_FFW(); 
		if(second>=5)
		{
			jishi=0; 
			BEEP=0;
			second=0;
			P1=0X00;
//			zhenfl=0;
//			fanfl=1;
		   
			fanzhuan=0;
			dsflj=1;	//窗子运行完成后打开主界面显示和传感器数据读取
	
			opensw=1;
			closesw=0;
			keysw=1;		 //允许手动开关窗
			menusw=1;		   //运行完毕置1
			write_com(0x80);
			for(num=0;num<13;num++)	 //字幕显示
			{
				write_date(logo4[num]);
			}
			write_com(0x80+0x40);
			for(num=0;num<13;num++)
			{							  //字幕显示
				write_date(logo3[num]);
			}
			write_com(0x80+13);				
			write_date('O');
			write_date('N');
			write_date(' ');
			if(ZD_SD_MODE==0)
			{
				write_com(0x80+0x40+13);				
				write_date('Z');
				write_date('D');
				write_date(' ');	
			}
			else
			{
				write_com(0x80+0x40+13);				
				write_date('S');
				write_date('D');
				write_date(' ');
			}
			BEEP=1;		 
		}
	}
}
 
void TEMPHRDSPLAY()		    //显示空气温湿度函数   写入1602
{ 
    write_com(0x80+8);				 //显示温度
	write_date(0x30+U8T_data_H/10);
	write_date(0x30+U8T_data_H%10);

    write_com(0x80+0x40+8);			 //显示湿度
	write_date(0x30+U8RH_data_H/10);
	write_date(0x30+U8RH_data_H%10);

}	

void SMOGDSPLAY()		    //显示空气 函数   写入1602
{  
    write_com(0x80+0x40+2);				 
	write_date(0x30+adc0%1000/100);
	write_date(0x30+adc0%100/10);
	write_date(0x30+adc0%10);
}

void BRDSPLAY()	
{ 
    write_com(0x80+2);			
	write_date(0x30+adc1%1000/100);
	write_date(0x30+adc1%100/10);
	write_date(0x30+adc1%10);
}

void write_rvalue()
{     
    num4[0]=num4[4]%10;         //显示ge位  
	num4[1]=num4[4]%100/10;   //显示shi位
	num4[2]=num4[4]%1000/100;//显示百位

    write_com(0x80+8);				 //显示温度
	write_date(0x30+num4[2]);
	write_date(0x30+num4[1]);
	write_date(0x30+num4[0]);
}

/*********************************************************************
 *                  C51中字符串函数的扩充                            *
 ** 函数名称    : strsearch ()
 ** 函数功能    : 在指定的数组里连续找到相同的内容
 ** 入口参数   ： ptr2=要找的内容, ptr1 当前数组
 **** 出口参数 ： 0-没有 找到   >1 查找到
 *********************************************************************/

uchar strsearch(uchar *ptr2,uchar *ptr1_at)//查字符串*ptr2在*ptr1中的位置
//本函数是用来检查字符串*ptr2是否完全包含在*ptr1中
//返回:  0  没有找到
//       1-255 从第N个字符开始相同
{
	uchar i,j,k;
	if(ptr2[0]==0) return(0);
	for(i=0,j=0;i<RsBuf_N;i++)
	{
        if(ptr1_at[i]==ptr2[j])
        {//第一个字符相同
        	k=i;
        	do {
        		if(ptr2[j]=='\0')//比较正确
        			return(i+1);               //返回值是整数，不含0
        		if(ptr1_at[k]!=ptr2[j]) break;
        		k++;j++ ;	 
        	} while(k<RsBuf_N) ;

        	j=0;
        }
	}
	return(0);
}

void init()
{			  
 	TMOD=0x11;	   				//定时器设置
	TH0=0; 
    TL0=0;              //定时中断置零

	TH1=0x3c;					//定时器0置初值 0.05S
	TL1=0xb0;
	ET1=1;
	TR1=1;

	EA=1;						//开总中断
	IT1=1;//下降沿有效
	EX1=1;//外部中断1开	  
}

void uart()
{
	TMOD=0x20;	   				//定时器设置
	TH1=0xfd;					//定时器0置初值 0.05S
	TL1=0xfd;
	ET1=0;
	TR1=1;

	SM0=0;	    // 设置串口的工作模式
	SM1=1; 	    //方式1

	REN=1; 		 // 允许串口接收数据 	 

	ES=1; 	     // 串口中断应许 

	EA=1;						//开总中断
	IT1=0;//下降沿有效
	EX1=0;//外部中断1开	 
}

void SendASC(uchar d)	//串口发送字符
{
	SBUF=d;				//数据赋值到缓冲区
	while(!TI);			//发送完

	TI=0;				//清零
}

void SendString(uchar *str)	  //串口发送字符串
{
	while(*str)				  //判断是否发送完
	{
		SendASC(*str) ;		  //发送字符
		str++;				  //字符位置加
	}
}

void GSM_work()		//发送短信函数
{
	unsigned char send_number;				  //定义变量用与写入接收手机号码
	uchar k,j;
	SendString("AT+CMGF=1\r\n");			 //设置文本模式
	
	delay(800);							 //延时，让GSM模块有一个反应时间
	
	SendString("AT+CSCS=\"UCS2\"\r\n");		 //设置短信格式，发送汉字模式
	
	delay(800);							 //延时
	
	RsPoint=0;
	for(j=0;j<RsBuf_N;j++)
	{
		RsBuf[j]=0;
	}

	SendString("AT+CSMP=17,0,2,25\r\n");	 //设置短信文本模式参数（具体内容参考开发资料内的模块资料）
	
	delay(300);							 //延时
	k=0;
	k=strsearch("OK",RsBuf);			//返回OK
	RsPoint=0;
	if(k!=0)
	{
		SendString("AT+CMGS=");	//信息发送指令 AT+CMGS=//
		SendASC('"');									 //引号
		for(send_number=0;send_number<11;send_number++)	 //在每位号码前加003
		{
			SendASC('0');
			SendASC('0');
			SendASC('3');
			SendASC('0'+dianhuahaoma[send_number]);				 //接收手机号码
		}     
		SendASC('"');									//引号
		SendASC('\r');			//发送回车指令//
		SendASC('\n');			//发送换行指令//
		
		delay(800);				//延时
	
		SendString(somebody);		//发送短信内容
	
		delay(800);				//延时
		
		SendASC(0x1a);				//确定发送短信
	}
}

void main()//主函数
{
	uchar num;
	uchar j;
 	init1602();
	uart();
	delay(5000);							  //延时
	SendString("AT\r\n");
    delay(800);							  //延时
	SendString("AT\r\n");
    delay(800);							  //延时
	SendString("ati\r\n");
    delay(800);							  //延时
	SendString("ati\r\n");
    delay(800);							  //延时
    SendString("ATH\r\n");
    delay(800);							  //延时
	SendString("AT+IPR=9600\r\n");
    delay(800);							  //延时
	RsPoint=0;
	for(j=0;j<RsBuf_N;j++)
	{
		RsBuf[j]=0;
	}
	delay(10000);							  //延时

	SendString("AT\r\n");
    delay(800);							  //延时
	SendString("ati\r\n");
    delay(800);							  //延时
    SendString("ATH\r\n");
    delay(800);							  //延时
    SendString("AT+CMGF=1\r\n");
    delay(800);							  //延时
	SendString("AT+CNMI=2,1,0,0,0\r\n");
    delay(800);							  //延时
	SendString("AT+CSCS=\"UCS2\"\r\n");		
	delay(800);							  //延时
	SendString("AT+CSMP=17,0,2,25\r\n");		
	delay(800);							  //延时
	SendString("AT+CMGL=\"REC UNREAD\",0\r\n");
	delay(800);							  //延时

	//***************************************************
   write_com(0x80);
   for(num=0;num<16;num++)	//开机字幕
	{
		write_date(logo6[num]);
	}

	write_com(0x80+0x40);
	for(num=0;num<16;num++)
	{
		write_date(logo10[num]);//开机字幕
	}

	init();
	init_eeprom();  //开始初始化保存的数据
  	zhenzhuan=1;//上电关正转
  	fanzhuan=0;//上电反转关闭关窗
  	dsflj=1;	// 显示打开		
  	keysw=0;	//初始状态 置0 不允许手动开窗关窗
  	menusw=1;	 //初始状态运行完毕
  	second=0;

	adc0 = ADC0832(1,0);  //差分模式，CH0-CH1
	adc1 = ADC0832(1,1);  //差分模式，CH0-CH1

	while(1)//大循环		   
	{
		keyscn();
		setmoto();
		if (dsflj==1)
		{
			if(flag_celiang==1)
			{
				RH(); //DHT11 调用 
				TEMPHRDSPLAY(); 
				adc0 = ADC0832(1,0);  //差分模式，CH0-CH1 烟雾
				SMOGDSPLAY();
				adc1 = ADC0832(1,1);  //差分模式，CH0-CH1 光线
				BRDSPLAY();
				flag_celiang=0;	
			}
			if(ZD_SD_MODE==0)		//自动模式
			{
				if(closesw==1)	  //关闭状态
				{	 //光线超上限   与   温度小于    与    湿度小于		或	 烟雾大于    没报警的状态
					if(((adc1> kwbrig&&U8T_data_H<uptemp&&U8RH_data_H<uphum)||adc0>upsmo)&&befl==0)//如果际亮度小于设定值 kai
				 	{	 //开窗
							write_com(0x80);
							for(num=0;num<16;num++)	//开机字幕
							{
								write_date(logo5[num]);	//开窗
							}
						
							write_com(0x80+0x40);
							for(num=0;num<16;num++)
							{
								write_date(logo10[num]);//开机字幕
							}
							closesw=0;
				  			zhenzhuan=0;
							fanzhuan=1;
							second=0;
							jishi=1;
							if(adc0>upsmo)
							befl=1;
					} 	 					
				}
				if(opensw==1)	   //打开状态
				{	//烟雾小于	  与      湿度大于    或    温度大于   或  光线小于下限
					if((adc0<=upsmo&&(U8RH_data_H>uphum||U8T_data_H>uptemp||adc1<dwbrig))||IR==0)		 //关窗
					{
						if(IR==0)
						{
							write_com(0x80);
							for(num=0;num<16;num++)	//开机字幕
							{
								write_date(logo17[num]);
							}
						
							write_com(0x80+0x40);
							for(num=0;num<16;num++)
							{
								write_date(logo10[num]);//开机字幕
							}
							uart();
							GSM_work();					  //发送短信函数
							init();
							befl=1;
						}
						write_com(0x80);
						for(num=0;num<16;num++)	//开机字幕
						{
							write_date(logo6[num]);
						}
					
						write_com(0x80+0x40);
						for(num=0;num<16;num++)
						{
							write_date(logo10[num]);//开机字幕
						}
						opensw=0;
						fanzhuan=0;
						zhenzhuan=1;
						second=0;
						jishi=1;
					}
					if(adc0>upsmo)
					befl=1;  
				}
			}
		}
	}
}

//  通讯中断接收程序   中断函数无返回值
void uart_rx(void)  interrupt 4	using 3	  //放在这里 和放在main（）里面是一样的
{
	ES=0;
   	if(RI) //必须判断RI是否为1
    {
    	RI=0;
    	RsBuf[RsPoint++]=SBUF;
		if(RsPoint>=RsBuf_N)
		RsPoint=0;
    }
	ES=1;
}

void time1() interrupt 3 	//定时中断子程序
{
	uchar count0;
	TH1=0x3c;							//重赋初值
	TL1=0xb0;
	count0++;
	con++;

  	if(count0>=20)
   	{
		count0=0;
		if(jishi==1)
    	second++;
		if(flag_celiang==0)
		flag_celiang=1;
	}			
    if(befl==1)	   //启动报警
   	{
		if(con>=10)
	   	{
			con=0;
	    	BEEP=~BEEP;			
		}	
	}	 
}

//*********************************中断服务函数**************************************
// 遥控 解码 函数 接收头 接外部 中断口 P3.3
/************************************************************************/	
//外部中断解码程序
void intersvr1() interrupt 2
{
	TR0=1;
    Tc=TH0*256+TL0; //定时器清空只记TC 值                                              //提取中断时间间隔时长
    TH0=0; 
    TL0=0;              //定时中断重新置零

	if((Tc>Imin)&&(Tc<Imax))//启动码判定 两个时间对比0.9MS	
	{  
        m=0;
        f=1;
        return;
	}       //找到启始码
	
	if(f==1)
	{
        if(Tc>Inum1&&Tc<Inum3) //判定是0该还是1 是0 1.125MS 是1  2.25MS
		{
			Im[m/8]=Im[m/8]>>1|0x80; m++;  //存0
		}
		if(Tc>Inum2&&Tc<Inum1) 
        {
			Im[m/8]=Im[m/8]>>1; m++; //	存1
		}
		if(m==32) 	//32位存储完毕
		{
			m=0;  
			f=0;
			if(Im[2]==~Im[3])  //反码检查
			{	 
				IrOK=1; 	//OK 完成
					TR0=0;
			}
			else IrOK=0;   //取码完成后判断读码是否正确
		}	//准备读下一码
	}	
}
/***************************END***********************/