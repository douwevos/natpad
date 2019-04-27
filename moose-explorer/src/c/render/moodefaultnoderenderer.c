/*
   File:    moodefaultnoderenderer.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "moodefaultnoderenderer.h"
#include "mooinoderenderer.h"
#include "../moonodelayout.h"
#include <viper.h>
#include <moose.h>
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooDefaultNodeRenderer"
#include <logging/catlog.h>

static void l_node_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooDefaultNodeRenderer, moo_default_node_renderer, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_node_renderer_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_default_node_renderer_class_init(MooDefaultNodeRendererClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_default_node_renderer_init(MooDefaultNodeRenderer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(moo_default_node_renderer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_default_node_renderer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooDefaultNodeRenderer *moo_default_node_renderer_new() {
	MooDefaultNodeRenderer *result = g_object_new(MOO_TYPE_DEFAULT_NODE_RENDERER, NULL);
	cat_ref_anounce(result);
	return result;
}

/********************* begin MooINodeRenderer implementation *********************/

static void pixbuf_loader_size_prepared (GdkPixbufLoader *loader, int width, int height, gpointer desired_size_ptr) {
    int size, desired_size;
    float scale;

    size = MAX (width, height);
    desired_size = GPOINTER_TO_INT (desired_size_ptr);

    if (size != desired_size) {
        scale = (float) desired_size / size;
        gdk_pixbuf_loader_set_size (loader, floor (scale * width + 0.5), floor (scale * height + 0.5));
    }
}

#define LOAD_BUFFER_SIZE 65536

GdkPixbuf *eel_gdk_pixbuf_load_from_stream_at_size (GInputStream  *stream, int size) {
    char buffer[LOAD_BUFFER_SIZE];
    gssize bytes_read;
    GdkPixbufLoader *loader;
    GdkPixbuf *pixbuf;
    gboolean got_eos;

    g_return_val_if_fail (stream != NULL, NULL);

    got_eos = FALSE;
    loader = gdk_pixbuf_loader_new ();

    if (size > 0) {
        g_signal_connect (loader, "size-prepared",
                          G_CALLBACK (pixbuf_loader_size_prepared),
                          GINT_TO_POINTER (size));
    }

    while (1) {
        bytes_read = g_input_stream_read (stream, buffer, sizeof (buffer), NULL, NULL);

        if (bytes_read < 0) {
            break;
        }
        if (bytes_read == 0) {
            got_eos = TRUE;
            break;
        }
        if (!gdk_pixbuf_loader_write (loader, (const guchar *) buffer, bytes_read, NULL)) {
            break;
        }
    }

    g_input_stream_close (stream, NULL, NULL);
    gdk_pixbuf_loader_close (loader, NULL);

    pixbuf = NULL;
    if (got_eos) {
        pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
        if (pixbuf != NULL) {
            g_object_ref (pixbuf);
        }
    }

    g_object_unref (loader);
    return pixbuf;
}

