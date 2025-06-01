#class Point
{
  #public property X = 0;
  #public property Y = 0;
  
  #public method Set($x, $y)
  {
    &$this.X = $x;
	&$this.Y = $y;
  }
  
  #public method Check($x, $y)
  {
    if (&$this.X == $x)
	  {
	    if (&$this.Y == $y){#return 1;}
	  }
	  
	#return 0;
  }
  
  #public method Point($x, $y){&$this.Set($x, $y);}
}