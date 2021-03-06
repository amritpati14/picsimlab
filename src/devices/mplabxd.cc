/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2015  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */


#include"../picsimlab1.h"

//system headers dependent
#ifndef _WIN_
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define        min(a,b)        ((a) < (b) ? (a) : (b))
#include <sys/unistd.h>
#include <sys/fcntl.h>
#else
#include<winsock.h>
WORD wVersionRequested = 2;
WSADATA wsaData;
#ifndef MSG_WAITALL
#define MSG_WAITALL (1 << 3)
#endif
#endif
//system headers independent
#include<errno.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
//defines
#define        TCP_SERV_PORT        1234

typedef struct sockaddr sockaddr;



#define STARTD 0xF0
#define STOPD  0xFF


#define STEP  0x01
#define RESET 0x05
#define RUN   0x10
#define HALT  0x15
#define GETPC 0x20
#define SETPC 0x25
#define SETBK 0x30
#define STRUN 0x31
#define GETID 0x35
#define PROGD 0x40
#define PROGP 0x45
#define PROGC 0x50
#define PROGI 0x55
#define PROGE 0x57
#define READD 0x60
#define READDV 0x61
#define READP 0x65
#define READC 0x70
#define READI 0x75
#define READE 0x80



int sockfd=-1;
int listenfd=-1;



void setnblock(int sock_descriptor)
{
#ifndef _WIN_
  int flags; 
  /* Set socket to non-blocking */ 

  if ((flags = fcntl(sock_descriptor, F_GETFL, 0)) < 0) 
  { 
    /* Handle error */ 
    //printf("Error fcntl nblock !!!!!!!\n");  
  } 


  if (fcntl(sock_descriptor, F_SETFL, flags | O_NONBLOCK) < 0) 
  { 
    /* Handle error */ 
    //printf("Error fcntl nblock !!!!!!!\n");  
  }
#else
	unsigned long iMode=1;
	ioctlsocket(sock_descriptor,FIONBIO,&iMode);
#endif 
}



void setblock(int sock_descriptor)
{ 
#ifndef _WIN_
  int flags; 
  /* Set socket to blocking */ 

  if ((flags = fcntl(sock_descriptor, F_GETFL, 0)) < 0) 
  { 
    /* Handle error */ 
    //printf("Error fcntl block !!!!!!!\n");  
  } 


  if (fcntl(sock_descriptor, F_SETFL, flags & (~O_NONBLOCK)) < 0) 
  { 
    /* Handle error */ 
    //printf("Error fcntl block !!!!!!!\n");  
  }
#else
   unsigned long iMode=0;
  ioctlsocket(sock_descriptor,FIONBIO,&iMode);
#endif
}

int server_started=0;

