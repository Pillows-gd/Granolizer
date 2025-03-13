#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Meter : public Component, public Timer
{
public:
    Meter()
    {
        alpha = exp(-1.0f / (FPS * RT));
        startTimerHz(FPS);
    }

    ~Meter()
    {
        stopTimer();
    }

    void paint(Graphics& g) override
    {
        auto W = getWidth();
        auto H = getHeight();

        //g.fillAll(Colours::black);
        //g.setColour(Colours::grey);
        //g.drawRect(0, 0, W, H, 1);

        for (int i = 0; i < MAX_VOICES; i++)
        {
            for (int j = 0; j < MAX_GN; j++)
            {
                if (observedEnvelope[i][j] != nullptr)
                {
                    auto envelopeSnapshot = *observedEnvelope[i][j];
                    //DBG("Voice: " << i << ", Grain: " << j << ": " << envelopeSnapshot);
                    *observedEnvelope[i][j] = envelopeSnapshot /* * alpha */;

                    //auto peak = Decibels::gainToDecibels(envelopeSnapshot);

                    //auto barHeight = (H - 2) * (peak - DB_FLOOR) / abs(DB_FLOOR);
                    auto barX = jmap(envelopeSnapshot, 0.0f, 1.0f, 0.0f, (float)W);
                    //barX = jlimit(0.0f, H - 2.0f, barX);

                    //auto topColour = peak >= 0.0f ? Colours::red : Colours::yellow;
                    //ColourGradient filler = ColourGradient(Colours::blue, 0, H, topColour, 0, 0, false);
                    //filler.addColour(0.8f, Colours::green);

                    //g.setColour(peak >= 0.0f ? Colours::red : Colours::blueviolet);
                    //g.setGradientFill(filler);
                    g.setColour(Colour(0xFF493E24));

                    g.fillRect(barX, 0.0f, 1.0f, (float)H);
                }
            }
        }
    }

    void resized() override {}

    void connectTo(float& targetVariable, int voice, int grain)
    {
        observedEnvelope[voice][grain] = &targetVariable;
    }

private:

    void timerCallback() override
    {
        //DBG("CIAO");
        repaint();
    }

    float alpha = 0.0f;

    float* observedEnvelope[MAX_VOICES][MAX_GN] = { {nullptr} };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter)
};


class CutBar : public Component
{
public:

    CutBar() {}

    ~CutBar() {}

    void paint(Graphics& g) override
    {
        g.fillAll(Colour(0x5F000000));
    }

    void resized() override {}

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CutBar)
};