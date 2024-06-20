// UniversityProgressionCalculator.cpp : Defines the entry point for the application.
//

#include "UniversityProgressionCalculator.h"

int main() {

    const int WIDTH = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().width) / 100) * 100;
    const int HEIGHT = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().height) / 100) * 100;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "University Progression Calculator");
    window.setVerticalSyncEnabled(true);
    auto _ = ImGui::SFML::Init(window);
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    float textScale = (sf::VideoMode::getDesktopMode().width < 1350) ? 1.4 : 1.8;

    std::vector<Year> years;

    bool appearancePopup = false;

    std::string path = "../../../userData/userData.json";
    if (std::filesystem::exists(path)) {
        years = loadYearsFromJson(path);
    }

    for (auto& y : years) {
        y.calculatePercentages();
    }

    std::string treePath = "../../../progression.json";
    ProgressionTree progTree(treePath);
    ImNodesStyle& style = ImNodes::GetStyle();
    unsigned int bg = style.Colors[ImNodesCol_NodeBackground] & ((-1) >> (32 - 24));
    bg = bg + (90 << 24);
    style.Colors[ImNodesCol_NodeBackground] = bg;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
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

        progTree.drawTree(size.x, size.y);


        ImNodes::EndNodeEditor();

        ImGui::End();

        //ImGui::ShowDemoWindow();

        //ImGui::Begin("FPS");
        //ImGui::Text(std::to_string(1.0 / t.asSeconds()).c_str());
        //ImGui::End();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImNodes::DestroyContext();
    ImGui::SFML::Shutdown();
}