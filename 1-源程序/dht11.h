#include <reg52.h>
#include <intrins.h>
sbit  date = P3^5;
#define uchar unsigned char
#define uint  unsigned int

//----------------定义区--------------------//
uchar   U8FLAG;
uchar   U8count,U8temp;
uchar   U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
uchar   U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
uchar   U8comdata; 

   //延时函数
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

void COM(void)	  //串行总线  ，接收每字节数据
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
		//超时则跳出for循环    
		if(U8FLAG==1)break;
		//判断数据位是0还是1  
		
		// 如果高电平高过预定0高电平值则数据位为 1 
		
		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof
}

 //--------------------------------
 //-----湿度读取子程序 ------------
 //--------------------------------
 //----以下变量均为全局变量--------
 //----温度高8位== U8T_data_H------
 //----温度低8位== U8T_data_L------
 //----湿度高8位== U8RH_data_H-----
 //----湿度低8位== U8RH_data_L-----
 //----校验 8位 == U8checkdata-----
 //----调用相关子程序如下----------
 //---- Delay();, Delay_10us();,COM(); 
 //--------------------------------

void RH(void)		//接收5字节数据
{
	//主机拉低18ms 
	date=0;
	Delay20MS(180);
	date=1;
	//总线由上拉电阻拉高 主机延时20us
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
	//主机设为输入 判断从机响应信号 
	date=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行   
	if(!date)   //T !   
	{
		U8FLAG=2;////////////////////////////////////////////////////
		//判断从机是否发出 80us 的低电平响应信号是否结束  
		while((!date)&&U8FLAG++);
		U8FLAG=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((date)&&U8FLAG++);
		//数据接收状态   
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
		//数据校验 
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