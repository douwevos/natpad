/*
   File:    draiconnectorrequestfactory.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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


#include "draiconnectorrequestfactory.h"

G_DEFINE_INTERFACE(DraIConnectorRequestFactory, dra_iconnector_request_factory, G_TYPE_OBJECT);

static void dra_iconnector_request_factory_default_init (DraIConnectorRequestFactoryInterface *iface) {
}