void l_node_renderer_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {
	int size = moo_node_layout_get_height(node_layout)-2;
	cat_log_debug("size=%d", size);

	MooNodeWo *node = moo_node_layout_get_node(node_layout);
	MooResourceContentWo *res_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());

	if (res_content!=NULL) {
		VipNode *viper_node = moo_resource_content_wo_get_viper_node(res_content);
		if (viper_node==NULL) {
			return;
		}
		VipIResource *content = vip_node_get_content(viper_node);
		if (VIP_IS_FS_FILE(content)) {
			VipPath *pp = vip_fs_file_get_path((VipFSFile *) content);
			CatStringWo *s = vip_path_to_string(pp);

			GFile *f = g_file_new_for_path(cat_string_wo_getchars(s));
			cat_unref_ptr(s);
			GError *error=NULL;
			GFileInfo *fi = g_file_query_info(f, G_FILE_ATTRIBUTE_STANDARD_ICON, G_FILE_QUERY_INFO_NONE, NULL, (GError **) &error);
			cat_log_debug("error=%o", error);
			if (error==NULL) {
				node_layout->icon = g_file_info_get_icon(fi);
				cat_log_debug("node_layout->icon=%s", g_type_name_from_instance((GTypeInstance *) node_layout->icon));

				if (G_IS_THEMED_ICON(node_layout->icon)) {
					GThemedIcon *themed_icon = (GThemedIcon *) node_layout->icon;
					const gchar * const *names = g_themed_icon_get_names(themed_icon);
					GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
//					icon = g_themed_icon_new_from_names ((char**) names, -1);

			        icon_theme = gtk_icon_theme_get_default ();
			        GtkIconInfo *gtkicon_info = gtk_icon_theme_choose_icon (icon_theme, (const char **)names, size, 0);
			        if (gtkicon_info) {
						GdkPixbuf *pixbuf = gtk_icon_info_load_icon(gtkicon_info, NULL);



						int pheight = gdk_pixbuf_get_height(pixbuf);
						if (pheight>size) {
							double scale = (double) size / pheight;
							GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
									gdk_pixbuf_get_width(pixbuf) * scale, pheight * scale,
											GDK_INTERP_BILINEAR);
							pixbuf = scaled_pixbuf;
						}



						cairo_format_t format = CAIRO_FORMAT_RGB24;
						if (gdk_pixbuf_get_has_alpha(pixbuf)) {
							format = CAIRO_FORMAT_ARGB32;
						}

						cairo_surface_t *surface = cairo_image_surface_create(format, gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));

						cairo_t *context = cairo_create(surface);
						gdk_cairo_set_source_pixbuf(context, pixbuf, 0.0, 0.0);
						cairo_paint(context);
						node_layout->surface = surface;
			        }


				} else if (G_IS_LOADABLE_ICON(node_layout->icon)) {
					GLoadableIcon *li = (GLoadableIcon *) node_layout->icon;
					cat_log_debug("li=%p", li);
					GInputStream *is = g_loadable_icon_load(li, size, NULL, NULL, NULL);
					GdkPixbuf *pixbuf = eel_gdk_pixbuf_load_from_stream_at_size(is, size);

					cairo_format_t format = CAIRO_FORMAT_RGB24;
					if (gdk_pixbuf_get_has_alpha(pixbuf)) {
						format = CAIRO_FORMAT_ARGB32;
					}

					cairo_surface_t *surface = cairo_image_surface_create(format, gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));
					cairo_t *context = cairo_create(surface);
					gdk_cairo_set_source_pixbuf(context, pixbuf, 0.0, 0.0);
					cairo_paint(context);
					node_layout->surface = surface;
				}
			}

		}
	}
}

void l_node_renderer_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
	MooNodeWo *node = moo_node_layout_get_node(node_layout);
//	int fontHeight = moo_node_layout_get_font_height(node_layout);

	int layout_x = moo_node_layout_get_x(node_layout);
	int layout_y = moo_node_layout_get_y(node_layout);
	int size = moo_node_layout_get_height(node_layout);

	layout_x += size;

	gboolean isExpanded = moo_node_layout_is_expanded(node_layout);
	gboolean paintMap = FALSE;

	MooResourceContentWo *res_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());

	if (res_content!=NULL) {
		cat_log_debug("res_content=%p", res_content);
		VipNode *viper_node = moo_resource_content_wo_get_viper_node(res_content);
		if (viper_node!=NULL) {
			cat_log_debug("viper_node=%p", viper_node);
			VipIResource *content = viper_node ? vip_node_get_content(viper_node) : NULL;
			if (VIP_IS_IMAP(content)) {
				paintMap = TRUE;
			}
		}
	}
	if (paintMap) {
		gboolean is_module = moo_node_wo_get_content(node, moo_module_content_wo_key())!=NULL;
		moo_default_node_renderer_draw_map(cairo, layout_x, layout_y, size, isExpanded, is_module);
	} else {
		if (node_layout->surface) {
			cairo_set_source_surface(cairo, node_layout->surface, layout_x, 0.0);
			cairo_paint(cairo);
		} else {
			moo_default_renderer_draw_file(cairo, layout_x, layout_y, size);
		}
	}
}



