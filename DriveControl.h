//---------------------------------------------------------------------------
#ifndef DriveControlH
#define DriveControlH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
#include "math.h" 
#define LOGICDRIVES  26
#define FIXEDDRIVES  0
#define MAXDRIVES    LOGICDRIVES + FIXEDDRIVES
#define MAXFILES     16384
#define MAXPANELS    2
#define MAXPROCESSES 32

class PACKAGE TDriveControl;
class PACKAGE TFilePanel;
class PACKAGE TCopyProgress;
class PACKAGE TFileList;
class TFile;

enum TDriveType1 {dt1NotDetected, dt1NotFound, dt1Floppy, dt1Hard, dt1CDRom, dt1Virtual, dt1Network, dt1Temp, dt1FTP};
enum TDriveLetter {dlA, dlB, dlC, dlD, dlE, dlF, dlG, dlH, dlI, dlJ, dlK, dlL,
  dlM, dlN, dlO, dlP, dlQ, dlR, dlS, dlT, dlU, dlV, dlW, dlX, dlY, dlZ, dlCurrent,dlDir};
enum TFileCopyMode {fcmOverwrite, fcmAddToExists, fcmConfirmWrite, fcmSkipExists, fcmRefreshOld};
enum TypeFileVerifyMode {fvmVerifyFree, fvmVerifyWrite, fvmCopyingDescription, fvmEraseSource};
typedef Set <TypeFileVerifyMode,fvmVerifyFree,fvmEraseSource> TFileVerifyMode;
enum TFileListMode {flmNormal, flmExpanded, flmSelectedOnly, flmFilesOnly, flmDirOnly};
enum TSortMode {smNone, smNum, smUnsort, smName, smExtension, smDateTime, smSize, smGroup};
//enum TFileCopyMode {fcmCopy, fcmMove, fcmDelete};

#include "CopyDlg.h"               
#include "CopyPrg.h"
#include "DeletePrg.h"

/*class PACKAGE TCopyDialog : public TCommonDialog
{
private:
  //data
  bool FVerifyFree;
  bool FVerifyWriting;
  bool FCopyDescriptions;
  bool FEraseSource;
  TFileMode FCopyMode;
  AnsiString FCopyPath;
  //property
  __property Template;
public:
  //constructor
  __fastcall TCopyDialog(Classes::TComponent*);
  //method
  bool __fastcall Execute(void);
  BOOL CALLBACK _export DlgProc (HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
__published:
  //properties
  __property bool VerifyFree = {read = FVerifyFree, write = FVerifyFree};
  __property bool VerifyWriting = {read = FVerifyWriting, write = FVerifyWriting};
  __property bool CopyDescriptions = {read = FCopyDescriptions, write = FCopyDescriptions};
  __property bool EraseSource = {read = FEraseSource, write = FEraseSource};
  __property TFileMode CopyMode = {read = FCopyMode, write = FCopyMode, default = fmConfirmWrite};
  __property AnsiString CopyPath = {read = FCopyPath, write = FCopyPath};
};*/

class TFile : public TPersistent
{
private:
//  int Fi;
//  int Fj;
  AnsiString FName;
  AnsiString FExtension;
  int FSize;
  int FTime;
  int FAttribute;
  bool FSelected;
  bool FVerified;
  bool FRoot;
  TFile* FOrigin;
  TFileList* FOwner;
  int FIndex;
  int FNumber;
    bool FUnmoveable;
    __int64 FNameSum;
  AnsiString __fastcall GetFullName();
  void __fastcall SetSelected(bool);
  void __fastcall SetOwner(TFileList* value);
    AnsiString __fastcall GetFullPath();
    __int64 __fastcall CalculateNameSum(AnsiString name);
public:
  //properties
  __property AnsiString FullName = {read = GetFullName};
  __property AnsiString Name = {read = FName};
  __property AnsiString Extension = {read = FExtension};
  __property int Size = {read = FSize};
  __property int Time = {read = FTime};
  __property int Attribute = {read = FAttribute};
  __property bool Selected = {read = FSelected, write = SetSelected};
  __property bool Verified = {read = FVerified, write = FVerified};
  //constructors
  __fastcall TFile (TFileList*, AnsiString);
  __fastcall TFile (TFileList*, TSearchRec *);
  __fastcall TFile (TFileList*, TFile *);
  //methods
  void __fastcall UpdateInfo();
  void __fastcall UpdateInfo(TSearchRec *);
  bool __fastcall IsDirectory();
  bool __fastcall AddPathToName(AnsiString);
    __property bool Root  = { read = FRoot , write = FRoot};
    __property TFile* Origin  = { read = FOrigin };
    __property TFileList* Owner  = { read = FOwner, write = SetOwner };
    __property AnsiString FullPath  = { read = GetFullPath };
    __property int Index = { read = FIndex, write = FIndex };
    __property int Number = { read = FNumber, write = FNumber };
__published:
    __property bool Unmoveable  = { read = FUnmoveable, write = FUnmoveable };
    __property __int64 NameSum  = { read = FNameSum };
};

