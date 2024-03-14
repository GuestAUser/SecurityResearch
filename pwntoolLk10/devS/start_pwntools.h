#include <stdio.h>
#include <gtk/gtk.h>

#define MAX_LIST 5000

void add_tool(GtkButton *addButton, void *user_data) {
	//LOGIC> add_tool adds a tool when addButton is clicked, addButton must create an ARRAY of tools, so it can be indexed in memory for performance.
  char list_tools[MAX_LIST]; //-> this is my array;

  //set up the rest of the app to get to the add_tool screen
  
  /* LISTCODE GOES HERE   */
}

void select_tool(GtkButton *startButton, void *user_data) {
	g_print("DEBUG: execution of select_tool function -> true");
	//set up function for selecting the respective tools
}
