//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Pavlo.res");
USEPACKAGE("vcl40.bpi");
USEFORM("CopyDlg.cpp", CopyDialog);
USEUNIT("DriveControl.cpp");
USEUNIT("FilePanel.cpp");
USEUNIT("KeyMaster.cpp");
USEPACKAGE("VCLX40.bpi");
USEFORM("CopyPrg.cpp", CopyProgress);
USEFORM("DeletePrg.cpp", DeleteProgressForm);
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
