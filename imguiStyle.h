#pragma once

#include <imgui.h>

#include "RobotoBlack.h"

//void setupFonts() {
//    ImGuiIO& io = ImGui::GetIO();
//    ImFontConfig config;
//    config.FontDataOwnedByAtlas = false;
//
//    io.Fonts->AddFontFromMemoryTTF((void*)Roboto_Black_ttf, sizeof(Roboto_Black_ttf), 16.0f, &config);
//
//    // Merge glyphs
//    io.Fonts->Build();
//
//    io.FontDefault = io.Fonts->Fonts[0];
//}

void setStyle(bool darkMode = false, float alpha = 1.0f) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);

    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);

    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

}

void SetLightThemeColours()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colours = style.Colors;

    // Main window background
    colours[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    // Child window background
    colours[ImGuiCol_ChildBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);

    // Borders
    colours[ImGuiCol_Border] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colours[ImGuiCol_BorderShadow] = ImVec4(0.75f, 0.75f, 0.75f, 0.50f);

    // Headers
    colours[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 0.65f);
    colours[ImGuiCol_HeaderHovered] = ImVec4(0.65f, 0.65f, 0.65f, 0.80f);
    colours[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Buttons
    colours[ImGuiCol_Button] = ImVec4(0.75f, 0.75f, 0.75f, 0.40f);
    colours[ImGuiCol_ButtonHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colours[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Frame backgrounds
    colours[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colours[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colours[ImGuiCol_FrameBgActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

    // Text
    colours[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colours[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

    // Check mark
    colours[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f);

    // Slider grab
    colours[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f);
    colours[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.55f, 1.00f, 1.00f);

    // Scrollbar
    colours[ImGuiCol_ScrollbarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colours[ImGuiCol_ScrollbarGrab] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Separators
    colours[ImGuiCol_Separator] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colours[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colours[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Title
    colours[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colours[ImGuiCol_TitleBgActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);

    // Resize grip
    colours[ImGuiCol_ResizeGrip] = ImVec4(0.75f, 0.75f, 0.75f, 0.40f);
    colours[ImGuiCol_ResizeGripHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colours[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    // Tab
    colours[ImGuiCol_Tab] = ImVec4(0.80f, 0.80f, 0.80f, 0.90f);
    colours[ImGuiCol_TabHovered] = ImVec4(0.65f, 0.65f, 0.65f, 0.80f);
    colours[ImGuiCol_TabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colours[ImGuiCol_TabUnfocused] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colours[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);

    // Plot
    colours[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f);
    colours[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.55f, 1.00f, 1.00f);
    colours[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f);
    colours[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.55f, 1.00f, 1.00f);

    // Text selections
    colours[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f);

    // Modal window dim
    colours[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    // Menu bar background
    colours[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);

    // Popups and dropdowns
    colours[ImGuiCol_PopupBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);

    // Additional adjustments for better visibility in light theme
    colours[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    colours[ImGuiCol_TableBorderStrong] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colours[ImGuiCol_TableBorderLight] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colours[ImGuiCol_TableRowBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colours[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
}

void SetDarkThemeColours()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colours = style.Colors;

    // Main window background
    colours[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    // Child window background
    colours[ImGuiCol_ChildBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    // Borders
    colours[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colours[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Headers
    colours[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.55f);
    colours[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
    colours[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // Buttons
    colours[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 0.40f);
    colours[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.60f);
    colours[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // Frame backgrounds
    colours[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colours[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colours[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);

    // Text
    colours[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colours[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Check mark
    colours[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

    // Slider grab
    colours[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colours[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    // Scrollbar
    colours[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colours[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Separators
    colours[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
    colours[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f);
    colours[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);

    // Title
    colours[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colours[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    // Resize grip
    colours[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
    colours[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
    colours[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    // Tab
    colours[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);
    colours[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
    colours[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colours[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colours[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    // Plot
    colours[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colours[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.75f, 0.00f, 1.00f);
    colours[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colours[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

    // Text selections
    colours[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.45f, 0.90f, 0.35f);

    // Modal window dim
    colours[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    // Menu bar background
    colours[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

    // Popups and dropdowns
    colours[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    // Additional adjustments for better visibility in dark theme
    colours[ImGuiCol_TableHeaderBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colours[ImGuiCol_TableBorderStrong] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colours[ImGuiCol_TableBorderLight] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    colours[ImGuiCol_TableRowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colours[ImGuiCol_TableRowBgAlt] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

}

