//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DeletePrg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDeleteProgressForm *DeleteProgressForm;
//---------------------------------------------------------------------------
__fastcall TDeleteProgressForm::TDeleteProgressForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDeleteProgressForm::TimerTimer(TObject *Sender)
{
  //
  static i;
  try
    {
    switch(FMode)
      {
      case cmInit:
        i = FFileList->Files - 1;
        FMode = cmDelete;
        break;
      case cmDelete:
        if(i >= 0)
          {
          if(FDeletingType == dtAll || FDeletingType == dtRoot && FFileList->File[i]->Root || FDeletingType == dtDirectory && FFileList->File[i]->IsDirectory())
            DeletingLabel->Caption = FFileList->FullName[i];
          if(!EraseFile(FFileList->FullPath[i],FFileList->File[i]->IsDirectory()))
            {
            //error may occurs: 2,3,5,12,14,15,16,18,19,25,27,29,31,32,33,34
            //throw Exception
            i--;
            return;
            }
          i--;
          }
        else
          Close();
      }
/*  TMsgDlgButtons btn;
  btn << mbRetry << mbAbort << mbIgnore;
  bool retry;
  do {
    if(dir)
      retry = !RemoveDir(name);
    else
      retry = !DeleteFile(name);
    if(retry)
      {
      //add verify error
      switch(MessageDlg("Couldn't erase file. Retry?",mtConfirmation,btn,-1))
        {
        case mrAbort:
          return false;
        case mrIgnore:
          retry = false;
          break;
        }
      }
    } while (retry);
  return true;*/

    }
  catch(...)
    {
    Close();
    }

}
//---------------------------------------------------------------------------
bool __fastcall TDeleteProgressForm::EraseFile(AnsiString name,bool dir)
{
  TMsgDlgButtons btn;
  btn << mbRetry << mbAbort << mbIgnore;
  bool retry;
  do {
    if(dir)
      retry = !RemoveDir(name);
    else
      retry = !DeleteFile(name);
    if(retry)
      {
      //add verify error
      switch(MessageDlg("Couldn't erase file. Retry?",mtConfirmation,btn,-1))
        {
        case mrAbort:
          return false;
        case mrIgnore:
          retry = false;
          break;
        }
      }
    } while (retry);
  return true;
}


void __fastcall TDeleteProgressForm::SetFileList(TFileList *value)
{
  //
  FFileList = value;
}

void __fastcall TDeleteProgressForm::FormShow(TObject *Sender)
{
  Timer->Enabled = true;
  FMode = cmInit;
}
//---------------------------------------------------------------------------

void __fastcall TDeleteProgressForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Timer->Enabled = false;
  Action = caHide;        
}
//---------------------------------------------------------------------------

void __fastcall TDeleteProgressForm::CancelButtonClick(TObject *Sender)
{
  //
  if(Application->MessageBox("Are you sure to abort deleting?","Question",MB_YESNO|MB_ICONQUESTION)==ID_YES)
    Close();
}
//---------------------------------------------------------------------------

