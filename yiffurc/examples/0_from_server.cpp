/* Note that 0x500330 is the start of the variable table. */
/* p is the current command line */
pos        = 1;
curr_var   = ftoi2( p + pos )
pos        += 2;

do
{
   int v = ftoi3( p + pos );
   pos   += 3;

   if (v == 0x4000) /* !mL in base-95 */
   {
      int count   = ftoi3( p + pos ) + 1;
      pos         += 3;
      v           = ftoi3( p + pos );
      pos         += 3;

      for (int i = 0; i < count; i++, curr_var++)
      {
         ((short*) 0x500330)[ curr_var ] = v;
      }
      
   }
   else
   {
      ((short*) 0x500330)[ curr_var ] = v;
      curr_var++;
   }  

} while (*(p + pos) > 31);