/*
   File:    chameleon.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 22, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef CHAMELEON_H_
#define CHAMELEON_H_

#include "chaeditor.h"
#include "document/chadocument.h"
#include "document/charevisionreader.h"
#include "document/chalineiterator.h"
#include "layout/chaidocumentviewlistener.h"
#include "layout/chadocumentview.h"
#include "layout/chaviewcursor.h"
#include "layout/chasublinecache.h"
#include "layout/chapangoattrholder.h"
#include "io/chadocumentmanager.h"
#include "io/chaioasync.h"
#include "io/chaasyncbasic.h"
#include "selection/chaselection.h"
#include "uow/chauow.h"
#include "uow/chauowclipboard.h"
#include "uow/chauowchangecase.h"
#include "uow/chauowselect.h"
#include "uow/chauowundoredo.h"
#include "uow/chauowremovetext.h"
#include "uow/chauowlinemarker.h"
#include "uow/chauowmovecursor.h"
#include "uow/chauowreplacetext.h"
#include "uow/chauowinserttext.h"
#include "uow/chauowlinesmove.h"
#include "uow/chauowlinesremove.h"
#include "uow/chauowinserttemplate.h"
#include "uow/chauownextformfield.h"
#include "uow/chauowformclear.h"
#include "preferences/chapreferenceswo.h"
#include "preferences/chaprefscolormapwo.h"
#include "preferences/chaprefstemplatewo.h"
#include "printing/chaeditorprinter.h"
#include "search/chasearchdialog.h"
#include "search/chasearchservice.h"
#include "search/chaoccurrencesresultset.h"
#include "search/chasearchpageoccurrences.h"
#include "search/chasearchoccurrence.h"
#include "search/chasearchquerywo.h"

#endif /* CHAMELEON_H_ */
