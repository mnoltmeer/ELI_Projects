#begin ELIShellEmulator;

//Скрипт имитирует работу в консоли;
//Вводимые команды будут интерпретированы в рантайм режиме;
//Допустимы любые конструкции на языке ELI;

#procedure RunShell()
{
  $res = string;

  while (_istreq($res, exit) != 1)
    {
      $line = '';
	  
      _writeout(#>);
      _readin($line);
		
      if (_istreq($line, exit) != 1)
        {  
          if (_run($line) == 0)
            {_writeout(_lasterror()); _writeout(#endl); $res = exit;}
        }
      else
        {$res = $line;}
    }
}

_writeout('[ELI Shell Emulator copyright 2018 Maxim Noltmeer m.noltmeer@gmail.com]');
_writeout(#endl);
_writeout('Type "exit" to end script');
_writeout(#endl);
_writeout(#endl);

:RunShell();

#end;