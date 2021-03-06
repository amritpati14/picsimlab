/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2015  Luis Claudio Gambôa Lopes

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

//include files
#include"../picsimlab1.h"
#include"../picsimlab4.h" //Oscilloscope
#include"../picsimlab5.h" //Spare Parts
#include"board_x.h"

/* ids of inputs of input map*/
#define I_ICSP	1  //ICSP connector
#define I_PWR	2  //Power button
#define I_RST	3  //Reset button
#define I_D0	4  //RD0 push button
#define I_D1	5  //RD1 switch

/* ids of outputs of output map*/
#define O_SD1	1  //switch position (On/Off)
#define O_LD0	2  //LED on RD0 push button
#define O_LD1	3  //LED on RD1 switch
#define O_LPWR	4  //Power LED
#define O_RB0	5  //LED on RB0 output
#define O_RB1	6  //LED on RB1 output

//return the input ids numbers of names used in input map
unsigned short 
cboard_x::get_in_id(char * name)
{
  if(strcmp(name,"I_ICSP")==0)return I_ICSP;
  if(strcmp(name,"I_PWR")==0)return I_PWR;  
  if(strcmp(name,"I_RST")==0)return I_RST; 
  if(strcmp(name,"I_D0")==0)return I_D0;
  if(strcmp(name,"I_D1")==0)return I_D1;
  
  printf("Erro input '%s' don't have a valid id! \n",name);
  return -1;
};

//return the output ids numbers of names used in output map
unsigned short 
cboard_x::get_out_id(char * name)
{ 
    
  if(strcmp(name,"O_SD1")==0)return O_SD1;
  if(strcmp(name,"O_LD0")==0)return O_LD0;
  if(strcmp(name,"O_LD1")==0)return O_LD1;
  if(strcmp(name,"O_LPWR")==0)return O_LPWR;
  if(strcmp(name,"O_RB1")==0)return O_RB1;
  if(strcmp(name,"O_RB0")==0)return O_RB0;
  
  printf("Erro output '%s' don't have a valid id! \n",name);
  return 1;
};

//Constructor called once on board creation 
cboard_x::cboard_x(void)
{
  proc="PIC18F4550";  //default microcontroller if none defined in preferences
  ReadMaps();     //Read input and output board maps

  //controls properties and creation
  //scroll1
  scroll1=new CScroll();   
  scroll1->SetFOwner(&Window1);
  scroll1->SetName(wxT("scroll1_px"));
  scroll1->SetX(12);
  scroll1->SetY(273-160);
  scroll1->SetWidth(140);
  scroll1->SetHeight(22);
  scroll1->SetEnable(1);
  scroll1->SetVisible(1);
  scroll1->SetRange(100);
  scroll1->SetPosition(50);
  scroll1->SetType(4);
  Window1.CreateChild(scroll1);
  //gauge1
  gauge1=new CGauge();
  gauge1->SetFOwner(&Window1);
  gauge1->SetName(wxT("gauge1_px"));
  gauge1->SetX(13);
  gauge1->SetY(382-160);
  gauge1->SetWidth(140);
  gauge1->SetHeight(20);
  gauge1->SetEnable(1);
  gauge1->SetVisible(1);
  gauge1->SetRange(100);
  gauge1->SetValue(0);
  gauge1->SetType(4);
  Window1.CreateChild(gauge1);
  //gauge2
  gauge2=new CGauge();
  gauge2->SetFOwner(&Window1);
  gauge2->SetName(wxT("gauge2_px"));
  gauge2->SetX(12);
  gauge2->SetY(330-160);
  gauge2->SetWidth(140);
  gauge2->SetHeight(20);
  gauge2->SetEnable(1);
  gauge2->SetVisible(1);
  gauge2->SetRange(100);
  gauge2->SetValue(0);
  gauge2->SetType(4);
  Window1.CreateChild(gauge2);   
  //label1
  label1=new CLabel();
  label1->SetFOwner(&Window1);
  label1->SetName(wxT("label1_px"));
  label1->SetX(12);
  label1->SetY(249-160);
  label1->SetWidth(60);
  label1->SetHeight(20);
  label1->SetEnable(1);
  label1->SetVisible(1);
  label1->SetText(wxT("AN0"));
  label1->SetAlign(1);
  Window1.CreateChild(label1); 
  //label2
  label2=new CLabel();
  label2->SetFOwner(&Window1);
  label2->SetName(wxT("label2_px"));
  label2->SetX(12);
  label2->SetY(306-160);
  label2->SetWidth(60);
  label2->SetHeight(20);
  label2->SetEnable(1);
  label2->SetVisible(1);
  label2->SetText(wxT("RB0"));
  label2->SetAlign(1);
  Window1.CreateChild(label2);    
  //label3
  label3=new CLabel();
  label3->SetFOwner(&Window1);
  label3->SetName(wxT("label3_px"));
  label3->SetX(13);
  label3->SetY(357-160);
  label3->SetWidth(60);
  label3->SetHeight(20);
  label3->SetEnable(1);
  label3->SetVisible(1);
  label3->SetText(wxT("RB1"));
  label3->SetAlign(1);
  Window1.CreateChild(label3);    
};

