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

	ImPlot::CreateContext();

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
    float yearWeights[4] = { 0.0f, 20.0f, 40.0f, 40.0f };

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

    std::string currentlySelectedTab = years[0].year;

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
                        if (ImGui::BeginTabItem(year.year.c_str(), &year.tabOpen, tabItemFlags)) {
                            if (ImGui::IsItemClicked()) {
                                currentlySelectedTab = year.year;
								//std::cout << year.year << '\n';
                            }
                            // Set Number Of Columns, for each term
                            ImGui::Columns(year.terms.size());

                            for (auto& t : year.terms) {
                                t.display();
                                ImGui::NextColumn();
                            }

                            

                            ImGui::EndTabItem();

                            keyInformation.calcData(year);
                        }
                        if (ImGui::IsItemClicked()) {
                            currentlySelectedTab = year.year;
                            //std::cout << year.year << '\n';
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

            /*
            ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
            ImGui::BeginTabBar("Data Tab Bar", tabBarFlags);
            // create 2 tabs, one for the tree and one for graphs
			if (ImGui::BeginTabItem("Tree View")) {
                //
                //
                //
                //
                //
                // Tree View
                ImGui::Begin("Tree View", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

                // Make window fill screen but positioned below the menu bar
                ImGui::SetWindowPos(ImVec2(ww / 2, menuBarHeight * 5));
                ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight * 5));

                ImNodes::BeginNodeEditor();

                auto size = ImGui::GetWindowSize();

                progTree.drawTree(size.x, size.y, keyInformation);


                ImNodes::EndNodeEditor();

                ImGui::End();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Graphs")) {
				//
				//
				//
				//
				//
				// Graphs
				ImGui::Begin("Graphs", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
				// Make window fill screen but positioned below the menu bar
				ImGui::SetWindowPos(ImVec2(ww / 2, menuBarHeight));
				ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));
				//ImGui::SetWindowFontScale(textScale);
				ImGui::Text("Graphs");
				ImGui::End();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();*/
            

            // Create the main window that will contain the tab bar
            ImGui::Begin("Editor Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
            // Make window fill screen but positioned below the menu bar
            ImGui::SetWindowPos(ImVec2(ww / 2, menuBarHeight));
            ImGui::SetWindowSize(ImVec2(ww / 2, wh - menuBarHeight));

            // Begin a tab bar
            if (ImGui::BeginTabBar("EditorTabBar")) {
                // Create a tab for the Tree View
                if (ImGui::BeginTabItem("Tree View")) {
                    // Calculate the available area for the node editor
                    // This accounts for the tab bar height
                    ImVec2 tabContentArea = ImGui::GetContentRegionAvail();

                    // Create a child window to hold the node editor with proper sizing
                    // We don't need to manually set the cursor position anymore
                    ImGui::BeginChild("NodeEditorChild", tabContentArea, false);

                    ImNodes::BeginNodeEditor();
                    auto size = ImGui::GetContentRegionAvail();
                    progTree.drawTree(size.x, size.y, keyInformation);
                    ImNodes::EndNodeEditor();

                    ImGui::EndChild();

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Graphs")) {



                    // Plot lines on the graph to show grade boundaries
                    auto plotGradeBoundaryLines = [](std::vector<double> positions) {
                        positions.insert(positions.begin(), 0.0f);
                        positions.push_back(static_cast<double>(positions.size()));

                        // Add horizontal lines at specific levels (e.g., 50%, 60%, 70%)
                        std::vector<double> lineLevels = { 40.0f, 50.0f, 60.0f, 70.0f };
                        for (float level : lineLevels) {
                            ImU32 lineColor;
                            if (level >= 70.0f)      lineColor = IM_COL32(0, 200, 0, 255);   // Green (First)
                            else if (level >= 60.0f) lineColor = IM_COL32(0, 150, 255, 255); // Blue (2:1)
                            else if (level >= 50.0f) lineColor = IM_COL32(255, 165, 0, 255); // Orange (2:2)
                            else if (level >= 40.0f) lineColor = IM_COL32(255, 0, 0, 255);   // Red (Pass)
                            else                                     lineColor = IM_COL32(100, 100, 100, 255); // Gray (Fail)

                            std::vector<double> yValues(positions.size(), level);  // Create a vector with constant level values
                            ImPlot::PushStyleColor(ImPlotCol_Line, lineColor);  // Set the color
                            ImPlot::PlotLine("Threshold Line", positions.data(), yValues.data(), static_cast<int>(yValues.size()), ImPlotLineFlags_None);  // Plot the line
                            ImPlot::PopStyleColor();  // Reset the color
                        }
                        };


                    // Graph for data on overall degree
                    ImGui::InputFloat4("Year Weights", yearWeights);

                    // Bar chart for current year with every module as well as overall percentage being a bar
                    ImPlot::SetNextAxesLimits(0, years.size() + 1, 0, 100, ImGuiCond_Always);
                    auto size = ImGui::GetContentRegionAvail();
                    if (ImPlot::BeginPlot("Year Progression", ImVec2(size.x, size.y / 2), ImPlotFlags_NoLegend)) {
                        std::vector<const char*> labels;
                        std::vector<float> values;
                        std::vector<double> positions;

                        for (size_t i = 0; i < years.size(); ++i) {
                            labels.push_back(years[i].year.c_str());
                            values.push_back(years[i].getOverallPercentage());
                            positions.push_back(static_cast<double>(i) + 0.5);
                        }

                        // Add "Degree" bar
                        labels.push_back("Degree");
                        values.push_back(0.0f);
                        for (size_t i = 0; i < years.size(); ++i) {
                            values.back() += values[i] * ((yearWeights[i] > 1.0f) ? yearWeights[i] / 100.0f : yearWeights[i]);
                        }
                        positions.push_back(static_cast<double>(years.size()) + 0.5);

                        // Set categorical x-axis labels
                        ImPlot::SetupAxisTicks(ImAxis_X1, positions.data(), positions.size(), labels.data());

						// Plot grade boundary lines
						plotGradeBoundaryLines(positions);

                        // Loop through bars and apply color dynamically
                        for (size_t i = 0; i < values.size(); ++i) {
                            ImU32 barColor;
                            if (values[i] >= 70.0f)      barColor = IM_COL32(0, 200, 0, 255);   // Green (First)
                            else if (values[i] >= 60.0f) barColor = IM_COL32(0, 150, 255, 255); // Blue (2:1)
                            else if (values[i] >= 50.0f) barColor = IM_COL32(255, 165, 0, 255); // Orange (2:2)
                            else if (values[i] >= 40.0f) barColor = IM_COL32(255, 0, 0, 255);   // Red (Pass)
                            else                         barColor = IM_COL32(100, 100, 100, 255); // Gray (Fail)

                            ImPlot::PushStyleColor(ImPlotCol_Fill, barColor);
                            ImPlot::PlotBars("Overall Percentage", &values[i], 1, 0.5f, positions[i]); // Single bar
                            ImPlot::PopStyleColor();
                        }



                        ImPlot::EndPlot();
                    }


                    // Graph for currently seletced year
                    Year selectedYear = *std::find_if(years.begin(), years.end(), [&currentlySelectedTab](const Year& y) { return y.year == currentlySelectedTab; });

					ImPlot::SetNextAxesLimits(0, selectedYear.getTotalNumModules(), 0, 100, ImGuiCond_Always);

                    auto plotsize = ImGui::GetContentRegionAvail();
                    if (ImPlot::BeginPlot("Module Progression", ImVec2(plotsize.x, plotsize.y), ImPlotFlags_NoLegend)) {

                        std::vector<double> positions;
						for (int i = 0; i < selectedYear.getTotalNumModules(); ++i) {
							positions.push_back(static_cast<double>(i) + 0.5);
						}

						std::vector<const char*> moduleNames = selectedYear.getModuleNames();
						std::vector<float> modulePercentages = selectedYear.getModulePercentages();

						ImPlot::SetupAxisTicks(ImAxis_X1, positions.data(), positions.size(), moduleNames.data());

						// Plot grade boundary lines
						plotGradeBoundaryLines(positions);
                        

                        for (size_t i = 0; i < modulePercentages.size(); ++i) {
                            ImU32 barColor;
                            if (modulePercentages[i] >= 70.0f)      barColor = IM_COL32(0, 200, 0, 255);   // Green (First)
                            else if (modulePercentages[i] >= 60.0f) barColor = IM_COL32(0, 150, 255, 255); // Blue (2:1)
                            else if (modulePercentages[i] >= 50.0f) barColor = IM_COL32(255, 165, 0, 255); // Orange (2:2)
                            else if (modulePercentages[i] >= 40.0f) barColor = IM_COL32(255, 0, 0, 255);   // Red (Pass)
                            else                                     barColor = IM_COL32(100, 100, 100, 255); // Gray (Fail)

                            ImPlot::PushStyleColor(ImPlotCol_Fill, barColor);
                            ImPlot::PlotBars("Module Percentage", &modulePercentages[i], 1, 0.5f, positions[i]); // Single bar
                            ImPlot::PopStyleColor();
                        }





                        ImPlot::EndPlot();
                    }
                    




                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();

            

            if (keyInformationWindow) {
                keyInformation.display(keyInformationWindow);
            }
        }

        window.end_frame();
    }

    saveYearsToJson(path, years);

    ImPlot::DestroyContext();
    ImNodes::DestroyContext();
}