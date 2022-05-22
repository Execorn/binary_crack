#include <stdio.h>
#include <math.h>
// Windows API and graphics library
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <Dwmapi.h>
#include <sys/stat.h>


void PatchBinary(const char* filename);

static const char* patched_filepath 		= "binaries/binary";
static const char* patched_binary_filepath 	= "binaries/binary_patched";
static const char* background_image_path    = "img/frame_0_delay-0.05s.png";

//static const char* img_frame_format         = "img/frame_%u_delay-0.05s.png";

// default patch window height
static const int default_patch_window_height 		= 498;
// default patch window width
static const int default_patch_window_width  		= 384;
// default patch window y scale
static const double default_patch_window_x_scale 	= 1.0f;
// default patch window y scale
static const double default_patch_window_y_scale 	= 1.0f;

int main() {
	sf::RenderWindow patch_window;
	patch_window.create(sf::VideoMode(default_patch_window_height, default_patch_window_width),
						"Patch!",
						sf::Style::Close);

	sf::Event patch_window_event;

    sf::Texture background_texture;
    if (!background_texture.loadFromFile(background_image_path)) {
        fprintf(stderr, "Error loading background.\n");
        return 1;
    }

    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);

    size_t counter   = 0;
    size_t shown     = 0;
    bool do_gif      = false;
    char cur_img_name[40] = {0};

    sf::Font font;
    font.loadFromFile("img/reg.ttf");

    //----------------------------------------------------------------
    // creating labels
    sf::Text help_label  = {};
    help_label.setFont(font);
    help_label.setCharacterSize(30);
    help_label.setString("press P to crack");
    help_label.setPosition(100, 0);
    help_label.setFillColor(sf::Color::Green);


    sf::Text crack_label = {};
    crack_label.setFont(font);
    crack_label.setCharacterSize(30);
    crack_label.setString("CRACKED!");
    crack_label.setPosition(165, 0);
    crack_label.setFillColor(sf::Color::Red);
    //---------------------------------------------------------------
    sf::Text label = help_label;

    //---------------------------------------------------------------
    // main loop, run until window is open
	while (patch_window.isOpen() == true) {
        if (do_gif == true) {
            if (counter == 35) {
                do_gif  = false;
                counter = 0;
                patch_window.clear();
                patch_window.draw(background_sprite);
                patch_window.display();

                background_texture.loadFromFile(background_image_path);
            } else {
                // creating file name
                snprintf(cur_img_name, 40, "img/frame_%u_delay-0.05s.png", counter);
                if (!background_texture.loadFromFile(cur_img_name)) {
                    fprintf(stderr, "Error loading background.\n");
                    return 1;
                }
                background_sprite.setTexture(background_texture);
                ++shown;
                if (shown >= 6) {
                    ++counter;
                    shown = 0;
                }
            }
        }

        // processing keyboard events
		while (patch_window.pollEvent(patch_window_event)) {
			switch (patch_window_event.type) {
				case sf::Event::Closed: {
					patch_window.close();
					break;
				}

				case sf::Event::KeyPressed: {
					if (patch_window_event.key.code == sf::Keyboard::Q) {
						patch_window.close();
						break;
					}

					if (patch_window_event.key.code == sf::Keyboard::P) {
						PatchBinary(patched_filepath);
                        do_gif = true;
                        label = crack_label;
						break;
					}
				}

				default: {
					break;
				}
			}
		}

        // draw everything
        patch_window.clear();
        patch_window.draw(background_sprite);
        patch_window.draw(label);
        patch_window.display();
	}

	return 0;
}



void PatchBinary(const char* filename) {
    const char* source_file_path = filename;

    struct stat source_file_stat;
    if (stat(source_file_path, &source_file_stat) != 0) {
        fprintf(stderr, "Can't get '%s' stat. Please check if path is correct and try again.\n", source_file_path);
        return;
    }

    size_t filebytes = source_file_stat.st_size;

    char* buffer = (char*) calloc(filebytes + 1, sizeof(char));
    if (buffer == NULL) {
        return;
    }

    FILE* source_file_stream = fopen(source_file_path, "rb");
    if (source_file_stream == NULL) {
        free(buffer);
        return;
    }

    if (fread(buffer, 1, filebytes, source_file_stream) != filebytes) {
        free(buffer);
        return;
    }
    fclose(source_file_stream);

    buffer[0x1223] -= 5;

    FILE* new_source = fopen(patched_binary_filepath, "wb");
    if (new_source == NULL) {
        free(buffer);
        return;
    }

    if (fwrite(buffer, 1, filebytes, new_source) != filebytes) {
        free(buffer);
        return;
    }

    fclose(new_source);
    free(buffer);

    fprintf(stderr, "Patched successfully!\n");
    return;
}