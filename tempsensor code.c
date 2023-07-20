#include<reg51.h>
#include<intrins.h>
sbit sda=P2^1;
sbit scl=P2^0;
sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;
sbit a0=B^0;
sbit a1=B^1;
sbit a2=B^2;
sbit a3=B^3;
sbit a4=B^4;
sbit a5=B^5;
sbit a6=B^6;
sbit a7=B^7;
void start();
void stop();
void check();
void delay();
void conversion(unsigned char);
void write(unsigned char);
unsigned char read();
void ack();
void disp(unsigned char *);
void enable();
unsigned char v;
void main()
{
	unsigned char com[4]={0x38,0x10,0x0c,0x80},s1;
	for(s1=0;s1<4;++s1)
	{
		P3=com[s1];
		rs=0;
		rw=0;
		enable();
	}
	disp("TEMPERATURE:");
	start(); // start i2c communication.
	write(0x90); // slave adress in write mode //PCF8591 I2C Address's
//The lower three bits of the address consist of the three digital inputs A2, A1, A0 while the upper bits are fixed at 1001xxx. The the last bit( LSB ' 'L) is ignored as it is the read write bit (R/Wn). Therefore the addresses available are:


	check(); // check acknowledgement
	write(0x00); // 00-we are using channel 0
	check();
	start();
	write(0x91); // read mode
	check();
	while(1)
	{
		v=read();
		ack();
		conversion(v); //to ascii
	}
	stop();}

	void start()
	{
		scl=1;
		sda=1; // making it high to low
		_nop_();
		_nop_();// no operation, provide 1 microsecond timedelay.
		sda=0;
		scl=0;
	}
	
	void write( unsigned char value)
	{
		unsigned char v1,v2,v3=0x80;
		v2=value;
		for(v1=0;v1<8;++v1)
		{
			sda=v2&v3; // here we get the msb
			scl=1;
			_nop_();
			_nop_();
			scl=0;
			v2=v2<<1;
		}}
	void check()
	{
		bit c;
		scl=1;
		c=sda; // c will become zero if there is acknowledgement.
		for(v=0;v<12;v++);
		scl=0;
		if(c==1)
		{stop();
		}}
	void stop()
	{
		scl=1;
		sda=0;
		_nop_();
		_nop_();
		sda=1;
		scl=0;
	}
	void enable()
	{
		unsigned int s2;
		en=1;
		for(s2=0;s2<2000;s2++);
		en=0;
	}
	void disp(unsigned char *s)
	{
		unsigned char s3;
		for(s3=0;s[s3]!='\0';s3++)
		{
			P3=s[s3];
			rs=1;
			enable();
		}
	}
	unsigned char read()
	{
		sda=1; //In I2C communication from the master to the slave, each transferred byte consists 8 data bits sent by the master and 1 acknowledge bit sent by the slave. So after bit 8, the master releases the SDA line so the slave can take control of it. here sda wants 8 bit to store data hence this is made 1 that is 0000 0001 last 1 is for notacknowledged. now values are stored in the zero space.
		a7=sda;    
		scl=1;
		_nop_();_nop_();
		scl=0; // collects the value from sda line.
		a6=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a5=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a4=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a3=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a2=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a1=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		a0=sda;
		scl=1;
		_nop_();_nop_();
		scl=0;
		v=B;
		return v - 0x01;
	}
	
	void ack()
	{
		sda=0; //acknowledged from master to slave.
		scl=1;
		_nop_();_nop_(); // this gives a clock pulse.
		scl=0;}
	
		void conversion(unsigned char value)
		{ unsigned char s4,s5,s6;
			s4=value;
			s5=s4/10;
			s6=s4%10;
			s5=s5|0x30; // ascii conversion
			s6=s6|0x30;
			P3=0x8b;  //cursor position
			rs=0;
			enable();
			P3 =s5;
			rs=1;
			enable();
			P3=s6;
			rs=1;
			enable();
		}
		