#include <gtk/gtk.h>

GtkWidget *lbl;
GtkWidget *entry;
char tmp[128];

int main(int argc, char *argv[])
{

    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  

    lbl = GTK_WIDGET(gtk_builder_get_object(builder, "lbl"));
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry"));

    gtk_builder_connect_signals(builder, NULL);
    
    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_btn_clicked(GtkButton *btn, gpointer *e){
    g_print("\n%s\n",tmp);
    gtk_label_set_text(GTK_LABEL(lbl), "GOOO");
}

void on_entry_changed(GtkEntry *e){
   sprintf(tmp,"entry=%s",gtk_entry_get_text(e));

}
