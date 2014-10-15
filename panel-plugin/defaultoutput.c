#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4panel/xfce-hvbox.h>
#include <sys/sysctl.h>
#include <assert.h>

#include "defaultoutput.h"

#define ICON "preferences-desktop-sound"
#define TARGET_SYSCTL "hw.snd.default_unit"

static const char *icons[] = {
	"audio-volume-high",
	"audio-volume-low",
	NULL
};

// get & set
static int defaultoutput_get_current_unit() {
	int buf;
	size_t len = sizeof(buf);
	int ret;

	ret = sysctlbyname(TARGET_SYSCTL, &buf, &len, NULL, 0);
	if (ret < 0) return -1;
	return buf;
}

static int defaultoutput_set_current_unit(int new_default) {
	return sysctlbyname(TARGET_SYSCTL, NULL, NULL, &new_default, sizeof(new_default));
}

// update
static void defaultoutput_update_units(DefaultOutputPlugin *defaultoutput) {
	// TODO: get the total unit count and update pixbufs
	defaultoutput->unit_count = 2;
}

static void defaultoutput_update_state(DefaultOutputPlugin *defaultoutput) {
	defaultoutput_update_units(defaultoutput);

	int current_unit = defaultoutput_get_current_unit();
	if (current_unit < 0) {
		g_critical("DefaultOutputPlugin: fail to get current unit\n");
		return;
	}
	assert(current_unit < defaultoutput->unit_count);

	GdkPixbuf *pixbuf = defaultoutput->pixbufs[current_unit];
	xfce_panel_image_set_from_pixbuf(XFCE_PANEL_IMAGE(defaultoutput->image),
			pixbuf);

	gchar *tip = g_strdup_printf(_("Current unit: %d"), current_unit);
	gtk_widget_set_tooltip_text(GTK_WIDGET(defaultoutput->button), tip);
	g_free(tip);
}

// about
static void defaultoutput_about(XfcePanelPlugin *plugin, gpointer data) {
	GdkPixbuf *icon = xfce_panel_pixbuf_from_source(ICON, NULL, 32);

	const gchar *authors[] = {"Henry Hu <henry.hu.sh@gmail.com>", NULL};
	gtk_show_about_dialog(NULL,
			"logo", icon,
			"license", xfce_get_license_text(XFCE_LICENSE_TEXT_BSD),
			"version", "0.1",
			"program-name", "xfce4-defaultoutput-plugin",
			"authors", authors,
			"comments", _("Monitor and change default sound output device"),
			"website", "https://github.com/HenryHu/xfce4-defaultoutput-plugin",
			"copyright", _("Copyright (c) 2014"),
			NULL);
	if (icon) g_object_unref(G_OBJECT(icon));
}

// size changed
static gboolean defaultoutput_size_changed(XfcePanelPlugin *plugin,
		gint size, DefaultOutputPlugin *defaultoutput) {

	gtk_container_set_border_width(GTK_CONTAINER(defaultoutput->ebox), 1);
	gtk_widget_set_size_request(GTK_WIDGET(defaultoutput->image), 32, 32);
	return TRUE;
}
// new and free

static DefaultOutputPlugin *defaultoutput_new(XfcePanelPlugin *plugin) {
	DefaultOutputPlugin *defaultoutput;

	defaultoutput = panel_slice_new0(DefaultOutputPlugin);
	defaultoutput->plugin = plugin;

	defaultoutput->piclib_count = 2;
	defaultoutput->pixbufs = malloc(
			sizeof(GdkPixbuf*) * defaultoutput->piclib_count);

	for (int i=0; i<2; i++) {
		defaultoutput->pixbufs[i] = xfce_panel_pixbuf_from_source(icons[i],
				NULL, 32);
	}

	defaultoutput->ebox = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(plugin), defaultoutput->ebox);
	gtk_widget_show(defaultoutput->ebox);

	defaultoutput->button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(defaultoutput->ebox), defaultoutput->button);
	gtk_widget_show(defaultoutput->button);

	defaultoutput->image = xfce_panel_image_new ();
	gtk_container_add(GTK_CONTAINER(defaultoutput->button), defaultoutput->image);
	gtk_widget_show (defaultoutput->image);

	/* Make the button look flat and make it never grab the focus */
	gtk_button_set_relief(GTK_BUTTON(defaultoutput->button), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(defaultoutput->button), FALSE);
	gtk_widget_set_can_default(GTK_WIDGET(defaultoutput->button), FALSE);
	gtk_widget_set_can_focus(GTK_WIDGET(defaultoutput->button), FALSE);

	defaultoutput_update_state(defaultoutput);

	
	return defaultoutput;
}

static void defaultoutput_free(XfcePanelPlugin *plugin,
		DefaultOutputPlugin *defaultoutput) {
	for (int i=0; i<defaultoutput->piclib_count; i++) {
		g_object_unref(G_OBJECT(defaultoutput->pixbufs[i]));
	}
	free(defaultoutput->pixbufs);
	gtk_widget_destroy(defaultoutput->ebox);
	panel_slice_free(DefaultOutputPlugin, defaultoutput);
}

static void defaultoutput_construct(XfcePanelPlugin *plugin) {
	DefaultOutputPlugin *defaultoutput;

//	xfce4_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

	defaultoutput = defaultoutput_new(plugin);

	g_signal_connect(G_OBJECT(plugin), "free-data",
			G_CALLBACK(defaultoutput_free), defaultoutput);
	g_signal_connect(G_OBJECT(plugin), "size-changed",
			G_CALLBACK(defaultoutput_size_changed), defaultoutput);

	xfce_panel_plugin_menu_show_about(plugin);
	g_signal_connect(G_OBJECT(plugin), "about",
			G_CALLBACK(defaultoutput_about), NULL);
}

XFCE_PANEL_PLUGIN_REGISTER(defaultoutput_construct)
