//---------------------------------------------------------------------------
#ifndef FilePanelH
#define FilePanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <Stdlib.h>
//---------------------------------------------------------------------------
#define SYMBOLWIDTH 5
#define clLtBlue (TColor)0x00ffff00
#define clDkBlue (TColor)0x00c00000
#define MAXDIRLISTS     16
#define FILEGROUPS      32

#define HOME  36
#define UP    38
#define PGUP  33
#define LEFT  37
#define RIGHT 39
#define END   35
#define DOWN  40
#define PGDN  34

class PACKAGE TFilePanel;
enum TPaintColumn {pcFull, pcUp, pcDown};

#include "DriveControl.h"

typedef struct tagITEMCOLOR
  {
    TColor Ink;
    TColor Paper;
  } ITEMCOLOR;

struct TItemColor
  {
    TItemColor() {};
    TItemColor(ITEMCOLOR& i)
    {
      Ink = i.Ink;
      Paper = i.Paper;
    }
    operator ITEMCOLOR()
    {
      ITEMCOLOR i;
      i.Ink = Ink;
      i.Paper = Paper;
      return i;
    }
    union
    {
      struct
      {
        TColor Ink;
        TColor Paper;
      };
    };
  };

class TFileGroup : public TPersistent
{
public:
  AnsiString Name;
  AnsiString ExtensionList;
  TItemColor Color;
  int Attributes;
  int Priority;
};

class PACKAGE TFilePanel : public TCustomControl
{
private:
//  TCanvas* FCanvas;
//  TControlCanvas* FTemp;
  //system data
  TDriveControl* FDriveControl;
  // file indexes
  int FBeginIndex;
  int FEndIndex;
  int FActiveIndex;
  //graph paint data
  int FColumnCount;
  int FColumnWidth;//var width
  int FSymbolWidth;//var symbolwidth
  int FSymbolHeight;//var height
  int FFullWidth;//var fullwidth
  int FFileListHeight;//var m;
  int FInfoBar;//var o;
  //file list
/*  TFileList **FFileList;//array of file list pointers
  int FDirs;//scanned directory lists
  int FMaxDirLists;//maximum directory lists
  bool FRescan;//rescan directory if directory history is used*/
  //settings
  bool FFixedWidth;
  int FFileColumns;
  //panel params query
  bool FShowFileInfo;
  bool FShowFreeSpace;
  bool FShowFileSelectInfo;
  bool FShowDiskSet;
  bool FShowFilesSpace;
  //files and paths
//  AnsiString FPath;
  AnsiString FCurrentDir;
  AnsiString FCurrentDrive;
//  int FFreeSpace;
  //AnsiString FSelectedFile;
  AnsiString FInitDir;
  TDriveLetter FInitDrive;
  
  unsigned char FHiddenSym;
  unsigned char FSystemSym;
  unsigned char FCommonSym;
  //system vars
  bool FActive;
  //colors
  //the file manager
  TItemColor FNormalText;
  TItemColor FSelectedText;
  TItemColor FNormalCursor;
  TItemColor FSelectedCursor;
  TItemColor FListDivisor;
  TItemColor FActiveCatalog;
  TItemColor FPassiveCatalog;
  TItemColor FColumnContents;
  //file groups
  int FFileGroups;
  TFileGroup **FFileGroup;
  //disk line
  TItemColor FDriveLetters;
  TItemColor FSelectedDrive;
  TItemColor FDriveFrame;
  //info
  TItemColor FActiveFile;
  TItemColor FTextSelectedFiles;
  TItemColor FQuantitySelected;
  TItemColor FAllText;
  TItemColor FAllDigits;
  TItemColor FFreeText;
  TItemColor FFreeDigits;
  //sorting
//  TSortMode FSortMode;//may be property
//  bool FDirectionSort;//true is undirectly, may be property
  

  void __fastcall SetFileColumns(int);
  void __fastcall SetDriveControl(TDriveControl*);
  TFileList *__fastcall GetFileList(int);
  TFile *__fastcall GetActiveFile();
  void __fastcall MoveCursor(int);
  void __fastcall ShowCursor(int, bool);
  void __fastcall ShowPath(bool);
  void __fastcall PaintFileInfo(int);
  int __fastcall PaintColumns(TPaintColumn, int);
  AnsiString __fastcall GetCurrentDrive ();
  void __fastcall SetCurrentDrive(AnsiString);