//Destructor called once on board destruction 
cboard_x::~cboard_x(void)
{
    //controls destruction 
    Window1.DestroyChild(scroll1);
    Window1.DestroyChild(gauge1);
    Window1.DestroyChild(gauge2);
    Window1.DestroyChild(label1);
    Window1.DestroyChild(label2);
    Window1.DestroyChild(label3);
}

//Reset board status
void 
cboard_x::Reset(void)
{
    pic_reset(1);
   
    p_BT1=1;//set push button  in default state (high) 
       
    //write button state to pic pin 19 (RD0)
    pic_set_pin(19,p_BT1); 
    //write switch state to pic pin 20 (RD1)
    pic_set_pin(20,p_BT2); 

        
  //verify serial port state and refresh status bar  
#ifndef _WIN_
    if(pic.serialfd > 0)
#else
    if(pic.serialfd != INVALID_HANDLE_VALUE)
#endif
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(":")+itoa(pic.serialbaud)+wxT("(")+
        String().Format("%4.1f",fabs((100.0*pic.serialexbaud-100.0*
        pic.serialbaud)/pic.serialexbaud))+wxT("%)"));
    else  
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(" (ERROR)"));
        

};

//Called ever 1s to refresh status
void 
cboard_x::RefreshStatus(void)
{
   //verify serial port state and refresh status bar   
#ifndef _WIN_
    if(pic.serialfd > 0)
#else
    if(pic.serialfd != INVALID_HANDLE_VALUE)
#endif
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(":")+itoa(pic.serialbaud)+wxT("(")+
        String().Format("%4.1f",fabs((100.0*pic.serialexbaud-100.0*
        pic.serialbaud)/pic.serialexbaud))+wxT("%)"));
    else  
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(" (ERROR)"));
    
};

//Called to save board preferences in configuration file
void 
cboard_x::WritePreferences(void)
{
    //write selected microcontroller of board_x to preferences
    Window1.saveprefs(wxT("px_proc"),proc);
    //write switch state of board_x to preferences 
    Window1.saveprefs(wxT("px_bt2"),String::Format("%i",p_BT2));
};

//Called whe configuration file load  preferences 
void 
cboard_x::ReadPreferences(char *name,char *value)
{
     //read switch state of board_x of preferences 
     if(!strcmp(name,"px_bt2"))
      {
        if(value[0] == '0')      
          p_BT2=0;
        else
          p_BT2=1; 
      }
    //read microcontroller of preferences
    if(!strcmp(name,"px_proc"))
    {
      proc=value; 
    }
};