int mplabxd_init(void)
{
  struct sockaddr_in serv;

if(!server_started)
{
#ifdef _WIN_
  WSAStartup (wVersionRequested, &wsaData);
  if (wsaData.wVersion != wVersionRequested)
    {
      fprintf (stderr, "\n Wrong version\n");
      return 1;
    }

#endif

  if ((listenfd = socket (PF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf ("socket error : %s \n", strerror (errno));
      return 1;
    };
    
    int reuse = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

  memset (&serv, 0, sizeof (serv));
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = htonl (INADDR_ANY);
  serv.sin_port = htons (TCP_SERV_PORT);

  if (bind (listenfd, (sockaddr *) & serv, sizeof (serv)) < 0)
    {
      printf ("bind error : %s \n", strerror (errno));
      return 1;
    };

  if (listen (listenfd, SOMAXCONN) < 0)
    {
      printf ("listen error : %s \n", strerror (errno));
      return 1;
    };
    server_started=1;
}
    return 0;
}     
 

int  mplabxd_start(void)
{
    struct sockaddr_in cli;
#ifndef _WIN_
    unsigned int clilen;
#else
    int clilen;
#endif
    clilen = sizeof (cli);

    setnblock(listenfd);

    if (
          (sockfd =
           accept (listenfd, (sockaddr *) & cli,  & clilen)) < 0)
        {
          return 1;
        }


    setnblock(sockfd);
#ifdef _DEBUG_
    printf("Debug connected!---------------------------------\n"); 
#endif
   return 0;
}


void mplabxd_stop(void)
{
#ifdef _DEBUG_
      printf("Debug disconnected!---------------------------------\n"); 
#endif
      close (sockfd);
      sockfd=-1; 
}


void mplabxd_end(void)
{
    close (listenfd);
    listenfd=-1;
    server_started=0;
#ifdef _WIN_
  WSACleanup ();
#endif
};


int bpc=0;
unsigned short bp[100];


int mplabxd_testbp(void)
{
int i; 
      if(!Window1.Get_picdbg())
      for(i=0;i<bpc;i++)
      {
        if((pic->pc == bp[i])&&(pic->s2 == 0))
        {
#ifdef _DEBUG_
          printf("breakpoint 0x%04X!!!!!=========================\n",bp[i]);
#endif
          Window1.SetCpuState(CPU_BREAKPOINT);
          Window1.Set_picdbg(1);
          return Window1.Get_picdbg();
        } 
      }
  
      return Window1.Get_picdbg();
}


int mplabxd_loop(void)
{

   int i;

   int n;
   int ret=0;
   

   unsigned char cmd, reply;
   
     //open connection  
     if(sockfd < 0)
        if(mplabxd_start() )return 1;

      if ((n = recv (sockfd, (char *)&cmd, 1, 0)) < 0)
        {
          //printf ("receive error : %s \n", strerror (errno));
          //exit (1);
        };


      if(n == 1)
      {
      setblock(sockfd);

        
#ifdef _DEBUG_
      printf("cmd %02X  ",cmd);
      fflush(stdout);  
#endif     
      reply=0x00; 

       switch(cmd)
       {
         case STARTD:
#ifdef _DEBUG_
	   printf("STARTD cmd ----------------------\n");
#endif
           Window1.Set_picdbg(1);  	
           break;
         case STOPD:
#ifdef _DEBUG_
	   printf("STOPD cmd -----------------------\n");
#endif
           ret=1;
           Window1.Set_picdbg(0); 	
           Window1.SetCpuState(CPU_RUNNING);
           bpc=0;
           Window1.SetJUMPSTEPS(DEFAULTJS);
           break;
         case STEP:
#ifdef _DEBUG_
	   printf("STEP cmd\n");
#endif
           pic_step();
           if(pic->s2 == 1)pic_step();
           Window1.SetCpuState(CPU_STEPPING);
           break;
         case RESET:
           Window1.Set_picdbg(1);
#ifdef _DEBUG_
	   printf("RESET cmd\n");
#endif
           if(pic->s2 == 1)pic_step();
           pic_reset(1);
           pic_step();
           if(pic->s2 == 1)pic_step();
           break;
         case RUN:
           Window1.Set_picdbg(0);
           Window1.SetJUMPSTEPS(DEFAULTJS);
#ifdef _DEBUG_
	   printf("RUN cmd\n");	
#endif
           pic_step();//to go out break point
           Window1.SetCpuState(CPU_RUNNING);
           break;
         case HALT:
           Window1.Set_picdbg(1);
           if(pic->s2 == 1)pic_step();
           Window1.SetJUMPSTEPS(1);
#ifdef _DEBUG_
	   printf("HALT cmd\n");	
#endif
           Window1.SetCpuState(CPU_HALTED);
           break;
         case GETPC:
#ifdef _DEBUG_
	   printf("GETPC %04Xcmd\n",pic->pc);	
#endif
           if (send (sockfd,(char *)&pic->pc, 2, 0) != 2)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
           break;
         case SETPC:
           if ((n = recv (sockfd, (char *)&pic->pc, 2, MSG_WAITALL)) !=  2)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("SETPC cmd\n");	
#endif
           break;
         case SETBK:
           if ((n = recv (sockfd, (char *)&bpc, 2,MSG_WAITALL )) != 2)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
           printf("bp count =%i\n",bpc);  
#endif
           if(bpc >=100)bpc=100;
           if(bpc > 0)
           {
             if ((n = recv (sockfd, (char *)&bp, bpc*2, MSG_WAITALL)) !=  bpc*2)
             {
               printf ("receive error : %s \n", strerror (errno));
               ret=1;
               reply=0x01; 
             };
#ifdef _DEBUG_
             for(i=0;i<bpc;i++)
               printf("bp %i = %#06X\n",i,bp[i]);
#endif
           }
#ifdef _DEBUG_
	   printf("SETBK cmd\n");	
#endif
           break;
         case STRUN:
           i=Window1.Get_picdbg();  
           if (send (sockfd, (char *)&i, 1, 0) != 1)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("STRUN cmd =%i\n",Window1.Get_picdbg());	
#endif
           break;
         case GETID:
#ifdef _DEBUG_
	   printf("GETID cmd\n");	
#endif
           if (send (sockfd, (char *)&pic->processor, 2, 0) != 2)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
           break;
         case PROGD:
           if ((n = recv (sockfd, (char *) pic->ram, pic->RAMSIZE,MSG_WAITALL )) !=  (int)pic->RAMSIZE)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("PROGD cmd\n");	
#endif
           break;
         case PROGP:
           if ((n = recv (sockfd, (char *)pic->prog, 2*pic->ROMSIZE,MSG_WAITALL )) != 2*(int)pic->ROMSIZE)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
           for(i=0;i< (int)pic->ROMSIZE;i++)printf("%#02X ",pic->prog[i]);
	   printf("PROGP cmd  %i of %i\n",n,2*pic->ROMSIZE);	
#endif
           break;
         case PROGC:
           if ((n = recv (sockfd, (char *)pic->config, 2*pic->CONFIGSIZE,MSG_WAITALL )) !=  2*(int)pic->CONFIGSIZE)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
           for(i=0;i< (int)pic->CONFIGSIZE;i++)printf("%#02X ",pic->config[i]);
	   printf("PROGC cmd  %i of %i\n",n,2*pic->CONFIGSIZE);	
#endif
           break;
         case PROGI:
           if ((n = recv (sockfd, (char *)pic->id, 2*pic->IDSIZE,MSG_WAITALL )) !=  2*(int)pic->IDSIZE)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };   
#ifdef _DEBUG_
           for(i=0;i< (int)pic->IDSIZE;i++)printf("%#02X ",pic->id[i]);
	   printf("PROGI cmd\n");	
#endif
           break;
         case PROGE:
           if ((n = recv (sockfd, (char *)pic->eeprom, pic->EEPROMSIZE,MSG_WAITALL )) !=  (int)pic->EEPROMSIZE)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };   
#ifdef _DEBUG_
           for(i=0;i< (int)pic->EEPROMSIZE;i++)printf("%#02X ",pic->eeprom[i]);
	   printf("PROGE cmd\n");	
#endif
           break;           
         case READD:
           if (send (sockfd, (char *)pic->ram, pic->RAMSIZE, 0) != (int)pic->RAMSIZE)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("READD cmd\n");	
#endif
           break;
         case READDV:
           if ((n = recv (sockfd, (char *)&bp, 4,MSG_WAITALL )) != 4)
           {
             printf ("receive error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
           printf("address=%02X  values=%i \n",bp[0],bp[1]);  
#endif
           if (send (sockfd, (char *)&pic->ram[bp[0]], bp[1], 0) != bp[1])
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("READDV cmd\n");	
#endif
           break;
         case READP:
           if (send (sockfd, (char *)pic->prog, 2*pic->ROMSIZE, 0) !=  2*(int)pic->ROMSIZE)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };
#ifdef _DEBUG_
	   printf("READP cmd\n");	
#endif
           break;
         case READC:  
           if (send (sockfd, (char *)pic->config, 2*pic->CONFIGSIZE, 0) !=  2*(int)pic->CONFIGSIZE)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };   
#ifdef _DEBUG_
	   printf("READC cmd\n");	
#endif
           break;
         case READI: 
           if (send (sockfd, (char *)pic->id, 2*pic->IDSIZE, 0) !=  2*(int)pic->IDSIZE)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };   
#ifdef _DEBUG_
	   printf("READI cmd\n");	
#endif
           break;
         case READE: 
           if (send (sockfd, (char *)pic->eeprom, pic->EEPROMSIZE, 0) !=  (int)pic->EEPROMSIZE)
           { 
             printf ("send error : %s \n", strerror (errno));
             ret=1;
             reply=0x01; 
           };   
#ifdef _DEBUG_
	   printf("READI cmd\n");	
#endif
           break;           
         default:
#ifdef _DEBUG_
	   printf("UNKNOWN cmd !!!!!!!!!!!!!\n");	
#endif
           break;
      }

         
        if (send (sockfd, (char *)&reply, 1, 0) != 1)
        {
          printf ("send error : %s \n", strerror (errno));
          ret=1;
        };

        setnblock(sockfd);
      }
             
      //close connection
      if(ret)mplabxd_stop();

      return ret;
}

