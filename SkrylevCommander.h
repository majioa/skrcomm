//---------------------------------------------------------------------------
#ifndef SkrylevCommanderH
#define SkrylevCommanderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DriveControl.h"
#include "FilePanel.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "KeyMaster.h"
//---------------------------------------------------------------------------
class TSkrylevForm : public TForm
{
__published:	// IDE-managed Components
        TDriveControl *DriveControl;
        TFilePanel *FilePanel2;
        TFilePanel *FilePanel1;
        TPanel *CommandEdit;
        TPanel *HintPanel;
        TKeyMaster *KeyMaster;
        void __fastcall CommandEditResize(TObject *Sender);
        void __fastcall HintPanelResize(TObject *Sender);
        void __fastcall FormClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSkrylevForm(TComponent* Owner);
        __fastcall ~TSkrylevForm();
        int __fastcall SwitchPanels(int);
        void __fastcall WndProc(Messages:: TMessage &message);

};
//---------------------------------------------------------------------------
extern PACKAGE TSkrylevForm *SkrylevForm;
//---------------------------------------------------------------------------
#endif
