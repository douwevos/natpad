/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2010, 2011 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef ___IO_ERROR_CODES_INCLUDED
#define ___IO_ERROR_CODES_INCLUDED

#define IO_ERR__EOF                     0x01
#define IO_ERR__HARDWARE_ERROR          0x02
#define IO_ERR__IS_DIRECTORY            0x03
#define IO_ERR__NO_DISK_SPACE_LEFT      0x04
#define IO_ERR__NO_READ_PERMISSION      0x05
#define IO_ERR__NO_SUCH_FILE            0x06
#define IO_ERR__NO_WRITE_PERMISSION     0x07
#define IO_ERR__STREAM_NOT_UTF8_ENCODED 0x08
#define IO_ERR__TOO_MANY_STREAMS_OPEN   0x09
#define IO_ERR__UNKNOWN_ERROR           0x0A

#endif
