//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CopyDlg.h"
//----------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link
#pragma resource "*.dfm"
TCopyDialog *CopyDialog;

static inline void ValidCtrCheck(TCopyDialog *)
{
    new TCopyDialog(NULL);
}
//----------------------------------------------------------------------------
__fastcall TCopyDialog::TCopyDialog(TComponent* AOwner)
    : TForm(AOwner)
{
}
//----------------------------------------------------------------------------
void __fastcall TCopyDialog::FormShow(TObject *Sender)
{
  FOver = Overwriteallexistsfiles->Checked;
  FAdd = Addtoallexistsfiles->Checked;
  FConfirm = Writingconfirmation->Checked;
  FSkip = Skipallexistsfils->Checked;
  FRefresh = Refresholdfiles->Checked;

  FFree = Verifydiskfreespace->Checked;
  FWrite = Verifywritingondisk->Checked;
  FCopy = Copydescriptions->Checked;
  FErase = Erasesourcefiles->Checked;

}
//---------------------------------------------------------------------------
void __fastcall TCopyDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(ModalResult==mrCancel)
    {
    Overwriteallexistsfiles->Checked = FOver;
    Addtoallexistsfiles->Checked = FAdd;
    Writingconfirmation->Checked = FConfirm;
    Skipallexistsfils->Checked = FSkip;
    Refresholdfiles->Checked = FRefresh;

    Verifydiskfreespace->Checked = FFree;
    Verifywritingondisk->Checked = FWrite;
    Copydescriptions->Checked = FCopy;
    Erasesourcefiles->Checked = FErase;
    }
  else
    {
    if(Overwriteallexistsfiles->Checked)
      FCopyMode = fcmOverwrite;
    if(Addtoallexistsfiles->Checked)
      FCopyMode = fcmAddToExists;
    if(Writingconfirmation->Checked)
      FCopyMode = fcmConfirmWrite;
    if(Skipallexistsfils->Checked)
      FCopyMode = fcmSkipExists;
    if(Refresholdfiles->Checked)
      FCopyMode = fcmRefreshOld;
    FVerifyMode.Clear();
    if(Verifydiskfreespace->Checked)
      FVerifyMode << fvmVerifyFree;
    if(Verifywritingondisk->Checked)
      FVerifyMode << fvmVerifyWrite;
    if(Copydescriptions->Checked)
      FVerifyMode << fvmCopyingDescription;
    if(Erasesourcefiles->Checked)
      FVerifyMode << fvmEraseSource;
    }
}
//---------------------------------------------------------------------------
namespace Copydialog
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TCopyDialog)};
        RegisterComponents("Dialogs", classes, 0);
    }
}
//---------------------------------------------------------------------------

