
#include <reg52.h>//�⺯��
#include <dht11.h> //�⺯��//��� ʵ�� ��
#include <adc0832.h> //ADת���⺯��
#include "eeprom52.h"

#define uchar unsigned char//�궨��
#define uint  unsigned int//�궨��
#define ulong unsigned long	//�궨��
bit s=1;
#define NULL 0
#define RsBuf_N			150

unsigned char xdata RsBuf[RsBuf_N];	//���崮�ڽ������ݻ�����
uchar RsPoint;

sbit rs=P2^5;//LCD 1602 ���Ŷ���		
sbit rd=P2^6;//LCD 1602 ���Ŷ���		
sbit lcden=P2^7;//LCD 1602 ���Ŷ���
sbit menu=P2^0;	//�����ܼ�	
sbit add=P2^1;//�Ӽ�		
sbit dec=P2^2;// ����
sbit open=P2^3;// �ֶ� ��
sbit close=P2^4;// �ֶ� ��	 

sbit IR=P3^6; //����� Ӧ
sbit BEEP=P3^4;// ����������
#define MOTORSTEP P1 			//�궨�壬����P1��Ϊ������������˿�

uchar kval,con;// ���ֱ������
uchar second;//ʱ�ӱ���
bit dsflj,opensw,closesw,befl,menusw,lofl,zhenzhuan,fanzhuan,keysw;	
uint adc0,adc1;
uchar dwbrig,kwbrig,uphum,uptemp,upsmo; // ���� ���� ����ʪ��// �����¶�

uchar   U8T_data_H,U8RH_data_H;

char dianhuahaoma[12]; //���պ��루ֻ�������ƶ�������ͨ�ĺ��룬����ģ���ϵĿ�Ҳ�������ƶ�������ͨ�����ǲ����õ��ţ�

uchar code somebody[]		="8BF76CE8610FFF0C67094EBA8FDB5165FF01";	//��ע�⣡���˽��룡�����Ͷ������ݵı��룩

bit jishi=1;
bit ZD_SD_MODE=0;
bit flag_celiang=0;

#define Imax 14000    //�˴�Ϊ����Ϊ11.0592ʱ��ȡֵ, 
#define Imin 8000    //��������Ƶ�ʵľ���ʱ,
#define Inum1 1450    //Ҫ�ı���Ӧ��ȡֵ��
#define Inum2 700 
#define Inum3 3000 

uchar f=0;
uchar Im[4]={0x00,0x00,0x00,0x00}; // ң����洢�ռ�
uchar show[2]={0,0};  // ��������
ulong m,Tc;	  // ��������
uchar IrOK;		// ��������

void delay(uint z);// ����ʱ�����

void write_rvalue(); // ��ʾ����
//**************�����Ǹ�����Ļ��ʾ������************************
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

void delaymoto() 					//�������ÿһ�����ӳٺ���
{  	
	uint y=200;
	while(y--);
}

uchar code FFW[]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09}; //�����������ֵ����
uchar code REV[]={0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01}; //�����������ֵ����

//uchar num1 [4] =    {0x00,0x00,0x00,0x00,};				
//uchar num2 [4] =    {0x00,0x00,0x00,0x00,};			 
//uchar num3 [4] =    {0x00,0x00,0x00,0x00,};		   
uchar num4 [4] =    {0x00,0x00,0x00,0x00,};	// ADת����������
//uchar num5 [4] =    {0x00,0x00,0x00,0x00,};	// ADת����������

/******************�����ݱ��浽��Ƭ���ڲ�eeprom��******************/
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

/******************�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*****************/
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

/**************�����Լ�eeprom��ʼ��*****************/
void init_eeprom() 
{
	read_eeprom();		//�ȶ�
	if(a_a != 1)		//�µĵ�Ƭ����ʼ��Ƭ������eeprom
	{
		a_a = 1;
  		uptemp=33; //�¶�����
	  	uphum=60;	 //ʪ������
	  	dwbrig=30;//�������
	  	kwbrig=180;	//��������
	  	upsmo=85;	//��������ֵ

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

		write_eeprom();	   //��������
	}	
}

