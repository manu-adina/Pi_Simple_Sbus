#include <gtk/gth.h>



static void cb_send_data() {

}

static void activate(GtkApplication *app, gpointer user_data) {
    
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    window = gtk_application_window_new(app);
    gtk_widow_set_title(GTK_WINDOW (window), "Ronin Control Simple GUI");
    gtk_container_set_border_width(GTK_CONTAINER (window), 10);

    slider1_box = 
    slider2_box = 
    slider3_box = 
    button_stop_box = 

    button = gtk_button_new_with_label("Stop");
    g_signal_connect(button_stop, "clicked", G_CALLBACK(cb_send_data), NULL); //TODO: need to send args.
    slider1 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 2074, 1);
    g_signal_connect(slider, );
    slider2 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 2074, 1); 
    g_signal_connect(slider, );
    slider3 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 2074, 1); 
    g_signal_connect(slider, );

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.exmaple", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}


