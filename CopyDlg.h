//----------------------------------------------------------------------------
#ifndef CopyDlgH
#define CopyDlgH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Menus.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\ExtCtrls.hpp>

class PACKAGE TCopyDialog;

#include "DriveControl.h"
//----------------------------------------------------------------------------
class PACKAGE TCopyDialog : public TForm
{
__published:
    TLabel *Source;
	  TButton *OK;
	  TButton *Cancel;
	  TButton *Tree;
	  TButton *Help;
    TComboBox *Destination;
	  TGroupBox *GroupBoxID1100;
	  TRadioButton *Overwriteallexistsfiles;
	  TRadioButton *Addtoallexistsfiles;
	  TRadioButton *Writingconfirmation;
	  TRadioButton *Skipallexistsfils;
	  TRadioButton *Refresholdfiles;
	  TGroupBox *GroupBoxID1101;
	  TCheckBox *Verifydiskfreespace;
	  TCheckBox *Verifywritingondisk;
	  TCheckBox *Copydescriptions;
	  TCheckBox *Erasesourcefiles;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    
private:
  //copy mode
  bool FOver;
  bool FAdd;
  bool FConfirm;
  bool FSkip;
  bool FRefresh;
  //verify mode
  bool FFree;
  bool FWrite;
  bool FCopy;
  bool FErase;
  TFileCopyMode FCopyMode;
  TFileVerifyMode FVerifyMode;
public:
  virtual __fastcall TCopyDialog(TComponent* AOwner);
  __property TFileCopyMode CopyMode = {read = FCopyMode};
  __property TFileVerifyMode VerifyMode = {read = FVerifyMode};
};
//----------------------------------------------------------------------------
extern TCopyDialog *CopyDialog;
//----------------------------------------------------------------------------
#endif
