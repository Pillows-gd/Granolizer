#pragma once
#include <JuceHeader.h>

class MyLookAndFeel : public LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
		// Set various colours
		setColour(Slider::textBoxOutlineColourId, Colour(0.0f, 0.0f, 0.0f, 0.0f));
		setColour(Slider::textBoxTextColourId, Colour(0xb0000000));
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
	{
		// Useful variables
		const auto radius = jmin(width, height) * 0.5f * SCALE_TO_KNOB_RATIO - BORDER_WIDTH * 0.5f;
		const auto centreX = x + width * 0.5f;
		const auto centreY = y + height * 0.5f;
		const auto rx = centreX - radius;
		const auto ry = centreY - radius;
		const auto rw = radius * 2.0f;

		// Prepare pointer shape
		Path p;
		const auto pointerLength = radius * POINTER_L;
		const auto pointerThickness = POINTER_THIC;
		p.addRectangle(pointerThickness * -0.5f, -radius, pointerThickness, pointerLength);

		// Draw pointer shape
		//const auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
		const auto angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		g.setGradientFill(juce::ColourGradient(darkColour, rx, ry + rw, brightColour, rx + rw, ry, false));
		g.fillPath(p);
	}

	void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override
	{
		// Useful variables
		const auto centreX = x + width * 0.5f;
		const auto centreY = y + height * 0.5f;

		if (!isCutter)	// Generic slider
		{
			// Prepare pointer shape
			Path p;
			const auto pointerRadius = height * POINTER_L;
			p.addEllipse(pointerRadius * -0.5f, centreY, pointerRadius, pointerRadius);

			// Draw pointer shape
			p.applyTransform(AffineTransform::translation(maxSliderPos - minSliderPos, 0).translated(sliderPos, 0));
			g.setColour(darkColour.darker(0.8f));
			g.fillPath(p);
		}
		else if (isStart)	// Start cut slider
		{
			// Prepare pointer shape
			Path p;
			const auto pointerSide = height * POINTER_L / 2;
			p.addTriangle(
				0.0, centreY,
				pointerSide, centreY,
				0.0, centreY + pointerSide
			);

			// Draw pointer shape
			p.applyTransform(AffineTransform::translation(maxSliderPos - minSliderPos, 0).translated(sliderPos, 0));
			g.setColour(darkColour);
			g.fillPath(p);
		}
		else	// End cut slider
		{	
			// Prepare pointer shape
			Path p;
			const auto pointerSide = -height * POINTER_L / 2;
			p.addTriangle(
				0.0, centreY,
				0.0, centreY + pointerSide,
				pointerSide, centreY
			);

			// Draw pointer shape
			p.applyTransform(AffineTransform::translation(maxSliderPos - minSliderPos, 0).translated(sliderPos, 0));
			g.setColour(darkColour);
			g.fillPath(p);
		}
	}

	void drawTickBox(Graphics& g, Component& button, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		button.setAlpha(0.8f);
		if (button.isMouseOver() || ticked)
			button.setAlpha(1.0f);
		auto idle = juce::ImageCache::getFromMemory(BinaryData::tinyButton_png, BinaryData::tinyButton_pngSize);
		auto down = juce::ImageCache::getFromMemory(BinaryData::tinyButtonDown_png, BinaryData::tinyButtonDown_pngSize);
		if (ticked)
			g.drawImage(down, 0, 0, button.getWidth(), button.getHeight(), 0, 0, down.getWidth(), down.getHeight());
		else
			g.drawImage(idle, 0, 0, button.getWidth(), button.getHeight(), 0, 0, idle.getWidth(), idle.getHeight());
	}

	void setCutter(bool isIt, bool start)
	{
		isCutter = isIt;
		isStart = start;
	}

private:

	const Colour brightColour = Colour(0xff666350);
	const Colour darkColour = Colour(0xb0000000);
	bool isCutter = false;
	bool isStart = false;

};