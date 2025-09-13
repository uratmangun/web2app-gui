#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cctype>

class Web2AppManager {
private:
    GtkWidget* window;
    GtkWidget* name_entry;
    GtkWidget* url_entry;
    GtkWidget* icon_entry;
    GtkWidget* app_list;
    GtkListStore* list_store;

public:
    Web2AppManager() {
        create_window();
        refresh_app_list();
    }

    void create_window() {
        // Create main window
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Web2App Manager");
        gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);
        
        // Connect close signal
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        // Create main vertical box
        GtkWidget* main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(window), main_vbox);

        // Create title label
        GtkWidget* title_label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(title_label), "<b><big>Web2App Manager</big></b>");
        gtk_box_pack_start(GTK_BOX(main_vbox), title_label, FALSE, FALSE, 0);

        // Create form frame
        GtkWidget* form_frame = gtk_frame_new("Create Web App");
        gtk_box_pack_start(GTK_BOX(main_vbox), form_frame, FALSE, FALSE, 0);

        // Create form grid
        GtkWidget* grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
        gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
        gtk_container_add(GTK_CONTAINER(form_frame), grid);

        // App Name
        GtkWidget* name_label = gtk_label_new("App Name:");
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
        
        name_entry = gtk_entry_new();
        gtk_widget_set_hexpand(name_entry, TRUE);
        gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);

        // URL
        GtkWidget* url_label = gtk_label_new("URL:");
        gtk_widget_set_halign(url_label, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(grid), url_label, 0, 1, 1, 1);
        
        url_entry = gtk_entry_new();
        gtk_widget_set_hexpand(url_entry, TRUE);
        gtk_grid_attach(GTK_GRID(grid), url_entry, 1, 1, 1, 1);

        // Icon URL (optional)
        GtkWidget* icon_label = gtk_label_new("Icon URL (optional):");
        gtk_widget_set_halign(icon_label, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(grid), icon_label, 0, 2, 1, 1);
        
        icon_entry = gtk_entry_new();
        gtk_widget_set_hexpand(icon_entry, TRUE);
        gtk_grid_attach(GTK_GRID(grid), icon_entry, 1, 2, 1, 1);

        // Create button
        GtkWidget* create_btn = gtk_button_new_with_label("Create Web App");
        gtk_grid_attach(GTK_GRID(grid), create_btn, 0, 3, 2, 1);
        g_signal_connect(create_btn, "clicked", G_CALLBACK(on_create_clicked), this);

        // Create apps list frame
        GtkWidget* list_frame = gtk_frame_new("Installed Web Apps");
        gtk_box_pack_start(GTK_BOX(main_vbox), list_frame, TRUE, TRUE, 0);

        // Create list box
        GtkWidget* list_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_container_set_border_width(GTK_CONTAINER(list_vbox), 10);
        gtk_container_add(GTK_CONTAINER(list_frame), list_vbox);

        // Refresh button
        GtkWidget* refresh_btn = gtk_button_new_with_label("Refresh List");
        gtk_box_pack_start(GTK_BOX(list_vbox), refresh_btn, FALSE, FALSE, 0);
        g_signal_connect(refresh_btn, "clicked", G_CALLBACK(on_refresh_clicked), this);

        // Create tree view for app list
        list_store = gtk_list_store_new(1, G_TYPE_STRING);
        app_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
        
        GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes(
            "Application Name", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(app_list), column);

        // Add scrolled window for list
        GtkWidget* scrolled = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                     GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_container_add(GTK_CONTAINER(scrolled), app_list);
        gtk_box_pack_start(GTK_BOX(list_vbox), scrolled, TRUE, TRUE, 0);

        // Buttons horizontal box
        GtkWidget* button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_pack_start(GTK_BOX(list_vbox), button_hbox, FALSE, FALSE, 0);

        // Edit button
        GtkWidget* edit_btn = gtk_button_new_with_label("Edit Selected App");
        gtk_box_pack_start(GTK_BOX(button_hbox), edit_btn, TRUE, TRUE, 0);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_clicked), this);

        // Add to favorites button
        GtkWidget* favorites_btn = gtk_button_new_with_label("Add to Favorites");
        gtk_box_pack_start(GTK_BOX(button_hbox), favorites_btn, TRUE, TRUE, 0);
        g_signal_connect(favorites_btn, "clicked", G_CALLBACK(on_add_to_favorites_clicked), this);

        // Remove button
        GtkWidget* remove_btn = gtk_button_new_with_label("Remove Selected App");
        gtk_box_pack_start(GTK_BOX(button_hbox), remove_btn, TRUE, TRUE, 0);
        g_signal_connect(remove_btn, "clicked", G_CALLBACK(on_remove_clicked), this);
    }

    static void on_create_clicked(GtkWidget* widget, gpointer user_data) {
        Web2AppManager* manager = static_cast<Web2AppManager*>(user_data);
        manager->create_web_app();
    }

    static void on_refresh_clicked(GtkWidget* widget, gpointer user_data) {
        Web2AppManager* manager = static_cast<Web2AppManager*>(user_data);
        manager->refresh_app_list();
    }

    static void on_edit_clicked(GtkWidget* widget, gpointer user_data) {
        Web2AppManager* manager = static_cast<Web2AppManager*>(user_data);
        manager->edit_web_app();
    }

    static void on_remove_clicked(GtkWidget* widget, gpointer user_data) {
        Web2AppManager* manager = static_cast<Web2AppManager*>(user_data);
        manager->remove_web_app();
    }

    static void on_add_to_favorites_clicked(GtkWidget* widget, gpointer user_data) {
        Web2AppManager* manager = static_cast<Web2AppManager*>(user_data);
        manager->add_to_favorites();
    }

    void create_web_app() {
        const char* name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char* url = gtk_entry_get_text(GTK_ENTRY(url_entry));
        const char* icon_url = gtk_entry_get_text(GTK_ENTRY(icon_entry));

        if (!name || strlen(name) == 0 || !url || strlen(url) == 0) {
            show_error_dialog("App name and URL are required!");
            return;
        }

        try {
            std::string home_dir = std::getenv("HOME");
            std::string apps_dir = home_dir + "/.local/share/applications";
            std::string icon_dir = home_dir + "/.local/share/icons/webapps";
            std::string data_dir = home_dir + "/.config/webapps/" + std::string(name);

            // Create directories
            std::filesystem::create_directories(apps_dir);
            std::filesystem::create_directories(icon_dir);
            std::filesystem::create_directories(data_dir);

            // Handle icon
            std::string icon_file = icon_dir + "/" + std::string(name) + ".png";
            if (strlen(icon_url) > 0) {
                std::string cmd = "curl -fsSL \"" + std::string(icon_url) + "\" -o \"" + icon_file + "\"";
                if (system(cmd.c_str()) != 0) {
                    generate_default_icon(name, icon_file);
                } else {
                    // Resize downloaded icon to 200x200
                    resize_icon_to_200x200(icon_file);
                }
            } else {
                generate_default_icon(name, icon_file);
            }

            // Find browser
            std::string browser_cmd = find_browser();
            if (browser_cmd.empty()) {
                show_error_dialog("No Chrome/Chromium browser found!");
                return;
            }

            // Create .desktop file
            std::string desktop_file = apps_dir + "/" + std::string(name) + ".desktop";
            std::string url_str = std::string(url);
            if (url_str.find("http://") != 0 && url_str.find("https://") != 0) {
                url_str = "https://" + url_str;
            }

            // Extract domain for better StartupWMClass
            std::string domain = url_str;
            size_t start = domain.find("://");
            if (start != std::string::npos) {
                domain = domain.substr(start + 3);
            }
            size_t end = domain.find("/");
            if (end != std::string::npos) {
                domain = domain.substr(0, end);
            }
            // Remove www. prefix if present
            if (domain.substr(0, 4) == "www.") {
                domain = domain.substr(4);
            }

            std::ofstream file(desktop_file);
            file << "[Desktop Entry]" << std::endl;
            file << "Version=1.0" << std::endl;
            file << "Type=Application" << std::endl;
            file << "Name=" << name << std::endl;
            file << "Comment=Web application for " << domain << std::endl;
            file << "Exec=" << browser_cmd << " --user-data-dir=\"" << data_dir << "\" --app=\"" << url_str << "\"" << std::endl;
            file << "Icon=" << icon_file << std::endl;
            file << "Terminal=false" << std::endl;
            file << "StartupNotify=true" << std::endl;
            file << "Categories=Network;WebBrowser;WebApps;" << std::endl;
            file << "MimeType=text/html;text/xml;application/xhtml+xml;" << std::endl;
            file << "StartupWMClass=crx_" << domain << std::endl;
            file.close();

            // Make executable
            std::filesystem::permissions(desktop_file, 
                                       std::filesystem::perms::owner_all | 
                                       std::filesystem::perms::group_read | 
                                       std::filesystem::perms::others_read);

            // Update desktop database and refresh icon cache
            system(("update-desktop-database \"" + apps_dir + "\"").c_str());
            system("gtk-update-icon-cache -f -t ~/.local/share/icons/ 2>/dev/null || true");
            system("gtk-update-icon-cache -f -t /usr/share/icons/hicolor/ 2>/dev/null || true");
            system("xdg-desktop-menu forceupdate 2>/dev/null || true");

            show_info_dialog("Web app '" + std::string(name) + "' created successfully!");

            // Clear form
            gtk_entry_set_text(GTK_ENTRY(name_entry), "");
            gtk_entry_set_text(GTK_ENTRY(url_entry), "");
            gtk_entry_set_text(GTK_ENTRY(icon_entry), "");

            refresh_app_list();

        } catch (const std::exception& e) {
            show_error_dialog("Failed to create web app: " + std::string(e.what()));
        }
    }

    void edit_web_app() {
        GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_list));
        GtkTreeIter iter;
        GtkTreeModel* model;

        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            gchar* app_name;
            gtk_tree_model_get(model, &iter, 0, &app_name, -1);

            try {
                std::string home_dir = std::getenv("HOME");
                std::string apps_dir = home_dir + "/.local/share/applications";
                std::string existing_desktop_file = apps_dir + "/" + std::string(app_name) + ".desktop";
                
                // Read current values from desktop file
                std::ifstream existing_file(existing_desktop_file);
                std::string current_name = app_name;
                std::string current_url, current_icon_url;
                
                if (existing_file.is_open()) {
                    std::string line;
                    while (std::getline(existing_file, line)) {
                        if (line.find("Exec=") == 0) {
                            size_t app_start = line.find("--app=\"");
                            if (app_start != std::string::npos) {
                                app_start += 7;
                                size_t app_end = line.find("\"", app_start);
                                if (app_end != std::string::npos) {
                                    current_url = line.substr(app_start, app_end - app_start);
                                }
                            }
                        }
                    }
                    existing_file.close();
                }

                // Create edit dialog
                GtkWidget* dialog = gtk_dialog_new_with_buttons("Edit Web App",
                    GTK_WINDOW(window),
                    GTK_DIALOG_MODAL,
                    "_Cancel", GTK_RESPONSE_CANCEL,
                    "_Save", GTK_RESPONSE_OK,
                    NULL);

                GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);

                // Create grid for form
                GtkWidget* grid = gtk_grid_new();
                gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
                gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
                gtk_container_add(GTK_CONTAINER(content_area), grid);

                // App name
                GtkWidget* name_label = gtk_label_new("App Name:");
                gtk_widget_set_halign(name_label, GTK_ALIGN_START);
                gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
                
                GtkWidget* edit_name_entry = gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(edit_name_entry), current_name.c_str());
                gtk_widget_set_hexpand(edit_name_entry, TRUE);
                gtk_grid_attach(GTK_GRID(grid), edit_name_entry, 1, 0, 1, 1);

                // URL
                GtkWidget* url_label = gtk_label_new("URL:");
                gtk_widget_set_halign(url_label, GTK_ALIGN_START);
                gtk_grid_attach(GTK_GRID(grid), url_label, 0, 1, 1, 1);
                
                GtkWidget* edit_url_entry = gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(edit_url_entry), current_url.c_str());
                gtk_widget_set_hexpand(edit_url_entry, TRUE);
                gtk_grid_attach(GTK_GRID(grid), edit_url_entry, 1, 1, 1, 1);

                // Icon URL
                GtkWidget* icon_label = gtk_label_new("Icon URL (optional):");
                gtk_widget_set_halign(icon_label, GTK_ALIGN_START);
                gtk_grid_attach(GTK_GRID(grid), icon_label, 0, 2, 1, 1);
                
                GtkWidget* edit_icon_entry = gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(edit_icon_entry), current_icon_url.c_str());
                gtk_widget_set_hexpand(edit_icon_entry, TRUE);
                gtk_grid_attach(GTK_GRID(grid), edit_icon_entry, 1, 2, 1, 1);

                gtk_widget_show_all(dialog);

                int result = gtk_dialog_run(GTK_DIALOG(dialog));
                
                if (result == GTK_RESPONSE_OK) {
                    const char* new_name = gtk_entry_get_text(GTK_ENTRY(edit_name_entry));
                    const char* new_url = gtk_entry_get_text(GTK_ENTRY(edit_url_entry));
                    const char* new_icon_url = gtk_entry_get_text(GTK_ENTRY(edit_icon_entry));

                    if (!new_name || strlen(new_name) == 0 || !new_url || strlen(new_url) == 0) {
                        gtk_widget_destroy(dialog);
                        show_error_dialog("App name and URL are required!");
                        g_free(app_name);
                        return;
                    }

                    // Remove old files
                    std::string icon_dir = home_dir + "/.local/share/icons/webapps";
                    std::string data_dir = home_dir + "/.config/webapps/" + std::string(app_name);
                    
                    std::filesystem::remove(existing_desktop_file);
                    std::filesystem::remove(apps_dir + "/" + std::string(app_name) + "_fav.desktop");
                    std::filesystem::remove(icon_dir + "/" + std::string(app_name) + ".png");
                    std::filesystem::remove(icon_dir + "/" + std::string(app_name) + "_fav.png");
                    std::filesystem::remove_all(data_dir);

                    // Create new app with updated values
                    std::filesystem::create_directories(apps_dir);
                    std::filesystem::create_directories(icon_dir);
                    std::string new_data_dir = home_dir + "/.config/webapps/" + std::string(new_name);
                    std::filesystem::create_directories(new_data_dir);

                    // Handle icon
                    std::string icon_file = icon_dir + "/" + std::string(new_name) + ".png";
                    if (strlen(new_icon_url) > 0) {
                        std::string cmd = "curl -fsSL \"" + std::string(new_icon_url) + "\" -o \"" + icon_file + "\"";
                        if (system(cmd.c_str()) != 0) {
                            generate_default_icon(new_name, icon_file);
                        } else {
                            // Resize downloaded icon to 200x200
                            resize_icon_to_200x200(icon_file);
                        }
                    } else {
                        generate_default_icon(new_name, icon_file);
                    }

                    // Create new desktop file
                    std::string new_desktop_file = apps_dir + "/" + std::string(new_name) + ".desktop";
                    std::string url_str = std::string(new_url);
                    if (url_str.find("http://") != 0 && url_str.find("https://") != 0) {
                        url_str = "https://" + url_str;
                    }

                    // Extract domain for better StartupWMClass
                    std::string domain = url_str;
                    size_t start = domain.find("://");
                    if (start != std::string::npos) {
                        domain = domain.substr(start + 3);
                    }
                    size_t end = domain.find("/");
                    if (end != std::string::npos) {
                        domain = domain.substr(0, end);
                    }
                    if (domain.substr(0, 4) == "www.") {
                        domain = domain.substr(4);
                    }

                    // Find browser
                    std::string browser_cmd = find_browser();
                    if (browser_cmd.empty()) {
                        gtk_widget_destroy(dialog);
                        show_error_dialog("No Chrome/Chromium browser found!");
                        g_free(app_name);
                        return;
                    }

                    std::ofstream file(new_desktop_file);
                    file << "[Desktop Entry]" << std::endl;
                    file << "Version=1.0" << std::endl;
                    file << "Type=Application" << std::endl;
                    file << "Name=" << new_name << std::endl;
                    file << "Comment=Web application for " << domain << std::endl;
                    file << "Exec=" << browser_cmd << " --user-data-dir=\"" << new_data_dir << "\" --app=\"" << url_str << "\"" << std::endl;
                    file << "Icon=" << icon_file << std::endl;
                    file << "Terminal=false" << std::endl;
                    file << "StartupNotify=true" << std::endl;
                    file << "Categories=Network;WebBrowser;WebApps;" << std::endl;
                    file << "MimeType=text/html;text/xml;application/xhtml+xml;" << std::endl;
                    file << "StartupWMClass=crx_" << domain << std::endl;
                    file.close();

                    // Make executable
                    std::filesystem::permissions(new_desktop_file, 
                                               std::filesystem::perms::owner_all | 
                                               std::filesystem::perms::group_read | 
                                               std::filesystem::perms::others_read);

                    // Update desktop database and refresh icon cache
                    system(("update-desktop-database \"" + apps_dir + "\"").c_str());
                    system("gtk-update-icon-cache -f -t ~/.local/share/icons/ 2>/dev/null || true");
                    system("gtk-update-icon-cache -f -t /usr/share/icons/hicolor/ 2>/dev/null || true");
                    system("xdg-desktop-menu forceupdate 2>/dev/null || true");

                    show_info_dialog("Web app '" + std::string(new_name) + "' updated successfully!");
                    refresh_app_list();
                }

                gtk_widget_destroy(dialog);

            } catch (const std::exception& e) {
                show_error_dialog("Error editing web app: " + std::string(e.what()));
            }

            g_free(app_name);
        } else {
            show_error_dialog("Please select an app to edit!");
        }
    }

    void refresh_app_list() {
        gtk_list_store_clear(list_store);

        try {
            std::string apps_dir = std::string(std::getenv("HOME")) + "/.local/share/applications";
            
            for (const auto& entry : std::filesystem::directory_iterator(apps_dir)) {
                if (entry.path().extension() == ".desktop") {
                    // Skip favorites desktop files in the list (they're duplicates)
                    std::string filename = entry.path().filename().string();
                    if (filename.find("_fav.desktop") != std::string::npos) {
                        continue;
                    }
                    
                    std::ifstream file(entry.path());
                    std::string line;
                    std::string app_name;
                    bool is_webapp = false;

                    while (std::getline(file, line)) {
                        if (line.find("Categories=") == 0 && line.find("WebApps") != std::string::npos) {
                            is_webapp = true;
                        }
                        if (line.find("Name=") == 0) {
                            app_name = line.substr(5);
                        }
                    }

                    if (is_webapp && !app_name.empty()) {
                        GtkTreeIter iter;
                        gtk_list_store_append(list_store, &iter);
                        gtk_list_store_set(list_store, &iter, 0, app_name.c_str(), -1);
                    }
                }
            }
            
            // Update desktop database after refresh
            system(("update-desktop-database \"" + apps_dir + "\"").c_str());
            
        } catch (const std::exception& e) {
            std::cerr << "Error refreshing app list: " << e.what() << std::endl;
        }
    }

    void remove_web_app() {
        GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_list));
        GtkTreeIter iter;
        GtkTreeModel* model;

        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            gchar* app_name;
            gtk_tree_model_get(model, &iter, 0, &app_name, -1);

            GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_YES_NO,
                "Are you sure you want to remove '%s'?", app_name);

            int result = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            if (result == GTK_RESPONSE_YES) {
                try {
                    std::string home_dir = std::getenv("HOME");
                    std::string apps_dir = home_dir + "/.local/share/applications";
                    std::string icon_dir = home_dir + "/.local/share/icons/webapps";
                    std::string data_dir = home_dir + "/.config/webapps/" + std::string(app_name);

                    // Remove main desktop file and icon
                    std::filesystem::remove(apps_dir + "/" + std::string(app_name) + ".desktop");
                    std::filesystem::remove(icon_dir + "/" + std::string(app_name) + ".png");
                    
                    // Also remove favorites desktop file and icon if they exist
                    std::filesystem::remove(apps_dir + "/" + std::string(app_name) + "_fav.desktop");
                    std::filesystem::remove(icon_dir + "/" + std::string(app_name) + "_fav.png");
                    
                    // Remove user data directory
                    std::filesystem::remove_all(data_dir);

                    show_info_dialog("Web app '" + std::string(app_name) + "' removed successfully!");
                    refresh_app_list();

                } catch (const std::exception& e) {
                    show_error_dialog("Failed to remove web app: " + std::string(e.what()));
                }
            }

            g_free(app_name);
        } else {
            show_error_dialog("Please select an app to remove!");
        }
    }

    std::string find_browser() {
        std::vector<std::string> browsers = {"brave-browser", "google-chrome", "chromium-browser", "chromium"};
        for (const auto& browser : browsers) {
            if (system(("which " + browser + " > /dev/null 2>&1").c_str()) == 0) {
                return browser;
            }
        }
        return "";
    }

    void resize_icon_to_200x200(const std::string& icon_path) {
        // Check if the icon exists
        if (!std::filesystem::exists(icon_path)) {
            return;
        }

        // Get image dimensions using identify command
        std::string cmd = "identify -format \"%wx%h\" \"" + icon_path + "\" 2>/dev/null";
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            return;
        }

        char buffer[128];
        std::string dimensions;
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            dimensions += buffer;
        }
        pclose(pipe);

        // Remove trailing newline
        if (!dimensions.empty() && dimensions.back() == '\n') {
            dimensions.pop_back();
        }

        // Check if already 200x200
        if (dimensions == "200x200") {
            return; // Already correct size
        }

        // Create backup of original
        std::string backup_path = icon_path + ".original";
        std::filesystem::copy_file(icon_path, backup_path, std::filesystem::copy_options::overwrite_existing);

        // Resize to 200x200 using ImageMagick convert
        std::string resize_cmd = "convert \"" + icon_path + "\" -resize 200x200 \"" + icon_path + "\" 2>/dev/null";
        int result = system(resize_cmd.c_str());
        
        if (result != 0) {
            // If convert failed, try with different approach
            resize_cmd = "convert \"" + backup_path + "\" -resize 200x200! \"" + icon_path + "\" 2>/dev/null";
            system(resize_cmd.c_str());
        }
    }

    void generate_default_icon(const std::string& app_name, const std::string& icon_path) {
        // Create a simple SVG icon with the first letter
        std::string letter = app_name.empty() ? "W" : std::string(1, std::toupper(app_name[0]));
        std::string svg_content = R"(<?xml version="1.0" encoding="UTF-8"?>
<svg width="200" height="200" xmlns="http://www.w3.org/2000/svg">
  <rect width="200" height="200" fill="#3b82f6"/>
  <text x="100" y="140" font-family="Arial, sans-serif" font-size="100" fill="white" text-anchor="middle">)" + letter + R"(</text>
</svg>)";
        
        std::string svg_file = icon_path + ".svg";
        std::ofstream file(svg_file);
        file << svg_content;
        file.close();

        // Convert SVG to PNG using imagemagick or rsvg-convert if available
        std::string cmd = "rsvg-convert \"" + svg_file + "\" -o \"" + icon_path + "\" 2>/dev/null || "
                         "convert \"" + svg_file + "\" \"" + icon_path + "\" 2>/dev/null";
        system(cmd.c_str());
        std::filesystem::remove(svg_file);
        
        // Ensure the generated icon is 200x200
        resize_icon_to_200x200(icon_path);
    }

    void show_error_dialog(const std::string& message) {
        GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "%s", message.c_str());
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    void add_to_favorites() {
        GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app_list));
        GtkTreeIter iter;
        GtkTreeModel* model;

        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            gchar* app_name;
            gtk_tree_model_get(model, &iter, 0, &app_name, -1);

            try {
                // Get the custom icon URL from the form
                const char* icon_url = gtk_entry_get_text(GTK_ENTRY(icon_entry));
                
                std::string home_dir = std::getenv("HOME");
                std::string apps_dir = home_dir + "/.local/share/applications";
                std::string icon_dir = home_dir + "/.local/share/icons/webapps";
                
                // Read the existing desktop file to get URL and other details
                std::string existing_desktop_file = apps_dir + "/" + std::string(app_name) + ".desktop";
                std::ifstream existing_file(existing_desktop_file);
                std::string url_str, browser_cmd, data_dir;
                
                if (existing_file.is_open()) {
                    std::string line;
                    while (std::getline(existing_file, line)) {
                        if (line.find("Exec=") == 0) {
                            // Extract browser command and URL from Exec line
                            size_t app_start = line.find("--app=\"");
                            if (app_start != std::string::npos) {
                                app_start += 7; // length of "--app=\""
                                size_t app_end = line.find("\"", app_start);
                                if (app_end != std::string::npos) {
                                    url_str = line.substr(app_start, app_end - app_start);
                                }
                            }
                            
                            size_t data_start = line.find("--user-data-dir=\"");
                            if (data_start != std::string::npos) {
                                data_start += 17; // length of "--user-data-dir=\""
                                size_t data_end = line.find("\"", data_start);
                                if (data_end != std::string::npos) {
                                    data_dir = line.substr(data_start, data_end - data_start);
                                }
                            }
                            
                            // Extract browser command (everything before --user-data-dir)
                            size_t browser_end = line.find(" --user-data-dir");
                            if (browser_end != std::string::npos) {
                                browser_cmd = line.substr(5, browser_end - 5); // skip "Exec="
                            }
                        }
                    }
                    existing_file.close();
                }
                
                if (url_str.empty() || browser_cmd.empty()) {
                    show_error_dialog("Could not read existing app configuration!");
                    g_free(app_name);
                    return;
                }

                // Create custom desktop file name for favorites
                std::string fav_desktop_filename = std::string(app_name) + "_fav.desktop";
                std::string fav_desktop_file = apps_dir + "/" + fav_desktop_filename;
                
                // Handle custom icon if provided
                std::string icon_file = icon_dir + "/" + std::string(app_name) + "_fav.png";
                if (strlen(icon_url) > 0) {
                    std::filesystem::create_directories(icon_dir);
                    std::string cmd = "curl -fsSL \"" + std::string(icon_url) + "\" -o \"" + icon_file + "\"";
                    if (system(cmd.c_str()) != 0) {
                        generate_default_icon(app_name, icon_file);
                    } else {
                        // Resize downloaded icon to 200x200
                        resize_icon_to_200x200(icon_file);
                    }
                } else {
                    // Use the original icon file
                    icon_file = icon_dir + "/" + std::string(app_name) + ".png";
                }

                // Extract domain for better StartupWMClass
                std::string domain = url_str;
                size_t start = domain.find("://");
                if (start != std::string::npos) {
                    domain = domain.substr(start + 3);
                }
                size_t end = domain.find("/");
                if (end != std::string::npos) {
                    domain = domain.substr(0, end);
                }
                if (domain.substr(0, 4) == "www.") {
                    domain = domain.substr(4);
                }

                // Create custom desktop file with the specified icon
                std::ofstream file(fav_desktop_file);
                file << "[Desktop Entry]" << std::endl;
                file << "Version=1.0" << std::endl;
                file << "Type=Application" << std::endl;
                file << "Name=" << app_name << std::endl;
                file << "Comment=Web application for " << domain << std::endl;
                file << "Exec=" << browser_cmd << " --user-data-dir=\"" << data_dir << "\" --app=\"" << url_str << "\"" << std::endl;
                file << "Icon=" << icon_file << std::endl;
                file << "Terminal=false" << std::endl;
                file << "StartupNotify=true" << std::endl;
                file << "Categories=Network;WebBrowser;WebApps;" << std::endl;
                file << "MimeType=text/html;text/xml;application/xhtml+xml;" << std::endl;
                file << "StartupWMClass=crx_" << domain << std::endl;
                file.close();

                // Make executable
                std::filesystem::permissions(fav_desktop_file, 
                                           std::filesystem::perms::owner_all | 
                                           std::filesystem::perms::group_read | 
                                           std::filesystem::perms::others_read);

                // Update desktop database and refresh icon cache
                system(("update-desktop-database \"" + apps_dir + "\"").c_str());
                system("gtk-update-icon-cache -f -t ~/.local/share/icons/ 2>/dev/null || true");
                system("gtk-update-icon-cache -f -t /usr/share/icons/hicolor/ 2>/dev/null || true");
                system("xdg-desktop-menu forceupdate 2>/dev/null || true");
                
                // Get current favorites list
                std::string get_cmd = "gsettings get org.gnome.shell favorite-apps";
                FILE* pipe = popen(get_cmd.c_str(), "r");
                if (!pipe) {
                    show_error_dialog("Failed to get current favorites list!");
                    g_free(app_name);
                    return;
                }

                char buffer[1024];
                std::string current_favorites;
                while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                    current_favorites += buffer;
                }
                pclose(pipe);

                // Remove trailing newline
                if (!current_favorites.empty() && current_favorites.back() == '\n') {
                    current_favorites.pop_back();
                }

                // Check if app is already in favorites
                if (current_favorites.find("'" + fav_desktop_filename + "'") != std::string::npos) {
                    show_info_dialog("'" + std::string(app_name) + "' is already in favorites!");
                    g_free(app_name);
                    return;
                }

                // Add custom desktop file to favorites using gsettings
                std::string set_cmd = "gsettings set org.gnome.shell favorite-apps \"$(gsettings get org.gnome.shell favorite-apps | sed 's/.$//'), '" + fav_desktop_filename + "']\"";
                
                int result = system(set_cmd.c_str());
                if (result == 0) {
                    show_info_dialog("'" + std::string(app_name) + "' added to favorites successfully with custom icon!\nYou may need to press Alt+F2, type 'r' and press Enter to restart GNOME Shell for changes to take effect.");
                } else {
                    show_error_dialog("Failed to add '" + std::string(app_name) + "' to favorites!");
                }

            } catch (const std::exception& e) {
                show_error_dialog("Error adding to favorites: " + std::string(e.what()));
            }

            g_free(app_name);
        } else {
            show_error_dialog("Please select an app to add to favorites!");
        }
    }

    void show_info_dialog(const std::string& message) {
        GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "%s", message.c_str());
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    void show() {
        gtk_widget_show_all(window);
    }
};

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);
    
    Web2AppManager manager;
    manager.show();
    
    gtk_main();
    return 0;
}
