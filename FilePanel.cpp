//---------------------------------------------------------------------------
//#pragma resource "Copydlg.rc"
#include <vcl.h>
#pragma hdrstop

#include "FilePanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFilePanel *)
{
  new TFilePanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFilePanel::TFilePanel(TComponent* Owner)
    : TCustomControl(Owner)
{
  Color = clBlue;
//  FFileList = new TFileList *[MAXDIRLISTS];
  FFileColumns = 3;
//  FMaxDirLists = MAXDIRLISTS;

  FFileGroup = new TFileGroup *[FILEGROUPS];
  FFileGroup[0] = new TFileGroup;
  FFileGroup[0]->Name = "Directories";
  FFileGroup[0]->ExtensionList = "*.*";
  FFileGroup[0]->Attributes = faDirectory;
  FFileGroup[1] = new TFileGroup;
  FFileGroup[1]->Name = "Executable";
  FFileGroup[1]->ExtensionList = "*.com;*.exe";
  FFileGroup[1]->Attributes = faHidden|faSysFile|faReadOnly|faArchive;
  FFileGroup[2] = new TFileGroup;
  FFileGroup[2]->Name = "Archives";
  FFileGroup[2]->ExtensionList = "*.arj;*.rar";
  FFileGroup[2]->Attributes = faHidden|faSysFile|faReadOnly|faArchive;
  FFileGroups = 3;
  SetDefaultColors();
  SetDefalutSelectSymbols();

  Canvas->CopyMode = cmSrcCopy;
}
//---------------------------------------------------------------------------
__fastcall TFilePanel::~TFilePanel()
{
/*  for (int i = 0; i < FDirs; i++)
    delete FFileList[i];
  delete FFileList;*/
  for (int i = 0; i < FFileGroups; i++)
    delete FFileGroup[i];
  delete FFileGroup;
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::SetDefalutSelectSymbols()
{
  FHiddenSym = '¤';
  FSystemSym = '¤';
  FCommonSym = '¤';    //System+Hidden
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::SetDefaultColors()
{
  FFileGroup[0]->Color.Ink = clWhite;
  FFileGroup[0]->Color.Paper = clDkBlue;
  FFileGroup[1]->Color.Ink = clTeal;
  FFileGroup[1]->Color.Paper = clDkBlue;
  FFileGroup[2]->Color.Ink = clMaroon;
  FFileGroup[2]->Color.Paper = clDkBlue;
  FNormalText.Ink = clLtBlue;
  FNormalText.Paper = clDkBlue;
  FSelectedText.Ink = clGreen;
  FSelectedText.Paper = clDkBlue;
  FNormalCursor.Ink = clDkGray;
  FNormalCursor.Paper = clTeal;
  FSelectedCursor.Ink = clGreen;
  FSelectedCursor.Paper = clTeal;
  FListDivisor.Ink = clLtBlue;
  FListDivisor.Paper = clDkBlue;
  FActiveCatalog.Ink = clDkGray;
  FActiveCatalog.Paper = clLime;
  FPassiveCatalog.Ink = clLtBlue;
  FPassiveCatalog.Paper = clDkBlue;
  FColumnContents.Ink = clYellow;
  FColumnContents.Paper = clDkBlue;
  //disk line
  FDriveLetters.Ink = clLtBlue;
  FDriveLetters.Paper = clDkBlue;
  FSelectedDrive.Ink = clYellow;
  FSelectedDrive.Paper = clDkBlue;
  FDriveFrame.Ink = clLtBlue;
  FDriveFrame.Paper = clDkBlue;
  //info
  FActiveFile.Ink = clLtBlue;
  FActiveFile.Paper = clDkBlue;
  FTextSelectedFiles.Ink = clYellow;
  FTextSelectedFiles.Paper = clDkBlue;
  FQuantitySelected.Ink = clYellow;
  FQuantitySelected.Paper = clDkBlue;
  FAllText.Ink = clLtBlue;
  FAllText.Paper = clDkBlue;
  FAllDigits.Ink = clYellow;
  FAllDigits.Paper = clDkBlue;
  FFreeText.Ink = clLtBlue;
  FFreeText.Paper = clDkBlue;
  FFreeDigits.Ink = clYellow;
  FFreeDigits.Paper = clDkBlue;
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::SetFileColumns(int i)
{
  if(i>0 && i<4)
    FFileColumns = i;
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::SetDriveControl(TDriveControl* i)
{
  FDriveControl = i;
  switch (FInitDrive)
    {
    case dlDir:
      FCurrentDir = FInitDir;
      FCurrentDrive = ExtractFileDrive (FInitDir);
      break;
    case dlCurrent:
      FCurrentDir = Sysutils::GetCurrentDir();
      FCurrentDrive = ExtractFileDrive(Sysutils::GetCurrentDir().UpperCase());
      break;
    default:
      FCurrentDrive = AnsiString((char)(((int) FInitDrive) + 'A')) + ":";
      FCurrentDir = FCurrentDrive + "\\";
    }
  i->RegisterPanel(this);
//  i->CurrentDir = FCurrentDir;
// i->CurrentDrive = FCurrentDrive;
  NewDir(FCurrentDir);
  Refresh();
}
//---------------------------------------------------------------------------
TFileList *__fastcall TFilePanel::GetFileList(int i)
{
  return FFileList[i];
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::Paint()
{
//verify client area
  if(!FDriveControl)
    return;
  if(!FFileList[0]->Scanned)
    return;
  FPainted = true;
  int symbolwidth,height;
  AnsiString temp, temp1, temp2, temp3, temp4;
  int len, len1, len2, len3, len4;
  if(FFixedWidth)
    symbolwidth = Canvas->TextWidth(" ");
  else
    symbolwidth = Canvas->TextWidth("abcdefghijklmnopqrstuvwxyz")/26 + 2;
  height = Canvas->TextHeight("Copy");
  FSymbolWidth = symbolwidth;
  FSymbolHeight = height;
  int width = (Width - symbolwidth*2 - symbolwidth*(FFileColumns - 1)) / FFileColumns;//width are width w/out border frame
  FColumnWidth = width;
  int fullwidth = Width - symbolwidth*2;
  FFullWidth = fullwidth;
  //text output
  //path output
  ShowPath(FActive);
  int shift, shift1;
  FFileListHeight = Height - height - (height*FShowFileInfo + height*
    FShowFreeSpace + height*FShowFileSelectInfo + height*FShowFilesSpace +
    (FShowFileInfo | FShowFreeSpace | FShowFileSelectInfo | FShowFilesSpace)*
    height);
  FInfoBar = FFileListHeight + FSymbolHeight;
  int o = FInfoBar;
  //file list
  PaintColumns(pcFull,0);
  //cursor
  ShowCursor(FActiveIndex,FActive);
  //info output
  if(FShowFileInfo)
    {
    //file info
    //standart width = 220, so tabulators are +0, +140, +170, +190
    PaintFileInfo(o);
    o += FSymbolHeight;
    }
  if(FShowFileSelectInfo)
    {
    //file select info
    shift = symbolwidth;
    if(!FFileList[0]->SelectedFiles)
      {
      Canvas->Brush->Color = FTextSelectedFiles.Paper;
      Canvas->Font->Color = FTextSelectedFiles.Ink;
      temp = "Files is not selected";
      Canvas->TextRect(Rect(shift, o, fullwidth + symbolwidth, o + height), shift + (fullwidth - Canvas->TextWidth(temp))/2, o, temp);
      }
    else
      {
      temp = FloatToStrF(FFileList[0]->SelectedBytes ,ffNumber,10,0);
      temp1 = " bytes in ";
      temp2 = FloatToStrF(FFileList[0]->SelectedFiles ,ffNumber,5,0);
      temp3 = " selected files";
      len = Canvas->TextWidth(temp);
      len1 = Canvas->TextWidth(temp1);
      len2 = Canvas->TextWidth(temp2);
      len3 = Canvas->TextWidth(temp3);
      shift += (fullwidth - len - len1 - len2 - len3)/2;
      Canvas->Brush->Color = FQuantitySelected.Paper;
      Canvas->Font->Color = FQuantitySelected.Ink;
      Canvas->TextRect(Rect(shift, o, shift1 = len + shift, o + height), shift, o, temp);
      Canvas->Brush->Color = FTextSelectedFiles.Paper;
      Canvas->Font->Color = FTextSelectedFiles.Ink;
      Canvas->TextRect(Rect(shift1, o, shift = len1 + shift1, o + height), shift1, o, temp1);
      Canvas->Brush->Color = FQuantitySelected.Paper;
      Canvas->Font->Color = FQuantitySelected.Ink;
      Canvas->TextRect(Rect(shift, o, shift1 = len2 + shift, o + height), shift, o, temp2);
      Canvas->Brush->Color = FTextSelectedFiles.Paper;
      Canvas->Font->Color = FTextSelectedFiles.Ink;
      Canvas->TextRect(Rect(shift1, o, len3 + shift1, o + height), shift1, o, temp3);
      }
    o += FSymbolHeight;
    }
  if(FShowFilesSpace)
    {
    //files space
    temp = " In all: ";
    temp1 = FloatToStrF(FFileList[0]->DataFiles ,ffNumber,5,0);
    temp2 = " files, ";
    temp3 = FloatToStrF(FFileList[0]->DataBytes ,ffNumber,10,0);
    temp4 = " bytes";
    len = Canvas->TextWidth(temp);
    len1 = Canvas->TextWidth(temp1);
    len2 = Canvas->TextWidth(temp2);
    len3 = Canvas->TextWidth(temp3);
    len4 = Canvas->TextWidth(temp4);
    shift = symbolwidth + (fullwidth - len - len1 - len2 - len3 - len4)/2;
    Canvas->Brush->Color = FAllText.Paper;
    Canvas->Font->Color = FAllText.Ink;
    Canvas->TextRect(Rect(shift, o, shift1 = len + shift, o + height), shift, o, temp);
    Canvas->Brush->Color = FAllDigits.Paper;
    Canvas->Font->Color = FAllDigits.Ink;
    Canvas->TextRect(Rect(shift1, o, shift = len1 + shift1, o + height), shift1, o, temp1);
    Canvas->Brush->Color = FAllText.Paper;
    Canvas->Font->Color = FAllText.Ink;
    Canvas->TextRect(Rect(shift, o, shift1 = len2 + shift, o + height), shift, o, temp2);
    Canvas->Brush->Color = FAllDigits.Paper;
    Canvas->Font->Color = FAllDigits.Ink;
    Canvas->TextRect(Rect(shift1, o, shift = len3 + shift1, o + height), shift1, o, temp3);
    Canvas->Brush->Color = FAllText.Paper;
    Canvas->Font->Color = FAllText.Ink;
    Canvas->TextRect(Rect(shift, o, len4 + shift, o + height), shift, o, temp4);
    o += FSymbolHeight;
    }
  if(FShowFreeSpace)
    {
    //free space
    temp = FloatToStrF(FDriveControl->Drive[FCurrentDrive]->Free ,ffNumber,10,0);
    temp1 = " bytes free on drive ";
    temp2 = FCurrentDrive;
//    temp2 = AnsiString((char)(FCurrentDrive + 'A')) + ":";
    len = Canvas->TextWidth(temp);
    len1 = Canvas->TextWidth(temp1);
    len2 = Canvas->TextWidth(temp2);
    shift = symbolwidth + (fullwidth - len - len1 - len2)/2;
    Canvas->Brush->Color = FFreeDigits.Paper;
    Canvas->Font->Color = FFreeDigits.Ink;
    Canvas->TextRect(Rect(shift, o, shift1 = len + shift, o + height), shift, o, temp);
    Canvas->Brush->Color = FFreeText.Paper;
    Canvas->Font->Color = FFreeText.Ink;
    Canvas->TextRect(Rect(shift1, o, shift = len1 + shift1, o + height), shift1, o, temp1);
    Canvas->Brush->Color = FFreeDigits.Paper;
    Canvas->Font->Color = FFreeDigits.Ink;
    Canvas->TextRect(Rect(shift, o, shift1 = len2 + shift, o + height), shift, o, temp2);
    }
  //frame output
}
//---------------------------------------------------------------------------
char __fastcall TFilePanel::ConvertAttribute (int i)
{
  if((i & faHidden)||(i & faSysFile))
    return FCommonSym;
  if(i & faHidden)
    return FHiddenSym;
  if(i & faSysFile)
    return FSystemSym;
  return ' ';
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::Click(void)
{
  TControl::Click();
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::DblClick(void)
{
  TControl::DblClick();
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
  TControl::MouseDown(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
  TControl::MouseMove(Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
  TControl::MouseUp(Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::Reset()
{
  FBeginIndex = 0;
  FEndIndex = 0;
  FActiveIndex = 0;
  Refresh();
}*/
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::DoEnter(void)
{
  TWinControl::DoEnter();
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::DoExit(void)
{
  TWinControl::DoExit();
} */
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::KeyDown(Word &Key, Classes::TShiftState Shift)
{
  TWinControl::KeyDown(Key,Shift);
  switch(Key)
    {
    case HOME://home
      MoveCursor(0);
      break;
    case UP://up
      MoveCursor(FActiveIndex - 1);
      break;
    case PGUP://pgup
      MoveCursor(FActiveIndex - FColumnCount * FFileColumns);
      break;
    case LEFT://left
      MoveCursor(FActiveIndex - FColumnCount);
      break;
    case RIGHT://right
      MoveCursor(FActiveIndex + FColumnCount);
      break;
    case END://end
      MoveCursor(FFileList->Files - 1);
      break;
    case DOWN://down
      MoveCursor(FActiveIndex + 1);
      break;
    case PGDN://pgdn
      MoveCursor(FActiveIndex + FColumnCount * FFileColumns);
    }
} */
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::KeyPress(char &Key)
{
  TWinControl::KeyPress(Key);
} */
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::KeyUp(Word &Key, Classes::TShiftState Shift)
{
  TWinControl::KeyUp(Key,Shift);
} */
//---------------------------------------------------------------------------
/*void __fastcall DoKeyDown(Messages::TWMKey &Message)
{

}
//---------------------------------------------------------------------------
void __fastcall DoKeyPress(Messages::TWMKey &Message)
{
}
//---------------------------------------------------------------------------
void __fastcall DoKeyUp(Messages::TWMKey &Message)
{
} */
//---------------------------------------------------------------------------
/*void __fastcall TFilePanel::WndProc(Messages::TMessage &Message)
{
  int i;
  switch (Message.Msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      switch(Message.WParam)
    {
    case HOME://home
      MoveCursor(0);
      break;
    case UP://up
      MoveCursor(FActiveIndex - 1);
      break;
    case PGUP://pgup
      MoveCursor(FActiveIndex - FColumnCount * FFileColumns);
      break;
    case LEFT://left
      MoveCursor(FActiveIndex - FColumnCount);
      break;
    case RIGHT://right
      MoveCursor(FActiveIndex + FColumnCount);
      break;
    case END://end
      MoveCursor(FFileList->Files - 1);
      break;
    case DOWN://down
      MoveCursor(FActiveIndex + 1);
      break;
    case PGDN://pgdn
      MoveCursor(FActiveIndex + FColumnCount * FFileColumns);
    }
      break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
      i=0;
      break;
    }
  int k=0;
  TWinControl::WndProc(Message);
} */
//---------------------------------------------------------------------------
void __fastcall TFilePanel::MoveCursor(int newpos)
{
  if(newpos < 0)
    newpos = 0;
  if(newpos >= FFileList[0]->Files)
    newpos = FFileList[0]->Files - 1;
  if(newpos == FActiveIndex)
    return;
  ShowCursor(FActiveIndex,false);
  if(newpos > FEndIndex)
    {
    PaintColumns(pcDown,newpos);
    FBeginIndex += newpos - FEndIndex;
    FEndIndex = newpos;
    }
  if(newpos < FBeginIndex)
    {
    PaintColumns(pcUp,newpos);
    FBeginIndex = newpos;
    FEndIndex = FFileColumns * FColumnCount + newpos - 1;
    }
  ShowCursor(newpos,true);
  FActiveIndex = newpos;
  PaintFileInfo(FInfoBar);
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::ShowCursor(int pos, bool f)
{
//  if(!FActive)
//    return;
  if(!FPainted)
    return;
  int column = (pos - FBeginIndex)/FColumnCount, height = Canvas->TextHeight("Word!");//height = abs(Canvas->Font->Height);
  int shiftx = column * FColumnWidth + FSymbolWidth * (column + 1),
    shifty = (pos - FBeginIndex - column * FColumnCount) * height + height * 2;
  int shiftext = shiftx + FColumnWidth - FSymbolWidth*4,
    shifts = shiftext - FSymbolWidth*3/2,
    shiftbreak = shiftext + FSymbolWidth*4;
  if(f)
    {
    if(FFileList[0]->Selected[pos])
      {
      Canvas->Brush->Color = FSelectedCursor.Paper;
      Canvas->Font->Color = FSelectedCursor.Ink;
      }
    else
      {
      Canvas->Brush->Color = FNormalCursor.Paper;
      Canvas->Font->Color = FNormalCursor.Ink;
      }
    }
  else
    {
    if(FFileList[0]->Selected[pos])
      {
      Canvas->Brush->Color = FSelectedText.Paper;
      Canvas->Font->Color = FSelectedText.Ink;
      }
    else
      {
      Canvas->Brush->Color = FNormalText.Paper;
      Canvas->Font->Color = FNormalText.Ink;
      }
    }
  Canvas->TextRect(Rect(shiftx, shifty, shifts, shifty + height),shiftx,shifty,MinimizeName(FFileList[0]->Name[pos],Canvas,shifts - FSymbolWidth/2 - shiftx));
  Canvas->TextRect(Rect(shifts, shifty, shiftext, shifty + height),shifts,shifty,ConvertAttribute(FFileList[0]->Attribute[pos]));
  Canvas->TextRect(Rect(shiftext, shifty, shiftbreak, shifty + height),shiftext,shifty,MinimizeName(FFileList[0]->Extension[pos],Canvas,shiftbreak - shiftext));
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::PaintFileInfo(int o)
{
  AnsiString temp;
  int height = Canvas->TextHeight("Word!");//height = abs(Canvas->Font->Height);
  Canvas->Brush->Color = FActiveFile.Paper;
  Canvas->Font->Color = FActiveFile.Ink;
  int shift = FSymbolWidth, shift1 = max(shift + 100, FFullWidth + FSymbolWidth - 145);
  TRect rect = Rect(shift, o, shift1, o + height);
  Canvas->TextRect(rect,rect.Left,rect.Top,FFileList[0]->FullName[FActiveIndex]);
  rect = Rect(shift1, o, shift1 + 55, o + height);
  if(FFileList[0]->File[FActiveIndex]->IsDirectory())
    {
    if((temp = (AnsiString) FFileList[0]->Size[FActiveIndex])==0)
      {
      if(FFileList[0]->Name[FActiveIndex]=="..")
        temp = ">Directory<";
      else
        temp = "<Directory>";
      }
    }
  else
    temp = (AnsiString) FFileList[0]->Size[FActiveIndex];
  Canvas->TextRect(rect,rect.Left,rect.Top,temp);
  rect = Rect(shift1 + 55 , o, FFullWidth + FSymbolWidth, o + height);
  Canvas->TextRect(rect,rect.Left,rect.Top,FFileList[0]->Time[FActiveIndex]);
}
//---------------------------------------------------------------------------
int __fastcall TFilePanel::PaintColumns(TPaintColumn pc,int newpos)
{
  int shift = FSymbolWidth, shiftext, shifts, shiftbreak, shifty, shifty1, shiftybreak;
  AnsiString temp = "Name";
  int j = FBeginIndex;
  FColumnCount = FFileListHeight/FSymbolHeight - 2;
  switch(pc)
    {
    case pcFull:
      FEndIndex = FBeginIndex + FFileColumns * FColumnCount - 1;
      if(FEndIndex > FFileList[0]->Files)
        FEndIndex = FFileList[0]->Files - 1;
/*      if(FEndIndex < FActiveIndex)
        {
        FBeginIndex += FActiveIndex - FEndIndex;
        FEndIndex = FActiveIndex;
        }*/
      for (int i = 0; i < FFileColumns; i++)
        {
        shiftext = shift + FColumnWidth - FSymbolWidth*4;
        shifts = shiftext - FSymbolWidth*3/2;
        shiftbreak = shiftext + FSymbolWidth*4;
        shifty = FSymbolHeight;
        TRect rect = Rect(shift,shifty,shiftbreak,shifty + FSymbolHeight);
        Canvas->Brush->Color = FColumnContents.Paper;
        Canvas->Font->Color = FColumnContents.Ink;
        Canvas->TextRect(rect,rect.Left + (FColumnWidth - Canvas->TextWidth(temp))/2,rect.Top,temp);
        shifty += FSymbolHeight;
        while (j < FFileList[0]->Files && shifty <= FFileListHeight - FSymbolHeight)
          {
          if(FFileList[0]->Selected[j])
            {
            Canvas->Brush->Color = FSelectedText.Paper;
            Canvas->Font->Color = FSelectedText.Ink;
            }
          else
            {
            Canvas->Brush->Color = FNormalText.Paper;
            Canvas->Font->Color = FNormalText.Ink;
            }
          Canvas->TextRect(Rect(shift, shifty, shifts - FSymbolWidth/2, shifty + FSymbolHeight),shift,shifty,MinimizeName(FFileList[0]->Name[j],Canvas,shifts - FSymbolWidth/2 - shift));
          Canvas->TextRect(Rect(shifts, shifty, shiftext - FSymbolWidth/2, shifty + FSymbolHeight),shifts,shifty,ConvertAttribute(FFileList[0]->Attribute[j]));
          Canvas->TextRect(Rect(shiftext, shifty, shiftbreak, shifty + FSymbolHeight),shiftext,shifty,MinimizeName(FFileList[0]->Extension[j],Canvas,shiftbreak - shiftext));
          shifty += FSymbolHeight;
          j++;
          }
        shift += FColumnWidth + FSymbolWidth;
        }
      break;
    case pcUp:
      shift = (FSymbolWidth + FColumnWidth) * FFileColumns - FColumnWidth;
      shifty = FSymbolHeight*2;
      shifty1 = shifty + FSymbolHeight*(FBeginIndex - newpos);
      shiftybreak = shifty + FColumnCount * FSymbolHeight;
      for (int i = FFileColumns - 1; i >= 0; i--)
        {
        shiftext = shift + FColumnWidth - FSymbolWidth*4;
        shifts = shiftext - FSymbolWidth*3/2;
        shiftbreak = shiftext + FSymbolWidth*4;
        if(shiftybreak > shifty1)
          Canvas->CopyRect(Rect(shift,shifty1,shiftbreak,shiftybreak),Canvas,Rect(shift,shifty,shiftbreak,shiftybreak - shifty1 + shifty));
        if(i)
          Canvas->CopyRect(Rect(shift,shifty,shiftbreak,shifty1),Canvas,Rect(shift - FColumnWidth - FSymbolWidth,shiftybreak - shifty1 + shifty,shiftbreak - FColumnWidth - FSymbolWidth,shiftybreak));
        else
          {
          for(j = newpos; j < FBeginIndex; j++)
            {
            Canvas->TextRect(Rect(shift, shifty, shifts - FSymbolWidth/2, shifty + FSymbolHeight),shift,shifty,MinimizeName(FFileList[0]->Name[j],Canvas,shifts - FSymbolWidth/2 - shift));
            Canvas->TextRect(Rect(shifts, shifty, shiftext - FSymbolWidth/2, shifty + FSymbolHeight),shifts,shifty,ConvertAttribute(FFileList[0]->Attribute[j]));
            Canvas->TextRect(Rect(shiftext, shifty, shiftbreak, shifty + FSymbolHeight),shiftext,shifty,MinimizeName(FFileList[0]->Extension[j],Canvas,shiftbreak - shiftext));
            shifty += FSymbolHeight;
            }
          }
        shift -= FColumnWidth + FSymbolWidth;
        }
      break;
    case pcDown:
      shift = FSymbolWidth;
      shifty = FSymbolHeight*2;
      shiftybreak = shifty + FColumnCount * FSymbolHeight;
      shifty1 = shiftybreak - FSymbolHeight*(newpos - FEndIndex);
      for (int i = 0; i < FFileColumns ; i++)
        {
        shiftext = shift + FColumnWidth - FSymbolWidth*4;
        shifts = shiftext - FSymbolWidth*3/2;
        shiftbreak = shiftext + FSymbolWidth*4;
        if(shifty < shifty1)
          Canvas->CopyRect(Rect(shift,shifty,shiftbreak,shifty1),Canvas,Rect(shift,shiftybreak - shifty1 + shifty,shiftbreak,shiftybreak));
        if(i != FFileColumns - 1)
          Canvas->CopyRect(Rect(shift,shifty1,shiftbreak,shiftybreak),Canvas,Rect(shift + FColumnWidth + FSymbolWidth,shifty,shiftbreak + FColumnWidth + FSymbolWidth,shiftybreak - shifty1 + shifty));
        else
          {
          for(j = FEndIndex + 1; j <=  newpos; j++)
            {
            Canvas->TextRect(Rect(shift, shifty1, shifts - FSymbolWidth/2, shifty1 + FSymbolHeight),shift,shifty1,MinimizeName(FFileList[0]->Name[j],Canvas,shifts - FSymbolWidth/2 - shift));
            Canvas->TextRect(Rect(shifts, shifty1, shiftext - FSymbolWidth/2, shifty1 + FSymbolHeight),shifts,shifty1,ConvertAttribute(FFileList[0]->Attribute[j]));
            Canvas->TextRect(Rect(shiftext, shifty1, shiftbreak, shifty1 + FSymbolHeight),shiftext,shifty1,MinimizeName(FFileList[0]->Extension[j],Canvas,shiftbreak - shiftext));
            shifty1 += FSymbolHeight;
            }
          }
        shift += FColumnWidth + FSymbolWidth;
        }
    }
  return j;
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::ShowPath(bool f)
{
  if(!FPainted)
    return;
  if(f)
    {
    Canvas->Brush->Color = FActiveCatalog.Paper;
    Canvas->Font->Color = FActiveCatalog.Ink;
    }
  else
    {
    Canvas->Brush->Color = FPassiveCatalog.Paper;
    Canvas->Font->Color = FPassiveCatalog.Ink;
    }
  int middle = Width/2;
  int k = Canvas->TextWidth(FCurrentDir)/2 + FSymbolWidth;
  int l = middle + k;
  k = middle - k;
  int height = Canvas->TextHeight(FCurrentDir);
  TRect rect = Rect(k,0,l,height);
  Canvas->TextRect(rect,rect.Left + FSymbolWidth,rect.Top,::MinimizeName(FCurrentDir,Canvas,rect.Right - rect.Left));

}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::ChangeFocus(TMessage msg)
{
  bool fl;
  switch(msg.Msg)
    {
    case WM_KILLFOCUS:
      fl = false;
      break;
    case WM_SETFOCUS:
      fl = true;
      if(FDriveControl)
        FDriveControl->ActivePanel = this;
    }
  FActive = fl;
  ShowCursor(FActiveIndex,fl);
  ShowPath(fl);
}
//---------------------------------------------------------------------------
int __fastcall TFilePanel::KeyProcess(int key)
{
  switch(key)
    {
    case HOME://home
      MoveCursor(0);
      break;
    case UP://up
      MoveCursor(FActiveIndex - 1);
      break;
    case PGUP://pgup
      MoveCursor(FActiveIndex - FColumnCount * FFileColumns);
      break;
    case LEFT://left
      MoveCursor(FActiveIndex - FColumnCount);
      break;
    case RIGHT://right
      MoveCursor(FActiveIndex + FColumnCount);
      break;
    case END://end
      MoveCursor(FFileList[0]->Files - 1);
      break;
    case DOWN://down
      MoveCursor(FActiveIndex + 1);
      break;
    case PGDN://pgdn
      MoveCursor(FActiveIndex + FColumnCount * FFileColumns);
    }
  return key;//???
}
//---------------------------------------------------------------------------
/*int __fastcall TFilePanel::Enter(int)
{
  TFile *file = FFileList[0]->File[FActiveIndex];
  AnsiString str;
  if(file->IsDirectory())
    {//verify for drivecontol is NULL;
    if(!FDriveControl->ChangeDir(file->FullName) && !SetCurrentDir(GetCurrentDrive() + "\\"))
      {
      return -1;//dir is unchangable
      }
    FCurrentDir = FDriveControl->CurrentDir;
    NewDir(FCurrentDir); //includes search in pushed dirs and pushdir functions
//    ScanDir(FCurrentDir);
//    Reset();   //reset visible properties of panel
    }
  else
    {
    DriveControl->ExecuteFile(file->FullName);
    }
  return 0;//ok
} */
//---------------------------------------------------------------------------
AnsiString __fastcall TFilePanel::GetCurrentDir()
{
  return FCurrentDir;
}
//---------------------------------------------------------------------------
TFile *__fastcall TFilePanel::GetActiveFile()
{
  return FFileList[0]->File[FActiveIndex];
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFilePanel::MinimizeName(AnsiString filename, Graphics::TCanvas * canvas, int maxlen)
{
  int len = canvas->TextWidth(filename);
  if(!len)
    len++;
  if(len <= maxlen)
    return filename;
  int sublen = canvas->TextWidth("...");
  do {
    filename.SetLength((maxlen - sublen) * filename.Length() / len);
  } while ((len = canvas->TextWidth(filename)) > maxlen - sublen);
/*for (int i = 2; len < maxlen - sublen; i += 2)
    {
    filename.Insert(" ",i);
    len = canvas->TextWidth(filename);
    if(i > len + 1)
      i--;
    }*/
  return filename + "...";
}
//---------------------------------------------------------------------------
namespace Filepanel
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TFilePanel)};
        RegisterComponents("Additional", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFilePanel::SetCurrentDrive(AnsiString str)
{
  FCurrentDrive = str;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFilePanel::GetCurrentDrive ()
{
  return FCurrentDrive;
}
//---------------------------------------------------------------------------


/*void __fastcall TFilePanel::SetRescan(bool value)
{
  for(int i = 0; i < FDirs; i++)
    {
    if(i)
      FFileList[i]->Main = false;
    else
      FFileList[i]->Main = true;
    FFileList[i]->Rescan = value;
    }
}

bool __fastcall TFilePanel::GetRescan()
{
  bool k = false;
  for(int i = 0; i < FDirs; i++)
    k |= FFileList[i]->Rescan;
  return k;
} */


void __fastcall TFilePanel::SetSortMode(TSortMode value)
{
  if(value == FSortMode)
    return;
  FSortMode = value;
  for(int i = 0; i < FDirs; i++)
    FFileList[i]->SortMode = value;
  if(FDirs)
    FFileList[0]->Sort();
}


void __fastcall TFilePanel::SetSortDirection(bool value)
{

}

