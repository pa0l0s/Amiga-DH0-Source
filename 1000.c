 #include <stdio.h>

 int main(void)
 {
   char str[20];
   int ch, n = 0;

   while ((ch = getchar()) != EOF && n < 20) {
      str[n] = ch;
      ++n;
   }
   
   for (int i = 0; i < n; ++i)
      putchar(str[i]);
   
   putchar('\n'); /* trailing '\n' needed in Standard C */

   return 0;
 }