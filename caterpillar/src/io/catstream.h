/*
   File:    catstream.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 15, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#ifndef CATSTREAM_H_
#define CATSTREAM_H_

enum _CatStreamState {
	STREAM_INIT,
	STREAM_OPENED,
	STREAM_CLOSED
};

typedef enum _CatStreamState CatStreamState;


enum _CatStreamStatus {
	CAT_STREAM_OK=0,
	CAT_STREAM_INVALID_ENCODING,
	CAT_STREAM_CLOSED
};

enum _CatStreamEol {
	CAT_STREAM_EOL_DEFAULT=0,
	CAT_STREAM_EOL_UNIX,
	CAT_STREAM_EOL_WIN,
	CAT_STREAM_EOL_NONE,
};

typedef enum _CatStreamStatus CatStreamStatus;



typedef enum _CatSeekType CatSeekType;

enum _CatSeekType {
	CAT_SEEK_SET,
	CAT_SEEK_SET_FROM_END,
	CAT_SEEK_FORWARD,
};


#endif /* CATSTREAM_H_ */
