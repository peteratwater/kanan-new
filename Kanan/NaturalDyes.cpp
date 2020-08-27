#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "NaturalDyes.hpp"

using namespace std;

namespace kanan {
    NaturalDyes::NaturalDyes()
        : m_isEnabled{ false },
        m_scorePatch{},
        m_redPatch{},
        m_greenPatch{},
        m_bluePatch{},
        m_redValue{},
        m_greenValue{},
        m_blueValue{}

    {
        auto rgbAddress = scan("client.exe", "C7 83 58 02 00 00 80 00 00 00");
        auto scoreAddress = scan("client.exe", "01 86 38 02 00 00");

        if (rgbAddress) {
            log("Got NaturalDyest %p", *rgbAddress);

            m_redPatch.address = *rgbAddress + 0x6;
            m_greenPatch.address = *rgbAddress + 0x10;
            m_bluePatch.address = *rgbAddress + 0x1A;
            m_scorePatch.address = *scoreAddress;
        }
        else {
            log("Failed to find NaturalDyes address.");
        }
    }

    void NaturalDyes::onUI() {
        if (m_redPatch.address == 0) {
            return;
        }
        if (ImGui::CollapsingHeader("NaturalDyes")) {
            ImGui::TextWrapped("Forces game to use specified RGB value for starting the minigame - disables scoring."
                "Use with client Side Devcat for instant complete. Enter 0 - 255 for each color value");
            ImGui::Checkbox("Enable NaturalDyes", &m_isEnabled);
            ImGui::InputInt("Red", &m_redValue);
            ImGui::InputInt("Green", &m_greenValue);
            ImGui::InputInt("Blue", &m_blueValue);
            apply();
        }
    }

    void NaturalDyes::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("NaturalDyes.Enabled").value_or(false);
        m_redValue = cfg.get<int>("NaturalDyes.Red").value_or(128);
        m_greenValue = cfg.get<int>("NaturalDyes.Green").value_or(128);
        m_blueValue = cfg.get<int>("NaturalDyes.Blue").value_or(128);
        
    }

    void NaturalDyes::onConfigSave(Config& cfg) {
        cfg.set<bool>("NaturalDyes.Enabled", m_isEnabled);
        cfg.set<int>("NaturalDyes.Red", m_redValue);
        cfg.set<int>("NaturalDyes.Green", m_greenValue);
        cfg.set<int>("NaturalDyes.Blue", m_blueValue);
    }

    void NaturalDyes::apply() {
        if (m_redPatch.address == 0) {
            return;
        }

        if (m_isEnabled) {
            log("NaturalDyes...");
            
            //cast first byte of our int to the bytes we patch. 
            m_redPatch.bytes = { (unsigned char)(m_redValue & 0xFF) };
            patch(m_redPatch);
            m_greenPatch.bytes = { (unsigned char)(m_greenValue & 0xFF) };
            patch(m_greenPatch);
            m_bluePatch.bytes = { (unsigned char)(m_blueValue & 0xFF) };
            patch(m_bluePatch);



        }
        else {
            log("Disabling NaturalDyes...");

            undoPatch(m_redPatch);
            undoPatch(m_greenPatch);
            undoPatch(m_bluePatch);
        }
    }
}