//Event on the board
void 
cboard_x::KeyPress(uint key, uint x, uint y,uint mask)
{
  //if keyboard key 1 is pressed then activate button (state=0)   
  if(key == '1')
  {
    p_BT1=0; 
  }
  
  //if keyboard key 2 is pressed then toggle switch state   
  if(key == '2')
  {
    p_BT2^=1; 
  }
  
};

//Event on the board
void
cboard_x::KeyRelease(uint key, uint x, uint y,uint mask)
{
  //if keyboard key 1 is pressed then deactivate button (state=1)     
  if(key == '1')
  {
    p_BT1=1; 
  }  
  
};

//Event on the board
void 
cboard_x::MouseButtonPress(uint button, uint x, uint y,uint state)
{
 
  int i;
  
  //search for the input area which owner the event    
  for(i=0;i<inputc;i++)
  {
    if(((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&
       (input[i].y2 >= y)))
    {
   
      switch(input[i].id) 
      { 
        //if event is over I_ISCP area then load hex file  
        case I_ICSP:
          Window1.menu1_File_LoadHex_EvMenuActive(NULL);
          break;  
        //if event is over I_PWR area then toggle board on/off
        case I_PWR:
          if(Window1.Get_picpwr()) //if on turn off
          { 
            Window1.Set_picrun(0); 
            Window1.Set_picpwr(0); 
            Reset();
            p_BT1=1; 
            Window1.statusbar1.SetField(0,wxT("Stoped"));
          }
          else //if off turn on
          {
            Window1.Set_picpwr(1);
            Window1.Set_picrun(1);
            Reset();
            Window1.statusbar1.SetField(0,wxT("Running..."));
          } 
          break;
        //if event is over I_RST area then turn off and reset
        case I_RST:
          if(Window1.Get_picpwr())//if powered
          {
            Window1.Set_picpwr(0);
            Window1.Set_picrst(1);
          } 
          break;
        //if event is over I_D0 area then activate button (state=0) 
        case I_D0:
          p_BT1=0;
          break;
        //if event is over I_D1 area then toggle switch state   
        case I_D1:
          p_BT2^=1; 
          break;
      }
    }
  }
    
};

//Event on the board
void 
cboard_x::MouseButtonRelease(uint button, uint x, uint y,uint state)
{
  int i;

  //search for the input area which owner the event  
  for(i=0;i<inputc;i++)
  {
    if(((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&
       (input[i].y2 >= y)))
    {
      switch(input[i].id)
      {
        //if event is over I_RST area then turn on  
        case I_RST:
          if(Window1.Get_picrst())//if powered
          {
            Window1.Set_picpwr(1);
            Window1.Set_picrst(0);

            if(pic_reset(-1))
            {  
              Reset();
            }
          } 
          break;
        //if event is over I_D0 area then deactivate button (state=1) 
        case I_D0:
          p_BT1=1; 
          break;
      }
    } 
  }

};


//Called ever 100ms to draw board
//This is the critical code for simulator running speed
void cboard_x::Draw(CDraw *draw,double scale)
{
  int i;
  
  draw->Canvas.Init(scale,scale); //initialize draw context
  
//board_x draw 
  for(i=0;i<outputc;i++) //run over all outputs
  {
    if(!output[i].r)//if output shape is a rectangle
    {
      if(output[i].id == O_SD1)//if output is switch
      {
       //draw a background white rectangle   
       draw->Canvas.SetBgColor (255, 255, 255);   
       draw->Canvas.Rectangle (1, output[i].x1, output[i].y1, 
          output[i].x2-output[i].x1,output[i].y2-output[i].y1 );
   
       if(!p_BT2) //draw switch off
       {
         //draw a grey rectangle  
         draw->Canvas.SetBgColor (70, 70, 70);
         draw->Canvas.Rectangle (1, output[i].x1,output[i].y1+
         ((int)((output[i].y2-output[i].y1)*0.35)),output[i].x2-output[i].x1 , 
         (int)((output[i].y2-output[i].y1)*0.65) );
       }
       else //draw switch on
       {
         //draw a grey rectangle  
         draw->Canvas.SetBgColor (70, 70, 70);
         draw->Canvas.Rectangle (1, output[i].x1, 
          output[i].y1,output[i].x2-output[i].x1 , 
          (int)((output[i].y2-output[i].y1)*0.65));
       };
      }
    }
    else //if output shape is a circle
    {
      
        draw->Canvas.SetFgColor (0, 0, 0);//black
 
        switch(output[i].id)//search for color of output
        {
          case O_LD0: //White using pin 19 mean value (RD0)
            draw->Canvas.SetColor (pic.pins[18].oavalue, pic.pins[18].oavalue, pic.pins[18].oavalue);
            break;
          case O_LD1: //Yelllow using pin 20 mean value (RD1)
            draw->Canvas.SetColor (pic.pins[19].oavalue, pic.pins[19].oavalue, 0);
            break;
          case O_LPWR: //Blue using picpwr value
            draw->Canvas.SetColor(0,0,225*Window1.Get_picpwr()+30);
            break;
          case O_RB0: //Green using pin 33 mean value (RB0)
            draw->Canvas.SetColor (0, pic.pins[32].oavalue, 0);
            break;
          case O_RB1: //Red using pin 34 mean value (RB1)
            draw->Canvas.SetColor (pic.pins[33].oavalue,0 , 0);
            break;
        }
        
        //draw a circle
        draw->Canvas.Circle (1,output[i].x1, output[i].y1,output[i].r );
    };    

  };

  //end draw
  draw->Canvas.End();
  draw->Update ();
  
  
 
     //RB0 mean value to gauge1
     gauge1->SetValue(0.4444*(pic.pins[33].oavalue-30)); 
     //RB1 mean value to gauge2
     gauge2->SetValue(0.44444*(pic.pins[32].oavalue-30)); 
   

};


void cboard_x::Run_CPU(void)
{
  int i;
  int j;
  unsigned char pi;
  const picpin * pins;
  unsigned int alm[40];
       
  int JUMPSTEPS = Window1.GetJUMPSTEPS(); //number of steps skipped
  long int NSTEPJ=Window1.GetNSTEPJ();  //number of steps in 100ms

  
  //reset mean value
  for(pi=0;pi < pic.PINCOUNT;pi++)
  {
    alm[pi]=0;
  };

 //read pic.pins to a local variable to speed up 
 pins = pic.pins;

  
 j=JUMPSTEPS+1;//step counter
 if(Window1.Get_picpwr()) //if powered
   for(i=0;i<Window1.GetNSTEP();i++) //repeat for number of steps in 100ms
      {
 
        if(j > JUMPSTEPS)//if number of step is bigger than steps to skip 
        {  
          pic_set_pin(19,p_BT1);//Set pin 19 (RD0) with button state 
          pic_set_pin(20,p_BT2);//Set pin 20 (RD1) with switch state 
        } 
        
        //verify if a breakpoint is reached if not run one instruction 
        if(!mplabxd_testbp())pic_step();
        if(use_oscope)Window4.SetSample();
        if(use_spare)Window5.Process();
        
        if(j > JUMPSTEPS)//if number of step is bigger than steps to skip 
        {  
          //increment mean value counter if pin is high  
          for(pi=0;pi < pic.PINCOUNT;pi++)
          {
           alm[pi]+=pins[pi].value;
          }
        
          //set analog pin 2 (AN0) with value from scroll  
          pic_set_apin(2,((5.0*(scroll1->GetPosition()))/
            (scroll1->GetRange()-1)));
          
          j=0;//reset counter
        } 
        j++;//counter increment
     }

     //calculate mean value
     for(pi=0;pi < pic.PINCOUNT;pi++)
     { 
       pic.pins[pi].oavalue= (int)(((225.0*alm[pi])/NSTEPJ)+30);
     }
       
}
