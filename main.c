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

#define PORT 8085

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *loginEntry;
GtkWidget *passwordEntry;
GtkBuilder *builder;

GtkWidget *err;

GtkWidget *waiterWindow;
GtkWidget *chefWindow;
GtkWidget *checkWindow;
GtkWidget *cashierWindow;


char login[128], password[128];
char server_message[1900];


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

	builder = gtk_builder_new_from_file("glade/login.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	loginEntry = GTK_WIDGET(gtk_builder_get_object(builder, "loginEntry"));
	passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
	err = GTK_WIDGET(gtk_builder_get_object(builder, "err"));	

	gtk_widget_show(window);
	gtk_main();
	return EXIT_SUCCESS;
}
// close login window
void hideLoginPage(){
	gtk_widget_hide(window);
}

// Waiter's window
void showWaiterWindow(){
	hideLoginPage();
	builder = gtk_builder_new_from_file("glade/waiter.glade");
	waiterWindow = GTK_WIDGET(gtk_builder_get_object(builder, "waiterWindow"));
	g_signal_connect(waiterWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(waiterWindow);
	gtk_main();
}
// Cashier's window
void showCashierWindow(){
	hideLoginPage();
	builder = gtk_builder_new_from_file("glade/cashier.glade");
	cashierWindow = GTK_WIDGET(gtk_builder_get_object(builder, "cashierWindow"));
	g_signal_connect(cashierWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(cashierWindow);
	gtk_main();
}
// close cashier window
void hideCashierWindow(){
	gtk_widget_hide(cashierWindow);
}
void showCheckWindow(){
	builder = gtk_builder_new_from_file("glade/check.glade");
	checkWindow = GTK_WIDGET(gtk_builder_get_object(builder, "checkWindow"));
	g_signal_connect(checkWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(checkWindow);
	gtk_main();
}
// Open Checks
void on_waiterCheckList_clicked(GtkButton *b) {
	hideCashierWindow();
	showCheckWindow();
}
// Chef's window
void showChefWindow(){
	hideLoginPage();
	builder = gtk_builder_new_from_file("glade/chef.glade");
	chefWindow = GTK_WIDGET(gtk_builder_get_object(builder, "chefWindow"));
	g_signal_connect(chefWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(chefWindow);
	gtk_main();
}
// login's sign in button handler
void on_sign_clicked(GtkButton *b) {
	int read_size;
	memset(server_message, 0, sizeof(server_message));	
	g_print("\nlogin: %s\npassword: %s\n", login, password);
	read_size = send(sock, "0", strlen("0"), 0);
	printf("\nRecived size of bytes: %d\n", read_size);
        read_size = send(sock, login, sizeof(login), 0);
	printf("\nRecived size of bytes: %d\n", read_size);
	read_size = send(sock, password, sizeof(password), 0);
	printf("\nRecived size of bytes: %d\n", read_size);
	recv(sock, server_message, 1 ,0);
	g_print("\nResived data: %s\n", server_message);
	if (strcmp(server_message, "1") == 0){
		gtk_label_set_text(GTK_LABEL(err), "");
		// When DB is connected check and decide which window to open
		showWaiterWindow();    //Go to waiter's window
		// showCashierWindow();		//Go to Cashier's window
		//showChefWindow();
	}else if (strcmp(server_message, "2") == 0){
		showCashierWindow();
	}
	else if (strcmp(server_message, "3") == 0){
		showChefWindow();
	}
	else{
		gtk_label_set_text(GTK_LABEL(err), "Login or Password is incorrect");
	}
};


void on_loginEntry_changed(GtkEntry *e){
	memset(login, 0, sizeof(login));
	strcpy(login, gtk_entry_get_text(e));
	
}

void on_passwordEntry_changed(GtkEntry *e){
	memset(password, 0, sizeof(password));
	strcpy(password,gtk_entry_get_text(e));
}


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