class PACKAGE TFileList : public TComponent
{
private:
  TFileListMode FMode;
  TSortMode FSortMode;
  bool FSortDirection;
  bool FAutoSorting;
  AnsiString FDirectory;
  TFile** FFile;
  int FFiles;
  int FSelectedFiles;
  int FSelectedBytes;
  int FDataFiles;
  int FDataBytes;
  int FActiveIndex;
  bool FScanned;
  //properties for file panel
  int FPanelBeginIndex;
  int FPanelEndIndex;
  int FPanelActiveIndex;
  //rescan properties
  bool FMain;
  bool FRescan;
  TTimer *FTimer;
  int FTimerCalls;
  TSearchRec *FFind;
  AnsiString FActiveFile;
  int FFindIndex;
//  AnsiString __fastcall SetName (AnsiString);
//  AnsiString __fastcall SetExtension (AnsiString);
//  void __fastcall SetDirectory (AnsiString);
  int __fastcall GetAttribute (int);
  AnsiString __fastcall GetName (int);
  AnsiString __fastcall GetExtension (int);
  AnsiString __fastcall GetFullName (int);
  AnsiString __fastcall GetPathName (int);
  int __fastcall GetFileSize(int); 
  TDateTime __fastcall GetFileTime(int);
  bool __fastcall GetSelected(int);
  void __fastcall SetSelected(int,bool);
  TFile* __fastcall GetFile(int);
//  bool __fastcall CopyFileName(TFile** &,int &,int);
  bool __fastcall GetRoot(int);
  int __fastcall GetIndex(TFile*);
  void __fastcall SetRescan(bool value);
  AnsiString __fastcall GetFullPath(int __index);
  void __fastcall RescanCall(System::TObject *Sender);
  void __fastcall VerifyDir(AnsiString &dir);
  bool __fastcall Verify(int);
  bool __fastcall FindNext();
  void __fastcall FindFirst();
  void __fastcall Sort(TSortMode, bool);
    void __fastcall SetSortMode(TSortMode mode);
    void __fastcall SetSortDirection(bool value);
protected:
  int __fastcall FileExist(AnsiString); //may be public
public:
  //constructor
  __fastcall TFileList(TComponent *);
  __fastcall TFileList(TComponent *, TFileList *, TFileListMode);
  //destructor
  __fastcall ~TFileList();
  //methods
  void __fastcall Clear();
  bool __fastcall Delete(int);
  bool __fastcall Add(TFile *);
  bool __fastcall Add(TSearchRec *);
  bool __fastcall Verify(TSearchRec *f);
  int __fastcall Fill(System::AnsiString,bool,TSortMode, bool);
  void __fastcall RescanImmediate();
  void __fastcall UpdateDataBytes(int value);
  void __fastcall Sort();
  int __fastcall GetNumber(int index);
//  bool __fastcall Expand(bool);
//  void __fastcall Insert(int, const System::AnsiString);
  __property int PanelBeginIndex = {read = FPanelBeginIndex, write = FPanelBeginIndex};
  __property int PanelEndIndex = {read = FPanelEndIndex, write = FPanelEndIndex};
  __property int PanelActiveIndex = {read = FPanelActiveIndex, write = FPanelActiveIndex};
  __property bool Rescan = { read = FRescan, write = SetRescan };
  __property bool Scanned = { read = FScanned };
  __property bool Main = { read = FMain, write = FMain };
__published:
  __property AnsiString Directory = {read = FDirectory};
  __property int Files = {read = FFiles};
  __property int SelectedFiles = {read = FSelectedFiles};
  __property int DataFiles = {read = FDataFiles};
  __property int SelectedBytes = {read = FSelectedBytes};
  __property int DataBytes = {read = FDataBytes};
  __property TFileListMode Mode = {read = FMode};
  __property TFile* File[int] = {read = GetFile};
  __property AnsiString Name[int] = {read = GetName};
  __property int Attribute[int] = {read = GetAttribute};
  __property int Size[int] = {read = GetFileSize};
  __property TDateTime Time[int] = {read = GetFileTime};
  __property AnsiString Extension[int] = {read = GetExtension};
  __property AnsiString FullName[int] = {read = GetFullName};
  __property AnsiString PathName[int] = {read = GetPathName};
  __property AnsiString FullPath[int]  = {read = GetPathName};
  __property bool Selected[int] = {read = GetSelected, write = SetSelected};
  __property bool Root[int]  = { read = GetRoot };
  __property int Index[TFile*] = { read = GetIndex};
  __property bool AutoSorting = { read = FAutoSorting, write = FAutoSorting };
  __property bool SortDirection = { read = FSortDirection, write = SetSortDirection };
  __property TSortMode SortMode = { read = FSortMode, write = SetSortMode };
};

