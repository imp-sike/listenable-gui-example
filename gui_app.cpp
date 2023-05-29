#include <gtk/gtk.h>
#include <functional>
#include <thread>
#include "listenable/listenable.cpp"

// Global Listenable object for the application
listenable<std::string> *textListenable;
listenable<int> *timerListenable;

// Global builder variable
GtkBuilder *builder;

// Callback function to update the label text
void updateLabel()
{
    std::string text = textListenable->get();
    GObject *labelObj = gtk_builder_get_object(builder, "label");
    GtkLabel *label = GTK_LABEL(labelObj);
    gtk_label_set_text(label, text.c_str());
}

void updateLabel2()
{
    int value = timerListenable->get();
    std::string text = std::to_string(value);
    GObject *labelObj = gtk_builder_get_object(builder, "label2");
    GtkLabel *label = GTK_LABEL(labelObj);
    gtk_label_set_text(label, text.c_str());
}

// Listener function to react to Listenable changes
void onTextChange()
{
    // Call the updateLabel function when Listenable changes
    updateLabel();
}

void onTextChange2()
{
    // Call the updateLabel function when Listenable changes
    updateLabel2();
}

// Simulated network request
void simulateNetworkRequest()
{
    // Simulate delay
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Update the Listenable object's value
    textListenable->change("Text updated by network request!");
}

void simulateTimer()
{
    // Simulate delay
    int i = 0;
    while (true) {
        // Update the Listenable object's value
        timerListenable->change(i++);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Create a new GTK+ builder
    builder = gtk_builder_new();

    // Load the UI file
    gtk_builder_add_from_file(builder, "ui.glade", NULL);

    // Get the main window object
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    // Create the Listenable objects
    textListenable = new listenable<std::string>("Hello, Listenable!");
    timerListenable = new listenable<int>(0);

    // Connect the destroy signal of the window to gtk_main_quit
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Call onTextChange() whenever the Listenable objects change
    textListenable->listen(onTextChange);
    timerListenable->listen(onTextChange2);

    // Show the main window
    gtk_widget_show_all(window);

    // Simulate a network request that changes the text
    std::thread networkThread(simulateNetworkRequest);
    std::thread networkThread2(simulateTimer);
    networkThread.detach();
    networkThread2.detach();

    // Start the GTK+ main loop
    gtk_main();

    // Clean up
    delete textListenable;
    delete timerListenable;
    g_object_unref(builder);

    return 0;
}
