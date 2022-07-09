#include <gtk/gtk.h>

extern void whiskermenu_open();

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    whiskermenu_open();

    gtk_main();
}