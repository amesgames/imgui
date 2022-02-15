#pragma once

#include "imgui.h"

namespace ImGui {
    namespace Amesgames {
        /**
         * Call these functions from the ImGui application
         */

        // Call before the first NewFrame at ImGui application initialization.
        void LoadFont(float size = 16.000000f);

        // Call before the first NewFrame at ImGui application initialization.
        void SetupStyle();

        // Call amy time outside of a frame to change to the Dark style.
        void StyleColorsDark(ImGuiStyle* ref = NULL);

        // Call amy time outside of a frame to change to the Light style.
        void StyleColorsLight(ImGuiStyle* ref = NULL);

        /**
         * The below functionality is for the demo Style Editor
         */

        // Call to display the Style Color combo selector.
        ImGuiStyle* ShowStyleColorsSelector();

        // Call to get the edited Dark style
        ImGuiStyle* GetEditedDarkStyle();

        // Call to get the edited Light style
        ImGuiStyle* GetEditedLightStyle();
    }
}
