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

#ifndef CPWINDOW1 
#define CPWINDOW1
   
#define TIMER           0.1      //timer period in s
#define CPUTUSEMAX      0.1      //max period cpu use in s 
#define NSTEPKT     25000.0      //TIMER constant 1MHz/(4.0*timer_freq) 
#define NSTEPKF        40.0      //Freq constant 4.0*timer_freq
#define DEFAULTJS       100      //IO refresh rate

#define MAX_MIC 20
extern char SERIALDEVICE[100];

#include <lxrad/lxrad.h>

#include "boards/board.h"
#include "boards/boards_defs.h"
#include "wx/stdpaths.h"

    
class CPWindow1:public CPWindow
{
  public:
//lxrad automatic generated block start, don't edit below!
  /*#Controls*/
  CTimer timer1;
  CTimer timer2;
  CFileDialog filedialog1;
  CDraw draw1;
  CCombo combo1;
  CLabel label1;
  CMenu menu1;
  CPMenu menu1_File;
  CPMenu menu1_Board;
  CPMenu menu1_Microcontroller;
  CPMenu menu1_Modules;
  CPMenu menu1_Tools;
  CPMenu menu1_Help;
  CItemMenu menu1_File_LoadHex;
  CItemMenu menu1_File_ReloadLast;
  CItemMenu menu1_File_Configure;
  CItemMenu menu1_File_SaveWorkspace;
  CItemMenu menu1_File_LoadWorkspace;
  CItemMenu menu1_File_Exit;
  CItemMenu menu1_Modules_Oscilloscope;
  CItemMenu menu1_Modules_Spareparts;
  CItemMenu menu1_Tools_SerialTerm;
  CItemMenu menu1_Help_Contents;
  CItemMenu menu1_Help_Examples;
  CItemMenu menu1_Help_About;
  CStatusbar statusbar1;
  CToggleButton togglebutton1;
  CFileDialog filedialog2;
  /*#Events*/
  void _EvOnCreate(CControl * control);
  void _EvOnDestroy(CControl * control);
  void _EvOnShow(CControl * control);
  void timer1_EvOnTime(CControl * control);
  void timer2_EvOnTime(CControl * control);
  void draw1_EvMouseButtonPress(CControl * control, uint button, uint x, uint y,uint state);
  void draw1_EvMouseButtonRelease(CControl * control, uint button, uint x, uint y,uint state);
  void draw1_EvKeyboardPress(CControl * control, uint key, uint x, uint y,uint mask);
  void draw1_EvKeyboardRelease(CControl * control, uint key, uint x, uint y,uint mask);
  void combo1_EvOnComboChange(CControl * control);
  void menu1_File_LoadHex_EvMenuActive(CControl * control);
  void menu1_File_ReloadLast_EvMenuActive(CControl * control);
  void menu1_File_Configure_EvMenuActive(CControl * control);
  void menu1_File_SaveWorkspace_EvMenuActive(CControl * control);
  void menu1_File_LoadWorkspace_EvMenuActive(CControl * control);
  void menu1_File_Exit_EvMenuActive(CControl * control);
  void menu1_Modules_Oscilloscope_EvMenuActive(CControl * control);
  void menu1_Modules_Spareparts_EvMenuActive(CControl * control);
  void menu1_Tools_SerialTerm_EvMenuActive(CControl * control);
  void menu1_Help_Contents_EvMenuActive(CControl * control);
  void menu1_Help_Examples_EvMenuActive(CControl * control);
  void menu1_Help_About_EvMenuActive(CControl * control);
  void togglebutton1_EvOnToggleButton(CControl * control);

  /*#Others*/
//lxrad automatic generated block end, don't edit above!
  
  CThread thread1;
  int tgo;
  
  CPWindow1(void);
  void saveprefs(String name, String value);
  void Configure(CControl * control, const char * home);
  void board_Event(CControl * control);
  void thread1_EvThreadRun(CControl * control);

  String GetSharePath(void){return share;};
  void SetplWidth(int pw){plWidth=pw;};
  void SetplHeight(int ph){plHeight=ph;};
  double GetScale(void){return scale;};
  int Get_picpwr(void){return picpwr;};
  int Get_picrun(void){return picrun;};
  int Get_picrst(void){return picrst;};
  int Get_picdbg(void){return picdbg;};
  void Set_picpwr(int pp){picpwr =pp;};
  void Set_picrun(int pr){picrun =pr;};
  void Set_picrst(int pr){picrst =pr;};
  void Set_picdbg(int pd){picdbg =pd;};
  void SetPATH(String path){PATH=path;};
  void SetFNAME(String fname){FNAME=fname;};
  long int GetNSTEP(void){return NSTEP;};
  long int GetNSTEPJ(void){return NSTEPJ;};
  int GetJUMPSTEPS(void){return JUMPSTEPS;};
  void SetJUMPSTEPS(int js){JUMPSTEPS=js;NSTEPJ=NSTEP/JUMPSTEPS;};
  board * GetBoard(void);
  void SetCpuState(unsigned char cs);
  void menu1_EvBoard(CControl * control);
  void menu1_EvMicrocontroller(CControl * control);
 private: 
  String share;
 
  int picrun;
  int picpwr;
  int picrst;
  int picdbg;
  int debug;
  int osc_on;
  int spare_on;
  char status;
  
  int plWidth;
  int plHeight;
  double scale;
  long int NSTEP;
  long int NSTEPJ;
  int JUMPSTEPS;
  String PATH;
  String FNAME;
  char cpustate;
  
  CStringList prefs;

  int lab;
  int lab_;

  String proc_;

  board *pboard;

  float over;
  int create;

  int ondraw;    
  
  CItemMenu MBoard[BOARDS_LAST];
  CItemMenu MMicro[MAX_MIC];
  
};

extern CPWindow1 Window1 ;

enum {CPU_RUNNING, CPU_STEPPING, CPU_HALTED, CPU_BREAKPOINT, CPU_ERROR};

#define ST_T1 0x01;
#define ST_T2 0x02;
#define ST_TH 0x04;

#endif /*#CPWINDOW1*/

