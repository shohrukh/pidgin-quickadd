#define PURPLE_PLUGINS

#include <glib.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"
#include "debug.h"

#include "gtkplugin.h"
#include "gtkprefs.h"
#include "gtkutils.h"
#include "gtkimhtml.h"

#include <string.h>

#define PLUGIN_ID "gtk-mccorry-quickadd"

PurplePlugin *quickadd_plugin = NULL;

static void add_buddy(GtkWidget *w, gchar *username)
{
	purple_blist_request_add_buddy(NULL, username, NULL, NULL);
}

static void url_copy(GtkWidget *w, gchar *url)
{

	GtkClipboard *clipboard;

	clipboard = gtk_widget_get_clipboard(w, GDK_SELECTION_PRIMARY);
	gtk_clipboard_set_text(clipboard, url, -1);

	clipboard = gtk_widget_get_clipboard(w, GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clipboard, url, -1);
}

static gboolean
add_menu(GtkIMHtml *imhtml, GtkIMHtmlLink *link, GtkWidget *menu)
{
	GtkWidget *img, *item;
	const char *text;
	char *address;
#define MAILTOSIZE  (sizeof("mailto:") - 1)

	text = gtk_imhtml_link_get_url(link);
	g_return_val_if_fail(text && strlen(text) > MAILTOSIZE, FALSE);
	address = (char*)text + MAILTOSIZE;

	/* Copy Email Address */
	img = gtk_image_new_from_stock(GTK_STOCK_COPY, GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_mnemonic(("_Copy Email Address"));
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), img);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(url_copy), address);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

	/* Add Buddy */
	//img = gtk_image_new_from_stock(GTK_STOCK_COPY, GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_mnemonic(("_Add Buddy"));
	//gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), img);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(add_buddy), address);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

	return TRUE;

//GtkIMHtml->protocols
}

static gboolean
url_clicked_idle_cb(gpointer data)
{
	purple_notify_uri(NULL, data);
	g_free(data);
	return FALSE;
}

static gboolean
url_clicked_cb(GtkIMHtml *unused, GtkIMHtmlLink *link)
{
	const char *uri = gtk_imhtml_link_get_url(link);
	g_idle_add(url_clicked_idle_cb, g_strdup(uri));
	return TRUE;
}

static gboolean
plugin_load(PurplePlugin *plugin) {



	gtk_imhtml_class_register_protocol("mailto:", NULL, NULL);

	gtk_imhtml_class_register_protocol("mailto:", url_clicked_cb, add_menu);

	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	return TRUE;
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    PLUGIN_ID,
    "QuickAdd",
    "0.1",

    "QuickAdd",          
    "QuickAdd Plugin",          
    "Matthew McCorry <mccorry@gmail.com>",                          
    "http://helloworld.tld",     
    
    plugin_load,                   
    plugin_unload,                          
    NULL,                          
                                   
    NULL,                          
    NULL,                          
    NULL,                        
    NULL,                   
    NULL,                          
    NULL,                          
    NULL,                          
    NULL                           
};                               
    
static void                        
init_plugin(PurplePlugin *plugin)
{                                  
}

PURPLE_INIT_PLUGIN(hello_world, init_plugin, info)
