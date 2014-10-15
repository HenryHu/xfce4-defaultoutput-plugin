#ifndef __DEFAULTOUTPUT_H_
#define __DEFAULTOUTPUT_H_

G_BEGIN_DECLS

typedef struct {
	XfcePanelPlugin *plugin;

	GtkWidget *ebox;
	GtkWidget *button;
	GtkWidget *image;

	int unit_count;
	int piclib_count;
	GdkPixbuf **pixbufs;
} DefaultOutputPlugin;

G_END_DECLS

#endif // __DEFAULTOUTPUT_H_
