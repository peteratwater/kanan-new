#pragma once

#include <Patch.hpp>

#include <imgui.h>

#include "PatchMod.hpp"

namespace kanan {
    class NaturalDyes : public Mod {
    public:
        NaturalDyes();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        Patch m_scorePatch;
        Patch m_redPatch;
        Patch m_greenPatch;
        Patch m_bluePatch;
        bool m_isEnabled;
        int m_redValue;
        int m_greenValue;
        int m_blueValue;

        void apply();
    };
}