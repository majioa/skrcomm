//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SkrylevCommander.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FilePanel"
#pragma link "DriveControl"
#pragma link "KeyMaster"
#pragma link "KeyMaster"
#pragma resource "*.dfm"
TSkrylevForm *SkrylevForm;
//---------------------------------------------------------------------------
__fastcall TSkrylevForm::TSkrylevForm(TComponent* Owner)
    : TForm(Owner)
{
  CopyDialog = new TCopyDialog(this);
  
  TShiftState kk;
  TMessage msg;
  Word keys[8];
  msg.Msg = WM_SETFOCUS;
  FilePanel1->ChangeFocus(msg);

  //FCurrentControl = FilePanel1;
  //CommandEdit->Align = alBottom;
  //FilePanel1->DriveControl = DriveControl1;
  KeyMaster->RegisterKey(VK_F5,kk,DriveControl->Copy);
  KeyMaster->RegisterKey(VK_TAB,kk,SwitchPanels);
  for(int i=0;i<8;i++)
    keys[i] = (Word) (VK_PRIOR +  i);
  KeyMaster->RegisterKeys(keys,8,kk,DriveControl->KeyProcess);
  KeyMaster->RegisterKey(VK_F8,kk,DriveControl->Delete);
  KeyMaster->RegisterKey(VK_RETURN,kk,DriveControl->Enter);
}
//---------------------------------------------------------------------------
__fastcall TSkrylevForm::~TSkrylevForm()
{
  delete CopyDialog;
}
//---------------------------------------------------------------------------
void __fastcall TSkrylevForm::CommandEditResize(TObject *Sender)
{
//  CommandEdit->Width = CommandPanel->Width;
}
//---------------------------------------------------------------------------
void __fastcall TSkrylevForm::HintPanelResize(TObject *Sender)
{
/*  int width = Width/8;
  for (int i=0;i<7;i++)
    HintPanel->Panels->Items[i]->Width = width;*/
}
//---------------------------------------------------------------------------
void __fastcall TSkrylevForm::FormClick(TObject *Sender)
{
  FilePanel1->InitDrive = dlD;    
}
//---------------------------------------------------------------------------
/*void __fastcall TSkrylevForm::KeyDown(Word &Key, Classes::TShiftState Shift)
{
  KeyMaster->KeyDown(Key,Shift);
  if(FCurrentControl)
    ((TFilePanel *) FCurrentControl)->KeyDown(Key,Shift);
  TWinControl::KeyDown(Key,Shift);
}
//---------------------------------------------------------------------------
void __fastcall TSkrylevForm::KeyPress(char &Key)
{
  KeyMaster->KeyPress((Word&) Key);
  if(FCurrentControl)
    ((TFilePanel *) FCurrentControl)->KeyPress(Key);
  TWinControl::KeyPress(Key);
}
//---------------------------------------------------------------------------
void __fastcall TSkrylevForm::KeyUp(Word &Key, Classes::TShiftState Shift)
{
  KeyMaster->KeyUp(Key,Shift);
  if(FCurrentControl)
    ((TFilePanel *) FCurrentControl)->KeyUp(Key,Shift);
  TWinControl::KeyUp(Key,Shift);
} */
//---------------------------------------------------------------------------

void __fastcall TSkrylevForm::FormResize(TObject *Sender)
{
 // HelpBar->BoundsRect = Rect();
//  CommandEdit->BoundsRect = Rect();
  FilePanel1->BoundsRect = Bounds(0,0,ClientWidth/2,ClientHeight - CommandEdit->Height - HintPanel->Height);
  FilePanel2->BoundsRect = Bounds(ClientWidth/2,0,ClientWidth/2+ClientWidth%2,ClientHeight - CommandEdit->Height - HintPanel->Height);
  CommandEdit->BoundsRect = Bounds(0,FilePanel1->Height,ClientWidth,CommandEdit->Height);
  HintPanel->BoundsRect = Bounds(0,FilePanel1->Height + CommandEdit->Height,ClientWidth,HintPanel->Height);
}


int __fastcall TSkrylevForm::SwitchPanels(int)
{
    //TODO: Add your source code here

    
 
 TMessage msg;
    if(FilePanel1->Active)
      {
      msg.Msg = WM_KILLFOCUS;
      FilePanel1->ChangeFocus(msg);
      msg.Msg = WM_SETFOCUS;
      FilePanel2->ChangeFocus(msg);
      }
    else
      {         
      msg.Msg = WM_KILLFOCUS;
      FilePanel2->ChangeFocus(msg);
      msg.Msg = WM_SETFOCUS;
      FilePanel1->ChangeFocus(msg);
      }
/*    if(FilePanel1->Active)
      FCurrentControl = FilePanel1;
  //    FocusControl (FilePanel1);
    if(FilePanel2->Active)
      FCurrentControl = FilePanel2;
  //    FocusControl (FilePanel2);*/
    return 0;
}


void __fastcall TSkrylevForm::WndProc(Messages:: TMessage &message)
{
    //TODO: Add your source code here
    switch(message.Msg)
      {
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
        KeyMaster->WndProc(message);
/*        return;
      case WM_CHAR:
      case WM_DEADCHAR:
      case WM_SYSCHAR:
      case WM_SYSDEADCHAR:
      case WM_KEYLAST:
        int i=0;*/
      //  KeyMaster->Perform (message.Msg,message.WParam,message.LParam);
//        PostMessage
        return;
      }
  TForm::WndProc(message);
}





