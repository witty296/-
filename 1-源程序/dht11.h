#include <reg52.h>
#include <intrins.h>
sbit  date = P3^5;
#define uchar unsigned char
#define uint  unsigned int

//----------------������--------------------//
uchar   U8FLAG;
uchar   U8count,U8temp;
uchar   U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
uchar   U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
uchar   U8comdata; 

   //��ʱ����
void Delay20MS(uint j)
{   
	uchar i;
	for(;j>0;j--)
	{  
		for(i=0;i<27;i++); 
	}
}

void Delay_10us(void)
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
	
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

void COM(void)	  //��������  ������ÿ�ֽ�����
{
	uchar i;
	for(i=0;i<8;i++)    
	{
		U8FLAG=2;
		while((!date)&&U8FLAG++);
		Delay_10us();
		Delay_10us();
		U8temp=0;
		if(date)U8temp=1;
		U8FLAG=2;
		while((date)&&U8FLAG++);
		//��ʱ������forѭ��    
		if(U8FLAG==1)break;
		//�ж�����λ��0����1  
		
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
		
		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof
}

 //--------------------------------
 //-----ʪ�ȶ�ȡ�ӳ��� ------------
 //--------------------------------
 //----���±�����Ϊȫ�ֱ���--------
 //----�¶ȸ�8λ== U8T_data_H------
 //----�¶ȵ�8λ== U8T_data_L------
 //----ʪ�ȸ�8λ== U8RH_data_H-----
 //----ʪ�ȵ�8λ== U8RH_data_L-----
 //----У�� 8λ == U8checkdata-----
 //----��������ӳ�������----------
 //---- Delay();, Delay_10us();,COM(); 
 //--------------------------------

void RH(void)		//����5�ֽ�����
{
	//��������18ms 
	date=0;
	Delay20MS(180);
	date=1;
	//������������������ ������ʱ20us
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
	//������Ϊ���� �жϴӻ���Ӧ�ź� 
	date=1;
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������   
	if(!date)   //T !   
	{
		U8FLAG=2;////////////////////////////////////////////////////
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����  
		while((!date)&&U8FLAG++);
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while((date)&&U8FLAG++);
		//���ݽ���״̬   
		COM();
		U8RH_data_H_temp=U8comdata;
		COM();
		U8RH_data_L_temp=U8comdata;
		COM();
		U8T_data_H_temp=U8comdata;
		COM();
		U8T_data_L_temp=U8comdata;
		COM();
		U8checkdata_temp=U8comdata;
		date=1;
		//����У�� 
		U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
		if(U8temp==U8checkdata_temp)
		{
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
			U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
		}	
	}
}