//---------------------------------------------------------------------------
#ifndef DeletePrgH
#define DeletePrgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TDeleteProgressForm;

#include "CopyPrg.h"
enum TDeletingType {dtAll, dtRoot, dtDirectory};

class TDeleteProgressForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *DeletingLabel;
        TBitBtn *CancelButton;
        TTimer *Timer;
        void __fastcall TimerTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall CancelButtonClick(TObject *Sender);
private:
    TCmdMode FMode;
    TFileList * FFileList;
    TDriveControl * FDriveControl;
    TDeletingType FDeletingType;
    void __fastcall SetFileList(TFileList *);	// User declarations
    bool __fastcall EraseFile(AnsiString,bool);
public:		// User declarations
    __property TFileList * FileList  = { read = FFileList, write = SetFileList };
    __property TDriveControl * DriveControl  = { read = FDriveControl, write = FDriveControl };
    __property TDeletingType DeletingType  = { read = FDeletingType, write = FDeletingType };
        __fastcall TDeleteProgressForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDeleteProgressForm *DeleteProgressForm;
//---------------------------------------------------------------------------
#endif
