/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 El�as Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 31-Aug-2014 by El�as Lozada-Benavente
// Based on original created 12-Feb-2010 by El�as Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Archive.h
This file declares the Archive class.
*/

#ifndef E3_ARCHIVE_H
#define E3_ARCHIVE_H

#include "Path.h"
#include <fstream>

namespace E
{
namespace FileSystem
{
/*----------------------------------------------------------------------------------------------------------------------
Archive
----------------------------------------------------------------------------------------------------------------------*/
class Archive
{
public:
  enum OpenMode
  {
    eOpenModeRead,
    eOpenModeWrite,
    eOpenModeAppend
  };

  Archive();
  ~Archive();

  // Accessors
  const Path&   GetPath() const;
  bool          IsOpen() const;

  // Methods
  void          Close();
  bool          Open(const Path& filePath, OpenMode openMode = eOpenModeRead);
  void          Read(char* pTarget, size_t length);
  void          Write(const char* pSource, size_t length);

private:
  Path          mFilePath;
  std::fstream  mFileStream;

  E_DISABLE_COPY_AND_ASSSIGNMENT(Archive);
};
}
}

#endif