class TDrive : public TPersistent
{
private:
  AnsiString FName;
  TDriveType1 FType;
  TDriveLetter FLetter;
  AnsiString FCurrentDir;
  long FSize;
  long FFree;
//  bool FActive;
  bool __fastcall SetCurrentDir(AnsiString);
  bool __fastcall SetActive(bool);
public:
  //constructor
  __fastcall TDrive(AnsiString);
  //method
  void __fastcall UpdateInfo();
  //properties
  __property AnsiString Name = {read = FName};
  __property TDriveType1 Type = {read = FType};
  __property TDriveLetter Letter = {read = FLetter};
  __property AnsiString CurrentDir = {read = FCurrentDir, write = SetCurrentDir};
  __property long Size = {read = FSize};
  __property long Free = {read = FFree};
//  __property bool Active = {read = FActive, write = SetActive};
};

class PACKAGE TDriveControl : public TComponent
{
private:
  //file list
  TFileList **FFileList;//array of file list pointers
  int FDirs;//scanned directory lists
  int FMaxDirLists;//maximum directory lists
  bool FRescan;//rescan directory if directory history is used
  //forms
  TCopyDialog *FCopyDialog;
  TCopyProgress *FProgressDlg;
  TDeleteProgressForm *FDeleteProgress;
  //system data
  TFilePanel *FPanel[MAXPANELS];
  int FPanels;
  int FLastError;
  __int64 FMemoryBuffer;//free memory in bytes
  __int64 FRWConst;/*constant value is used for reading and writing
    operations, free memory (FMemoryBuffer) always divisible for this constant*/ 
  char* FDriveStrings;//"A\\\0B\\\0..."
  int FDriveSet; //100010100001111h - a,b,c,d,i,k,o - drives
  int FDrives; //number of drives
  TDrive *FDrive[MAXDRIVES];
  AnsiString FCurrentDrive;
  TFilePanel* FActivePanel;
  AnsiString FCurrentDir;
  //process vars
  LPSTARTUPINFO FStartUpInfo;
  LPPROCESS_INFORMATION *FProcessInfo;
  int FProcesses;
  //system methods
  const char* GetDriveString(AnsiString);
  AnsiString __fastcall GetCurrentDir();
  bool __fastcall SetCurrentDir(AnsiString);
  bool __fastcall SetCurrentDrive(AnsiString);
  TDrive* __fastcall GetDrive(AnsiString);
  AnsiString __fastcall GetNewName(AnsiString, AnsiString, AnsiString);
  int __fastcall CopyMove(bool);
  bool __fastcall EraseFile(AnsiString, bool);
  void __fastcall SetRescan(bool);
  bool __fastcall GetRescan();
  void __fastcall SetActivePanel(TFilePanel *);
  void __fastcall LoadIndexes(int);
  void __fastcall SaveIndexes(int);
protected:
public:
  //constructor
  __fastcall TDriveControl(TComponent*);
  //destructor
  __fastcall ~TDriveControl();
  //methods
  bool __fastcall RegisterPanel(TFilePanel*);
  int __fastcall Copy(int);
  int __fastcall Move(int);
  int __fastcall Delete(int);
  int __fastcall KeyProcess(int);
  int __fastcall Enter(int);

  AnsiString __fastcall VerifyDir(AnsiString);
  int __fastcall ExecuteFile(AnsiString file);
  bool __fastcall ChangeDir(AnsiString dir);
  void __fastcall NewDir(AnsiString);
  void __fastcall ScanDir(AnsiString, TFileList *);
  void __fastcall ReScanDir(TFileList *);
  void __fastcall ReScanDir();
  //properties
  __property int Drives = {read = FDrives};
  __property TDrive* Drive[AnsiString] = {read = GetDrive};
  __property const char* DriveString[AnsiString] = {read = GetDriveString};
  __property TFilePanel* ActivePanel  = { read = FActivePanel, write = SetActivePanel };
  __property AnsiString CurrentDrive = {read = FCurrentDrive, write = SetCurrentDrive};
  __property AnsiString CurrentDir = {read = GetCurrentDir, write = SetCurrentDir};
    __property bool Rescan  = { read = GetRescan, write = SetRescan };
//    __property bool Rescan  = { read = GetRescan, write = SetRescan };
__published:
  __property __int64 MemoryBuffer = {read = FMemoryBuffer, write = FMemoryBuffer};
  __property __int64 ReadWriteConstant = {read = FRWConst, write = FRWConst};
};
//---------------------------------------------------------------------------
#endif




