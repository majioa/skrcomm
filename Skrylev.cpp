//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Skrylev.res");
USEFORM("SkrylevCommander.cpp", SkrylevForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Skrylev Commander";
                 Application->CreateForm(__classid(TSkrylevForm), &SkrylevForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
