static void delay100us(void) //@12.000MHz
{
  unsigned char data i;

  i = 47;
  while (--i)
    ;
}

// Call delay100us 10 times to make 1 millisecond
void delayMiliseconds(unsigned long miliseconds)
{
  // i is a counter for 100us delay
  unsigned char i;

  while (miliseconds--)
  {
    for (i = 0; i < 10; i++)
    {
      delay100us();
    }
  }
}
