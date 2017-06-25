
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <piece.h>
int errno;
unsigned char vbuff[128*88];
static unsigned char draw;

static unsigned long datatmp;
static unsigned long data0;
static unsigned char mode;
static unsigned char bitc;
static unsigned char cc;
int num=0;
int num2=0;
int num3=0;
int nu=0;
int ni=0;

#define MAXBUF 2000
#define NOPCESPRINTF
static unsigned short timebuf[MAXBUF];
static unsigned short bufip;
static unsigned short bufop;

static unsigned short numa[2000];
static long numa2[2000];


	char *tok;
	char *tok2;
		char *tok4;
	char *time3;
	char *file;
	
int data[512];
	char tokw;
	
		char*
		strtok(char* s1, const char* s2)
		{ 
		static char* ssave = ""; 
		char *sbegin, *send;

		sbegin = s1 ? s1 : ssave;
		sbegin += strspn(sbegin, s2);
		if(*sbegin == '\0') { 
		ssave = ""; 
		return NULL;
		}

		send = sbegin + strcspn(sbegin, s2);
		if(*send != '\0') *send++ = '\0';
		ssave = send;
		return sbegin;
		}
void disp_mode( void )
{
	memset( vbuff, 0, 128*88 );
	pceFontSetPos( 0, 0 );
	pceFontPrintf( "■赤外線リモコン実験" );
	pceFontSetPos( 0, 10 );
	pceFontPrintf(
		mode ?
		"送信モード" :
		"解析モード" );
	pceFontSetPos( 70, 10 );
	pceFontPrintf( "%d" ,nu);
	pceFontSetPos( 100, 10 );
	pceFontPrintf( "%d" ,num3);
	pceFontSetPos( 110, 10 );
	pceFontPrintf( "%d" ,ni);
	sprintf(file, "irdata%d.csv", num3);
	draw = 1;
}

void readfile(void)
{
		FILEACC pfb;
		char time2[4096];
		pceFileOpen(&pfb,file, FOMD_RD);
		pceFileReadSct(&pfb,time2, 0,4096);
		pceFileClose(&pfb);
		tok4=time2;
	}



int TxSub( int flag )
{
	
	int a = 0;

	a = data[num2];

num2++;

if (num2 == ni){
	pceIRStop();
		num2=0;
	}
	return a;
}



void send( void ){
int i;
readfile();
	ni=0;
for(i = 0; i < 512; i++){
	data[i] = 0;
}
	
time3 = strtok( tok4, "," );
  while( time3 != NULL){
    data[ni]=atof(time3);
    data[ni]=(data[ni]/2640)+0.5;

    time3 = strtok( NULL, "," );  

    ni++;
  }
  	disp_mode();
	pceIRStartTxPulse( 0, TxSub );
}








static int xx, yy;


void RxSub( int flag, unsigned short time )
{
	static unsigned short otime;

	if ( bufip < MAXBUF && !((flag ^ bufip) & 1) ) {
		int t = (time - otime) & 0xffff;
		timebuf[bufip++] = t / 225;
	}

	otime = time;
	
	
	if (num < 2000){
		numa[num]=time;
		num++;
	}
       		
}

void recv( void )
{
	xx = 0;
	yy = 30;
	bufip = 0;
	bufop = 0;
	num=0;
	pceIRStartRxPulse( 0, RxSub, 0 );
}






void disp_data( void )
{
	while ( bufop < bufip ) {
		
		
		int n = timebuf[bufop];
		
		
		while ( n-- ) {
			if ( yy <= 80 ) {
				vbuff[(yy-(bufop&1)*4)*128+xx] = 3;
			}
			if ( ++xx == 128 ) {
				xx = 0;
				yy += 10;
			}
		}

		
		bufop++;
	

	

	}		

	


}







void pceAppInit( void )
{
	
	tok=&tokw;
	pceCPUSetSpeed(CPU_SPEED_HALF);
	pceLCDDispStop();

	pceLCDSetBuffer( vbuff );
	pceAppSetProcPeriod( 50 );
mode = 1;
	disp_mode();
	pceLCDTrans();
	pceLCDDispStart();
}


void pceAppProc( int cnt )
{
	
	FILEACC pfa;
	
	
int a = pcePadGet();

if ( a & TRG_SELECT ) {
	if (mode == 0){
				mode = 1;
	}
	else {
		mode = 0;
	}
	pceIRStop();
	disp_mode();
}




if ( a & TRG_A ) {
		int id_length = strlen( tok );	
	if (mode == 0){
		disp_mode();
		nu=0;
		

tok[0] = '\0';
		
		
		
		
while ( nu <= num ) {
	if(nu>0){
		numa2[nu]=numa[nu]-numa[nu-1];
	}else {
		numa2[0]=numa[0];
	}
	if(numa2[nu]<0){
		numa2[nu]=65535-numa[nu-1]+numa[nu];
	}
	numa2[nu]=(numa2[nu]*0.67+0.5)*100;
			sprintf(tok2, "%d,", numa2[nu]);
			strcat(tok, tok2);
		
			nu++;
}
disp_mode();

num=0;
		
		pceFileCreate(file,4096);
		pceFileOpen(&pfa, file, FOMD_WR);
		pceFileWriteSct(&pfa,tok, 0,4096);	
		pceFileClose(&pfa);
	}
	else {
 		pceIRStop();
		}
	
	}


if ( a & TRG_B ) {
		if (mode == 0){
   recv();
   disp_mode();
	}
	else {
		send();
}
}


if ( a & TRG_RI ) {
	num3=num3+1;
	disp_mode();
}

if ( a & TRG_LF ) {
	num3=num3-1;
	disp_mode();
}

disp_data();
if ( draw || !(cnt & 15) ) {
		pceLCDTrans();
		draw = 0;
	}
}


void pceAppExit( void )
{
	pceIRStop();
	pceCPUSetSpeed(CPU_SPEED_NORMAL);
}

