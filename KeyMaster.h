//---------------------------------------------------------------------------
#ifndef KeyMasterH
#define KeyMasterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#define MAXKEYS 1024

#define KE_NO            0
#define KE_BUFFERFULL    1
#define KE_KEYNOTFOUND   2
#define KE_ALREADYEXIST  3


typedef int __fastcall (__closure *TKeyFunc) (int);

class TKey : public TPersistent
{
private:
  Word FKey;
  TShiftState FShift;
  TKeyFunc FFunc;
protected:
public:
  __fastcall TKey(Word, Classes::TShiftState, TKeyFunc);
  __property Word Key = {read = FKey};
  __property TShiftState Shift = {read = FShift};
  __property TKeyFunc Function = {read = FFunc};
__published:
};

class PACKAGE TKeyMaster : public TComponent
{
private:
  TKey** FKey;
  int FKeys;
  int FLastError;//errors: 0 - no error, 1 - buffer is full, 2 - key is not found
  TShiftState FShift;
  void __fastcall ExecuteFunction(int);
protected:
/*    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, KeyDown)
        VCL_MESSAGE_HANDLER(WM_KEYUP, TWMKey, KeyUp)
        VCL_MESSAGE_HANDLER(WM_SYSKEYDOWN, TWMKey, KeyDown)
        VCL_MESSAGE_HANDLER(WM_SYSKEYUP, TWMKey, KeyUp)
    END_MESSAGE_MAP(TComponent)*/
/*BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_CHANGECOLOR, TMessage, CMChangeColor)
END_MESSAGE_MAP(TControl)*/
public:
  __fastcall TKeyMaster(TComponent*);
  __fastcall ~TKeyMaster();
  bool __fastcall RegisterKeys (Word*, int, Classes::TShiftState, TKeyFunc);
  bool __fastcall RegisterKey (Word, Classes::TShiftState, TKeyFunc);
  bool __fastcall UnRegisterKey (Word, TShiftState);
/*  void __fastcall KeyPress (Word &);
  void __fastcall KeyDown (Word &, Classes::TShiftState);
  void __fastcall KeyUp (Word &, Classes::TShiftState);
  void __fastcall KeyDown(TWMKey);
  void __fastcall KeyUp(TWMKey);*/
    void __fastcall WndProc(Messages::TMessage & message);
  __property int LastError = {read = FLastError};
__published:
};
//---------------------------------------------------------------------------
#endif
 