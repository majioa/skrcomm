//---------------------------------------------------------------------------
#ifndef FileListH
#define FileListH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#define MAXFILES 16384

class TFile : public TPersistent
{
private:
  AnsiString FName;
  AnsiString FExtension;
  int FSize;
  int FTime;
  int FAttribute;
  bool FSelected;
  bool FVerified;
  AnsiString __fastcall GetFullName();
public:
  //properties
  __property AnsiString FullName = {read = GetFullName};
  __property AnsiString Name = {read = FName};
  __property AnsiString Extension = {read = FExtension};
  __property int Size = {read = FSize};
  __property int Time = {read = FTime};
  __property int Attribute = {read = FAttribute};
  __property bool Selected = {read = FSelected, write = FSelected};
  __property bool Verified = {read = FVerified, write = FVerified};
  //constructor
  __fastcall TFile (AnsiString);
  __fastcall TFile (TSearchRec *);
  //methods
  void __fastcall UpdateInfo();
  void __fastcall UpdateInfo(TSearchRec *);
  bool __fastcall IsDirectory();
};

class PACKAGE TFileList : public TPersistent
{
private:
  TFile** FBuffer;
  int FFiles;
  int FSelectedFiles;
  int FSelectedBytes;
  int FDataFiles;
  int FDataBytes;
//  AnsiString __fastcall SetName (AnsiString);
//  AnsiString __fastcall SetExtension (AnsiString);
  int __fastcall GetAttribute (int);
  AnsiString __fastcall GetName (int);
  AnsiString __fastcall GetExtension (int);
  AnsiString __fastcall GetFullName (int);
  int __fastcall GetFileSize(int); 
  TDateTime __fastcall GetFileTime(int);
  bool __fastcall GetSelected(int);
  void __fastcall SetSelected(int,bool);
  TFile* __fastcall GetFile(int);
protected:
  int __fastcall FileExist(AnsiString); //may be public
public:
  //methods
  void __fastcall Clear();
  bool __fastcall Delete(int);
  bool __fastcall Add(TFile*);
  int __fastcall Fill(System::AnsiString,bool);
//  void __fastcall Insert(int, const System::AnsiString);
  //constructor
  __fastcall TFileList();
  //destructor
  __fastcall ~TFileList();
__published:
  __property TFile* File[int] = {read = GetFile};
  __property int Files = {read = FFiles};
  __property int SelectedFiles = {read = FSelectedFiles};
  __property int DataFiles = {read = FDataFiles};
  __property int SelectedBytes = {read = FSelectedBytes};
  __property int DataBytes = {read = FDataBytes};
  __property AnsiString Name[int] = {read = GetName};
  __property int Attribute[int] = {read = GetAttribute};
  __property int Size[int] = {read = GetFileSize};
  __property TDateTime Time[int] = {read = GetFileTime};
  __property AnsiString Extension[int] = {read = GetExtension};
  __property AnsiString FullName[int] = {read = GetFullName};
  __property bool Selected[int] = {read = GetSelected, write = SetSelected};
};
//---------------------------------------------------------------------------
#endif
