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

#ifndef BOARD_4_H
#define	BOARD_4_H

#include "board_pic.h"


class cboard_4:public board_pic
{
  private:
     int p_BT1; 
     int p_BT2;  
     int p_BT3; 
     int p_BT4; 
     int p_BT5; 
     int p_BT6; 
     int p_BT7;  
     
     int p_KEY1; 
     int p_KEY2; 
     int p_KEY3; 
     int p_KEY4; 
     int p_KEY5; 
     int p_KEY6; 
     int p_KEY7; 
     int p_KEY8; 
     int p_KEY9; 
     int p_KEY10; 
     int p_KEY11; 
     int p_KEY12; 
     
     int vtc;
     int vt;
     
     lcd_t lcd;

     mi2c_t mi2c;
     rtc2_t rtc2;

     int lcde;

     int sound_on;
     wxSound buzz;

     float vp1in;
     float vp2in;
     float vp2[2];
     float temp[2];
     float ref;

     int rpmstp;
     int rpmc;

    unsigned char d;
    unsigned char sda,sck;

    int jmp[1];
    int dip[20];

    unsigned int lm1[40]; //luminosidade media display
    unsigned int lm2[40]; //luminosidade media display
    unsigned int lm3[40]; //luminosidade media display
    unsigned int lm4[40]; //luminosidade media display
    wxBitmap * vent[2];
    wxBitmap * lcdbmp[2];
    
    CGauge *gauge1;
    CGauge *gauge2;
    CScroll *scroll1;
    CScroll *scroll2;
    CLabel *label1;
    CLabel *label2;
    CLabel *label3;
    CLabel *label4;
    CLabel *label5;
    CLabel *label6;
    CCombo *combo1; 
  public:
      cboard_4(void);
      ~cboard_4(void);
      void Draw(CDraw *draw,double scale);
      void Run_CPU(void);
#ifdef _EXPERIMENTAL_
      String GetSupportedDevices(void){return wxT("PIC16F777,PIC16F877A,PIC18F452,PIC18F4520,PIC18F4550,PIC18F45K50,PIC18F4620,");};
#else
      String GetSupportedDevices(void){return wxT("PIC16F777,PIC16F877A,PIC18F452,PIC18F4520,PIC18F4550,PIC18F4620,");};
#endif
      String GetPictureFileName(void){return wxT("picsimlab4.png");};
      String GetInputMapFile(void){return wxT("input4.map");};
      String GetOutputMapFile(void){return wxT("output4.map");};
      void Reset(void);
      void MouseButtonPress(uint button, uint x, uint y,uint state);
      void MouseButtonRelease(uint button, uint x, uint y,uint state);
      void KeyPress(uint key, uint x, uint y,uint mask);
      void KeyRelease(uint key, uint x, uint y,uint mask);
      void OnShow(void);
      void RefreshStatus(void);
      void WritePreferences(void);
      void ReadPreferences(char *name,char *value);
      unsigned short get_in_id(char * name);
      unsigned short get_out_id(char * name);
      
      void board_Event(CControl * control);
};


#endif	/* BOARD_4_H */

