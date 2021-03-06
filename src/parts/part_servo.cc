/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2017  Luis Claudio Gambôa Lopes

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
#include"../picsimlab4.h"
#include"part_servo.h"

/* outputs */
enum {O_P1,O_AXIS};



cpart_servo::cpart_servo(unsigned x, unsigned y)
{
   X=x;
   Y=y;      
   input_pin=0;
   angle=0;
   angle_=0;
   in_[0]=0;
   in_[1]=0;
   time=0;
   
   ReadMaps();   
     
   wxImage image;
   image.LoadFile(Window1.GetSharePath()+wxT("parts/")+GetPictureFileName());

   Bitmap = new wxBitmap(image);
   BackGround = new wxBitmap(image);
   
   canvas.Create(Bitmap);
};

cpart_servo::~cpart_servo(void)
{
    delete Bitmap;
    delete BackGround;
}

 


void cpart_servo::Draw(void)
{ 
   board *pboard=Window1.GetBoard();
  
   canvas.SetBitmap (BackGround,1.0,1.0);
  
  int i;
  
  
  if(angle > angle_)
  {
      angle-=0.2;
      if(angle < angle_) angle = angle_;
  }
  
  if(angle < angle_)
  {
      angle+=0.2;
      if(angle > angle_) angle = angle_;
  }

  canvas.Init();
  
  wxFont font(9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD );
  canvas.SetFont(font);
  
  for(i=0;i<outputc;i++)
  {

      if(output[i].id == O_P1)
        {
         canvas.SetFgColor (255, 255, 255);
         if(input_pin == 0)
            canvas.Text ("NC",output[i].x1,output[i].y1);
         else
            canvas.Text (pboard->MGetPinName (input_pin),output[i].x1,output[i].y1);  
        }

      if(output[i].id == O_AXIS)
        {
         canvas.SetFgColor (255, 255, 255);
         canvas.SetLineWidth (20);
         canvas.Line (output[i].x1,output[i].y1,output[i].x1+output[i].r*sin(angle),output[i].y1-output[i].r*cos(angle));
        }

      
  };

  canvas.End();
    
}

 
void cpart_servo::Process(void)
{

   const picpin * ppins=Window1.GetBoard()->MGetPinsValues();

   in_[1]=in_[0];
   in_[0]=ppins[input_pin-1].value;
   
   if((in_[0]==1)&&(in_[1]==0))//rise
   {
       time=0;
   }
  
   if((in_[0]==0)&&(in_[1]==1))//low
   {
     angle_=((time/Window1.GetBoard()->MGetInstClock())-0.0015)*3141.59265359;
    
     //printf("time=%f  angle=%f\n",time/Window1.GetBoard()->MGetInstClock(),angle*180.0/M_PI);
   
     if(angle_ > M_PI/2.0)angle_ =M_PI/2.0;
     if(angle_ < -M_PI/2.0)angle_ =-M_PI/2.0;
     
   }
   
   time++;
}
 
unsigned short 
cpart_servo::get_in_id(char * name)
{
  printf("Erro input '%s' don't have a valid id! \n",name);
  return -1;
};

unsigned short 
cpart_servo::get_out_id(char * name)
{

  if(strcmp(name,"P1")==0)return O_P1;
  if(strcmp(name,"AXIS")==0)return O_AXIS;

  printf("Erro output '%s' don't have a valid id! \n",name);
  return 1;
};


String
cpart_servo::WritePreferences(void)
{
  char prefs[256];
  
  sprintf(prefs,"%hhu",input_pin);
  
  return prefs;
};

void 
cpart_servo::ReadPreferences(String value)
{
    sscanf(value.c_str (),"%hhu",&input_pin);      
};

CPWindow * WProp_servo;

void 
cpart_servo::ConfigurePropertiesWindow(CPWindow *  wprop)
{
    String Items="0  NC,";
    String spin;
    WProp_servo=wprop;
    board *pboard=Window1.GetBoard();
    
    for(int i=1; i<= pboard->MGetPinCount();i++ )
    {
       spin= pboard->MGetPinName(i);
      
      if(spin.Cmp(wxT("error")))
      {
        Items=Items+itoa(i)+"  "+spin+",";
      }
    }
    
    ((CCombo*)WProp_servo->GetChildByName("combo1"))->SetItems(Items);
    if(input_pin == 0)
        ((CCombo*)WProp_servo->GetChildByName("combo1"))->SetText("0  NC");
    else
    {
        spin= pboard->MGetPinName(input_pin);
        ((CCombo*)WProp_servo->GetChildByName("combo1"))->SetText(itoa(input_pin)+"  "+spin);
    }
    
  
    ((CButton*)WProp_servo->GetChildByName("button1"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & cpart_servo::PropButton;
    ((CButton*)WProp_servo->GetChildByName("button1"))->SetTag(1);
    
    ((CButton*)WProp_servo->GetChildByName("button2"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & cpart_servo::PropButton;
}

void 
cpart_servo::ReadPropertiesWindow(void)
{
   if(WProp_servo->GetTag())
   {
      input_pin=atoi(((CCombo*)WProp_servo->GetChildByName("combo1"))->GetText());
   }
} 

void
cpart_servo::PropButton (CControl * control, uint button, uint x, uint y, uint state)
{
  WProp_servo->SetTag(control->GetTag());   
  WProp_servo->HideExclusive ();
};
