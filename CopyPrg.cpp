//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CopyPrg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCopyProgress *CopyProgress;
//---------------------------------------------------------------------------
__fastcall TCopyProgress::TCopyProgress(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCopyProgress::TimerTimer(TObject *Sender)
{
//Надо переделать постановку в очередь файлов загруженных в память и выгрузку их
//переменные i,j,k 
  static __int64 memsize,rwconst;
  static int i, j, k;
  static bool firstwrite;
  static int exitcode = EX_EOF, len;
  try
    {
  switch(FMode)
    {
    case cmInit:
      memsize = FMemoryBuffer;
      rwconst = FRWConst;
      i = 0;
      j = 0;
      k = 0;
      FMode = cmLoad;
      ReadingProgressBar->Position = ReadingProgressBar->Min;
      ReadingLabel->Show();
      ReadingProgressBar->Show();
      firstwrite = true;
      break;

    case cmLoad:
      if(i != FFileList->Files)
        {
//        if(FFileList->Selected[i])
//          {
          AnsiString temp;
          if(!(FFile && FFileList->File[i]->Root))
            temp = FFileList->File[i]->FullName;
          else
            temp = GetNewName(FDestinationName,FFileList->File[i]->FullName);//get name from mask
          if(!VerifyStatements(FTarget + temp,i))
            {
            i++;
            break;
            }
          FMem[k] = new TFileCmd(FSource,FTarget,temp,FFileList->File[i],FBuffer,FAddFile);
          if(FMem[k]->SourceHandle == -1)
            throw EFOpenError("Couldn't open file " + FMem[k]->SourceName);
          if(FMem[k]->Type == fstFile)
            {
            AnsiString path = ExtractFilePath(FSource + FMem[k]->SourceName), temp = ExtractFileName(FMem[k]->SourceName);
            len = ReadingLabel->Canvas->TextWidth(temp);
            ReadingLabel->Caption = MinimizeName(path,ReadingLabel->Canvas,
              ReadingLabel->ClientWidth - len) + temp;
            ReadingProgressBar->Position = ReadingProgressBar->Min;
            }
          FMode = cmLoading;
          i++;
//          }
//        else
//          i++;
        }
      else
        {
        memsize = FMemoryBuffer;
        FMode = cmSave;
        }
      break;
      
    case cmLoading:
      exitcode = FMem[k]->LoadFileToMemory(memsize,rwconst);
      if(FMem[k]->Type == fstFile)
        ReadingProgressBar->Position = (int) ((FMem[k]->FileSourcePos + 1) * (ReadingProgressBar->Max -
          ReadingProgressBar->Min) / (FMem[k]->FileSize + 1));
      switch(exitcode)
        {
        case EX_NEXT:
          rwconst = FRWConst;
          memsize = FMemoryBuffer;
          FMode = cmSave;
          break;
        case EX_EOF:
          k++;
          rwconst = FRWConst;
          FMode = cmLoad;
        }
      break;
      
    case cmSave:
      if(firstwrite)
        {
        WritingProgressBar->Position = WritingProgressBar->Min;
        WritingLabel->Show(); 
        WritingProgressBar->Show(); 
        firstwrite = false;
        }
      if(j == k && i == FFileList->Files && exitcode == EX_EOF)
        {
        for (i = k - 1; i >= 0; i--)
          {
          if(FMove && FMem[i]->Type == fstDirectory)
            {
            if(!RemoveDir(FMem[j]->SourcePath + FMem[j]->SourceName))
              throw EInOutError("The directory " + FMem[j]->SourceName + " can't be deleted");
            }
          delete FMem[i];
          }
        FCopied = true;
        Close();
        return;
        }
      if(!FMem[j]->SaveContinue)
        {
        FMem[j]->CreateFile();
        if(FMem[j]->Type == fstFile)
          {
          AnsiString path = ExtractFilePath(FTarget + FMem[j]->DestinationName), temp = ExtractFileName(FMem[j]->DestinationName);
          len = WritingLabel->Canvas->TextWidth(temp);  
          WritingLabel->Caption = MinimizeName(path,WritingLabel->Canvas,
            WritingLabel->ClientWidth - len) + temp;
          WritingProgressBar->Position = WritingProgressBar->Min;
          }
        }
      FMode = cmSaving;//verify if directory then next save (not saving)
      break;
      
    case cmSaving:
      exitcode = FMem[j]->SaveMemoryToFile(memsize,rwconst);
      if(FMem[j]->Type == fstFile)
        WritingProgressBar->Position = (int) ((FMem[j]->FileDestinationPos + 1) * (WritingProgressBar->Max - 
          WritingProgressBar->Min) / (FMem[j]->FileSize + 1));
      switch(exitcode)
        {
        case EX_EOF:
          if(FMove && FMem[j]->Type == fstFile)
            {
            if(!DeleteFile(FMem[j]->SourcePath + FMem[j]->SourceName))
              throw EInOutError("Error occurs during deleting file " + FMem[j]->SourceName);
            }
          rwconst = FRWConst;
          j++;
          FMode = cmSave;
          break;
        case EX_NEXT:
          rwconst = FRWConst;
          memsize = FMemoryBuffer;
          FMode = cmLoading;
          break;
        case EX_TOBECONTINUED:
          break;
        }
    }
    }
  catch(...)
    {
/*      if(FMode == cmLoad)
        k --;*/
      for(j = 0; j <= k; j++)
        delete FMem[j];
      FCopied = true;
      Close();
      return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCopyProgress::Initialize(COPYPROGRESSDLGSTRUCT i)
{
    //TODO: Add your source code here
  FMemoryBuffer = i.MemoryFree;
  FRWConst = i.Portion;
  FDestinationName = i.DestinationName;
  FTarget = i.TargetDir;
  FSource = i.SourceDir;
  FFileList = i.FileList;
  FVerifyMode = i.VerifyMode;
  FCopyMode = i.CopyMode;
  Timer->Interval = i.Interval;
  FFile = !i.DestinationName.IsEmpty();
  FMove = FVerifyMode.Contains(fvmEraseSource);
  FConfirmMode = cmAsk;
  if(FCopyMode == fcmAddToExists)
    FAddFile = true;
  else
    FAddFile = false;
}
//---------------------------------------------------------------------------
void __fastcall TCopyProgress::FormShow(TObject *Sender)
{
  FMem = new TFileCmd* [MAXFILES];
  Timer->Enabled = true;      
  FMode = cmInit;
  FBuffer = new char [(int)FRWConst];
}
//---------------------------------------------------------------------------
bool __fastcall TCopyProgress::VerifyStatements(AnsiString file, int num)
{
  bool exist = FileExists(file);
  switch(FCopyMode)
    {
    case fcmConfirmWrite:
      if(exist && FConfirmMode != cmYesToAll)
        {
        if(FConfirmMode == cmAsk)
          {
          TMsgDlgButtons btn;
          btn << mbYes << mbNo << mbCancel << mbNoToAll << mbYesToAll;
          int m = MessageDlg("File " + ExtractFileName(file) + " already exist. Overwrite?",mtConfirmation,btn,-1);
          if(m == mrNo)  //??
            return false;//??
          }
        else  
          return false;
        }
      break;
    case fcmSkipExists:
      return !exist;
    case fcmRefreshOld:
      if(exist)
        {
        int i = FileOpen(file,fmOpenRead|fmShareCompat);
        if (i == -1)
          break;
        int time = FileGetDate(i);
        FileClose(i);
        if(FileDateToDateTime(time) > FileDateToDateTime(FFileList->File[num]->Time))
          return false;
        }
    }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TCopyProgress::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  delete FBuffer;
  delete FMem;
  Timer->Enabled = false;
  Action = caHide;
  WritingProgressBar->Hide(); 
  WritingLabel->Hide(); 
  WritingLabel->Caption = ""; 
  ReadingProgressBar->Hide(); 
  ReadingLabel->Hide(); 
  ReadingLabel->Caption = ""; 
}
//---------------------------------------------------------------------------
void __fastcall TCopyProgress::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  CanClose = FCopied || Application->MessageBox("Selected files are not copied yet! Stop copying?",
    "Copying breaking",MB_YESNO|MB_ICONQUESTION) == ID_YES;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TCopyProgress::GetNewName(AnsiString mask, AnsiString source)
{
  //TODO: Add your source code here
  AnsiString temp = ExtractFileName(source);
  source = ExtractFilePath(source);
  if(source.IsEmpty())
    return mask;
  return source + "\\" + mask;//???
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TFileCmd::TFileCmd(AnsiString source,AnsiString target,AnsiString temp,TFile* file, char* buffer, bool add) : TMemoryStream()
{
  //             
  AnsiString name = file->FullName;   
  TSearchRec F;
  
  if(FindFirst (source + name,faAnyFile,F))
    {
    //destroy this class
    FType = fstNone;
    return;
    }
  switch(F.Attr)
    {
    case faVolumeID:
      FType = fstVolume;
      break;
    case faDirectory:
      FType = fstDirectory;
      break;
    default:
      FType = fstFile;
      FSourceHandle = FileOpen (source + name, fmShareExclusive|fmOpenRead);
/*      if(FSourceHandle == -1)
        throw EFOpenError("Couldn't open file " + FSourceName);*/
      FFileSize = FileSeek(FSourceHandle,0,2);
      FileSeek(FSourceHandle,0,0);
    }
  FSourceName = name;
  FSourcePath = source;
  FDestinationName = temp;
  FDestinationPath = target;
  FFile = file;
  FBuffer = buffer;
//  FSave = true;
  FAddFile = add;
}
//---------------------------------------------------------------------------
int __fastcall TFileCmd::LoadFileToMemory(__int64 &memsize, __int64 &portion)
{
//  TMsgDlgButtons buttons;
//  buttons<<mbAbort<<mbRetry<<mbIgnore;
  switch(FType)
    {
    case fstDirectory:
      return EX_EOF;
    case fstFile:
      __int64 size = min (FFileSize - (int) FFileSourcePos,memsize), len;
//      char *memory = new char[(int) portion];
      bool retry = false;
      if(size < portion)
        portion = size;
      do {
        if(((len = (__int64) FileRead(FSourceHandle,FBuffer,(int)portion))!= portion))
          {
          switch (Application->MessageBox(("File " + FSourceName + " can't be read").c_str(),
            "File Error",MB_ABORTRETRYIGNORE|MB_ICONSTOP))
            {
            case ID_ABORT:
//              delete FBuffer;
              throw EInOutError("An error occurs during reading file " + FSourceName);
            case ID_IGNORE:
              len = Write(FBuffer,(int)portion);
              retry = false;
              break;
            case ID_RETRY:
              FileSeek(FSourceHandle,0,(int) FFileSourcePos);
              portion /= 2;
              if( portion < 512)
                portion = 512;
              retry = true;
              break;
            }
          }
        else
          {
          len = Write(FBuffer,(int)len);
          retry = false;
          }
        } while (retry);

      memsize -= len;
      FFileSourcePos += len;
//      delete FBuffer;

      if(FFileSourcePos == FFileSize)
        {
        FileClose(FSourceHandle);
        FSourceHandle = 0;
        FLoadContinue = false;  
        Position = 0;
        return EX_EOF;
        }
      FLoadContinue = true;  
      if(memsize)
        return EX_TOBECONTINUED;
      Position = 0;
      return EX_NEXT;//to saving operations
    }
  return EX_NOTFILE;//exit code -1, not a file
}
//---------------------------------------------------------------------------
bool __fastcall TFileCmd::CreateFile()
{
  AnsiString name = FDestinationName, path = FDestinationPath;
  if(FType == fstDirectory)
    {
    ForceDirectories(path + name);
    if(!DirectoryExists(path + name))
      throw EInOutError("Couldn't create the directory " + FDestinationName);
    }
  else
    {
    if(FAddFile && FileExists(path + name))
      {
      if((FDestinationHandle = FileOpen(path + name,fmOpenWrite|fmShareDenyWrite)) == -1)
        throw EFOpenError("Couldn't open the file " + FDestinationName);
      FileSeek(FDestinationHandle,0,2);
      }
    FDestinationHandle = FileCreate(path + name);
    if(FDestinationHandle == -1)
      return false;
    Position = 0;
    }
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TFileCmd::SaveMemoryToFile(__int64 &memsize,__int64 &portion)
{
  switch(FType)
    {
    case fstDirectory:
      FFile->Selected = false;
      return EX_EOF;
    case fstFile:
      __int64 len;
//      char *memory = new char[(int) portion];
      bool retry = false;
      if(Size - Position < portion)
        portion = Size - Position;
      
      do {
        len = Read(FBuffer,(int)portion);
        if(((len = (__int64) FileWrite(FDestinationHandle,FBuffer,(int)portion))!= portion))
          {
          switch (Application->MessageBox(("File " + FDestinationName + " can't be read").c_str(),
            "File Error",MB_ABORTRETRYIGNORE|MB_ICONSTOP))
            {
            case ID_ABORT:
//              delete memory;
              throw EInOutError("An error occurs during writing file " + FDestinationName);
            case ID_IGNORE:
              retry = false;
              break;
            case ID_RETRY:
              Position -= (int)portion;
              FileSeek(FDestinationHandle,0,(int)FFileDestinationPos);
              portion /= 2;
              if( portion < 512)
                portion = 512;
              retry = true;
              break;
            }
          }
        else
          retry = false;
        } while (retry);
      
      memsize -= len;
      FFileDestinationPos += len;
//      delete memory;
 
      if(FFileDestinationPos == FFileSize)
        {
        FileClose(FDestinationHandle);
        FDestinationHandle = 0;
//    if(FMove)
//      {
//     if(!DeleteFile(FSourcePath + FSourceName))
//       throw EInOutError("Error occurs during deleting file " + FSourceName);
//     }
//  else
        FFile->Selected = false;
        FSaveContinue = false;  
        Clear();
        return EX_EOF;
        }
      FSaveContinue = true;
      if(memsize)
        return EX_TOBECONTINUED;
      Clear();
      return EX_NEXT;//to loading operations
      
    }  
  return EX_NOTFILE;//exit code -1, not a file
}
//---------------------------------------------------------------------------
__fastcall TFileCmd::~TFileCmd()
{
  if(FSourceHandle)
    FileClose(FSourceHandle);
  if(FDestinationHandle)
    FileClose(FDestinationHandle);
}



