#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4panel/xfce-hvbox.h>
#include <sys/sysctl.h>

#include "defaultoutput.h"

#define ICON "preferences-desktop-sound"
#define TARGET_SYSCTL "hw.snd.default_unit"

// get & set
static int get_current_unit() {
	int buf;
	size_t len = sizeof(buf);
	int ret;

	ret = sysctlbyname(TARGET_SYSCTL, &buf, &len, NULL, 0);
	if (ret < 0) return -1;
	return buf;
}

static int set_current_unit(int new_default) {
	return sysctlbyname(TARGET_SYSCTL, NULL, NULL, &new_default, sizeof(new_default));
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

// new and free

static DefaultOutputPlugin *defaultoutput_new(XfcePanelPlugin *plugin) {
	DefaultOutputPlugin *defaultoutput;

	defaultoutput = panel_slice_new0(DefaultOutputPlugin);
	defaultoutput->plugin = plugin;

	int current_unit = get_current_unit();
	
	return defaultoutput;
}

static void defaultoutput_free(XfcePanelPlugin *plugin,
		DefaultOutputPlugin *defaultoutput) {
	panel_slice_free(DefaultOutputPlugin, defaultoutput);
}

static void defaultoutput_construct(XfcePanelPlugin *plugin) {
	DefaultOutputPlugin *defaultoutput;

//	xfce4_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

	defaultoutput = defaultoutput_new(plugin);

	g_signal_connect(G_OBJECT(plugin), "free-data",
			G_CALLBACK(defaultoutput_free), defaultoutput);

	xfce_panel_plugin_menu_show_about(plugin);
	g_signal_connect(G_OBJECT(plugin), "about",
			G_CALLBACK(defaultoutput_about), NULL);
}

XFCE_PANEL_PLUGIN_REGISTER(defaultoutput_construct)