void delay(uint z)//��ʱ����
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void delay_1602(uint z)//��ʱ����
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=10;y>0;y--);
}

void SETP_MOTOR_FFW()	 //��ת
{
 	uint i=0;
	for(i=0;i<8;i++) //8��һ��ѭ��
	{
		MOTORSTEP=FFW[i]; //ȡֵ����P1������
		delaymoto(); 
	}
 }

void SETP_MOTOR_REV()  //��ת
{
    uint i=0;
	for(i=0;i<8;i++) //8��һ��ѭ��
	{
		MOTORSTEP=REV[i]; //ȡֵ����P1������
		delaymoto(); 
	}
}

void write_com(uchar com) //LCD 1602 дָ��
{
	rs=0;
	lcden=0;
	P0=com;
	delay_1602(5);
	lcden=1;
	delay_1602(5);
	lcden=0;	
}

void write_date(uchar date)//LCD 1602 д����
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

void init1602()//LCD 1602 ��ʼ��
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
		write_date(logo1[num]);//������Ļ
	}
	write_com(0x80+0x40);
	for(num=0;num<16;num++)
	{
		write_date(logo2[num]);	//������Ļ
	}
}

void keyscn()//����ɨ�躯��
{
	uchar num; //�ֲ�����
	if(kval==0)	  //û�н�������״̬ʱ
	{
		if(add==0||(IrOK==1&&Im[2]==0x15))		//�Ӱ������ֶ��Զ��л�����
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
				if(ZD_SD_MODE==0)		 //����0���Զ�
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
				befl=0;			 //�رձ���
				BEEP=1;	
				while(dec==0);
			}
		}
	}
	if(ZD_SD_MODE==1)	//�ֶ�ģʽ
	{
		if(keysw==1)	   //�����ֶ����ش�
		{
			if(closesw==1)	   //�ر�״̬ʱ
			{
				if(open==0||(IrOK==1&&Im[2]==0x09))	 // �ֶ�������������
				{				    
				    delay(20);
					if(open==0||(IrOK==1&&Im[2]==0x09))
					{
						BEEP=0;
						delay(50);
						BEEP=1;
						write_com(0x80);
						for(num=0;num<16;num++)//��ʾ������ʾ��
						{
							write_date(logo5[num]);
						}
						write_com(0x80+0x40);
						for(num=0;num<16;num++)//��ʾ��ʾ��
						{
							write_date(logo10[num]);
						}
						Im[2]=0x00;
						
						zhenzhuan=0;// ��ת ����
						fanzhuan=1;	 // ��ת�ر�
						second=0;// ������ ������� ��ͣ
						jishi=1;
						keysw=0;  //��ֹ�ֶ����ش�
						menusw=0; //�ֶ���ʼ���У�����
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
					for(num=0;num<16;num++)//��ʾ������ʾ��
					{
						write_date(logo5[num]);
					}
					write_com(0x80+0x40);
					for(num=0;num<16;num++)//��ʾ��ʾ��
					{
						write_date(logo10[num]);
					}
					Im[2]=0x00;
					
					zhenzhuan=0;// ��ת ����
					fanzhuan=1;	 // ��ת�ر�
					second=0;// ������ ������� ��ͣ
					jishi=1;
					keysw=0;  //��ֹ�ֶ����ش�
					menusw=0; //�ֶ���ʼ���У�����
				}
			}
			if(opensw==1)//��������Ǵ򿪵�
			{
			 	if(close==0||(IrOK==1&&Im[2]==0x07))//�����ֶ���
				{
				    delay(20);
					if(close==0||(IrOK==1&&Im[2]==0x07))
					{
						BEEP=0;
						delay(50);
						BEEP=1;
						write_com(0x80);
						for(num=0;num<16;num++)//��ʾ��ʾ��
						{
							write_date(logo6[num]);
						}
					 	write_com(0x80+0x40);
						for(num=0;num<16;num++)
						{
							write_date(logo10[num]);//��ʾ��ʾ��
						}
						Im[2]=0x00;
						zhenzhuan=1;  //��ת��
						fanzhuan=0;	// ��ת ����
						second=0;  	// ��ʱ����
						jishi=1;
						keysw=0; // ��ֹ�ֶ����ش�
						menusw=0;	//�ֶ���ʼ���У�����
						while(!close);
					}	 
				} 
				if(adc0>upsmo)
				befl=1; 	
			}
		}
	}

	if(menusw==1)//��������������
	{
 	 	if(menu==0||(IrOK==1&&Im[2]==0x47))// ��������ܼ�����
		{
		    delay(20);	//��ʱ������
			if(menu==0||(IrOK==1&&Im[2]==0x47))
			{
				BEEP=0;
				delay(50);
				BEEP=1;
				Im[2]=0x00;
				dsflj=0;// ����ҳ��ʾ
				kval++; // ��������++  ���ò�ͬ��ֵ
				keysw=0;//�������ֶ� ����  �͹ش�
				befl=0;
				lofl=1;	//��Ļֻ��Ҫ����һ��
				write_eeprom();			   //��������
				while(!menu);//���ּ��
			}
		} 

//*************************************************************************8
	
		
	 //**************************���� ���Ȼ����趨  ������Сʱһ��
	 //ֻ�Ǿ�����һ�� ����ע��*******************************************
	 //**************
	
	
	  //********************************�趨��������************************************
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
	
	
	   //********************************�趨��������************************************
	 
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
	
	 //**************************���� ʪ�Ȼ����趨  ������Сʱһ��
	 //ֻ�Ǿ�����һ�� ����ע��*******************************************
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
	 //**************************���� �¶ȶȻ����趨  ������Сʱһ��
	 //ֻ�Ǿ�����һ�� ����ע��*******************************************
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
	
	 //**************************��������Ȼ����趨  ������Сʱһ��
	 //ֻ�Ǿ�����һ�� ����ע��*******************************************
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
				write_com(0x80+0x40+5);	   //ѡ��Һ�����ϵĵڶ��е�ʮһ��
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
				write_com(0x0f);//��˸
				write_com(0x80+0x40+5);//��˸λ��
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
				write_com(0x80+0x40+6);//λ��
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
				write_com(0x80+0x40+7);//λ��
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
				write_com(0x80+0x40+8);//λ��
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
				write_com(0x80+0x40+9);//λ��
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
				write_com(0x80+0x40+10);//λ��
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
				write_com(0x80+0x40+11);//λ��
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
				write_com(0x80+0x40+12);//λ��
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
				write_com(0x80+0x40+13);//λ��
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
				write_com(0x80+0x40+14);//λ��
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
				write_com(0x80+0x40+15);//λ��
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
	
	 //*****�˳��趨*********		
		if(kval>=17)
		{
			write_com(0x0c);
			kval=0;		   //�����˳�����״̬
			dsflj=1;	   //����ҳ��ʾ����ȡ����������
//			lofl=1;
			keysw=1;  //�����ֶ����ش�
			if(lofl==1)
			{
			 	write_com(0x80);
			 	for(num=0;num<16;num++)
				{
					write_date(logo4[num]);//��Ļ��ʾ
				}
				write_com(0x80+0x40);
				for(num=0;num<16;num++)//��Ļ��ʾ
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
	
//�����������
void setmoto()
{
	char num;
   	if(zhenzhuan==1)	 //��ת�ش�
	{
		dsflj=0;		//���ڿ��ش�ʱ�ر���������ʾ�ʹ��������ݶ�ȡ
		SETP_MOTOR_REV();	 
		if(second>=5)
		{
			jishi=0;
			BEEP=0;
			zhenzhuan=0;
			P1=0X00;
			dsflj=1;	 //����������ɺ����������ʾ�ʹ��������ݶ�ȡ
			opensw=0;	  //��
			closesw=1;
			keysw=1;	  //�����ֶ����ش�
		   	menusw=1;	 //���������1
	
//		  	zhenfl=1;
//			fanfl=0;

			write_com(0x80);
			for(num=0;num<13;num++)
			{						   //��Ļ��ʾ
				write_date(logo4[num]);
			}
			write_com(0x80+0x40);	//��Ļ��ʾ
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
    if(fanzhuan==1)  //��ת����
	{
		dsflj=0;	  //���ڿ��ش�ʱ�ر���������ʾ�ʹ��������ݶ�ȡ
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
			dsflj=1;	//����������ɺ����������ʾ�ʹ��������ݶ�ȡ
	
			opensw=1;
			closesw=0;
			keysw=1;		 //�����ֶ����ش�
			menusw=1;		   //���������1
			write_com(0x80);
			for(num=0;num<13;num++)	 //��Ļ��ʾ
			{
				write_date(logo4[num]);
			}
			write_com(0x80+0x40);
			for(num=0;num<13;num++)
			{							  //��Ļ��ʾ
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
 
void TEMPHRDSPLAY()		    //��ʾ������ʪ�Ⱥ���   д��1602
{ 
    write_com(0x80+8);				 //��ʾ�¶�
	write_date(0x30+U8T_data_H/10);
	write_date(0x30+U8T_data_H%10);

    write_com(0x80+0x40+8);			 //��ʾʪ��
	write_date(0x30+U8RH_data_H/10);
	write_date(0x30+U8RH_data_H%10);

}	

void SMOGDSPLAY()		    //��ʾ���� ����   д��1602
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
    num4[0]=num4[4]%10;         //��ʾgeλ  
	num4[1]=num4[4]%100/10;   //��ʾshiλ
	num4[2]=num4[4]%1000/100;//��ʾ��λ

    write_com(0x80+8);				 //��ʾ�¶�
	write_date(0x30+num4[2]);
	write_date(0x30+num4[1]);
	write_date(0x30+num4[0]);
}

/*********************************************************************
 *                  C51���ַ�������������                            *
 ** ��������    : strsearch ()
 ** ��������    : ��ָ���������������ҵ���ͬ������
 ** ��ڲ���   �� ptr2=Ҫ�ҵ�����, ptr1 ��ǰ����
 **** ���ڲ��� �� 0-û�� �ҵ�   >1 ���ҵ�
 *********************************************************************/

uchar strsearch(uchar *ptr2,uchar *ptr1_at)//���ַ���*ptr2��*ptr1�е�λ��
//����������������ַ���*ptr2�Ƿ���ȫ������*ptr1��
//����:  0  û���ҵ�
//       1-255 �ӵ�N���ַ���ʼ��ͬ
{
	uchar i,j,k;
	if(ptr2[0]==0) return(0);
	for(i=0,j=0;i<RsBuf_N;i++)
	{
        if(ptr1_at[i]==ptr2[j])
        {//��һ���ַ���ͬ
        	k=i;
        	do {
        		if(ptr2[j]=='\0')//�Ƚ���ȷ
        			return(i+1);               //����ֵ������������0
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
 	TMOD=0x11;	   				//��ʱ������
	TH0=0; 
    TL0=0;              //��ʱ�ж�����

	TH1=0x3c;					//��ʱ��0�ó�ֵ 0.05S
	TL1=0xb0;
	ET1=1;
	TR1=1;

	EA=1;						//�����ж�
	IT1=1;//�½�����Ч
	EX1=1;//�ⲿ�ж�1��	  
}

void uart()
{
	TMOD=0x20;	   				//��ʱ������
	TH1=0xfd;					//��ʱ��0�ó�ֵ 0.05S
	TL1=0xfd;
	ET1=0;
	TR1=1;

	SM0=0;	    // ���ô��ڵĹ���ģʽ
	SM1=1; 	    //��ʽ1

	REN=1; 		 // �����ڽ������� 	 

	ES=1; 	     // �����ж�Ӧ�� 

	EA=1;						//�����ж�
	IT1=0;//�½�����Ч
	EX1=0;//�ⲿ�ж�1��	 
}

void SendASC(uchar d)	//���ڷ����ַ�
{
	SBUF=d;				//���ݸ�ֵ��������
	while(!TI);			//������

	TI=0;				//����
}

void SendString(uchar *str)	  //���ڷ����ַ���
{
	while(*str)				  //�ж��Ƿ�����
	{
		SendASC(*str) ;		  //�����ַ�
		str++;				  //�ַ�λ�ü�
	}
}

void GSM_work()		//���Ͷ��ź���
{
	unsigned char send_number;				  //�����������д������ֻ�����
	uchar k,j;
	SendString("AT+CMGF=1\r\n");			 //�����ı�ģʽ
	
	delay(800);							 //��ʱ����GSMģ����һ����Ӧʱ��
	
	SendString("AT+CSCS=\"UCS2\"\r\n");		 //���ö��Ÿ�ʽ�����ͺ���ģʽ
	
	delay(800);							 //��ʱ
	
	RsPoint=0;
	for(j=0;j<RsBuf_N;j++)
	{
		RsBuf[j]=0;
	}

	SendString("AT+CSMP=17,0,2,25\r\n");	 //���ö����ı�ģʽ�������������ݲο����������ڵ�ģ�����ϣ�
	
	delay(300);							 //��ʱ
	k=0;
	k=strsearch("OK",RsBuf);			//����OK
	RsPoint=0;
	if(k!=0)
	{
		SendString("AT+CMGS=");	//��Ϣ����ָ�� AT+CMGS=//
		SendASC('"');									 //����
		for(send_number=0;send_number<11;send_number++)	 //��ÿλ����ǰ��003
		{
			SendASC('0');
			SendASC('0');
			SendASC('3');
			SendASC('0'+dianhuahaoma[send_number]);				 //�����ֻ�����
		}     
		SendASC('"');									//����
		SendASC('\r');			//���ͻس�ָ��//
		SendASC('\n');			//���ͻ���ָ��//
		
		delay(800);				//��ʱ
	
		SendString(somebody);		//���Ͷ�������
	
		delay(800);				//��ʱ
		
		SendASC(0x1a);				//ȷ�����Ͷ���
	}
}

void main()//������
{
	uchar num;
	uchar j;
 	init1602();
	uart();
	delay(5000);							  //��ʱ
	SendString("AT\r\n");
    delay(800);							  //��ʱ
	SendString("AT\r\n");
    delay(800);							  //��ʱ
	SendString("ati\r\n");
    delay(800);							  //��ʱ
	SendString("ati\r\n");
    delay(800);							  //��ʱ
    SendString("ATH\r\n");
    delay(800);							  //��ʱ
	SendString("AT+IPR=9600\r\n");
    delay(800);							  //��ʱ
	RsPoint=0;
	for(j=0;j<RsBuf_N;j++)
	{
		RsBuf[j]=0;
	}
	delay(10000);							  //��ʱ

	SendString("AT\r\n");
    delay(800);							  //��ʱ
	SendString("ati\r\n");
    delay(800);							  //��ʱ
    SendString("ATH\r\n");
    delay(800);							  //��ʱ
    SendString("AT+CMGF=1\r\n");
    delay(800);							  //��ʱ
	SendString("AT+CNMI=2,1,0,0,0\r\n");
    delay(800);							  //��ʱ
	SendString("AT+CSCS=\"UCS2\"\r\n");		
	delay(800);							  //��ʱ
	SendString("AT+CSMP=17,0,2,25\r\n");		
	delay(800);							  //��ʱ
	SendString("AT+CMGL=\"REC UNREAD\",0\r\n");
	delay(800);							  //��ʱ

	//***************************************************
   write_com(0x80);
   for(num=0;num<16;num++)	//������Ļ
	{
		write_date(logo6[num]);
	}

	write_com(0x80+0x40);
	for(num=0;num<16;num++)
	{
		write_date(logo10[num]);//������Ļ
	}

	init();
	init_eeprom();  //��ʼ��ʼ�����������
  	zhenzhuan=1;//�ϵ����ת
  	fanzhuan=0;//�ϵ練ת�رչش�
  	dsflj=1;	// ��ʾ��		
  	keysw=0;	//��ʼ״̬ ��0 �������ֶ������ش�
  	menusw=1;	 //��ʼ״̬�������
  	second=0;

	adc0 = ADC0832(1,0);  //���ģʽ��CH0-CH1
	adc1 = ADC0832(1,1);  //���ģʽ��CH0-CH1

	while(1)//��ѭ��		   
	{
		keyscn();
		setmoto();
		if (dsflj==1)
		{
			if(flag_celiang==1)
			{
				RH(); //DHT11 ���� 
				TEMPHRDSPLAY(); 
				adc0 = ADC0832(1,0);  //���ģʽ��CH0-CH1 ����
				SMOGDSPLAY();
				adc1 = ADC0832(1,1);  //���ģʽ��CH0-CH1 ����
				BRDSPLAY();
				flag_celiang=0;	
			}
			if(ZD_SD_MODE==0)		//�Զ�ģʽ
			{
				if(closesw==1)	  //�ر�״̬
				{	 //���߳�����   ��   �¶�С��    ��    ʪ��С��		��	 �������    û������״̬
					if(((adc1> kwbrig&&U8T_data_H<uptemp&&U8RH_data_H<uphum)||adc0>upsmo)&&befl==0)//���������С���趨ֵ kai
				 	{	 //����
							write_com(0x80);
							for(num=0;num<16;num++)	//������Ļ
							{
								write_date(logo5[num]);	//����
							}
						
							write_com(0x80+0x40);
							for(num=0;num<16;num++)
							{
								write_date(logo10[num]);//������Ļ
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
				if(opensw==1)	   //��״̬
				{	//����С��	  ��      ʪ�ȴ���    ��    �¶ȴ���   ��  ����С������
					if((adc0<=upsmo&&(U8RH_data_H>uphum||U8T_data_H>uptemp||adc1<dwbrig))||IR==0)		 //�ش�
					{
						if(IR==0)
						{
							write_com(0x80);
							for(num=0;num<16;num++)	//������Ļ
							{
								write_date(logo17[num]);
							}
						
							write_com(0x80+0x40);
							for(num=0;num<16;num++)
							{
								write_date(logo10[num]);//������Ļ
							}
							uart();
							GSM_work();					  //���Ͷ��ź���
							init();
							befl=1;
						}
						write_com(0x80);
						for(num=0;num<16;num++)	//������Ļ
						{
							write_date(logo6[num]);
						}
					
						write_com(0x80+0x40);
						for(num=0;num<16;num++)
						{
							write_date(logo10[num]);//������Ļ
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

//  ͨѶ�жϽ��ճ���   �жϺ����޷���ֵ
void uart_rx(void)  interrupt 4	using 3	  //�������� �ͷ���main����������һ����
{
	ES=0;
   	if(RI) //�����ж�RI�Ƿ�Ϊ1
    {
    	RI=0;
    	RsBuf[RsPoint++]=SBUF;
		if(RsPoint>=RsBuf_N)
		RsPoint=0;
    }
	ES=1;
}

void time1() interrupt 3 	//��ʱ�ж��ӳ���
{
	uchar count0;
	TH1=0x3c;							//�ظ���ֵ
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
    if(befl==1)	   //��������
   	{
		if(con>=10)
	   	{
			con=0;
	    	BEEP=~BEEP;			
		}	
	}	 
}

//*********************************�жϷ�����**************************************
// ң�� ���� ���� ����ͷ ���ⲿ �жϿ� P3.3
/************************************************************************/	
//�ⲿ�жϽ������
void intersvr1() interrupt 2
{
	TR0=1;
    Tc=TH0*256+TL0; //��ʱ�����ֻ��TC ֵ                                              //��ȡ�ж�ʱ����ʱ��
    TH0=0; 
    TL0=0;              //��ʱ�ж���������

	if((Tc>Imin)&&(Tc<Imax))//�������ж� ����ʱ��Ա�0.9MS	
	{  
        m=0;
        f=1;
        return;
	}       //�ҵ���ʼ��
	
	if(f==1)
	{
        if(Tc>Inum1&&Tc<Inum3) //�ж���0�û���1 ��0 1.125MS ��1  2.25MS
		{
			Im[m/8]=Im[m/8]>>1|0x80; m++;  //��0
		}
		if(Tc>Inum2&&Tc<Inum1) 
        {
			Im[m/8]=Im[m/8]>>1; m++; //	��1
		}
		if(m==32) 	//32λ�洢���
		{
			m=0;  
			f=0;
			if(Im[2]==~Im[3])  //������
			{	 
				IrOK=1; 	//OK ���
					TR0=0;
			}
			else IrOK=0;   //ȡ����ɺ��ж϶����Ƿ���ȷ
		}	//׼������һ��
	}	
}
/***************************END***********************/