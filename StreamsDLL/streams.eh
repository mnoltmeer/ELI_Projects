$StreamsHinst = _ConnectLib(.\lib\eliStreams.dll);

if ($StreamsHinst != -1)
  {
//return: 0 - when error and stream handle in other case;
	_importfunc($StreamsHinst, eCreateMemoryStream, '_CreateMemStream', '');

//return: 0 - when error and stream handle in other case;
//return: pData - string that initialises stream;
	_importfunc($StreamsHinst, eCreateStringStream, '_CreateStrStream', 'sym pData');
	
//return: 0 - when error and stream handle in other case;
//return: pFile - path to file, pMode - access mode (r - read, w - write, rw - read & write);
	_importfunc($StreamsHinst, eCreateFileStream, '_CreateFileStream', 'sym pFile,sym pMode');

//return: 0 - when error, 1 - if stream deleted successfully;
	_importfunc($StreamsHinst, eDeleteStream, '_DeleteStream', 'num pHandle');
	
//return: -1 - when error and stream size in other case;
	_importfunc($StreamsHinst, eGetSize, '_GetSize', 'num pHandle');
	
//return: -1 - when error and stream position in other case;
	_importfunc($StreamsHinst, eGetPos, '_GetPos', 'num pHandle');
	
//return: 0 - when error and 1 in other case;
	_importfunc($StreamsHinst, eSetPos, '_SetPos', 'num pHandle,num pPos');
	
//reading string from stream, read starts at current position;
//return: "-errr-" - when error and string value in other case;
//pHandle - stream handle, pCount - count bytes for reading;
	_importfunc($StreamsHinst, eReadSym, '_ReadSym', 'num pHandle,num pCount');
	
//write string into stream, starts at current position;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pData - string value for write;
	_importfunc($StreamsHinst, eWriteSym, '_WriteSym', 'num pHandle,sym pData');
	
//reading number from stream, read starts at current position;
//return: 0 - when error and number value in other case;
//pHandle - stream handle, pCount - count bytes for reading;
	_importfunc($StreamsHinst, eReadNum, '_ReadNum', 'num pHandle,num pCount');
	
//write number into stream, starts at current position;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pData - number value for write;
	_importfunc($StreamsHinst, eWriteNum, '_WriteNum', 'num pHandle,num pData');

//load data from file into stream;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pFile - path to file;
	_importfunc($StreamsHinst, eStreamLoadFromFile, '_StreamLoadFromFile', 'num pHandle,sym pFile');

//load data from another stream;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pSource - handle of source stream;
	_importfunc($StreamsHinst, eStreamLoadFromStream, '_StreamLoadFromStream', 'num pHandle,num pSource');

//save data from stream into file;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pFile - path to file;
	_importfunc($StreamsHinst, eStreamSaveToFile, '_StreamSaveToFile', 'num pHandle,sym pFile');

//save data to another stream;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pSource - handle of source stream;
	_importfunc($StreamsHinst, eStreamSaveToStream, '_StreamSaveToStream', 'num pHandle,num pSource');
	
//clear sttream;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle;
	_importfunc($StreamsHinst, eClearStream, '_StreamClearStream', 'num pHandle');	

//---------------------------------------------------------------------------;
	#class Stream
	  {
		#public property Handle = 0;
		
		#public method Stream($handle){&$this.Handle = $handle;}
		
		#public method GetSize(){#return _GetSize(&$this.Handle);}
		#public method GetPos(){#return _GetPos(&$this.Handle);}
		#public method SetPos($pos){#return _SetPos(&$this.Handle, $pos);}
		
		#public method ReadSym($cnt){#return _ReadSym(&$this.Handle, $cnt);}
		#public method WriteSym($data){#return _WriteSym(&$this.Handle, $data);}
		#public method ReadNum($cnt){#return _ReadNum(&$this.Handle, $cnt);}
		#public method WriteNum($data){#return _WriteSym(&$this.Handle, $data);}
		#public method Clear(){#return _StreamClearStream(&$this.Handle);}
	  }
//---------------------------------------------------------------------------;
	
	#class MemoryStream : Stream
	  {
		#public method MemoryStream(){&$this.Handle = _CreateMemStream();}		  
		#public method ~MemoryStream(){_DeleteStream(&$this.Handle);}
		
		#public method LoadFromFile($file){#return _StreamLoadFromFile(&$this.Handle, $file);}
		#public method SaveToFile($file){#return _StreamSaveToFile(&$this.Handle, $file);}
		#public method LoadFromStream($source){#return _StreamLoadFromStream(&$this.Handle, $source);}
		#public method SaveToStream($source){#return _StreamSaveToStream(&$this.Handle, $source);}
	  }
//---------------------------------------------------------------------------;
	
	#class StringStream : Stream
	  {
		#public method StringStream($data){&$this.Handle = _CreateStrStream($data);}		  
		#public method ~StringStream(){_DeleteStream(&$this.Handle);}
		
		#public method LoadFromFile($file){#return _StreamLoadFromFile(&$this.Handle, $file);}
		#public method SaveToFile($file){#return _StreamSaveToFile(&$this.Handle, $file);}
		#public method LoadFromStream($source){#return _StreamLoadFromStream(&$this.Handle, $source);}
		#public method SaveToStream($source){#return _StreamSaveToStream(&$this.Handle, $source);}
	  }
//---------------------------------------------------------------------------;
	
	#class FileStream : Stream
	  {
		#public method FileStream($file, $mode){&$this.Handle = _CreateFileStream($file, $mode);}		  
		#public method ~FileStream(){_DeleteStream(&$this.Handle);}
	  }
//---------------------------------------------------------------------------;
  }