  //system handlers functions
  void __fastcall Click(void);
  void __fastcall DblClick(void);
  void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
  void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
  void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
  AnsiString __fastcall GetCurrentDir();
//  void __fastcall DoEnter(void);
//  void __fastcall DoExit(void);
//  void __fastcall WndProc(Messages::TMessage &Message);
  //system handlers data
  bool FPressed;
  bool FPainted;
  TSortMode FSortMode;
  bool FSortDirection;
//    void __fastcall SetRescan(bool value);
//    bool __fastcall GetRescan();
  void __fastcall SetSortMode(TSortMode value);
  void __fastcall SetSortDirection(bool value);
protected:
/*    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_KILLFOCUS, TMessage, ChangeFocus)
        VCL_MESSAGE_HANDLER(WM_SETFOCUS, TMessage, ChangeFocus)
    END_MESSAGE_MAP(TCustomControl)*/
/*  void __fastcall WMKeyDown(TWMKey);
  void __fastcall WMKeyUp(TWMKey);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WMKeyDown)
    MESSAGE_HANDLER(WM_SYSKEYDOWN, TWMKey, WMKeyDown)
    MESSAGE_HANDLER(WM_KEYUP, TWMKey, WMKeyUp)
    MESSAGE_HANDLER(WM_SYSKEYUP, TWMKey, WMKeyUp)
  END_MESSAGE_MAP(TCustomControl)*/
public:
  //property
//  __property TCanvas* Canvas = {read = FCanvas};
  __property Canvas;
  __property AnsiString CurrentDir = {read = GetCurrentDir};
  __property TFileList *FileList[int] = {read = GetFileList};
  __property int ActiveIndex = {read = FActiveIndex};
  __property TFile *ActiveFile = {read = GetActiveFile};
  //constructors
  __fastcall TFilePanel(TComponent *);
  __fastcall TFilePanel(TComponent *, AnsiString);
  //destructor
  __fastcall ~TFilePanel();
  //methods
/*  void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
  void __fastcall KeyPress(char &Key);
  void __fastcall KeyUp(Word &Key, Classes::TShiftState Shift);*/
/*  DoKeyDown(Messages::TWMKey &Message);
  DoKeyPress(Messages::TWMKey &Message);
  DoKeyUp(Messages::TWMKey &Message);*/
  char __fastcall ConvertAttribute (int);
  void __fastcall SetDefalutSelectSymbols();
  void __fastcall SetDefaultColors();
  void __fastcall Paint();
//  void __fastcall Reset();
  void  __fastcall ChangeFocus(TMessage);
  int __fastcall KeyProcess(int key);
//  int __fastcall Enter(int);
  AnsiString __fastcall MinimizeName(AnsiString, Graphics::TCanvas *, int);
//    __property bool Rescan  = { read = GetRescan, write = SetRescan };
__published:
  __property OnClick;
  __property OnDblClick;
  __property OnDragDrop;
  __property OnDragOver;
  __property OnEndDrag;
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnStartDrag;
 /* __property OnEnter;
  __property OnExit;
  __property OnKeyDown;
  __property OnKeyPress;
  __property OnKeyUp;*/
  __property Align;
  __property Color;
  __property Enabled;
  __property Font;
  __property ShowHint;
  __property ParentShowHint;
  __property TabStop;
  __property AnsiString InitDir = {read = FInitDir, write = FInitDir};
  __property TDriveLetter InitDrive = {read = FInitDrive, write = FInitDrive};
  __property int FileColumns = {read = FFileColumns, write = SetFileColumns};
  __property unsigned char HiddenSymbol = {read = FHiddenSym, write = FHiddenSym};
  __property unsigned char SystemSymbol = {read = FSystemSym, write = FSystemSym};
  __property unsigned char CommonSymbol = {read = FCommonSym, write = FCommonSym};
  __property bool ShowFileInfo = {read = FShowFileInfo, write = FShowFileInfo};
  __property bool ShowFreeSpace = {read = FShowFreeSpace, write = FShowFreeSpace};
  __property bool ShowFileSelectInfo = {read = FShowFileSelectInfo, write = FShowFileSelectInfo};
  __property bool ShowDiskSet = {read = FShowDiskSet, write = FShowDiskSet};
  __property bool ShowFilesSpace = {read = FShowFilesSpace, write = FShowFilesSpace};
  __property bool Active = {read = FActive};
  __property TDriveControl* DriveControl = {read = FDriveControl, write = SetDriveControl};
  __property AnsiString CurrentDrive = {read = GetCurrentDrive, write = SetCurrentDrive};
  __property TSortMode SortMode  = { read = FSortMode, write = SetSortMode };
  __property bool SortDirection  = { read = FSortDirection, write = SetSortDirection };
};
//---------------------------------------------------------------------------
#endif
