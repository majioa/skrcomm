//---------------------------------------------------------------------------
#ifndef CopyPrgH
#define CopyPrgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <algorithm>
//---------------------------------------------------------------------------
//define exit codes
#define EX_EOF              0x0
#define EX_NEXT             -0x1
#define EX_NOTFILE          -0x2
#define EX_TOBECONTINUED    -0x3
#define EX_CANTOPEN         -0x4
#define EX_CANTCREATE       -0x5
#define EX_CANTCLOSE        -0x6
#define EX_CANTREAD         -0x7
#define EX_CANTWRITE        -0x8

enum TFileStreamType {fstNone, fstFile, fstDirectory, fstVolume};
enum TCmdMode {cmInit,cmDelete,cmLoad,cmLoading,cmSave,cmSaving};
enum TConfirmMode {cmAsk, cmYesToAll, cmNoToAll};

class PACKAGE TCopyProgress;
class TFileCmd;

#include "DriveControl.h"
//class TFile;//????
//class PACKAGE TFileList;

typedef struct tagCOPYPROGRESSDLGSTRUCT {
  TFileList* FileList;
  AnsiString TargetDir;
  AnsiString SourceDir;
  AnsiString DestinationName;
  __int64 MemoryFree;
  __int64 Portion;
  TFileCopyMode CopyMode;
  TFileVerifyMode VerifyMode;
  int Interval;
  } COPYPROGRESSDLGSTRUCT;

class TFileCmd : public TMemoryStream
{
private:
//    __int64 FMaxSize;
//    TCopyProgress* FProgressDlg;
    bool FLoadContinue;
    bool FSaveContinue;
    __int64 FFileSourcePos;
    __int64 FFileDestinationPos;
    __int64 FFileSize;
    int FMemorySourcePos;
    int FMemoryDestinationPos;
    TFileStreamType FType;
    int FSourceHandle;
    int FDestinationHandle;
    AnsiString FSourceName;
    AnsiString FSourcePath;
    AnsiString FDestinationName;
    AnsiString FDestinationPath;
    TFile* FFile;
    TFile* FOriginFile;
    char* FBuffer;
//    bool FSave;
    bool FAddFile;
public:
    __fastcall TFileCmd(AnsiString, AnsiString, AnsiString, TFile*, char*, bool);
    int __fastcall LoadFileToMemory(__int64 &,__int64 &);
    int __fastcall SaveMemoryToFile(__int64 &,__int64 &);
    __fastcall ~TFileCmd();
    bool __fastcall CreateFile();
    __property bool LoadContinue = { read = FLoadContinue };
    __property bool SaveContinue = { read = FSaveContinue };
    __property __int64 FileSourcePos  = { read = FFileSourcePos };
    __property __int64 FileDestinationPos  = { read = FFileDestinationPos };
    __property __int64 FileSize  = { read = FFileSize };
    __property TFileStreamType Type  = { read = FType };
    __property AnsiString SourceName = {read = FSourceName};
    __property AnsiString SourcePath = {read = FSourceName};
    __property AnsiString DestinationName = {read = FDestinationName};
    __property AnsiString DestinationPath = {read = FDestinationName};
    __property int SourceHandle = {read = FSourceHandle};
    __property TFile* File = {read = FFile};
  //  __property bool Save = { read = FSave, write = FSave};
};

class PACKAGE TCopyProgress : public TForm
{
__published:	// IDE-managed Components
        TLabel *ReadingLabel;
        TProgressBar *ReadingProgressBar;
        TLabel *WritingLabel;
        TProgressBar *WritingProgressBar;
        TBitBtn *CancelButton;
        TTimer *Timer;
        void __fastcall TimerTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:
    __int64 FMemoryBuffer;
    __int64 FRWConst;
    TFileList *FFileList;
    AnsiString FTarget;
    AnsiString FSource;
    AnsiString FDestinationName;
    TFileCmd **FMem;	// User declarations
    TCmdMode FMode;
    bool FMove;
    bool FFile;
    bool FCopied;
    char* FBuffer;
    TFileCopyMode FCopyMode;
    TFileVerifyMode FVerifyMode;
    TConfirmMode FConfirmMode;
    bool FAddFile;
public:		// User declarations
        __fastcall TCopyProgress(TComponent* Owner);
    void __fastcall Initialize(COPYPROGRESSDLGSTRUCT);
    AnsiString __fastcall GetNewName(AnsiString mask, AnsiString source);
    bool __fastcall VerifyStatements(AnsiString, int);
/*    __int64 FMemoryBuffer;
    __int64 FRWConst;
    TFileList *FFileList;
    AnsiString FTarget;
    AnsiString FSource;
    AnsiString FDestinationName;*/
};
//---------------------------------------------------------------------------
extern PACKAGE TCopyProgress *CopyProgress;
//---------------------------------------------------------------------------
#endif
