// UniversityProgressionCalculator.cpp : Defines the entry point for the application.
//

#include "UniversityProgressionCalculator.h"

float update_ts(float z1, float z2, int x1, int y1, int x2, int y2, int x, int y, float tsm) {
    return std::abs(tsm * (z1 + ((z2 - z1) / (x2 - x1 + y2 - y1)) * ((x - x1) + (y - y1))));
}

void LoadCustomFont(float fontSize) {
    ImGuiIO& io = ImGui::GetIO();

    // Clear existing fonts
    io.Fonts->Clear();

    // Load new font with desired size
    io.Fonts->AddFontFromFileTTF("font/Roboto-Black.ttf", fontSize);

    // Rebuild the font atlas
    io.Fonts->Build();

    // Tell ImGui to rebuild the rendering resources
    ImGui_ImplOpenGL3_DestroyFontsTexture();  // Ensure old font texture is removed
    ImGui_ImplOpenGL3_CreateFontsTexture();   // Upload new texture
}

int main() {

    //int WIDTH = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().width) / 100) * 100;
    //int HEIGHT = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().height) / 100) * 100;
    Window::init_glfw();
    
    int num_monitors = 0;
    GLFWmonitor* monitor = glfwGetMonitors(&num_monitors)[0];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int WIDTH = mode->width;
    int HEIGHT = mode->height;

    //WIDTH = 1920;
    //HEIGHT = 1080;

    Window window("Progression Calculator", WIDTH * 0.8, HEIGHT * 0.8);
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    float z1 = 1.2;
    int x1 = 1920;
    int y1 = 1080;
    float z2 = 1.8;
    int x2 = 3840;
    int y2 = 2160;
    
	//float textScale = 1.0f;
    //float textScale = (HEIGHT < 1350) ? 1.2 : 1.8;
    //float textScaleMult = 1.0f;
    //float textScale = update_ts(z1, z2, x1, y1, x2, y2, WIDTH, HEIGHT, textScaleMult);
    float fontSize = WIDTH < 2000 ? 16.0f : 40.0f;
    bool fontChangeRequested = false;
    float newFontSize = WIDTH < 2000 ? 16.0f : 40.0f;

    std::vector<Year> years;

    //ImGui::StyleColorsLight();

    //setupFonts();
	LoadCustomFont(fontSize);

    bool appearancePopup = false;

    std::string path = "userData/userData.json";
    bool userDataFileExists = false;
    if (std::filesystem::exists(path)) {
        years = loadYearsFromJson(path);
        userDataFileExists = true;
    }

    for (auto& y : years) {
        y.calculatePercentages();
    }

    std::string treePath = "progressionData/progressionData.json";
    bool progressionTreeFileExists = false;
    ProgressionTree progTree;
    if (std::filesystem::exists(treePath)) {
        progTree = ProgressionTree(treePath);
        progressionTreeFileExists = true;
    }

    ImNodesStyle& style = ImNodes::GetStyle();
    unsigned int bg = style.Colors[ImNodesCol_NodeBackground] & ((-1) >> (32 - 24));
    bg = bg + (90 << 24);
    style.Colors[ImNodesCol_NodeBackground] = bg;

    bool windowClose = false;

    bool keyInformationWindow = false;

    KeyInformation keyInformation;

    bool light = false;
    setStyle();

    while(window.is_open()){
        window.poll_events();

		// update text scale if requested
		if (fontChangeRequested) {
			fontSize = newFontSize;
			LoadCustomFont(fontSize);
			fontChangeRequested = false;
		}

        window.begin_frame();

        // Create a menu bar at the top of the screen
        if (ImGui::BeginMainMenuBar()) {
            // Set font scale for menu bar
            //ImGui::SetWindowFontScale(textScale);
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    saveYearsToJson(path, years);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Data")) {
                if (ImGui::MenuItem("Key Information")) {
                    keyInformationWindow = !keyInformationWindow;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Appearance")) {
                    // Open the appearance popup
                    appearancePopup = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Get the height of the main menu bar
        float menuBarHeight = ImGui::GetFrameHeightWithSpacing();
        auto [ww, wh] = window.get_window_size();
        //textScale = update_ts(z1, z2, x1, y1, x2, y2, ww, wh, textScaleMult);

        if (!userDataFileExists) {
            ImGui::Begin("NO USER DATA");
            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(0.0f, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));

            ImGui::Text("COULD NOT FIND FILE WITH USER DATA");
            ImGui::Text(("To fix, download file from [https://github.com/MattR2718/UniversityProgressionCalculator/blob/master/userData/userData.json] and put into location [" + path + "]").c_str());

            ImGui::End();
        }

        if (!progressionTreeFileExists) {
            ImGui::Begin("NO PROGRESSION DATA");
            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(ww / 2, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));

            ImGui::Text("COULD NOT FIND FILE WITH PROGRESSION DATA");
            ImGui::Text(("To fix, download file from [https://github.com/MattR2718/UniversityProgressionCalculator/blob/master/progressionData/progressionData.json] and put into location [" + treePath + "]").c_str());

            ImGui::End();
        }


        if (userDataFileExists && progressionTreeFileExists) {
            //
            // Main Screen
            ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoDecoration);

            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(0.0f, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));

            // Apply font scaling for the content
            //ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            //io.Fonts->Fonts[0]->Scale = textScale;

            // Create tabs for years
            if (years.size()) {
                ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar("Years Tab Bar", tabBarFlags)) {
                    ImGuiTabItemFlags tabItemFlags = ImGuiTabItemFlags_NoAssumedClosure | ImGuiTabItemFlags_NoReorder;
                    for (auto& year : years) {
                        if (year.tabOpen && ImGui::BeginTabItem(year.year.c_str(), &year.tabOpen, tabItemFlags)) {
                            // Set Number Of Columns, for each term
                            ImGui::Columns(year.terms.size());

                            for (auto& t : year.terms) {
                                t.display();
                                //t.fontScale = 1.0f;
                                //t.fontScale = &textScale;
                                ImGui::NextColumn();
                            }

                            ImGui::EndTabItem();

                            keyInformation.calcData(year);
                        }
                    }
                    ImGui::EndTabBar();
                }
            }
            //ImGui::PopFont();

            ImGui::End();

            //
            // Appearance code
            if (appearancePopup && ImGui::Begin("AppearanceSettings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                //ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                //io.Fonts->Fonts[0]->Scale = textScale;

                //ImGui::SetWindowFontScale(textScale);

                ImGui::Text("Appearance Settings");
                ImGui::Separator();

                //ImGui::InputFloat("Text Scale", &textScaleMult, 0.025f, 0.25f, "%.3f");

                if (ImGui::InputFloat("Font Size", &newFontSize, 1.0f, 5.0f, "%.0f")) {
                    fontChangeRequested = true;
					//LoadCustomFont(fontSize);
                }

                if (ImGui::Button("Toggle Theme")) {
                    light = !light;
                    if (light) {
                        SetLightThemeColours();
                    }
                    else {
                        ImGui::StyleColorsClassic();
                        setStyle();
                    }
                }

                if (!ImGui::IsWindowFocused()) {
                    appearancePopup = false;
                }

                if (ImGui::Button("Close")) {
                    appearancePopup = false;
                }
                //ImGui::PopFont();
                ImGui::End();
            }


            //
            //
            //
            //
            //
            // Tree View
            ImGui::Begin("Tree View", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(ww / 2, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));

            ImNodes::BeginNodeEditor();

            auto size = ImGui::GetWindowSize();

            progTree.drawTree(size.x, size.y, keyInformation);


            ImNodes::EndNodeEditor();

            ImGui::End();

            if (keyInformationWindow) {
                keyInformation.display(keyInformationWindow);
            }
        }

        window.end_frame();
    }

    saveYearsToJson(path, years);

    ImNodes::DestroyContext();
}