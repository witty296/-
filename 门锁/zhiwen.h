    
        
//*************************************//
void SFG_GetEcho()			      //����
{
   uchar i;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X03;
   while(TI==0);
   TI=0;
        
   SBUF=0X53;
   while(TI==0);
   TI=0;
         
   SBUF=0X00;
   while(TI==0);
   TI=0;
   summaf=0x57;
   SBUF=summaf;
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
        
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
}
 
        
//***************************************//
        
void SFG_getimage()			      //¼��ָ��ͼ��
{
   uchar i;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X03;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   summaf=0x05;
   SBUF=summaf;
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
        
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
}
        
void SFG_genchar(uchar bufferID) //��������������charbuffer1/2  ���ú�Ƭ�������ʱ仯@@@
{
   uchar i;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X04;
   while(TI==0);
   TI=0;
        
   SBUF=0X02;
   while(TI==0);
   TI=0;
        
   SBUF=bufferID;
   while(TI==0);
   TI=0;
        
   summaf=0x07+bufferID;
   sum[0]=summaf;
   sum[1]=summaf>>8;
   SBUF=sum[1];
   while(TI==0)
   TI=0;
   SBUF=sum[0];
   while(TI==0)
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
}

void SFG_fastsearch(uchar bufferID)  //����ָ�Ʒ���ָ��ID��   sum��pagenum>255����ʹ����@@@
{
   uchar i,ID1,ID2;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X08;
   while(TI==0);
   TI=0;
        
   SBUF=0X1b;
   while(TI==0);
   TI=0;
        
   SBUF=bufferID;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=180;
   while(TI==0);
   TI=0;
        
   summaf=9+0x1b+bufferID+180;
   sum[0]=summaf;
   sum[1]=summaf>>8;
   SBUF=sum[1];
   while(TI==0);
   TI=0;
   SBUF=sum[0];
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
	        
   while(RI==0);
   RI=0;
   querenma=SBUF;
        
   while(RI==0);
   RI=0;
   ID1=SBUF;
   while(RI==0);
   RI=0;
   ID2=SBUF;				  //���յ���ID��
   while(RI==0);
   RI=0;
   while(RI==0);
   RI=0;
        
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
   //PageID=ID1;
   PageID=(ID1<<8)+ID2;
}
        
void SFG_enroll()				 //�Զ�ע��ģ�巵�ش洢ID  =¼ͼ��+�ϲ�����ģ��+����ģ��
{
   uchar i,ID1,ID2;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X03;
   while(TI==0);
   TI=0;
        
   SBUF=0X10;
   while(TI==0);
   TI=0;
           
   SBUF=0X00;
   while(TI==0);
   TI=0;
   summaf=0x14;
   SBUF=summaf;	   //У���
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   ID1=SBUF;
   while(RI==0);
   RI=0;
   ID2=SBUF;
        
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
   //PageID=ID1;
   PageID=(ID1<<8)+ID2;
}
void SFG_deletchar(uint pageID)   //ɾ��ָ��		     У�����2�ֽڵ�ҳ�봦Ӧ�ָߵ��ֽ����
{
   uchar i,ID1,ID2;
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X07;
   while(TI==0);
   TI=0;
        
   SBUF=0X0c;
   while(TI==0);
   TI=0;
        
   ID1=pageID;ID2=pageID>>8;
   SBUF=ID2;
   while(TI==0);
   TI=0;
   SBUF=ID1;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=1;
   while(TI==0);
   TI=0;
        
   summaf=0x15+ID1+ID2;
   sum[0]=summaf;
   sum[1]=summaf>>8;
   SBUF=sum[1];
   while(TI==0);
   TI=0;
   SBUF=sum[0];
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
}
    
void SFG_identify()				//�Զ���ָ֤��     ¼ͼ��+��������+����
{
   uchar i,ID1,ID2; 
   SBUF=0xef;
   while(TI==0);
   TI=0;
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
   SBUF=0XFF;
   while(TI==0);
   TI=0;
        
   SBUF=0X01;
   while(TI==0);
   TI=0;
        
   SBUF=0X00;
   while(TI==0);
   TI=0;
   SBUF=0X03;
   while(TI==0);
   TI=0;
        
   SBUF=0X11;
   while(TI==0);
   TI=0;
           
   SBUF=0X00;
   while(TI==0);
   TI=0;
   summaf=0x15;
   SBUF=summaf;	   //У���
   while(TI==0);
   TI=0;
        
   for(i=0;i<9;i++)
   {
    while(RI==0);
    RI=0;}
   while(RI==0);
   RI=0;
   querenma=SBUF;
   while(RI==0);
   RI=0;
   ID1=SBUF;
   while(RI==0);
   RI=0;
   ID2=SBUF;
        
   while(RI==0);
   RI=0;
   while(RI==0);
   RI=0;			  //�÷�
        
   while(RI==0);
   RI=0;
   sum[1]=SBUF;
   while(RI==0);
   RI=0;
   sum[0]=SBUF;
   summas=(sum[1]<<8)+sum[0];									 
   //PageID=ID1;
   PageID=(ID1<<8)+ID2;
}