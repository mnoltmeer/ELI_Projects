/*!
Copyright 2021 Maxim Noltmeer (m.noltmeer@gmail.com)

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include "..\..\work-functions\MyFunc.h"
#include "..\..\ELI\eli_interface.h"

String initdir;

extern "C"
{
__declspec(dllexport) void __stdcall eIsFileExist(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);
  wchar_t res[3];

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   int i = FileExists(path);
	   swprintf(res, L"%d", i);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCopyFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  wchar_t res[3];

  try
	 {
       String path_fr = ep->GetParamToStr(L"pFrom"), path_to = ep->GetParamToStr(L"pTo");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path_fr[1] == '.')
		 {
		   path_fr.Delete(1, 1);
		   path_fr = String(ep->GetInitDir()) + "\\" + path_fr;
		 }

	   if (path_to[1] == '.')
		 {
		   path_to.Delete(1, 1);
	  	   path_to = String(ep->GetInitDir()) + "\\" + path_to;
		 }

	   int i = CopyFile(path_fr.c_str(), path_to.c_str(), bool(ep->GetParamToInt(L"pFailExist")));
	   swprintf(res, L"%d", i);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDeleteFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  wchar_t res[3];

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   int i = DeleteFile(path);
	   swprintf(res, L"%d", i);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eReadTextFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   res = LoadTextFile(path);
	 }
  catch (Exception &e)
	 {
	   res = "";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eWriteTextFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   String mode = ep->GetParamToStr(L"pMode");

	   if (mode == "ap")
		 {
		   AddToFile(ep->GetParamToStr(L"pFile"), ep->GetParamToStr(L"pText"));
		   res = "1";
		 }
	   else if (mode == "ow")
		 {
		   SaveToFile(ep->GetParamToStr(L"pFile"), ep->GetParamToStr(L"pText"));
		   res = "1";
		 }
	   else
		 {
           res = 0;
         }
	 }
  catch (Exception &e)
	 {
	   res = "-1";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eFileSize(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  wchar_t res[3];

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   int sz = GetFileSize(path);
       swprintf(res, L"%d", sz);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCopyDir(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  wchar_t res[3];

  try
	 {
	   String path_src = ep->GetParamToStr(L"pDirSrc"),
			  path_dst = ep->GetParamToStr(L"pDirDest");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path_src[1] == '.')
		 {
		   path_src.Delete(1, 1);
		   path_src = String(ep->GetInitDir()) + "\\" + path_src;
		 }

	   if (path_dst[1] == '.')
		 {
		   path_dst.Delete(1, 1);
		   path_dst = String(ep->GetInitDir()) + "\\" + path_dst;
		 }

	   int i = CopyDirs(path_src, path_dst);
	   swprintf(res, L"%d", i);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eClearDir(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  wchar_t res[3];

  try
	 {
       String path = ep->GetParamToStr(L"pFile");

//����������� ���� ���� ".\file.eli" - ������������ ������� �������
	   if (path[1] == '.')
		 {
		   path.Delete(1, 1);
		   path = String(ep->GetInitDir()) + "\\" + path;
		 }

	   DeleteAllFromDir(ep->GetParamToStr(L"pDir"));
       swprintf(res, L"%d", 1);
	 }
  catch (Exception &e)
	 {
	   swprintf(res, L"%d", -1);
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res);
}
//-------------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}

