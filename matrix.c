/*
 * Read_Keyboard_Matrix.c
 *
 * Compile with gcc (GCC) 4.2.4
 * gcc -o Read_Keyboard_Matrix Read_Keyboard_Matrix.c
 * 
 * Run from CLI only
 */
 
#include <proto/exec.h>
#include <proto/dos.h>
#include <devices/keyboard.h>
#include <stdint.h>
 
/*
 * There are keycodes from 0x00 to 0x7F, so the matrix needs to be
 * of 0x80 bits in size, or 0x80/8 which is 0x10 or 16 bytes...
 */
#define MATRIX_SIZE 16L
 
/*
 * This assembles the matrix for display that translates directly
 * to the RAW key value of the key that is up or down
 */
 
void Display_Matrix ( int *keyMatrix )
{
  int bitcount;
  int bytecount;
  int mask;
  int twobyte;
 
  Printf( "\n    0 1 2 3 4 5 6 7" );
  Printf( "\n  +-----------------" );
  for ( bitcount = 0; bitcount < 16; bitcount++ )
  {
    Printf( "\n%lx |", bitcount );
    mask = ( 1 << bitcount );
    for ( bytecount = 0; bytecount < 16; bytecount += 2 )
    {
      twobyte = keyMatrix [ bytecount ] | ( keyMatrix [ bytecount + 1 ] << 8 );
      if ( twobyte & mask )
      {
        Printf( " *" );
      }
      else
      {
        Printf( " -" );
      }
    }
  }
 
  Printf( "\n\n" );
}
 
int main ( int argc, char *argv[] )
{
  struct IOStdReq *KeyIO;
  struct MsgPort *KeyMP;
  int *keyMatrix;
 
  if ( KeyMP = AllocSysObjectTags ( ASOT_PORT, TAG_END ) )
  {
    if ( KeyIO = AllocSysObjectTags ( ASOT_IOREQUEST, ASOIOR_ReplyPort, KeyMP, ASOIOR_Size, sizeof ( struct IOStdReq ), TAG_END ) )
    {
      if ( ! OpenDevice ( "keyboard.device", 0, ( struct IORequest * ) KeyIO, 0 ) )
      {
        if ( keyMatrix = AllocVecTags ( MATRIX_SIZE, AVT_ClearWithValue, 0, TAG_END ) )
        {
          KeyIO->io_Command = KBD_READMATRIX;
          KeyIO->io_Data    = ( APTR ) keyMatrix;
          KeyIO->io_Length  = MATRIX_SIZE;
          DoIO ( ( struct IORequest *) KeyIO );
 
          /* Check for CLI startup... */
          if ( argc )
          {
            Display_Matrix( keyMatrix );
          }
 
          FreeVec ( keyMatrix );
        }
        else
        {
          Printf( "Error: Could not allocate keymatrix memory\n" );
        }
 
        CloseDevice ( ( struct IORequest * ) KeyIO );
      }
      else
      {
        Printf( "Error: Could not open keyboard.device\n" );
      }
 
      FreeSysObject ( ASOT_IOREQUEST, KeyIO );
    }
    else
    {
      Printf( "Error: Could not create I/O request\n" );
    }
 
    FreeSysObject ( ASOT_PORT, KeyMP );
  }
  else
  {
    Printf( "Error: Could not create message port\n" );
  }
 
  return ( 0 );
}