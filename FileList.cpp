//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FileList.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFileList *)
{
    new TFileList();
}
//---------------------------------------------------------------------------
__fastcall TFileList::TFileList()
    : TPersistent()
{
  FBuffer = new TFile*[MAXFILES];
}
//---------------------------------------------------------------------------
__fastcall TFileList::~TFileList()
{
  delete FBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::Clear()
{
  for(int i=0;i<FFiles;i++)
    delete FBuffer[i];
  FFiles = 0;
  FSelectedFiles = 0;
  FSelectedBytes = 0;
  FDataFiles = 0;
  FDataBytes = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Delete(int i)
{
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::Add(TFile* file)
{
/*  if(FFiles == 1024)
    return false;
  FBuffer[FFiles] = new TFile();
  FBuffer[FFiles]->Name = file->Name;
  FBuffer[FFiles]->Attribute = file->Attribute;
  FBuffer[FFiles]->Time = file->Time;
  FBuffer[FFiles]->Size = file->Size;
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
    if(FBuffer[i]->FullName == name)
      return i; 
  return -1;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::Fill(System::AnsiString path, bool f)
{
  int e,k,newselect = 0;
  //correct path
  if(f)
    Clear();
  else
    {
    //half clear
    FSelectedBytes = 0;
    FDataFiles = 0;
    FDataBytes = 0;
    }
  TSearchRec F;
  if(!path.IsPathDelimiter(path.Length()))
    path += AnsiString ("\\*.*");
  else
    path += AnsiString ("*.*");
  if((e = FindFirst(path ,faReadOnly|faHidden|faSysFile|faDirectory|faArchive,F))!=0)
    {
    FindClose(F);
    return e;
    }
  do {
    if(F.Name != ".")
      {
      if((k = FileExist(F.Name))==-1)
        {
        if(FFiles == MAXFILES)
          return 1;//if cannot add return error code 1
        FBuffer[FFiles] = new TFile(&F);
        FBuffer[FFiles]->Verified = true;
        if(!FBuffer[FFiles]->IsDirectory())
          {
          FDataBytes += F.Size;
          FDataFiles++;
          }
        FFiles++;
        }
      else
        {
        FBuffer[k]->UpdateInfo(&F);
        FBuffer[k]->Verified = true;
        if(!FBuffer[k]->IsDirectory())
          {
          FDataBytes += F.Size;
          FDataFiles++;
          }
        if(FBuffer[k]->Selected)
          {
          FSelectedBytes += F.Size;
          newselect++;
          }
        }
      }
    if((e = FindNext (F))!=18 && e)
      {
      FindClose(F);
      return e;
      }
    } while (e!=18);
  FindClose(F);
  FSelectedFiles = newselect;
  return 0;
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
  return FBuffer[i]->Attribute;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetName (int i)
{
  return FBuffer[i]->Name;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetExtension (int i)
{
  return FBuffer[i]->Extension;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFileList::GetFullName (int i)
{
  return FBuffer[i]->FullName;
}
//---------------------------------------------------------------------------
int __fastcall TFileList::GetFileSize(int i)
{
  return FBuffer[i]->Size;
}
//---------------------------------------------------------------------------
TDateTime __fastcall TFileList::GetFileTime(int i)
{
  return FileDateToDateTime(FBuffer[i]->Time);
}
//---------------------------------------------------------------------------
bool __fastcall TFileList::GetSelected(int i)
{
  return FBuffer[i]->Selected;
}
//---------------------------------------------------------------------------
void __fastcall TFileList::SetSelected(int i,bool s)
{
  FSelectedFiles += (FBuffer[i]->Selected ^ s)*(s - !s);
  FSelectedBytes += (FBuffer[i]->Selected ^ s)*(s - !s)*FBuffer[i]->Size;
  FBuffer[i]->Selected = s;
}
//---------------------------------------------------------------------------
TFile* __fastcall TFileList::GetFile(int i)
{
  return FBuffer[i];
}
//---------------------------------------------------------------------------
/*void __fastcall Insert(int i, const System::AnsiString S)
{

} */
//---------------------------------------------------------------------------
__fastcall TFile::TFile(AnsiString name)
{
  int k = 1;
  if(name != ".." || (k = name.LastDelimiter("."))!=0)
    {
    FExtension = name;
    name.SetLength(k-1);
    FExtension.Delete (1,k);
    }
  FName = name;
  UpdateInfo();
}
//---------------------------------------------------------------------------
__fastcall TFile::TFile(TSearchRec *ts)
{
  int k = 1;
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
  FSize = ts->Size;
  FTime = ts->Time;
  FAttribute = ts->Attr;
}
//---------------------------------------------------------------------------
bool __fastcall TFile::IsDirectory()
{
  return (FAttribute & faDirectory) && !(FAttribute & faVolumeID); 
}
//---------------------------------------------------------------------------

