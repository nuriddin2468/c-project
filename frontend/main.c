#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>


#include <stdio.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8082

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *loginEntry;
GtkWidget *passwordEntry;
GtkBuilder *builder;

GtkWidget *waiterWindow;


int sock;

// login window
int main(int argc, char *argv[]){




/*

		Socket connection

*/

	struct sockaddr_in address;
	int valread;
	struct sockaddr_in serv_addr;
	char *hello = "hello from client";
	char buffer[1024] = {0}, cchat[1024];
	char *bye = "bye";
	printf("CREATING CLIENT SOCKET ...\n");
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	
	printf("DEFINING SOCKET FAMILY, ADDRESS & PORT ....\n");	
	
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	//Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("Invalid address Address not supported \n");
	}
	
	printf("CLIENT CONNECTING ON PORT 8080 TO COMMUNICATE WITH SERVER..\n");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Connection Failed\n");
		return -1;
	}





/*

		End of connection

*/




	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("login.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	loginEntry = GTK_WIDGET(gtk_builder_get_object(builder, "loginEntry"));
	passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
	
	gtk_widget_show(window);
	gtk_main();
	return EXIT_SUCCESS;
}
// close window
void hideLoginPage(){
	gtk_widget_hide(window);
}

// Waiter's window
void showWaiterWindow(){
	hideLoginPage();
	builder = gtk_builder_new_from_file("waiter.glade");
	waiterWindow = GTK_WIDGET(gtk_builder_get_object(builder, "waiterWindow"));
	g_signal_connect(waiterWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(waiterWindow);
	gtk_main();
}

// login's sign in button handler
void on_sign_clicked(GtkButton *b) {
	send(sock, "login", strlen("login"), 0);
	gchar *login_text;
	gchar *password_text;
	showWaiterWindow();
	// login_text = gtk_entry_get_text(GTK_ENTRY(loginEntry));   // get user's login but xz error gives  
 //    password_text = gtk_entry_get_text(GTK_ENTRY(passwordEntry)); // get user's password
};




// Waiter window handlers
void on_orderButton_clicked(GtkButton *b) {
	printf("%s\n", "order button clicked");
}

void on_check1_toggled(GtkToggleButton *t) {
	printf("%s\n", "check button clicked clicked");
}


// void on_button1_clicked(GtkButton *b){
// 	char tmp[128];
// 	gdouble val = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin1));
// 	sprintf(tmp, "spin=%d", (int)val);
// 	gtk_label_set_text(GTK_LABEL(label1), (const gchar* ) tmp);

// }

// void on_check1_toggled(GtkToggleButton *b){
// 	gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
// 	if (T) 	gtk_label_set_text(GTK_LABEL(label1), (const gchar* ) "check1 box toggled");
// 	else gtk_label_set_text(GTK_LABEL(label1), (const gchar*) "");
// }
