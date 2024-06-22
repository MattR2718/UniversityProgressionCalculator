// UniversityProgressionCalculator.cpp : Defines the entry point for the application.
//

#include "UniversityProgressionCalculator.h"

int main() {

    int WIDTH = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().width) / 100) * 100;
    int HEIGHT = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().height) / 100) * 100;

    //WIDTH = 1920;
    //HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "University Progression Calculator");
    window.setVerticalSyncEnabled(true);
    auto _ = ImGui::SFML::Init(window);
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    float textScale = (HEIGHT < 1350) ? 1.2 : 1.8;

    std::vector<Year> years;

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

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                saveYearsToJson(path, years);
                window.close();
            }
        }

        auto t = deltaClock.restart();
        ImGui::SFML::Update(window, t);
        io.DeltaTime = t.asSeconds();
        window.clear(sf::Color::Black);

        // Create a menu bar at the top of the screen
        if (ImGui::BeginMainMenuBar()) {
            // Set font scale for menu bar
            ImGui::SetWindowFontScale(textScale);
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    saveYearsToJson(path, years);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Appearance")) {
                    // Open the appearance popup
                    appearancePopup = true;
                }
                if (ImGui::MenuItem("Key Information")) {
                    keyInformationWindow = !keyInformationWindow;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Get the height of the main menu bar
        float menuBarHeight = ImGui::GetFrameHeightWithSpacing() * textScale;
        auto windowSize = window.getSize();

        if (!userDataFileExists) {
            ImGui::Begin("NO USER DATA");
            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(0.0f, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

            ImGui::Text("COULD NOT FIND FILE WITH USER DATA");
            ImGui::Text(("To fix, download file from [https://github.com/MattR2718/UniversityProgressionCalculator/blob/master/userData/userData.json] and put into location [" + path + "]").c_str());

            ImGui::End();
        }

        if (!progressionTreeFileExists) {
            ImGui::Begin("NO PROGRESSION DATA");
            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(windowSize.x / 2, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

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
            ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

            // Apply font scaling for the content
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            io.Fonts->Fonts[0]->Scale = textScale;

            // Create tabs for years
            if (years.size()) {
                ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar("Years Tab Bar", tabBarFlags)) {
                    ImGuiTabItemFlags tabItemFlags = ImGuiTabItemFlags_NoAssumedClosure | ImGuiTabItemFlags_NoReorder;
                    for (auto& year : years) {
                        if (year.tabOpen && ImGui::BeginTabItem(year.year.c_str(), &year.tabOpen, tabItemFlags)) {
                            // Set Number Of Columns, for each term
                            ImGui::Columns(year.terms.size());
                            //year.display();

                            for (auto& t : year.terms) {
                                t.display();
                                t.fontScale = &textScale;
                                ImGui::NextColumn();
                            }

                            ImGui::EndTabItem();

                            keyInformation.calcData(year);
                        }
                    }
                    ImGui::EndTabBar();
                }
            }
            ImGui::PopFont();

            ImGui::End();

            //
            // Popup code
            if (appearancePopup && ImGui::Begin("AppearanceSettings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                io.Fonts->Fonts[0]->Scale = textScale;

                ImGui::SetWindowFontScale(textScale);

                ImGui::Text("Appearance Settings");
                ImGui::Separator();

                ImGui::InputFloat("Text Scale", &textScale, 0.1f, 0.25f, "%.3f");

                if (!ImGui::IsWindowFocused()) {
                    appearancePopup = false;
                }

                if (ImGui::Button("Close")) {
                    appearancePopup = false;
                }
                ImGui::PopFont();
                ImGui::End();
            }


            //
            //
            //
            //
            //
            // Tree View
            ImGui::Begin("Tree View");

            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(windowSize.x / 2, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

            ImNodes::BeginNodeEditor();

            auto size = ImGui::GetWindowSize();

            progTree.drawTree(size.x, size.y, textScale, keyInformation);


            ImNodes::EndNodeEditor();

            ImGui::End();

            //ImGui::ShowDemoWindow();

            //ImGui::Begin("FPS");
            //ImGui::Text(std::to_string(1.0 / t.asSeconds()).c_str());
            //ImGui::End();

            if (keyInformationWindow) {
                keyInformation.display(textScale, keyInformationWindow);
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }
    ImNodes::DestroyContext();
    ImGui::SFML::Shutdown();
}