#begin ELIShellEmulator;

//This script emulates work in the console;
//Entered commands will be interpreted in runtime mode;
//Any ELI constructs are acceptable;

#procedure RunShell()
{
  $res = "";

  while (!_istreq($res, exit))
    {
      $line = "";
	  
      _writeout(#>);
      _readin($line);
		
      if (!_istreq($line, exit))
        {  
          if (_run($line) == 0)
            {
			  _writeout(_lasterror());
			  _writeout(#endl);
			  
			  $res = exit;
			}
        }
      else
        {$res = $line;}
    }
}

_writeout("[ELI Shell Emulator copyright 2018-2025 Maxim Noltmeer m.noltmeer@gmail.com]");
_writeout(#endl);
_writeout("Entered commands will be interpreted in runtime mode");
_writeout(#endl);
_writeout("Any ELI constructs are acceptable");
_writeout(#endl);
_writeout("Type "exit" to end work");
_writeout(#endl);
_writeout(#endl);

:RunShell();

#end;