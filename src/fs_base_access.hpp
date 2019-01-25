/*
  ISC License

  Copyright (c) 2016, Antonio SJ Musumeci <trapexit@spawn.link>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once

#include <string>

#include <fcntl.h>
#include <unistd.h>

namespace fs
{
  static
  inline
  int
  access(const int          dirfd_,
         const std::string &path_,
         const int          mode_,
         const int          flags_)
  {
    return ::faccessat(dirfd_,path_.c_str(),mode_,flags_);
  }

  static
  inline
  int
  access(const std::string &path_,
         const int          mode_,
         const int          flags_)
  {
    return fs::access(AT_FDCWD,path_,mode_,flags_);
  }

  static
  inline
  int
  eaccess(const std::string &path_,
          const int          mode_)
  {
    return fs::access(path_,mode_,AT_EACCESS);
  }
}
