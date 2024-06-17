// UniversityProgressionCalculator.cpp : Defines the entry point for the application.
//

#include "UniversityProgressionCalculator.h"

int main() {

    //const int WIDTH = 800;
    //const int HEIGHT = 800;
    const int WIDTH = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().width) / 100) * 100;
    const int HEIGHT = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().height) / 100) * 100;

    // float bounds = 1.0;
    // noise::utils::NoiseMap heightMap = generateHeightMap(WIDTH, HEIGHT, bounds);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "PerlinTerrain");
    window.setVerticalSyncEnabled(true);
    auto _ = ImGui::SFML::Init(window);
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.FontAllowUserScaling = true;
    
    float textScale = (sf::VideoMode::getDesktopMode().width < 1350) ? 1.4 : 1.8;

    std::vector<Year> years;

    bool appearancePopup = false;

    std::string path = "../../../data/data.json";
    if (std::filesystem::exists(path)) {
        years = loadYearsFromJson(path);
    }

    for (auto& y : years) {
        y.calculatePercentages();
    }

    std::string treePath = "../../../progression.json";
    ProgressionTree progTree(treePath);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        ImGui::SFML::ProcessEvent(window, event);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear(sf::Color::Black);


        // Create a menu bar at the top of the screen
        if (ImGui::BeginMainMenuBar()) {
            // Set font scale for menu bar
            ImGui::SetWindowFontScale(textScale);
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Create")) {
                    // Handle Create action
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    // Handle Open action
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    // Handle Save action
                }
                if (ImGui::MenuItem("Save as..")) {
                    // Handle Save as action
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
        float menuBarHeight = ImGui::GetFrameHeightWithSpacing() * textScale;

        //
        // Main Screen
        ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoDecoration);
        auto windowSize = window.getSize();

        // Make window fill screen but positioned below the menu bar
        ImGui::SetWindowPos(ImVec2(0.0f, menuBarHeight));
        ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

        // Apply font scaling for the content
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::GetIO().Fonts->Fonts[0]->Scale = textScale;

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
            ImGui::GetIO().Fonts->Fonts[0]->Scale = textScale;

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

        /*ImNodes::BeginNode(1);
        const int output_attr_id = 2;
        ImNodes::BeginOutputAttribute(output_attr_id);
        ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();
        ImGui::Dummy(ImVec2(80.0f, 45.0f));

        ImNodes::SetNodeEditorSpacePos(1, ImVec2(500.0, 500.0));
        ImNodes::EndNode();

        ImNodes::BeginNode(2);
        ImNodes::BeginOutputAttribute(3);
        ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();
        ImGui::Dummy(ImVec2(80.0f, 45.0f));

        ImNodes::SetNodeEditorSpacePos(2, ImVec2(1500.0, 1500.0));
        ImNodes::EndNode();*/

        auto size = ImGui::GetWindowSize();

        progTree.drawTree(size.x, size.y);


        ImNodes::EndNodeEditor();

        ImGui::End();





        ImGui::SFML::Render(window);
        window.display();
    }
    ImNodes::DestroyContext();
    ImGui::SFML::Shutdown();
}