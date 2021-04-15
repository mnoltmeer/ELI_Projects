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
	_importfunc($StreamsHinst, eDeleteStream, '_DeleteStream', '');
	
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
	_importfunc($StreamsHinst, eWriteSym, '_WriteSym', 'num pHandle,num pData');
	
//reading number from stream, read starts at current position;
//return: 0 - when error and number value in other case;
//pHandle - stream handle, pCount - count bytes for reading;
	_importfunc($StreamsHinst, eReadNum, '_ReadNum', 'num pHandle,num pCount');
	
//write number into stream, starts at current position;
//return: 0 - when error and 1 in other case;
//pHandle - stream handle, pData - number value for write;
	_importfunc($StreamsHinst, eWriteNum, '_WriteNum', 'num pHandle,num pData');	
  }