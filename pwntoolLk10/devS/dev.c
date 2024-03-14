//lk10 development program
#include <stdio.h>
#include <gtk/gtk.h>
#include "start_pwntools.h"

void main_window(GtkWidget *widget, gpointer window) {
  GtkWidget *dialog;
  dialog =  gtk_message_dialog_new(GTK_WINDOW(window),
                                   GTK_DIALOG_DESTROY_WITH_PARENT, 
                                   GTK_MESSAGE_INFO, 
                                   GTK_BUTTONS_OK, 
                                   "Version 1.0.0, visit github at GuestAUser for more info!");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
  printf("↨ -- devanalyser2.0 -- ↨\n\n");
  //start gui 
  int x, y;
  x = 500; //width
  y = 400; //heigth

  printf("[debug]: window size = %d by %dpx\n", x, y); //REMOVE LATERE 

  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *startButton;  //start general interface
  GtkWidget *exitButton;
  GtkCssProvider *provider; //this is resposible for the app css style

  gtk_init(&argc, &argv); 

  //TODO: 
  //*1  - set up css file support for styling -> [FINISHED]
  // 2  - Make the start button actually start something;
  // 3  - fuck my self up aaaaaa lulzsec for life
   
  //setting up window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "lk10pwn Center");
  gtk_window_set_default_size(GTK_WINDOW(window), x, y);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window),90);
  //border width for the whole window to set the box sizes is a bad code practice
  //review it later for no problems when dealing with interface management with such a big border sucking up space..

  //setting up containers and layout management
  box = gtk_box_new(TRUE, 1);
  gtk_container_add(GTK_CONTAINER(window), box);

  //set up style for gtk window with css 
  provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "styleDev.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), 
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER); //notes: used for user permission inside the gtk app I believe

  //set up buttons
  startButton = gtk_button_new_with_label("Start PwnTools");
  exitButton = gtk_button_new_with_label("Exit");

  //configure box container
  gtk_box_pack_start(GTK_BOX(box), startButton, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), exitButton, TRUE, TRUE, 0);

  //set up button size
  gtk_widget_set_size_request(startButton, 100, 120);
  //make button connect to function
  g_signal_connect(startButton, "clicked", 
                   G_CALLBACK(main_window), NULL);
  g_signal_connect(exitButton, "clicked", 
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window); //gui appears

  g_signal_connect(window, "destroy", 
                   G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
  return 0;
}
