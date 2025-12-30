#ifndef BOXO_FILEIO_H
#define BOXO_FILEIO_H

//============================================================================

#include "types.h"

//--------------------------------------------------------

//--------------------------------------------------------
extern int GetFileSize(FILE* f);
extern int GetFileSizeByName(char* name);
extern int ReadFile(char* name, UBYTE* buffer, int size);
extern int WriteFile(char* name, UBYTE*buffer, int size);

//============================================================================

#endif // BOXO_FILEIO_H