void moo_default_node_renderer_draw_map(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean as_opened, gboolean with_blue) {
	double x1  = 0.5+xoffset+(int) (size*0.05);
	double x2  = 0.5+xoffset+(int) (size*0.10);
	double x3  = 0.5+xoffset+(int) (size*0.25);
	double x3b = 0.5+xoffset+(int) (size*0.35);
	double x4  = 0.5+xoffset+(int) (size*0.75);
	double x5  = 0.5+xoffset+(int) (size*0.95);

	double y1 = 0.5+yoffset+(int) (size*0.20);
	double y2 = 0.5+yoffset+(int) (size*0.30);
	double y3 = 0.5+yoffset+(int) (size*0.45);
	double y4 = 0.5+yoffset+(int) (size*0.80);

	cairo_set_line_width(cairo, 0.5);

	if (!as_opened) {
		cairo_move_to(cairo, x4, y3);
		cairo_line_to(cairo, x4, y4);
		cairo_line_to(cairo, x1, y4);
		cairo_line_to(cairo, x1, y2);
		cairo_line_to(cairo, x2, y1);
		cairo_line_to(cairo, x3b, y1);
		cairo_line_to(cairo, x3b, y2);
		cairo_line_to(cairo, x4, y2);
		cairo_line_to(cairo, x4, y3);

		if (with_blue) {
			cairo_set_source_rgb(cairo, 0.7,0.7,1.0);
		} else {
			cairo_set_source_rgb(cairo, 1.0,1.0,0.5);
		}
		cairo_fill_preserve(cairo);

		cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
		cairo_stroke(cairo);

	} else {
		if (with_blue) {

			cairo_move_to(cairo, x1, y4);
			cairo_line_to(cairo, x1, y2);
			cairo_line_to(cairo, x2, y1);
			cairo_line_to(cairo, x3b, y1);
			cairo_line_to(cairo, x3b, y2);
			cairo_line_to(cairo, x4, y2);
			cairo_line_to(cairo, x4, y3);
			cairo_line_to(cairo, x3, y3);
			cairo_line_to(cairo, x1, y4);

			cairo_set_source_rgb(cairo, 1.0,1.0,0.5);
			cairo_fill_preserve(cairo);

			cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
			cairo_stroke(cairo);

			cairo_move_to(cairo, x1, y4);
			cairo_line_to(cairo, x3, y3);
			cairo_line_to(cairo, x5, y3);
			cairo_line_to(cairo, x4, y4);
			cairo_line_to(cairo, x1, y4);

			cairo_set_source_rgb(cairo, 0.7,0.7,1.0);
			cairo_fill_preserve(cairo);

			cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
			cairo_stroke(cairo);

		} else {
			cairo_move_to(cairo, x1, y4);
			cairo_line_to(cairo, x3, y3);
			cairo_line_to(cairo, x5, y3);
			cairo_line_to(cairo, x4, y4);
			cairo_line_to(cairo, x1, y4);
			cairo_line_to(cairo, x1, y2);
			cairo_line_to(cairo, x2, y1);
			cairo_line_to(cairo, x3b, y1);
			cairo_line_to(cairo, x3b, y2);
			cairo_line_to(cairo, x4, y2);
			cairo_line_to(cairo, x4, y3);

			cairo_set_source_rgb(cairo, 1.0,1.0,0.5);
			cairo_fill_preserve(cairo);

			cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
			cairo_stroke(cairo);
		}
	}

}

void moo_default_renderer_draw_file(cairo_t *cairo, double xoffset, int yoffset, double size) {
	double x1  = 0.5+xoffset+(int) (size*0.20);
	double x2  = 0.5+xoffset+(int) (size*0.60);
	double x3  = 0.5+xoffset+(int) (size*0.80);


	double y1 = 0.5+yoffset+(int) (size*0.10);
	double y2 = 0.5+yoffset+(int) (size*0.30);
	double y3 = 0.5+yoffset+(int) (size*0.90);

	cairo_set_line_width(cairo, 0.5);

	cairo_move_to(cairo, x1, y1);
	cairo_line_to(cairo, x1, y3);
	cairo_line_to(cairo, x3, y3);
	cairo_line_to(cairo, x3, y2);
	cairo_line_to(cairo, x2, y1);
	cairo_line_to(cairo, x1, y1);

	cairo_set_source_rgb(cairo, 1.0,1.0,1.0);
	cairo_fill_preserve(cairo);

	cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
	cairo_stroke(cairo);

}


static void l_node_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_node_renderer_update_layout;
	iface->paint = l_node_renderer_paint;
}

/********************* end MooINode implementation *********************/
