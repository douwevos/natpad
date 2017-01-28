
/*
   File:    spiservice.h
   Project: spider
   Author:  Douwe Vos
   Date:    Jul 18, 2016
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

#ifndef SPISERVICE_H_
#define SPISERVICE_H_

#include <caterpillar.h>
#include <elk.h>
#include <worm.h>

G_BEGIN_DECLS

#define SPI_TYPE_SERVICE              (spi_service_get_type())
#define SPI_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_service_get_type(), SpiService))
#define SPI_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_SERVICE, SpiServiceClass))
#define SPI_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_SERVICE))
#define SPI_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_SERVICE))
#define SPI_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_SERVICE, SpiServiceClass))


typedef struct _SpiService               SpiService;
typedef struct _SpiServicePrivate        SpiServicePrivate;
typedef struct _SpiServiceClass          SpiServiceClass;


struct _SpiService {
	GObject parent;
};

struct _SpiServiceClass {
	GObjectClass parent_class;
};


GType spi_service_get_type();

SpiService *spi_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* SPISERVICE_H_ */
