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
#include <vector>

#pragma hdrstop
#pragma argsused

#include "..\..\work-functions\Logs.h"
#include "..\..\ELI\eli_interface.h"

unsigned int CurrentHandle;

struct eliStreamHandle
{
  TMemoryStream *Stream;
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

  return res;
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
	   ep->AddToLog(String("eliStreams::eGetSize: " + e.ToString()).c_str());

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
	   ep->AddToLog(String("eliStreams::eGetPos: " + e.ToString()).c_str());

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
	   ep->AddToLog(String("eliStreams::eSetPos: " + e.ToString()).c_str());

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
		 {
		   unsigned long sz = stream->Stream->Size - stream->Stream->Position;

		   wchar_t *buf = new wchar_t[sz / sizeof(wchar_t) + 1];

		   try
			  {
				stream->Stream->Read(buf, sz);

				res = buf;

				if (res == "")
                  res = "-err-";
			  }
		   __finally {delete[] buf;}
         }
	   else
		 res = "-err-";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eReadSym: " + e.ToString()).c_str());

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
		   stream->Stream->Write(data.c_str(), data.Length() * sizeof(wchar_t));

		   res = "1";
         }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eWriteSym: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eReadNum(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  float res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 stream->Stream->Read(&res, sizeof(float));
	   else
		 res = 0;
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eReadNum: " + e.ToString()).c_str());

       res = 0;
	 }

  char buf[8];

  sprintf(buf, "%.3f", res);

  ep->SetFunctionResult(ep->GetCurrentFuncName(), String(buf).c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eWriteNum(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   float data = ep->GetParamToFloat(L"pData");

	   eliStreamHandle *stream = FindStream(handle);

	   if (stream)
		 {
		   stream->Stream->Write(&data, sizeof(int));

		   res = "1";
		 }
	   else
		 res = "0";
	 }
  catch (Exception &e)
	 {
       ep->AddToLog(String("eliStreams::eWriteNum: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamLoadFromFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   String file = ep->GetParamToStr(L"pFile");

	   eliStreamHandle *stream = FindStream(handle);

       if (!stream)
		 throw Exception("Target Stream not found");

	   stream->Stream->LoadFromFile(file);

	   res = "1";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eStreamLoadFromFile: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamLoadFromStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned int source = ep->GetParamToInt(L"pSource");

	   eliStreamHandle *source_stream = FindStream(source);
	   eliStreamHandle *stream = FindStream(handle);

	   if (!source_stream)
		 throw new Exception("Source Stream not found");

	   if (!stream)
		 throw new Exception("Target Stream not found");

	   stream->Stream->LoadFromStream(source_stream->Stream);
	   res = "1";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eStreamLoadFromStream: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamSaveToFile(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   String file = ep->GetParamToStr(L"pFile");

	   eliStreamHandle *stream = FindStream(handle);

       if (!stream)
		 throw Exception("Target Stream not found");

	   stream->Stream->SaveToFile(file);
	   res = "1";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eStreamSaveToFile: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eStreamSaveToStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");
	   unsigned int target = ep->GetParamToInt(L"pTarget");

	   eliStreamHandle *target_stream = FindStream(target);
	   eliStreamHandle *stream = FindStream(handle);

	   if (!target_stream)
		 throw Exception("Source Stream not found");

	   if (!stream)
		 throw Exception("Target Stream not found");

	   stream->Stream->SaveToStream(target_stream->Stream);
	   res = "1";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eStreamSaveToStream: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eClearStream(void *p)
{
  ELI_INTERFACE *ep = static_cast<ELI_INTERFACE*>(p);

  String res;

  try
	 {
	   unsigned int handle = ep->GetParamToInt(L"pHandle");

	   eliStreamHandle *stream = FindStream(handle);

	   if (!stream)
		 throw Exception("Stream not found");

	   stream->Stream->Clear();
	   res = "1";
	 }
  catch (Exception &e)
	 {
	   ep->AddToLog(String("eliStreams::eClearStream: " + e.ToString()).c_str());

       res = "0";
	 }

  ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
}
//-------------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateStream(void *p)
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
	   ep->AddToLog(String("eliStreams::eCreateStream: " + e.ToString()).c_str());

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
	   ep->AddToLog(String("eliStreams::eDeleteStream: " + e.ToString()).c_str());

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
  EntryPoint(reason);

  return 1;
}
//-------------------------------------------------------------------------------

