#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <windows.h>
#include <ShellApi.h>
#include <gio/gio.h> //Needed to release builds

static void load_css (void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    const gchar *css_style_file = "style.css";// In order to embed the CSS in EXE, "resource:///gui/gui.glade
    GFile *css_fp = g_file_new_for_path(css_style_file);// In order to embed the CSS in EXE, use g_file_new_for_uri
    GError *error = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_css_provider_load_from_file(provider,css_fp,&error);
    g_object_unref(provider);
}

GtkWidget *filechooser, *entrysig2, *entrysig3, *entrysig4, *window2, *window3;
char name[120];
char nametrimmed[120];
char mode[7];
int l=0;
int main(int argc, char *argv[])
{
    putenv ("LANG=en_US");
    GtkBuilder *builder;
    GtkWindow *window, *vpanedc;
    GtkWidget *hostsig, *clientsig, *playsig, *optionsig, *exitsig, *aboutsig, *labelcolor1, *labelcolor2, *labelcolor3, *labelcolor4, *labelcolor5,
              *labelcolor6, *labelcolor7, *tablesig, *tablesig2;
    GtkStyleContext *context;

    gtk_init(&argc, &argv);
    load_css();
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui.glade" , NULL);//In order to embed the Glade GUI to EXE, use gtk_builder_add_from_resource;

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    window2 = GTK_WIDGET(gtk_builder_get_object(builder, "aboutdialog1"));
    window3 = GTK_WIDGET(gtk_builder_get_object(builder, "dialog1"));
    filechooser = GTK_WIDGET(gtk_builder_get_object(builder, "filechooserbutton"));
    hostsig = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1"));
    clientsig = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2"));
    playsig = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    optionsig = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    exitsig = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    aboutsig = GTK_WIDGET(gtk_builder_get_object(builder, "button4"));
    tablesig = GTK_WIDGET(gtk_builder_get_object(builder, "table1"));
    labelcolor1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    labelcolor2 = GTK_WIDGET(gtk_builder_get_object(builder, "label2"));
    labelcolor3 = GTK_WIDGET(gtk_builder_get_object(builder, "label3"));
    labelcolor4 = GTK_WIDGET(gtk_builder_get_object(builder, "label4"));
    labelcolor5 = GTK_WIDGET(gtk_builder_get_object(builder, "label5"));
    labelcolor6 = GTK_WIDGET(gtk_builder_get_object(builder, "label6"));
    labelcolor7 = GTK_WIDGET(gtk_builder_get_object(builder, "label7"));
    entrysig2 = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));
    entrysig3 = GTK_WIDGET(gtk_builder_get_object(builder, "entry3"));
    entrysig4 = GTK_WIDGET(gtk_builder_get_object(builder, "entry4"));
    gtk_widget_set_name(labelcolor1, "label_color1");
    gtk_widget_set_name(labelcolor2, "label_color2");
    gtk_widget_set_name(labelcolor3, "label_color3");
    gtk_widget_set_name(labelcolor4, "label_color4");
    gtk_widget_set_name(labelcolor5, "label_color5");
    gtk_widget_set_name(labelcolor6, "label_color6");
    gtk_widget_set_name(labelcolor7, "label_color7");
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

G_MODULE_EXPORT void on_filechooserbutton_file_set()
{
    int i, j,slashes, trim=0, sub;
    strcpy(name,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser)));
    for(i=0; i<120; i++)
    {
        if(name[i] == 92)
        {
            j++;
        }
    }
    int length = strlen(name);
    for(i=0; i<120; i++)
    {
        trim++;
        if(name[i] == 92)
        {
            slashes++;
        }
        if(slashes == j)
        {
            break;
        }
    }
    i = 0;
    while(name[trim] != '.')
    {
        nametrimmed[i] = name[trim];
        i++;
        trim++;
    }
}

G_MODULE_EXPORT void on_radiobutton1_toggled()
{
    l++;
    if(l%2 == 1)
    {
        gtk_widget_set_sensitive(entrysig3, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(entrysig3, FALSE);
    }
}

G_MODULE_EXPORT void on_button1_clicked(GtkWidget* widget, gpointer window)
{
    const gchar *text1 = gtk_entry_get_text(GTK_ENTRY (entrysig2));
    const gchar *text2 = gtk_entry_get_text(GTK_ENTRY (entrysig3));
    const gchar *text3 = gtk_entry_get_text(GTK_ENTRY (entrysig4));
    char *mamearg = malloc(sizeof(char));
    if(nametrimmed[0] == '\0' || text1[0] == '\0' || text2[0] == '\0' && l%2==1 || text3[0] == '\0')
    {
        gint response = gtk_dialog_run(GTK_DIALOG(window3));
        gtk_widget_hide(window3);
    }
    else
    {
        if(l%2 == 0)
        {
            char host[] = " -server ";
            strcat(mode, host);
        }
        if(l%2 == 1)
        {
            char client[] = " -client ";
            strcat(mode, client);
        }
        strcat(mamearg, "/c mame64 ");

        strcat(mamearg, " ");
        strcat(mamearg, nametrimmed);
        strcat(mamearg, mode);
        strcat(mamearg, "-port ");
        strcat(mamearg, text3);
        if(l%2 == 1)
        {
            strcat(mamearg, " -hostname ");
            strcat(mamearg, text2);
        }
        strcat(mamearg, " -username ");
        strcat(mamearg, text1);

        printf("%s", mamearg);
        ShellExecuteA(GetDesktopWindow(),"open","cmd",mamearg,NULL,SW_NORMAL);
        free(mamearg);
        exit(0);// temporary
    }
}

G_MODULE_EXPORT void on_button3_clicked()
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_button4_clicked(GtkWidget* widget, gpointer window)
{
    gint response = gtk_dialog_run(GTK_DIALOG(window2));
    gtk_widget_hide(window2);
}

G_MODULE_EXPORT void on_window_main_destroy()
{
    gtk_main_quit();
}
