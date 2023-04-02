#include <reg52.h>
#include <intrins.h>
#include"24C0x.h"
#include"LCD12864.h"
#define uchar unsigned char
#define uint unsigned int
 
#define buffer1ID  0x01
#define buffer2ID  0x02
#define queren  0x88
#define tuichu  0x84
#define shanchu 0x82

#define User 1				     

void fan();
        
sbit B0=B^0;
sbit B7=B^7;
sbit k2=P3^7;
sbit jidianqi=P3^6;
sbit Feng=P2^0;
        
extern unsigned char times[];
uchar  Member=1,sec;
uchar  ErrorCount;
uchar  code buffer[User][6]=
{
 {"000000"}
};
        
        
uchar dz[4];
uchar time[4];			 
uchar mima[6];
uchar mimag[6];
uchar mimaID[6]={1,2,3,4,5,6};


uchar Address1=20,tempx=20; 
uchar Address2=20;   
        
uchar data K;
uchar data Key;
        
uint PageID;
uchar data querenma=11;
uchar sum[2];
int summaf,summas;

uchar  xdata UserPassword[7]={0};//����
uchar  xdata DSY_BUFFER[16] ={0};//����


void Red_Init(void);
#include "zhiwen.h"
//���������
unsigned char check(void);

unsigned char redchange(unsigned char rednum);
        

