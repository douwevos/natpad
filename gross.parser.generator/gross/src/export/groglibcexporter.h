//
///*
//   File:    groglibcexporter.h
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 25, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef EXPORT_GROGLIBCEXPORTER_H_
//#define EXPORT_GROGLIBCEXPORTER_H_
//
//
//#include "../model/gromodel.h"
//#include "../state/grostatemachine.h"
//#include "../parser/bnf/grobnfspec.h"
//#include "gropurechelper.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_GLIBC_EXPORTER              (gro_glibc_exporter_get_type())
//#define GRO_GLIBC_EXPORTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_glibc_exporter_get_type(), GroGlibCExporter))
//#define GRO_GLIBC_EXPORTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_GLIBC_EXPORTER, GroGlibCExporterClass))
//#define GRO_IS_GLIBC_EXPORTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_GLIBC_EXPORTER))
//#define GRO_IS_GLIBC_EXPORTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_GLIBC_EXPORTER))
//#define GRO_GLIBC_EXPORTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_GLIBC_EXPORTER, GroGlibCExporterClass))
//
//
//typedef struct _GroGlibCExporter               GroGlibCExporter;
//typedef struct _GroGlibCExporterPrivate        GroGlibCExporterPrivate;
//typedef struct _GroGlibCExporterClass          GroGlibCExporterClass;
//
//
//struct _GroGlibCExporter {
//	GObject parent;
//};
//
//struct _GroGlibCExporterClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_glibc_exporter_get_type();
//
//GroGlibCExporter *gro_glibc_exporter_new();
//
//void gro_glibc_exporter_do_export(GroGlibCExporter *exporter, GroModel *model, GroStateMachine *state_machine, GroBnfSpec *spec);
//
//G_END_DECLS
//
//#endif /* EXPORT_GROGLIBCEXPORTER_H_ */
