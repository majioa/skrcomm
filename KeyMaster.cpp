//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "KeyMaster.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TKeyMaster *)
{
    new TKeyMaster(NULL);
}
//---------------------------------------------------------------------------
__fastcall TKeyMaster::TKeyMaster(TComponent* Owner) : TComponent(Owner)
{
  FKey = new TKey*[MAXKEYS];
}
//---------------------------------------------------------------------------
__fastcall TKeyMaster::~TKeyMaster()
{
  for(int i = 0; i < FKeys; i++)
    delete FKey[i];
  delete FKey;
}
//---------------------------------------------------------------------------
bool __fastcall TKeyMaster::RegisterKeys (Word* keys, int num, Classes::TShiftState shift, TKeyFunc func)
{
  for ( int i = 0; i < num; i++ )
    if(!RegisterKey(keys[i],shift,func))
      return false;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TKeyMaster::RegisterKey (Word key, Classes::TShiftState shift, TKeyFunc func)
{
  if(FKeys == MAXKEYS)
    {
    FLastError = KE_BUFFERFULL;
    return false;
    }
  for(int i = 0; i < FKeys; i++)
    if(FKey[i]->Key == key && FKey[i]->Shift == shift)
      {
      FLastError = KE_ALREADYEXIST;
      return false;
      }
  FKey[FKeys] = new TKey(key,shift,func);
  FKeys++;
  FLastError = KE_NO;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TKeyMaster::UnRegisterKey (Word key, TShiftState shift)
{
  int i;
  for(i = 0; i < FKeys; i++)
    if(FKey[i]->Key == key && FKey[i]->Shift == shift)
      {
      delete FKey[i];
      break;
      }
  if(i == FKeys)
    {
    FLastError = KE_KEYNOTFOUND;
    return false;
    }
  for(i; i < FKeys; ++i)
    FKey[i - 1] == FKey[i];
  FLastError = KE_NO;
  return true;
}
//---------------------------------------------------------------------------
/*void __fastcall TKeyMaster::KeyPress (Word &key)
{
}
//---------------------------------------------------------------------------
void __fastcall TKeyMaster::KeyDown (Word &key, Classes::TShiftState shift)
{
  FShift = shift;
}
//---------------------------------------------------------------------------
void __fastcall TKeyMaster::KeyUp (Word &key, Classes::TShiftState shift)
{
  FShift = shift;
  for(int i=0;i<FKeys;i++)
    if(FKey[i]->Key == key && FKey[i]->Shift == FShift)
      {
      (FKey[i]->Function)();
      return;
      }
} */
//---------------------------------------------------------------------------
void __fastcall TKeyMaster::WndProc(Messages::TMessage & message)
{
  switch(message.Msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      switch(message.WParam)
        {
        case VK_SHIFT:
          FShift<<ssShift;
          break;
        case VK_MENU:
          FShift<<ssAlt;
          break;
        case VK_CONTROL:
          FShift<<ssCtrl;
          break;
        default:
          ExecuteFunction(message.WParam);
          }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
      switch(message.WParam)
        {
        case VK_SHIFT:
          FShift>>ssShift;
          break;
        case VK_MENU:
          FShift>>ssAlt;
          break;
        case VK_CONTROL:
          FShift>>ssCtrl;
        case VK_TAB:
          ExecuteFunction(VK_TAB);
        }
    }
}

void __fastcall TKeyMaster::ExecuteFunction(int key)
{
   for(int i = 0; i < FKeys; i++)
     if(FKey[i]->Key == key && FKey[i]->Shift == FShift)
       {
       (FKey[i]->Function)(key);
       return;
       }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TKey::TKey(Word key, Classes::TShiftState shift, TKeyFunc func) : TPersistent()
{
  FKey = key;
  FShift = shift;
  FFunc = func;
}
//---------------------------------------------------------------------------
namespace Keymaster
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TKeyMaster)};
        RegisterComponents("System", classes, 0);
    }
}
//---------------------------------------------------------------------------


/*void __fastcall TKeyMaster::KeyDown(TWMKey key)
{
    //TODO: Add your source code here
    switch(key.CharCode)
      {
      case VK_SHIFT:
        FShift << ssShift;
        return;
      case VK_MENU:
        FShift << ssAlt;
        return;
      case VK_CONTROL:
        FShift << ssCtrl;
        return;
      }

  for(int i=0;i<FKeys;i++)
    if(FKey[i]->Key == key.CharCode && FKey[i]->Shift == FShift)
      {
      (FKey[i]->Function)();
      return;
      }
}


void __fastcall TKeyMaster::KeyUp(TWMKey key)
{
    //TODO: Add your source code here
    switch(key.CharCode)
      {
      case VK_SHIFT:
        FShift >> ssShift;
        break;
      case VK_MENU:
        FShift >> ssAlt;
        break;
      case VK_CONTROL:
        FShift >> ssCtrl;
        break;
      }
} */



