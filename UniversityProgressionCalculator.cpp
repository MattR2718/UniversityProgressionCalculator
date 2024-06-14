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
    ImGuiIO& io = ImGui::GetIO();
    //io.FontAllowUserScaling = true;
    
    float textScale = 2.5;

    std::vector<Year> years = { Year("one"), Year("two") , Year("three") };

    for (auto& y : years) {
        for (auto& t : y.terms) {
            t.fontScale = &textScale;
        }
    }

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

        //
        // 
        //Main Screen
        ImGui::Begin("Frame");
        auto windowSize = window.getSize();
        //Make window fill screen
        ImGui::SetWindowPos(ImVec2(0.0, 0.0));
        ImGui::SetWindowSize(ImVec2(windowSize.x, windowSize.y));
        
        // Set Number Of Columns, one for year and one for style
        ImGui::Columns(2);


        //Create tabs for years
        ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Years Tab Bar", tabBarFlags)) {
            ImGuiTabItemFlags tabItemFlags = ImGuiTabItemFlags_NoAssumedClosure | ImGuiTabItemFlags_NoReorder;
            for (auto& year : years) {
                if (year.tabOpen && ImGui::BeginTabItem(year.year.c_str(), &year.tabOpen, tabItemFlags)) {
                    year.display();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabItem();
        }

        ImGui::NextColumn();

        //Fps
        ImGui::Text(("FPS: " + std::to_string(io.Framerate)).c_str());
        ImGui::InputFloat("Text Scale", &textScale, 0.1f, 0.25f, "%.3f");
        ImGui::SetWindowFontScale(textScale);


        ImGui::End();

        ImGui::ShowDemoWindow();


        //End of Main Screen
        //
        //

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}