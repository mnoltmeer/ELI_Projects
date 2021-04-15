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

extern String UsedAppLogDir;

unsigned int CurrentHandle;

enum StreamType {stMemory = 0, stString = 1, stFile = 2};

struct eliStreamHandle
{
  TStream *Stream;
  StreamType Type;
  unsigned int Handle;
};

std::vector<eliStreamHandle> vecStreams;

inline unsigned int GenHandle(){return ++CurrentHandle;}
//-------------------------------------------------------------------------------

eliStreamHandle *FindStream(unsigned int handle)
{
  eliStreamHandle *res;

  try
	 {
	   for (int i = 0; i < vecStreams.size(); i++)
		  {
			if (vecStreams[i].Handle == handle)
			  {
				res = &vecStreams[i];
				break;
			  }
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("eliStream.log", "ELI", "FindStream: " + e.ToString());
	   res = NULL;
	 }

  return res;
}
//-------------------------------------------------------------------------------

eliStreamHandle *AddMemoryStream()
{
  eliStreamHandle *res;

  try
	 {
       eliStreamHandle stream;

	   stream.Stream = new TMemoryStream();
	   stream.Type = stMemory;
	   stream.Handle = GenHandle();

	   vecStreams.push_back(stream);

       res = &stream;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("eliStream.log", "ELI", "AddMemoryStream: " + e.ToString());
	   res = NULL;
	 }

  return res;
}
//-------------------------------------------------------------------------------

eliStreamHandle *AddStringStream(const String &value)
{
  eliStreamHandle *res;

  try
	 {
	   eliStreamHandle stream;

	   stream.Stream = new TStringStream(value, TEncoding::UTF8, false);
	   stream.Type = stString;
	   stream.Handle = GenHandle();

	   vecStreams.push_back(stream);

	   res = &stream;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("eliStream.log", "ELI", "AddStringStream: " + e.ToString());
	   res = NULL;
	 }

  return res;
}
//-------------------------------------------------------------------------------

eliStreamHandle *AddFileStream(const String &file, unsigned short mode)
{
  eliStreamHandle *res;

  try
	 {
	   eliStreamHandle stream;

	   stream.Stream = new TFileStream(file, mode);
	   stream.Type = stFile;
	   stream.Handle = GenHandle();

	   vecStreams.push_back(stream);

	   res = &stream;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("eliStream.log", "ELI", "AddFileStream: " + e.ToString());
	   res = NULL;
	 }

  return res;
}
//-------------------------------------------------------------------------------

bool RemoveStream(unsigned int handle)
{
  bool res;

  try
	 {
	   for (int i = 0; i < vecStreams.size(); i++)
		  {
			if (vecStreams[i].Handle == handle)
			  {
				delete vecStreams[i].Stream;
				vecStreams.erase(vecStreams.begin() + i);
				res = true;
                break;
			  }
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("eliStream.log", "ELI", "RemoveStream: " + e.ToString());
       res = false;
	 }
}
//-------------------------------------------------------------------------------

void ClearStreams()
{
  try
	 {
	   for (int i = 0; i < vecStreams.size(); i++)
		  delete vecStreams[i].Stream;

	   vecStreams.clear();
	 }
  catch (Exception &e)
	 {
       SaveLogToUserFolder("eliStream.log", "ELI", "ClearStreams: " + e.ToString());
	 }
}
//-------------------------------------------------------------------------------

extern "C"
{
__declspec(dllexport) void __stdcall eGetSize(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 res = stream->Stream->Size;
	   else
		 res = -1;
	 }
  catch (Exception &e)
	 {
	   res = -1;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eGetPos(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 res = stream->Stream->Position;
	   else
		 res = -1;
	 }
  catch (Exception &e)
	 {
	   res = -1;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eSetPos(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned int pos = ep->GetParamToInt(L"pPos");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   stream->Stream->Position = pos;
		   res = "1";
         }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eReadSym(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned int cnt = ep->GetParamToInt(L"pCount");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 res = static_cast<TStringStream*>(stream->Stream)->ReadString(cnt);
	   else
		 res = "-err-";
	 }
  catch (Exception &e)
	 {
	   res = "-err-";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eWriteSym(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   String data = ep->GetParamToStr(L"pData");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   if (stream->Type == stString)
			 {
			   TStringStream *ss = static_cast<TStringStream*>(stream->Stream);
			   ss->WriteString(data);
			 }
		   else
			 WriteStringIntoBinaryStream(stream->Stream, data);

		   res = "1";
         }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eReadNum(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned int cnt = ep->GetParamToInt(L"pCount");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 stream->Stream->Read(&res, cnt);
	   else
		 res = 0;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eWriteNum(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned data = ep->GetParamToInt(L"pData");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   stream->Stream->Write(&data, sizeof(data));
		   res = "1";
		 }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamLoadFromFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned data = ep->GetParamToInt(L"pData");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   stream->Stream->Write(&data, sizeof(data));
		   res = "1";
		 }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamSaveToFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned data = ep->GetParamToInt(L"pData");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   stream->Stream->Write(&data, sizeof(data));
		   res = "1";
		 }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateMemoryStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   eliStreamHandle *stream = AddMemoryStream();

	   if (stream)
		 res = stream->Handle;
	   else
		 res = 0;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateStringStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   String data = ep->GetParamToStr(L"pData");
	   eliStreamHandle *stream = AddStringStream(data);

	   if (stream)
		 res = stream->Handle;
	   else
		 res = 0;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateFileStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  int res;

  try
	 {
	   String file = ep->GetParamToStr(L"pFile");
	   String pmode = ep->GetParamToStr(L"pMode");
	   unsigned short mode;

	   if (pmode == UpperCase("rw"))
		 mode = fmOpenReadWrite|fmShareDenyNone;
	   else if (pmode == UpperCase("w"))
		 mode = fmOpenWrite|fmShareDenyNone;
	   else if (pmode == UpperCase("r"))
		 mode = fmOpenRead|fmShareDenyNone;
	   else
		 throw new Exception("Invalid argument(s)");

	   eliStreamHandle *stream = AddFileStream(file, mode);

	   if (stream)
		 res = stream->Handle;
	   else
		 res = 0;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(res).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDeleteStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");

	   if (RemoveStream(handle))
		 res = "1";
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------
}

void WINAPI EntryPoint(unsigned long reason)
{
  if (reason == DLL_PROCESS_DETACH)
	ClearStreams();
  else if (reason == DLL_THREAD_DETACH)
    ClearStreams();
}
//-------------------------------------------------------------------------------

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  UsedAppLogDir = "ELI";

  EntryPoint(reason);

  return 1;
}
//-------------------------------------------------------------------------------

