#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

sbit ADCS = P1^4;	//adc0832 ʱ�����Ŷ���
sbit ADCLK =P1^5;
sbit ADDI = P1^6;	//adc0832  �������Ŷ���
sbit ADDO = P1^6;	//adc0832  Ƭѡ���Ŷ���
		  
uchar ADC0832(bit mode,bit channel)    //ADת�������ؽ��
{
	uchar i,dat,ndat;
	
	ADCS = 0;//����CS��
	_nop_();
	_nop_();
	
	ADDI = 1;	//��1���½���Ϊ�ߵ�ƽ
	ADCLK = 1;//����CLK��
	_nop_();
	_nop_();
	ADCLK = 0;//����CLK��,�γ��½���1
	_nop_();
	_nop_();
	
	ADDI = mode;	//�͵�ƽΪ���ģʽ���ߵ�ƽΪ��ͨ��ģʽ��	
	ADCLK = 1;//����CLK��
	_nop_();
	_nop_();
	ADCLK = 0;//����CLK��,�γ��½���2
	_nop_();
	_nop_();
	
	ADDI = channel;	//�͵�ƽΪCH0���ߵ�ƽΪCH1	
	ADCLK = 1;//����CLK��
	_nop_();
	_nop_();
	ADCLK = 0;//����CLK��,�γ��½���3
	
	ADDI = 1;//�����������(���������)
	dat = 0;
	//���濪ʼ��ȡת��������ݣ������λ��ʼ���������D7~D0��
	for(i = 0;i < 8;i++)
	{
		dat <<= 1;
		ADCLK=1;//����ʱ�Ӷ�
		_nop_();
		_nop_();
		ADCLK=0;//����ʱ�Ӷ��γ�һ��ʱ������
		_nop_();
		_nop_();
		dat |= ADDO;
	}
	ndat = 0; 	   //��¼D0
	if(ADDO == 1)
	ndat |= 0x80;
	//���濪ʼ������ȡ��������ݣ���D1��D7�� 
	for(i = 0;i < 7;i++)
	{
		ndat >>= 1;
		ADCLK = 1;//����ʱ�Ӷ�
		_nop_();
		_nop_();
		ADCLK=0;//����ʱ�Ӷ��γ�һ��ʱ������
		_nop_();
		_nop_();
		if(ADDO==1)
		ndat |= 0x80;
	}	  
	ADCS=1;//����CS��,����ת��
	ADCLK=0;//����CLK��
	ADDI=1;//�������ݶ�,�ص���ʼ״̬
	if(dat==ndat)
	return(dat);
	else
	return 0;   
}
