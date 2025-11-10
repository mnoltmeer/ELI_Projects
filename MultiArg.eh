//this class helps create multi-argument string for object constructor;
//without creating long line in script body;
//just create object MultiArg, use AddArg() method to add lot of arguments;
//then use GetString() to get final constant string and send it to constructor;

#class MultiArg
{
  #property ResultString = "";
  
  #public property Count = 0;
    
  #public method Clear(){&$this.ResultString = ""; &$this.Count = 0;}

  #public method AddArg($arg)
  {
	if (!_strlen(&$this.ResultString))
	  {&$this.ResultString = $arg;}
	else
	  {&$this.ResultString = &$this.ResultString + "," + $arg;}
	  
	&$this.Count = ++1;
  }
  
  #public method GetString(){#return &$this.ResultString;}
}
