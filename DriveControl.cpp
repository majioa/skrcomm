//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DriveControl.h"
#include "FilePanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFileList *)
{
    new TFileList(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFileList::TFileList(TComponent *Owner) : TComponent(Owner)
{
  FTimer = new TTimer(this);
  FTimer->Enabled = false;
  FTimer->Interval = 1;
  FTimer->OnTimer = RescanCall;
  FFile = new TFile *[MAXFILES];
  FFind = new TSearchRec;
  FMode = flmNormal;
}
//---------------------------------------------------------------------------
__fastcall TFileList::TFileList(TComponent *Owner, TFileList *source, TFileListMode mode) :  TComponent (Owner)
{
  FTimer = new TTimer(this);
  FTimer->Enabled = false;
  FTimer->Interval = 1;
  FFile = new TFile *[MAXFILES];
  FTimer->OnTimer = RescanCall;
  FFind = new TSearchRec;
  FMode = mode;
  FDirectory = source->Directory;
  if(FDirectory.LastDelimiter("\\") != FDirectory.Length())
    FDirectory += "\\";
  FSelectedFiles = source->SelectedFiles;
  FDataFiles = source->DataFiles;
  FSelectedBytes = source->SelectedBytes;
  FDataBytes = source->DataBytes;
  if(mode == flmNormal)
    {
    for (int i = 0; i < source->Files; i++)
      FFile[i] = new TFile(this,source->File[i]);
    FFiles = source->Files;
    }
//  TFile** buffer = new TFile *[MAXFILES];
  else
    {
    int j = 0,l,m,e;
    bool changed,select;
    TFile* temp;
    TSearchRec F;
    AnsiString path = FDirectory, path1;
    for (int i = 0; i < source->FFiles; i++)
      {
      if((source->Selected[i] || mode!=flmSelectedOnly) && source->File[i]->FullName != "..")
        {
        if(j == MAXFILES)
          return;//out of files
        FFile[j] = new TFile (this,source->File[i]);
        j++;
        if(source->File[i]->IsDirectory())
          {
          //scan sub dir
//  level = 0;
          path1 = source->File[i]->FullName + "\\";
          select = source->File[i]->Selected;
//          CopyFileName(FFile,j,i);
          changed = true;
          l = j; // pointer to last performed file
          m = j; // pointer to last file
  
          do {
            if((e = ::FindFirst(path + path1 + "*.*",faReadOnly|faHidden|faSysFile|faDirectory|faArchive,F))!=0)
              {
              FindClose(F);
              return;//error in find first
              }
            do {
              if(F.Name != "." && F.Name != "..")
                {
                if(mode != flmDirOnly || (F.Attr & faDirectory))
                  {
                  if(m == MAXFILES)
                    return;//if cannot add return error code 1
                  FFile[m] = new TFile(this,&F);
                  FFile[m]->AddPathToName(path1);
                  FFile[m]->Selected = select;     
                  FFile[m]->Root = false;
                  if(!FFile[m]->IsDirectory())
                    {
                    FDataBytes += F.Size;
                    FDataFiles++;
                    }
                  m++;
                  }
                }
              if((e = ::FindNext (F))!=18 && e)
                {
                FindClose(F);
                return;//error in find next
                }
              } while (e!=18);
            changed = false;  
            FindClose(F);

            for (l ; l < m; l++)
              {
              if(FFile[l]->IsDirectory())
                {
                path1 = FFile[l]->FullName + "\\";
                changed = true;
                l++;
                break;
                }
              }
    
            } while (changed);

          e = l;
          if(mode == flmFilesOnly)
            {
            //delete dirs
            e = 0;
            for(l = 0; l < m; l++)
              if (FFile[l]->IsDirectory())
                {
                delete FFile[l];
                }
              else
                {
                FFile[e] = FFile[l];
                e++;
                }
            }

  //sorting
          for(l = 0; l < e - 1; l++)
            for (m = 0; m < e - l - 1; m++)
              if(FFile[m]->FullName > FFile[m+1]->FullName)
                {
                temp = FFile[m+1];
                FFile[m+1] = FFile[m];
                FFile[m] = temp;
                }
          j = e;  
          }
        }
      }
    FFiles = j;
    }
}
//---------------------------------------------------------------------------
__fastcall TFileList::~TFileList()
{
  delete FFind;
  delete FFile;
  delete FTimer;
}
//---------------------------------------------------------------------------
/*bool __fastcall TFileList::CopyFileName(TFile** &buffer, int &j, int i)
{
  if(j == MAXFILES)
    return false;
  buffer[j] = new TFile (FFile[i]);
  j++;
  return true;
} */
//---------------------------------------------------------------------------
void __fastcall TFileList::Clear()
{
  for(int i=0;i<FFiles;i++)
    delete FFile[i];
  FFiles = 0;
  FSelectedFiles = 0;
  FSelectedBytes = 0;
  FDataFiles = 0;
  FDataBytes = 0;
  FPanelBeginIndex = 0;
  FPanelEndIndex = 0;
  FPanelActiveIndex = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Delete(int i)
{
  if(!FFile[i]->IsDirectory())
    {
    FDataBytes -= FFile[i]->Size;
    FDataFiles--;
    }
  if(FFile[i]->Selected)
    {
    FSelectedBytes -= FFile[i]->Size;
    FSelectedFiles--;
    }
  delete FFile[i];
  for(i = i + 1; i < FFiles; i++)
    {
    FFile[i - 1] = FFile[i];
    FFile[i - 1]->Number = i - 1;
    }
  FFiles--;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Add(TFile* file)
{
/*  if(FFiles == 1024)
    return false;
  FFile[FFiles] = new TFile();
  FFile[FFiles]->Name = file->Name;
  FFile[FFiles]->Attribute = file->Attribute;
  FFile[FFiles]->Time = file->Time;
  FFile[FFiles]->Size = file->Size;
  if(!(file->Attribute & (faDirectory | faVolumeID)))
    {
    FDataBytes += file->Size;
    FDataFiles++;
    }
  FFiles++;   */
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::FileExist(AnsiString name)
{
  for(int i=0;i<FFiles;i++)
    if(FFile[i]->FullName == name)
      return i; 
  return -1;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::Fill(System::AnsiString path, bool f,TSortMode sort, bool direction)
{
  bool fl = false;
//  FTimer->Enabled = true;
  if(f)
    Clear();
  FRescan = !f;
  VerifyDir(path);
  FDirectory = path;
  FSortMode = sort;
  FSortDirection = direction;
  FFindIndex = 0;
  int m = FFiles;
  int k = FPanelEndIndex - FPanelBeginIndex + 1, i;
/*  if(k < 300)
    k = 300;*/
  k = MAXFILES;
  if(!f)
    for(i = 0; i < FFiles; i++)
      FFile[i]->Verified = false;
  FindFirst();
  for(i = 0; i < k; i++)
    if((fl = FindNext()) == true)
      {
//      FTimer->Enabled = false;
      break;
      }
  if(!f)
    {
    for(int j = m; j < FFiles; j++)
      if(!Verify(j))
        return -1;//error
    for(int j = FFiles - 1; j >= 0; j--)
      if(!FFile[j]->Verified)
        Delete(j);
    }
  if(fl)
    Sort(sort,direction);
  FTimerCalls = 0;
  if(FMain)
    {
    FScanned = true;
//    ((TFilePanel *) Owner)->Refresh();
    }
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::RescanCall(System::TObject* Sender)
{
  bool fl = false;;
  FScanned = false;
  if(FFiles == MAXFILES)
    {
    FTimer->Enabled = false;
    //SetLastError();
    //add set last error
    return;//if cannot add return error code 1
    }
  int m = FFiles;
  for(int i = 0; i < 300; i++)
    if((fl = FindNext()) == true)
      {
      FTimer->Enabled = false;
      break;
      }
  for(int j = m; j < FFiles; j++)
    if(!Verify(j))
      return;//error
  if(fl)
    Sort(FSortMode,FSortDirection);
  FTimerCalls++;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::VerifyDir(AnsiString &dir)
{
  if(!dir.IsPathDelimiter(dir.Length()))
    dir += AnsiString ("\\");
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Add(TSearchRec *f)
{
  try
    {
    FFile[FFiles] = new TFile(this,f);
    if(f->Name == "..")
      FFile[FFiles]->Unmoveable = true;
    FFile[FFiles]->Number = FFiles;
    FFile[FFiles]->Index = FFindIndex;
    FFile[FFiles]->Verified = true;
    FFile[FFiles]->Root = true;
    if(!FFile[FFiles]->IsDirectory())
      {
      FDataBytes += f->Size;
      FDataFiles++;
      }
    FFiles++;
    return true;
    }
  catch(...)
    {
    return false;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Verify(TSearchRec *f)
{
  if(f->Name != FFile[FFindIndex]->FullName)
    return false;
  FFile[FFindIndex]->Index = FFindIndex;
  FFile[FFindIndex]->UpdateInfo(f);
  FFile[FFindIndex]->Verified = true;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Verify(int i)
{
  int j;
  for(j = 0; j < FFiles; j++)
    if(FFile[j]->FullName == FFile[i]->FullName)
      break;
  if(i == j)
    return true;
  if(FFile[i]->Verified)
    FFile[i]->Selected = FFile[j]->Selected;
  else
    FFile[j]->Selected = FFile[i]->Selected;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::UpdateDataBytes(int value)
{
  FDataBytes += value;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::FindNext()
{
    //TODO: Add your source code here
    switch(::FindNext (*FFind))
      {
      case NO_ERROR:
        if(!(FRescan && Verify(FFind)))
          Add(FFind);
        FFindIndex++;
        break;
      case ERROR_NO_MORE_FILES:
        FTimer->Enabled = false;
        FRescan = false;
        FScanned = true;
        FindClose(*FFind);
        FScanned = true;
/*        if(FMain)
          ((TFilePanel *) Owner)->Refresh();*/
        return true;
      default:
        FTimer->Enabled = false;
        FRescan = false;
        FScanned = true;
        //may be throw an exception
        return false;;//if cannot add return error code 1
      }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::FindFirst()
{
  //TODO: Add your source code here
    if(::FindFirst(FDirectory + "*.*" ,faReadOnly|faHidden|faSysFile|faDirectory|faArchive,*FFind)!=0)
      {
      FTimer->Enabled = false;
      FindClose(*FFind);
      //wrong directory
      //throw an exception
      //add set last error, may be throw an exception
      return;
      }
    if(FFind->Name != ".")
      {
      if(!(FRescan && Verify(FFind)))
        Add(FFind);
      }
    FFindIndex++;

}
//---------------------------------------------------------------------------
void __fastcall TFileList::Sort(TSortMode sort, bool direction)
{

  if(sort == smNone)
    return;
  int i,j,k;
  TFile* temp;
  bool flag = false;
  for (i = 0; i < FFiles - 1; i++)
    for (j = 0; j < FFiles - i - 1; j++)
      {
      if(!FFile[j]->Unmoveable)
        {
        for(k = j + 1; k < FFiles - i; k++)
          if(!FFile[k]->Unmoveable)
            break;
        if(k != FFiles - i)
          {
          switch(sort)
            {
            case smNum:
              flag = FFile[j]->NameSum > FFile[j+1]->NameSum;
              break;
            case smUnsort:
              flag = FFile[j]->Index > FFile[j+1]->Index;
              break;
            case smName:
              flag = (FFile[j]->Name > FFile[j+1]->Name) ||
                (FFile[j]->Name == FFile[j+1]->Name && FFile[j]->Extension > FFile[j+1]->Extension);
              break;
            case smExtension:
              flag = (FFile[j]->Extension > FFile[j+1]->Extension) ||
                (FFile[j]->Extension == FFile[j+1]->Extension && FFile[j]->Name > FFile[j+1]->Name);
              break;
            case smSize:
              flag = (FFile[j]->Size > FFile[j+1]->Size) ||
                (FFile[j]->Size == FFile[j+1]->Size && FFile[j]->Name > FFile[j+1]->Name);
              break;
            case smDateTime:
              flag = (FFile[j]->Time > FFile[j+1]->Time) ||
                (FFile[j]->Time == FFile[j+1]->Time && FFile[j]->Name > FFile[j+1]->Name);
              break;
            case smGroup:
              break;
            }
          if(flag ^ direction)
            {
            temp = FFile[j];
            FFile[j] = FFile[j+1];
            FFile[j+1] = temp;
            }
          }
        }
      }
}
//---------------------------------------------------------------------------
void __fastcall TFileList::RescanImmediate()
{
  Fill("",false,FSortMode,FSortDirection);
}
//---------------------------------------------------------------------------
void __fastcall TFileList::SetRescan(bool value)
{
  FRescan = value;
  FScanned = !value;
//  Fill("",true,smNone,false);         //check for error ??!
//  FTimer->Enabled = value;
}
//---------------------------------------------------------------------------
/*AnsiString __fastcall TFileList::SetName (AnsiString S)
{
  if(S == "..")
    return "..";
  int k = S.AnsiPos(".");
  if(k)
    S.SetLength(k-1);
  return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::SetExtension (AnsiString S)//function needs to upgrade
{
  if(S == "..")
    return "";
  int k = S.AnsiPos(".");
  if(k)
    {
    S.Delete (1,k);
    return S;
    }
  return ""; 
} */
//---------------------------------------------------------------------------
int __fastcall TFileList::GetAttribute (int i)
{
  return FFile[i]->Attribute;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetName (int i)
{
  return FFile[i]->Name;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetExtension (int i)
{
  return FFile[i]->Extension;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetFullName (int i)
{
  return FFile[i]->FullName;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetPathName (int i)
{
  return FDirectory + FFile[i]->FullName;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::GetFileSize(int i)
{
  return FFile[i]->Size;
}
//---------------------------------------------------------------------------
TDateTime __fastcall TFileList::GetFileTime(int i)
{
  return FileDateToDateTime(FFile[i]->Time);
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::GetSelected(int i)
{
  return FFile[i]->Selected;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::SetSelected(int i,bool s)
{
  FSelectedFiles += (FFile[i]->Selected ^ s)*(s - !s);
  FSelectedBytes += (FFile[i]->Selected ^ s)*(s - !s)*FFile[i]->Size;
  FFile[i]->Selected = s;
}
//---------------------------------------------------------------------------
TFile* __fastcall TFileList::GetFile(int i)
{
  return FFile[i];
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::GetRoot(int i)
{
  return FFile[i]->Root;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::GetIndex(TFile* file)
{
  for (int i = 0; i < FFiles; i++)
    if(FFile[i]==file)
      return i;
  return -1;
}
//---------------------------------------------------------------------------
/*void __fastcall Insert(int i, const System::AnsiString S)
{

} */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TFile::TFile(TFileList* owner,AnsiString name) : TPersistent()
{
  int k = 1;
  FNameSum = CalculateNameSum(name);
  if(name != ".." || (k = name.LastDelimiter("."))!=0)
    {
    FExtension = name;
    name.SetLength(k-1);
    FExtension.Delete (1,k);
    }
  FName = name;
  FOwner = owner;
  UpdateInfo();
}
//---------------------------------------------------------------------------
__fastcall TFile::TFile(TFileList *owner,TSearchRec *ts) : TPersistent()
{
  int k = 1;
  FNameSum = CalculateNameSum(ts->Name);
  if((ts->Name != "..") && (k = (ts->Name).LastDelimiter("."))!=0)
    {
    FExtension = ts->Name;
    ts->Name.SetLength(k-1);
    FExtension.Delete (1,k);
    }
  FName = ts->Name;
  FSize = ts->Size;
  FTime = ts->Time;
  FAttribute = ts->Attr;
  FOwner = owner;
}
//---------------------------------------------------------------------------
__fastcall TFile::TFile(TFileList* owner,TFile *file) : TPersistent()
{
  FName = file->Name;
  FExtension = file->Extension;
  FNameSum = CalculateNameSum(FullName);
  FSize = file->Size;
  FTime = file->Time;
  FAttribute = file->Attribute;
  FSelected = file->Selected;
  FVerified = file->Verified;
  FRoot = file->Root;
  FOrigin = file;
  FOwner = owner;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFile::GetFullName ()
{
  if(FExtension!="")
    return FName+"."+FExtension;
  return FName;
}
//---------------------------------------------------------------------------
void __fastcall TFile::UpdateInfo()
{
  FAttribute = Sysutils::FileGetAttr(GetFullName());
  //????
}
//---------------------------------------------------------------------------
void __fastcall TFile::UpdateInfo(TSearchRec *ts)
{
  bool k = FSelected;
  if(FOwner && k)
    FOwner->Selected[FNumber] = false;
  if(FOwner && !IsDirectory())
    FOwner->UpdateDataBytes(ts->Size - FSize);
  FSize = ts->Size;
  FTime = ts->Time;
  FAttribute = ts->Attr;
  if(FOwner && k)
    FOwner->Selected[FNumber] = true;
}
//---------------------------------------------------------------------------
bool __fastcall TFile::IsDirectory()
{
  return (FAttribute & faDirectory) && !(FAttribute & faVolumeID); 
}
//---------------------------------------------------------------------------
void __fastcall TFile::SetSelected(bool f)
{
  FSelected = f;
  if(FOrigin)
    FOrigin->Owner->Selected[FOrigin->Owner->Index[FOrigin]] = f;
}
//---------------------------------------------------------------------------
bool __fastcall TFile::AddPathToName(AnsiString path)
{
    //TODO: Add your source code here
    //warn: path must end on '\\'
  path += FName;
  FName = path;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TFile::SetOwner(TFileList* value)
{
  FOwner = value;  
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


static inline void ValidCtrCheck(TDriveControl *)
{
    new TDriveControl(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDriveControl::TDriveControl(TComponent* Owner)
    : TComponent (Owner)
{
  FCopyDialog = new TCopyDialog(this);
  FProgressDlg = new TCopyProgress(this);
  FDeleteProgress = new TDeleteProgressForm(this);
  FStartUpInfo = new STARTUPINFO;
  FProcessInfo = new LPPROCESS_INFORMATION[MAXPROCESSES];
  
  FFileList = new TFileList *[MAXDIRLISTS];
  FMaxDirLists = MAXDIRLISTS;
  //process settings
/*  FStartUpInfo->cb = sizeof (STARTUPINFO);
  FStartUpInfo->dwFlags = STARTF_FORCEONFEEDBACK|STARTF_USESHOWWINDOW|STARTF_USEPOSITION|STARTF_USESIZE|STARTF_USECOUNTCHARS|STARTF_USEFILLATTRIBUTE|STARTF_USESTDHANDLES;
  FStartUpInfo->lpDesktop = NULL;
  FStartUpInfo->lpTitle = NULL;
  FStartUpInfo->dwX = 70;
  FStartUpInfo->dwY = 40;
  FStartUpInfo->dwXSize = 500;
  FStartUpInfo->dwYSize = 400;
  FStartUpInfo->dwXCountChars = 50;
  FStartUpInfo->dwYCountChars = 40;
  FStartUpInfo->wShowWindow = SW_SHOWDEFAULT;*/

  //drives settings
  FDriveSet = GetLogicalDrives();
  int i, j = 0, len = GetLogicalDriveStrings(0,NULL);
  FDriveStrings = new char [len];
  GetLogicalDriveStrings(len,FDriveStrings);
  for(i = 0; i < LOGICDRIVES; i++)
    {
    if((FDriveSet>>i) & 1)
      {
      FDrive[j] = new TDrive(ExtractFilePath(AnsiString(FDriveStrings + j*4)));
      j++;
      }
    }
  for(i = 0; i < FIXEDDRIVES; i++)
    {
    FDrive[j] = new TDrive("");
    j++;
    }
  
  FDrives = j;
  SetCurrentDir(Sysutils::GetCurrentDir());
  FMemoryBuffer = 4*1024*1024;
  FRWConst = 512*1024;
}
//---------------------------------------------------------------------------
__fastcall TDriveControl::~TDriveControl()
{
  for (int i = 0; i < FDirs; i++)
    delete FFileList[i];
  delete FFileList;
  int i;
  delete FStartUpInfo;
  for(i = 0; i < FProcesses; i++)
    delete FProcessInfo[i];
  delete FProcessInfo;
  delete FDeleteProgress;
  delete FCopyDialog;
  delete FProgressDlg;
  delete FDriveStrings;
  for(i = 0; i < MAXDRIVES; i++)
    if((FDriveSet>>i) & 1)
      delete FDrive[i];
//  delete FDrive;
}
//---------------------------------------------------------------------------
bool __fastcall TDriveControl::RegisterPanel(TFilePanel* panel)
{
  if(FPanels == MAXPANELS)
    return false;
  FPanel[FPanels] = panel;
  FPanels++;
  if(FPanels == 1)
    {
    SetCurrentDir(panel->CurrentDrive);
//    FCurrentDrive = panel->CurrentDrive;
//    FCurrentDir = panel->CurrentDir;
    FActivePanel = panel;
    }
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::Copy(int)
{
  return CopyMove(false);
}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::Move(int)
{
  return CopyMove(true);
}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::Enter(int i)
{
  TFile *file = ActivePanel->ActiveFile;
  AnsiString str;
  if(file->IsDirectory())
    {//verify for drivecontol is NULL;
    if(!ChangeDir(file->FullName) && !SetCurrentDir(FCurrentDrive + "\\"))
      {
      return -1;//dir is unchangable
      }
    NewDir(FCurrentDir); //includes search in pushed dirs and pushdir functions
    }
  else
    {
    ExecuteFile(file->FullName);
    }
  return 0;//ok


}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::Delete(int)
{
  //
//  TFile *file;
  TFileList* filelist = FActivePanel->FileList[0];
  int k = FActivePanel->ActiveIndex;//i = 0;
  TMsgDlgButtons btn;
  btn << mbYes << mbNo;
  if(filelist->SelectedFiles)
    {
    if(MessageDlg(("Do you wish to erase " + AnsiString(filelist->SelectedFiles) + " selected files"),mtConfirmation,btn,-1) == mrNo)
      return -1;//not deleted
    }
  else
    {
    if(filelist->File[k]->IsDirectory())
      {
      if(RemoveDir(filelist->FullPath[k]))
        return 0;
      if(MessageDlg("Directory " + filelist->FullName[k] + " is not empty! Erase it",mtConfirmation,btn,-1) == mrNo)
        return -1;//not deleted
      filelist->Selected[k] = true;
      }
    else
      {
      if(MessageDlg("Do you wish to erase selected file",mtConfirmation,btn,-1) == mrNo)
        return -1;//not deleted
      //add rescan
      return -((int) !EraseFile(filelist->FullPath[k],false));
      }
    }
  filelist = new TFileList(this,filelist,flmSelectedOnly);//get filelist to expand it
  FDeleteProgress->FileList = filelist;
  FDeleteProgress->DriveControl = this;
  FDeleteProgress->DeletingType = dtDirectory;
  FDeleteProgress->ShowModal();

/*  for(i = 0; i < filelist->Files; i++)
    if(!filelist->File[i]->IsDirectory())
      if(!EraseFile(filelist->FullPath[i],false))
        return -1;
  for(i = i - 1; i >= 0; i--)
    if(filelist->File[i]->IsDirectory())
      if(!EraseFile(filelist->FullPath[i],true))
        return -1;*/
  delete filelist;
//  FActivePanel->FileList[0]->Rescan = true;  //set rescan directories to true for active file panel
  FActivePanel->ReScanDir();
  FActivePanel->Repaint();
  return 0;
}
//---------------------------------------------------------------------------
bool __fastcall TDriveControl::EraseFile(AnsiString name,bool dir)
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
//---------------------------------------------------------------------------
int __fastcall TDriveControl::CopyMove(bool move)
{
  int active = 0;
  bool noselect = false;
  if(FPanel[1]->Active)
    active = 1;
  AnsiString source, destname, target = FPanel[!active]->CurrentDir;
  TFileList* filelist = FActivePanel->FileList[0];
  if(!filelist->SelectedFiles)
    {
    if(filelist->File[FActivePanel->ActiveIndex]->IsDirectory())
//    if(filelist->Attribute[FPanel[active]->ActiveIndex] & faDirectory)
      {
      if(filelist->Name[FActivePanel->ActiveIndex]=="..")
        return -1;
      source = "Copy the directory ";
      }
    else
      source = "Copy the file ";
    source += filelist->FullName[FActivePanel->ActiveIndex];
    if(FActivePanel->CurrentDir != FPanel[!active]->CurrentDir)
      {
      if(target.IsPathDelimiter(target.Length()))
        target.SetLength(target.Length() - 1);
      if(filelist->File[FActivePanel->ActiveIndex]->IsDirectory())
        FCopyDialog->Destination->Text = target + "\\";
      else
        FCopyDialog->Destination->Text = target + "\\" + filelist->FullName[FActivePanel->ActiveIndex];
      }
    }
  else
    {
    source = "Copy the " + (AnsiString) filelist->SelectedFiles + " files";
    if(FPanel[active]->CurrentDir != FPanel[!active]->CurrentDir)
      FCopyDialog->Destination->Text = target;
    }
  FCopyDialog->Source->Caption = source;
//  FCopyDialog->Destination->Items->Add(target);
  if(FCopyDialog->ShowModal()==mrCancel)
    return -1;
  int i,k;
  for(i = 0; i < FCopyDialog->Destination->Items->Count; i++)
    if(FCopyDialog->Destination->Items->Strings[i] == FCopyDialog->Destination->Text)
      break;
  if(i == FCopyDialog->Destination->Items->Count)
    FCopyDialog->Destination->Items->Add(FCopyDialog->Destination->Text);

  target = FCopyDialog->Destination->Text;
  if(!filelist->SelectedFiles)
    {
    noselect = true;
    filelist->Selected[FActivePanel->ActiveIndex] = true;
    }

  //analize
  //  проанализировать target на файл или директорию, если директория, то создать если не создана
  TSearchRec Rec;
  bool file = false;
  if(!target.IsPathDelimiter(target.Length()))
    {
    if(FindFirst(target,faAnyFile,Rec)==0)
      {
      if(!(Rec.Attr & faDirectory))
        file = true;   
      else
        target += "\\";
      }
    else
      {
      file = true;
      if((k = FindFirst(ExtractFilePath(target),faAnyFile,Rec))!=0 && k!=2)    //2 - ?; 3 - dir is not exist
        ForceDirectories(ExtractFilePath(target));
      }
    }
  if(file)
    {
    destname = ExtractFileName(target);
    target = ExtractFilePath(target);
    }
  source = FActivePanel->CurrentDir;
  //new file list
  filelist = new TFileList(this,filelist,flmSelectedOnly);//get filelist to expand it
  COPYPROGRESSDLGSTRUCT ii;
  ii.FileList = filelist;
  ii.TargetDir = target;
  ii.SourceDir = source;
  ii.DestinationName = destname;
  ii.MemoryFree = FMemoryBuffer;
  ii.Portion = FRWConst;
  ii.VerifyMode = FCopyDialog->VerifyMode;
  ii.CopyMode = FCopyDialog->CopyMode;
  ii.Interval = 1;//???
  FProgressDlg->Initialize(ii);
  FProgressDlg->ShowModal();

  delete  filelist;

  if(noselect)
    FActivePanel->FileList[0]->Selected[FActivePanel->ActiveIndex] = false;

  FPanel[!active]->ReScanDir();
  FPanel[!active]->Repaint();
  if(move)
    {
    FActivePanel->ReScanDir();
    FActivePanel->Repaint();
    }
  return 0;
}
//private--------------------------------------------------------------------
AnsiString __fastcall TDriveControl::GetNewName(AnsiString srcpath,AnsiString mask,AnsiString srcname)
{
  int j,k;
  bool brfl;
  AnsiString destname, destpath = ExtractFilePath(mask);
  mask = ExtractFileName(mask);
        if(mask=="")
          destname = srcname;
        else
          {
          destname = "";
          k = 0;
          for(j = 0;j <= mask.Length(); j++)
            {
            if(mask[j]!='*' && mask[j]!='?')
              destname += mask[j];
            else
              {
              if(mask[j]=='?')
                destname += srcname[k];
              else
                {
                while(j <= mask.Length() && mask[j]!='.')
                  {
                  j++;
                  }
                brfl = srcname[j]!='.';
                j--;
                while(k <= srcname.Length() && srcname[k]!='.')//????
                  {
                  destname += srcname[k];
                  k++;
                  }
//                if(srcname[k]=='.')
                  k--;
                if(brfl)
                  break;
                }
              }
            k++;
            }
          }
  return srcpath + "\\" + destname;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDriveControl::GetCurrentDir()
{
  return Drive[FCurrentDrive]->CurrentDir;
}
//---------------------------------------------------------------------------
bool __fastcall TDriveControl::SetCurrentDir(AnsiString dir)
{
  //extract drive
  dir = VerifyDir(dir);
  if(!Sysutils::SetCurrentDir(dir))
    {
    FLastError = 0x1;//Invalid directory in path
    //throw EInvalidPath;
    return false;
    }
  FCurrentDir = Sysutils::GetCurrentDir();
  FCurrentDrive = ExtractFileDrive(FCurrentDir);
  Drive[FCurrentDrive]->CurrentDir = dir;
  Drive[FCurrentDrive]->UpdateInfo();
  return true;//ok
}
//---------------------------------------------------------------------------
const char* TDriveControl::GetDriveString(AnsiString i)
{
  return ExtractFileDrive(i).c_str();
//  return FDriveStrings + (int)i*4;
}
//---------------------------------------------------------------------------
bool __fastcall TDriveControl::SetCurrentDrive(AnsiString drive)
{
/*  if(drive == dlCurrent || !FDrive[drive])
    return false;
//  FDrive[FCurrentDrive]->Active = false;
  FCurrentDrive = drive;
  FDrive[drive]->UpdateInfo();
//  FDrive[drive]->Active = true;
  return true;*/
  if(drive == "")
    drive == ExtractFileDrive(GetCurrentDir());

  drive = ExtractFileDrive(drive);
  if(!Drive[drive])
    return false;
//  Drive[FCurrentDrive]->Active = false;
  FCurrentDrive = drive;
  Drive[drive]->UpdateInfo();
//  Drive[drive]->Active = true;
  return true;
  
}
//---------------------------------------------------------------------------
TDrive* __fastcall TDriveControl::GetDrive(AnsiString d)
{
  d = d.UpperCase();
  for(int i = 0; i < FDrives; i++)
    if (FDrive[i]->Name == d)
      return FDrive[i];
  return NULL;
//  return FDrive[d];
}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::KeyProcess(int key)
{
  return FActivePanel->KeyProcess(key);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDriveControl::VerifyDir(AnsiString dir)
{
  if(dir.LastDelimiter("\\") != dir.Length())
    dir += "\\";
  return dir;
}
//---------------------------------------------------------------------------
bool __fastcall TDriveControl::ChangeDir(AnsiString dir)
{
  return SetCurrentDir(VerifyDir(FCurrentDir) + dir);
}
//---------------------------------------------------------------------------
int __fastcall TDriveControl::ExecuteFile(AnsiString file)
{
  int k = 0;
  FProcessInfo[FProcesses] = new PROCESS_INFORMATION;
  AnsiString name = VerifyDir(FCurrentDir) + file;
//  if(!CreateProcess(name.c_str(),NULL,NULL,NULL,false,CREATE_DEFAULT_ERROR_MODE|NORMAL_PRIORITY_CLASS|CREATE_SEPARATE_WOW_VDM|DETACHED_PROCESS,NULL,FCurrentDir.c_str(),FStartUpInfo,FProcessInfo[FProcesses]))
  if(!CreateProcess(name.c_str(),NULL,NULL,NULL,false,NORMAL_PRIORITY_CLASS|CREATE_SEPARATE_WOW_VDM|DETACHED_PROCESS,NULL,NULL,FStartUpInfo,FProcessInfo[FProcesses]))
    {
    k = GetLastError();
    return k;
    //throw
    }
  FProcesses++;
  return 0;//okey
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::NewDir(AnsiString dir)
{
  AnsiString l;
  int dirs = FDirs, i, j;
  TFileList *k = NULL;
  SaveIndexes(0);
  dir = VerifyDir(dir);
  for(i = 0; i < FDirs; i++)
    if(FFileList[i]->Directory == dir)
      {
      dirs = i;
      break;
      }

  if(dirs == FDirs)
    {
    if(dirs == FMaxDirLists)
      {
      dirs--;
      k = FFileList[dirs];
      }
    else
      {
      k = new TFileList(this);
      FDirs++;
      }
    ScanDir(dir,k);
    if(FDirs > 1 && FFileList[0]->FullName[FActiveIndex] == "..")
      {
      l = FFileList[0]->Directory;
      l.SetLength(l.Length() - 1);
      l = ExtractFileName(l);
      for(j = 0; j < k->Files; j++)
        if(l == k->FullName[j])
          {
          k->PanelActiveIndex = j;
          break;
          }
      }
    }
  else
    {
    k = FFileList[dirs];
    if(FRescan)
      ReScanDir(k);
    }

  for(i = dirs; i >= 1; i--)
    FFileList[i] = FFileList[i - 1];
  FFileList[0] = k;
  FCurrentDir = FFileList[0]->Directory;
  FCurrentDrive = ExtractFileDrive(FCurrentDir);
  LoadIndexes(0);
//  Reset();             //reset visible properties of panel
  FActivePanel->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::SaveIndexes(int i)
{
  if(!FFileList[i])
    return;
  FFileList[i]->PanelBeginIndex = FBeginIndex;
  FFileList[i]->PanelEndIndex = FEndIndex;
  FFileList[i]->PanelActiveIndex = FActiveIndex;
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::LoadIndexes(int i)
{
  if(!FFileList[i])
    return;
  FBeginIndex = FFileList[i]->PanelBeginIndex;
  FEndIndex = FFileList[i]->PanelEndIndex;
  FActiveIndex = FFileList[i]->PanelActiveIndex;
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::ScanDir(AnsiString dir,TFileList *list)
{
  list->Main = true;
  if(!list->Fill(dir,true,FSortMode,FSortDirection))
    return;
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::ReScanDir(TFileList *list)
{
  list->Main = true;
  if(!list->Fill("",false,FSortMode,FSortDirection))
    return;
}
//---------------------------------------------------------------------------
void __fastcall TDriveControl::ReScanDir()
{
  ReScanDir(FFileList[0]);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TDrive::TDrive(AnsiString buffer)
{
  FCurrentDir = buffer.UpperCase();
  FName = ExtractFileDrive(FCurrentDir);
  FLetter = (TDriveLetter) (FName[1]-'A');
  switch(GetDriveType(buffer.c_str()))
    {
    case 0:
      FType = dt1NotDetected;
      break;
    case 1:
      FType = dt1NotFound;
      break;
    case DRIVE_REMOVABLE:
      FType = dt1Floppy;
      break;
    case DRIVE_FIXED:
      FType = dt1Hard;
      break;
    case DRIVE_REMOTE:
      FType = dt1Network;
      break;
    case DRIVE_CDROM:
      FType = dt1CDRom;
      break;
    case DRIVE_RAMDISK:
      FType = dt1Virtual;
    }
}
//---------------------------------------------------------------------------
#pragma warn -sig
void __fastcall TDrive::UpdateInfo()
{
  FFree = DiskFree((int)FLetter + 1);
  FSize = DiskSize((int)FLetter + 1);
}
#pragma warn .sig
//---------------------------------------------------------------------------
bool __fastcall TDrive::SetCurrentDir(AnsiString newdir)
{
  FCurrentDir = newdir;
  return true;
}
//---------------------------------------------------------------------------
/*bool __fastcall TDrive::SetActive(bool active)
{
  if(!Sysutils::SetCurrentDir(FCurrentDir))
    {
    //Throw EInvalidPath;
    return;
    }
  FActive = active;
} */
//---------------------------------------------------------------------------
namespace Drivecontrol
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TDriveControl)};
        RegisterComponents("Additional", classes, 0);
    }
}
//---------------------------------------------------------------------------


void __fastcall TDriveControl::SetRescan(bool value)
{
  for(int i = 0; i < FDirs; i++)
    {
    if(i)
      FFileList[i]->Main = false;
    else
      FFileList[i]->Main = true;
    FFileList[i]->Rescan = value;
    }
/*  for(int i = 0; i < FPanels; i++)
    FPanel[i]->Rescan = value;*/
}

bool __fastcall TDriveControl::GetRescan()
{
  bool k = false;
  for(int i = 0; i < FDirs; i++)
    k |= FFileList[i]->Rescan;
  return k;
/*  bool k = false;
  for(int i = 0; i < FPanels; i++)
    k |= FPanel[i]->Rescan;
  return k;*/
}



AnsiString __fastcall TFile::GetFullPath()
{
  AnsiString k;
  if(FOwner)
    {
    k = FOwner->Directory;
    if(k.LastDelimiter("\\") != k.Length())
      k += "\\";
    }
  return k + GetFullName();
}



void __fastcall TDriveControl::SetActivePanel(TFilePanel *panel)
{
  SetCurrentDir(panel->CurrentDir);
  FActivePanel = panel;
}



void __fastcall TFileList::Sort()
{
  Sort(FSortMode,FSortDirection);
}


void __fastcall TFileList::SetSortMode(TSortMode mode)
{
  if(FSortMode == mode)
    return;
  FSortMode = mode;
  if(FAutoSorting)
    Sort(mode,FSortDirection);
}


void __fastcall TFileList::SetSortDirection(bool value)
{
  if(FSortDirection == value)
    return;
  FSortDirection = value;
  if(FAutoSorting)
    Sort(FSortMode,value);
}



__int64 __fastcall TFile::CalculateNameSum(AnsiString name)
{
  __int64 sum = 0,k;
  int l = name.Length();
  for(int i = 1; i <= l; i++)
    {
    k = name[i];
    k *= k;
    sum += k;
    }
  return sqrt(k);
}


