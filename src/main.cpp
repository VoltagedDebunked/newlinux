#include <gtk/gtk.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>

static std::string custom_kernel_path;
static GtkWidget *info_label;
static GtkWidget *progress_bar;
static GtkWidget *status_label;
static GtkWidget *log_text_view;
static GtkTextBuffer *log_buffer;

void show_message(GtkWindow *parent, const std::string &message, GtkMessageType type = GTK_MESSAGE_INFO) {
    GtkWidget *dialog = gtk_message_dialog_new(parent, GTK_DIALOG_DESTROY_WITH_PARENT,
                                                type, GTK_BUTTONS_OK, "%s", message.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void update_log(const std::string &message) {
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(log_buffer, &iter);
    gtk_text_buffer_insert(log_buffer, &iter, (message + "\n").c_str(), -1);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(log_text_view), &iter, 0.0, FALSE, 0.0, 0.0);
}

void execute_command(const std::string &command) {
    update_log("Executing: " + command);
    int result = system(command.c_str());
    if (result != 0) {
        update_log("Error executing command: " + command);
        show_message(NULL, "Error executing command: " + command, GTK_MESSAGE_ERROR);
    } else {
        update_log("Command executed successfully: " + command);
        show_message(NULL, "Command executed successfully: " + command);
    }
}

void update_info_label(const char *kernel_type) {
    std::string info = "Selected Kernel Type: ";
    info += kernel_type;
    info += "\nProperties:\n";
    if (strcmp(kernel_type, "lts") == 0) {
        info += "- Long-term support\n- Stable and reliable\n- Recommended for production systems\n";
    } else if (strcmp(kernel_type, "zen") == 0) {
        info += "- Optimized for desktop usage\n- Improved responsiveness\n- May sacrifice some stability\n";
    } else if (strcmp(kernel_type, "specific version") == 0) {
        info += "- Allows selection of a specific kernel version\n- Useful for compatibility or testing\n";
    }
    gtk_label_set_text(GTK_LABEL(info_label), info.c_str());
}

void on_kernel_type_changed(GtkComboBox *combo, gpointer data) {
    const char *kernel_type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    update_info_label(kernel_type);
}

void update_progress(double fraction) {
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), fraction);
}

void update_status(const std::string &status) {
    gtk_label_set_text(GTK_LABEL(status_label), status.c_str());
}

void perform_update(GtkWidget *version_combo) {
    const char *version = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(version_combo));
    std::string package_manager;

    update_status("Detecting package manager...");
    update_progress(0.1);

    if (system("which pacman > /dev/null 2>&1") == 0) {
        package_manager = "pacman";
    } else if ( system("which apt > /dev/null 2>&1") == 0) {
        package_manager = "apt";
    } else if (system("which dnf > /dev/null 2>&1") == 0) {
        package_manager = "dnf";
    }

    if (package_manager.empty()) {
        update_status("No supported package manager found.");
        show_message(NULL, "No supported package manager found.", GTK_MESSAGE_ERROR);
        return;
    }

    update_status("Preparing update...");
    update_progress(0.3);

    std::string command;
    if (!custom_kernel_path.empty()) {
        command = "cd " + custom_kernel_path + " && sudo makepkg -i --noconfirm";
    } else if (package_manager == "pacman") {
        command = "sudo pacman -Syu " + std::string(version);
    } else if (package_manager == "apt") {
        command = "sudo apt update && sudo apt upgrade " + std::string(version);
    } else if (package_manager == "dnf") {
        command = "sudo dnf upgrade " + std::string(version);
    }

    update_status("Updating kernel...");
    update_progress(0.6);

    execute_command(command);

    update_status("Update complete.");
    update_progress(1.0);
}

void on_update_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *version_combo = GTK_WIDGET(data);
    std::thread update_thread(perform_update, version_combo);
    update_thread.detach();
}

void on_custom_kernel_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWindow *parent = GTK_WINDOW(data);
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Select Custom Kernel Directory", parent,
                                                    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Select", GTK_RESPONSE_OK, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        custom_kernel_path = filename;
        show_message(parent, "Selected custom kernel path: " + std::string(custom_kernel_path));
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "NewLinux Kernel Updater");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *kernel_type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(kernel_type_combo), "lts");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(kernel_type_combo), "zen");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(kernel_type_combo), "specific version");
    gtk_combo_box_set_active(GTK_COMBO_BOX(kernel_type_combo), 0);
    g_signal_connect(kernel_type_combo, "changed", G_CALLBACK(on_kernel_type_changed), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), kernel_type_combo, TRUE, TRUE, 0);

    info_label = gtk_label_new("Select a kernel type to see its properties.");
    gtk_box_pack_start(GTK_BOX(vbox), info_label, TRUE, TRUE, 0);

    GtkWidget *version_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_combo), "linux");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_combo), "linux-lts");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(version_combo), "linux-zen");
    gtk_box_pack_start(GTK_BOX(vbox), version_combo, TRUE, TRUE, 0);

    GtkWidget *custom_kernel_button = gtk_button_new_with_label("Select Custom Kernel");
    g_signal_connect(custom_kernel_button, "clicked", G_CALLBACK(on_custom_kernel_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), custom_kernel_button, TRUE, TRUE, 0);

    GtkWidget *update_button = gtk_button_new_with_label("Update Kernel");
    g_signal_connect(update_button, "clicked", G_CALLBACK(on_update_button_clicked), version_combo);
    gtk_box_pack_start(GTK_BOX(vbox), update_button, TRUE, TRUE, 0);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    progress_bar = gtk_progress_bar_new();
    gtk_box_pack_start(GTK_BOX(hbox), progress_bar, TRUE, TRUE, 0);

    status_label = gtk_label_new("Ready");
    gtk_box_pack_start(GTK_BOX(hbox), status_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    GtkWidget * log_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(log_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_box_pack_start(GTK_BOX(vbox), log_scrolled_window, TRUE, TRUE, 0);

    log_text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(log_scrolled_window), log_text_view);

    log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_text_view));

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