void delayms(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

/********************************************************************
* ���� : Chack(uchar User_Number)
* ���� : ��һ�û�������
* ���� : User_Number
* ��� : ������ȷ����󣬴��󷵻�0����ȷ����1
********************************************************************/
uchar Chack(uchar User_Number)
{
	uchar flag, i,temp[6],Address;
	Address=User_Number*10;
	for(i=0;i<6;i++)
	{
		temp[i]=x24c02_read(Address);
		delayms(10);
		Address++;
	}
	for(i=0;i<6;i++)
	{
		if(temp[i]==UserPassword[i])
			flag=1;
		else 	
			flag=0;		
	}
	return flag;
}

/********************************************************************
* ���� : PassWord_Chack()
* ���� : �����û�������
* ���� : ��
* ��� : ������ȷ����󣬴��󷵻�0����ȷ����1
********************************************************************/
uchar PassWord_Chack()
{
	uchar i=0;
	while(i<User)
	{
		if(Chack(i)==1)
		{
			Member=i+1;
			return 1;
		}
		i++;	
	}
	return 0;	
}
   
void delay(uint tt)
{  uchar i;
   while(tt--)
   {
   for(i=0;i<125;i++);
   }
}
/********************************************************************
* ���� : UART_Init()
* ���� : ��Ƭ�����ڳ�ʼ��
* ��Ƭ��������11.0592MHz ������Ϊ9600bps
* ָ��ģ�飺������Ϊ9600bps��
********************************************************************/        
void UART_Init()
{
	SCON= 0x50;               //���ڷ�ʽ1	//REN=1; �������
	PCON=0x00;                //SMOD=0
	TMOD= 0x21;               //��ʱ��1��ʱ��ʽ2
	TH1= 0xFD;                //11.0592MHz  ģ��Ĭ�ϲ�����Ϊ9600bps
	TL1= 0xFD;										 
	TR1= 1;                   //������ʱ�� 
	EA=1;       
}
    
unsigned char Keycan(void) //����ɨ����� P1.0--P1.3Ϊ���� P1.4--P1.7Ϊ����
{
  unsigned char rcode, ccode;
  P1 = 0xF0;      // ��ȫ0��ɨ���룬��������
  if((P1&0xF0) != 0xF0) // ���м�����
  {
    delay(1);// ��ʱȥ���� 
    if((P1&0xF0) != 0xF0)
    {  rcode = 0xFE;         // ����ɨ���ֵ
    while((rcode&0x10) != 0)
    {
       P1 = rcode;         // �����ɨ����
      if((P1&0xF0) != 0xF0) // �����м�����
      {
       ccode = (P1&0xF0)|0x0F;
       //do{;}
	   while((P1&0xF0) != 0xF0); //�ȴ����ͷ�
       return ((~rcode) + (~ccode)); // ���ؼ�����
      }
    else
    rcode = (rcode<<1)|0x01; // ��ɨ��������һλ
    }
    }
  }  
  return 0; // �޼����£�����ֵΪ0
}
        
void KeyDeal(unsigned char Key)
{  //unsigned char n;
   if(Key!=0)
 {
           
		   switch(Key)
		   {
		   case 0x11: K=1; break;	 
		   case 0x21: K=2; break;  
		   case 0x41: K=3; break;  
		   case 0x81:  break;       
		   case 0x12: K=4; break;
		   case 0x22: K=5; break;
		   case 0x42: K=6; break;
		   case 0x82: K=34;break;		
		   case 0x14: K=7; break;
		   case 0x24: K=8; break;
		   case 0x44: K=9; break;
		   case 0x84:  break;		
		   case 0x18:  break;		
		   case 0x28: K=0; break;
		   case 0x48:  K=11; break;
		   case 0x88:  break;		
		   default: break;
		   }
  }
}
        
 
void shuazhiwen()
{
    uchar IDs1,IDs2,IDs3;	 
	LcmClearTXT();
	PutStr(1,1,"�밴��ָ����");      
	SFG_identify();
	while(querenma==2)
	SFG_identify(); 
	if(querenma==0)
   {
		LcmClearTXT();
		PutStr(1,2,"���Ѵ�"); 
		IDs1=PageID/100;
		IDs2=PageID/10%10;
		IDs3=PageID%10;
		PutStr(2,1,"���Ϊ:");
		WriteCommand(0x8D);		//ָ����������ʾλ��
		WriteData(0x30+IDs1);
		WriteData(0x30+IDs2);
		WriteData(0x30+IDs3);
		ErrorCount=0;
		Feng=1;
		jidianqi=0;
		delay(2500); 
		jidianqi=1;
		
	}
   	else if(querenma==9)
 		{
			LcmClearTXT();
			PutStr(1,1,"û������ָ��");
			PutStr(2,1,"�����°���ָ");
			ErrorCount=3;
			Feng=0;
		}
   		else 
		{
			LcmClearTXT();
			PutStr(1,1,"���հ�����");
		}
		delay(2000);
   //	 while(Keycan()==0);
}
void addfinger()
{
	uchar IDa1,IDa2,IDa3;
	LcmClearTXT();
	PutStr(1,2,"�밴��ָ");    
	SFG_getimage();
	while(querenma!=0)
	SFG_getimage();
	SFG_genchar(buffer1ID);
	UART_Init();
	SFG_fastsearch(buffer1ID);
	while(querenma==1)
	SFG_fastsearch(buffer1ID);
	if(querenma==0)
	{
		LcmClearTXT();
		PutStr(1,1,"��ָ���Ѵ洢");
		PutStr(3,0,"  �����������"); 
	    while(Keycan()==0);
	}
	else if(querenma==9)
		{
			LcmClearTXT();
			PutStr(1,1,"���ٴΰ���ָ");
			SFG_enroll();
			while(querenma==2)
			SFG_enroll();
			LcmClearTXT();
			if(querenma==0)
			{ 
				IDa1=PageID/100;IDa2=PageID/10%10;IDa3=PageID%10;
				PutStr(1,1,"ָ�Ʋɼ��ɹ�");
				PutStr(2,1,"���Ϊ:");
				WriteCommand(0x8D); 
				WriteData(0x30+IDa1);WriteData(0x30+IDa2);WriteData(0x30+IDa3);
	  		}
	  	else if(querenma!=0)
	  		{
				PutStr(1,1,"ָ�Ʋɼ�ʧ��");
				PutStr(2,1,"�����²���");
	 		}
		PutStr(3,0,"  �����������"); 
	    while(Keycan()==0);
   		}
	LcmClearTXT();
		 	PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
}
void deletfinger()
{
	uchar i,j=0;
	LcmClearTXT();
	PutStr(1,0,"����ɾȥ��ָ�ƺ�");
	for(i=0;i<5;i++)dz[i]=0;	
	   Key=Keycan();
	   while(Key!=queren)
	   {
	     Key=Keycan();
	     KeyDeal(Key);
	     delay(30);				                              //�����ж���@@@
	     if(Key==0)K=10;
	     if((K>=0)&&(K<=9))
	     {
	      dz[j]=K;
		  if(j<3)
		  {	
				WriteCommand(0x88+j);		//ָ����������ʾλ��
				WriteData(0x30+dz[j]);	
		  }
	      ++j;
		  if(j==4)
	      j=3;												   //@@#yinhuang
	     }   //��ʾLCD12864������ʾ 
		 if(K==34)		//����ɾ����
		  {
		    if(j==0)
			{
				WriteCommand(0x88);		//ָ����������ʾλ��
				WriteData(0x20);
			}
			else
			{
				--j;
				WriteCommand(0x88+j);	    //ָ����������ʾλ��
				WriteData(0x20);
			}
		}
	}
	if(j>=2)
	PageID=dz[2]+dz[1]*10+dz[0]*100;
	if(j==1)
	PageID=dz[1]+dz[0]*10;
	if(j==0)
	PageID=dz[0];
	SFG_deletchar(PageID);
	if(querenma==0)
	{
		LcmClearTXT();
		PutStr(1,1,"ɾȥָ�ƺųɹ���"); 
	}
	else
	{
		LcmClearTXT();
		PutStr(1,2,"ɾȥָ�ƺ�ʧ�ܣ�");  
	}
	PutStr(2,0,"  �����������"); 
	while(Keycan()==0);
	LcmClearTXT();
		 	PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
}
        
void gaimima()
{
	uchar i,j=0,mima1[6],mima2[6];
	uchar k,temp;
	LcmClearTXT();
	PutStr(1,1,"������������");
	for(i=0;i<6;i++)mima1[i]=0;									
	   Key=Keycan();
      while(Key!=queren)
	   {
	     Key=Keycan();
	     KeyDeal(Key);
	     delay(30);				                              
	     if(Key==0)K=10;
	     if((K>=0)&&(K<=9))
	     {
	      mima1[j]=K;
		  if(j<6)
		  {	
				WriteCommand(0x89+j);		//ָ����������ʾλ��
				WriteData(0x0f);	
		  }
	      ++j;
		  if(j==7)
	      j=6;												   //@@#yinhuang
	     }   //��ʾLCD12864������ʾ 
		 if(K==34)		//����ɾ����
		  {
		    if(j==0)
			{
				WriteCommand(0x89);		//ָ����������ʾλ��
				WriteData(0x20);
			}
			else
			{
				--j;
				WriteCommand(0x89+j);	    //ָ����������ʾλ��
				WriteData(0x20);
			}
		}
	}
	LcmClearTXT();
	LcmClearTXT();
	j=0;
	PutStr(1,0,"���ٴ�����������");
	for(i=0;i<6;i++)mima2[i]=0;									
	   Key=Keycan();
      while(Key!=queren)
	   {
	     Key=Keycan();
	     KeyDeal(Key);
	     delay(30);				                              
	     if(Key==0)K=10;
	     if((K>=0)&&(K<=9))
	     {
	      mima2[j]=K;
		  if(j<6)
		  {		
				WriteCommand(0x89+j);		//ָ����������ʾλ��
				WriteData(0x0f);	
		  }
	      ++j;
		  if(j==7)
	      j=6;												   //@@#yinhuang
	     }   //��ʾLCD12864������ʾ 
		 if(K==34)		//����ɾ����
		  {
		    if(j==0)
			{
				WriteCommand(0x89);		//ָ����������ʾλ��
				WriteData(0x20);
			}
			else
			{
				--j;
				WriteCommand(0x89+j);	    //ָ����������ʾλ��
				WriteData(0x20);
			}
		}
	}
	LcmClearTXT();
	if((mima1[0]==mima2[0])&&(mima1[1]==mima2[1])&&(mima1[2]==mima2[2])&&(mima1[3]==mima2[3])&&(mima1[4]==mima2[4])&&(mima1[5]==mima2[5]))
	{
		for(i=0;i<6;i++)
		mimaID[i]=mima1[i];
		/////////////////////////////////////////////////////
		for(i=0;i<6;i++)           //����������6λ����
		{
			UserPassword[i]=mima1[i]+0x30;                          	
		}
//		temp=(Member-1)*10;
		temp=1;	
		delayms(5);
		for(k=0;k<6;k++)
		{
			x24c02_write(temp,UserPassword[k]);
			delayms(10);
			temp++;
		}  
		//////////////////////////////////////////////////////
		PutStr(0,1,"�����޸ĳɹ�"); 
		PutStr(3,0,"  �����������");
		ErrorCount=0;
		while(Keycan()==0);
	}
	else
	{
		PutStr(0,0,"  �����޸�ʧ��  ");
		PutStr(1,0,"������������벻"); 
		PutStr(2,0,"һ�£������²���");       
		PutStr(3,0,"  �����������"); 
		while(Keycan()==0);
	}
	LcmClearTXT();
		 	PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
}


void zhu()
{  	 
    LcmClearTXT();
	PutStr(1,2,"���Ѵ�");
   	jidianqi=0;
	delay(2500); 
	jidianqi=1;
	PutStr(3,0,"  �����������");
	while(Keycan()==0);
		 	PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
}

void guanliyuan()
{    
      uchar i,j=0,x=1;
   	  uchar Right_flag;
      LcmClearTXT();
	  PutStr(1,1,"���������룺");	
	   for(i=0;i<6;i++)mima[i]=0;									
	   Key=Keycan();
      while(Key!=queren)
	   {
	     Key=Keycan();
	     KeyDeal(Key);
	     delay(30);				                              
	     if(Key==0)K=10;
	     if((K>=0)&&(K<=9))
	     {
	      mima[j]=K;

		  if(j<6)
		  {	
		   WriteCommand(0x89+j);		//ָ����������ʾλ��
	       WriteData(0x0f);	

		  }
	      ++j;
		  if(j==7)
	      j=6;												  
	     }   //��ʾLCD12864������ʾ 
		 if(K==34)		//����ɾ����
		  {
		    if(j==0)
			{
		      WriteCommand(0x89);		//ָ����������ʾλ��
	          WriteData(0x20);
			}
			else
			{
			  --j;
		      WriteCommand(0x89+j);	    //ָ����������ʾλ��
	          WriteData(0x20);
			}
		  }
	   }
    LcmClearTXT();


	  for(i=0;i<6;i++)
         {
            UserPassword[i]=mima[i]+0x30;
	
          } 
	 if(j==6)
	  {
	   if(mima[0]==1&&mima[1]==3&&mima[2]==1&&mima[3]==4&&mima[4]==2&&mima[5]==0)
	    Right_flag=1;
		else
		Right_flag=0;
		      }
	  if (Right_flag==1)
	 {
	  	   Right_flag=0;
		    
		   for(i=0;i<6;i++)mima[i]=0;
		PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
     while(i!=tuichu)
      {	 
		 i=Keycan();
	
		KeyDeal(i);  
	    switch(K)
	    {
	     case 1:    addfinger();K=6; break;
	     case 2:    deletfinger();    break;
		 case 3:	 zhu();	K=8;  break;
		 case 4:    gaimima();        break;
	     default: break;
	    }
      }
     }
    else
    {

		PutStr(1,2,"�������");
		PutStr(2,0,"  �����²�����"); 
		PutStr(3,0,"  �����������"); 
	     while(Keycan()==0);
		PutStr(0,0,"����1 : ����ָ��"); 
		PutStr(1,0,"����2 : ɾȥָ��");
		PutStr(2,0,"����3 : �ֶ�����");
		PutStr(3,0,"����4 : �޸�����");
    }
  Key=0;
   
}
void mimakaisuo()
{
	uchar i,j=0,x=1;
	uchar Right_flag;
	LcmClearTXT();
	PutStr(1,0,"���������뿪����");	
	for(i=0;i<6;i++)mima[i]=0;									
	Key=Keycan();
	while(Key!=queren)
	{
		Key=Keycan();
		KeyDeal(Key);
		delay(30);				                              
		if(Key==0)K=10;
		if((K>=0)&&(K<=9))
		{
			mima[j]=K;
			if(j<6)
			{	
				WriteCommand(0x89+j);		//ָ����������ʾλ��
				WriteData(0x0f);	
			}
			++j;
			if(j==7)
			j=6;												  
		}   //��ʾLCD12864������ʾ 
		if(K==34)		//����ɾ����
		{
			if(j==0)
			{
				WriteCommand(0x89);		//ָ����������ʾλ��
				WriteData(0x20);
			}
			else
			{
				--j;
				WriteCommand(0x89+j);	    //ָ����������ʾλ��
				WriteData(0x20);
			}
		}
	}
	LcmClearTXT();
	for(i=0;i<6;i++)
	{
		UserPassword[i]=mima[i]+0x30;
	} 
	if(j==6){Right_flag=PassWord_Chack(); }
	if (Right_flag==1&&ErrorCount<3)
	{
		Right_flag=0;
		zhu();ErrorCount=0;  
		Feng=1;
		for(i=0;i<6;i++)mima[i]=0;
	}
	else
	{
		if(ErrorCount<3)
		{
			ErrorCount++;
			PutStr(1,2,"�������");
			PutStr(2,0,"  ����  �λ���"); 
			PutStr(3,0,"  �����������");
			WriteCommand(0x8a);
			WriteData(' ');
			WriteData(3-ErrorCount+0x30);
		}
		if(ErrorCount==3)
		{
			Feng=0;
			LcmClearTXT();
			PutStr(1,0,"  ����������");
			PutStr(2,0,"  ��ָ�ƿ���"); 
		}
		delay(1000);
	}
	Key=0;
	LcmClearTXT();
}
void fan()

{
//	PutStr(1,2,"���Ȱ���");
//	PutStr(2,2,"��ˢָ��");
  if(Key!=0x81)			//����Ա����
  {
	PutStr(0,2,"��ӭʹ��");
	PutStr(1,1,"ָ���Ž�ϵͳ");
	PutStr(2,1,"�밴 C������");
	PutStr(3,1,"�밴��ָ����");
   }
  Key=Keycan();
if(k2==1)		//ָ��ˢ��
  {
  	LcmClearTXT();
 	shuazhiwen();
 	LcmClearTXT();
  }
  if(Key==0x81)			//����Ա����
  {
	LcmClearTXT();
	guanliyuan();
    LcmClearTXT();
  }
  if(Key==0x84&&ErrorCount<3)			//���뿪��
  {
	LcmClearTXT();
	mimakaisuo();
    LcmClearTXT();
  }

}	
     
void main()
{ 	

//PSB=0;
	UART_Init();	   //���ڳ�ʼ��		  
  	x24c02_init();     //24C02��ʼ��
	LcmInit();	       //LCD12864��ʼ��				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	LcmClearTXT();	   //LCD12864����	
	while(1) 
		{ 
			fan();
			delay(100);		  
		}	
}