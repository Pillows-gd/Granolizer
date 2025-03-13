#pragma once
#include <JuceHeader.h>

// Useful constants (#define ... )
#define MAX_NUM_CH 2 // could use JucePlugin_MaxNumOutputChannels instead
#define GLIDE_TIME 0.01
#define SMOOTHING_TIME 0.04
#define WF_SINE 0
#define WF_TRI 1
#define WF_SAW_UP 2
#define WF_SAW_DOWN 3
#define WF_SQUARE 4
#define WF_SnH 5
#define WF_COS_WINDOW 6
#define WINDOW_MUL_TH 0.85
#define MIN_TR -72
#define MAX_TR +24
#define MAX_GN 7
#define MAX_VOICES 10
#define MOV_REDUCTION 500
#define SS_REDUCTION 10
#define LFO_SMOOTH_RED 10
#define DEFAULT_KEYSYNC false
#define FILTER_Q 0.707f
#define DEFAULT_DIR File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)

#define BORDER_WIDTH 1.5f
#define SCALE_TO_KNOB_RATIO 0.80f
#define POINTER_L 0.33f
#define POINTER_THIC 2.0f
#define NUM_TICKS 21
#define TICK_THIC 2.0f
#define FPS 60
#define DB_FLOOR -48.0f
#define RT 0.5f

// Actual parameters (#define ... )
#define NAME_P "position"
#define NAME_SPD "position speed"
#define NAME_A "attack"
#define NAME_D "decay"
#define NAME_S "sustain"
#define NAME_R "release"
#define NAME_TR "transpose"
#define NAME_G "gain"
#define NAME_GN "grain number"
#define NAME_GDMA "grain dispersion mov amt"
#define NAME_GDMF "grain dispersion mov fr"
#define NAME_GDMWF "grain dispersion mov wf"
#define NAME_RPMA "read position mov amt"
#define NAME_RPMF "read position mov fr"
#define NAME_RPMWF "read position mov wf"
#define NAME_GD "grain dispersion"
#define NAME_RPS "read position spread"
#define NAME_SC "start cut"
#define NAME_EC "end cut"
#define NAME_MOD "mode"

#define DEFAULT_P 0.0000f
#define DEFAULT_SPD 1.00f
#define DEFAULT_A 0.01f
#define DEFAULT_D 0.01f
#define DEFAULT_S 1.00f
#define DEFAULT_R 0.01f
#define DEFAULT_TR 0
#define DEFAULT_G 0.00f
#define DEFAULT_GN 1
#define	DEFAULT_RPMA 0.000f
#define	DEFAULT_RPMF 0.1000f
#define DEFAULT_RPMWF 0
#define	DEFAULT_GDMA 0.000f
#define	DEFAULT_GDMF 0.1000f
#define DEFAULT_GDMWF 0
#define DEFAULT_GD 0.50f
#define DEFAULT_RPS 0.0000f
#define DEFAULT_SC 0.0000f
#define DEFAULT_EC 1.0000f

#define POS_CTRL_RES 0.00001f

namespace Parameters
{
	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<RangedAudioParameter>> params;

		params.push_back(std::make_unique<AudioParameterFloat>(NAME_P, "Position", NormalisableRange<float>(0.0000f, 1.0000f, POS_CTRL_RES), DEFAULT_P));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_SPD, "Speed", NormalisableRange<float>(-2.00f, 2.00f, 0.01f, 0.8, true), DEFAULT_SPD));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_A, "Attack", NormalisableRange<float>(0.0f, 5.0f, 0.01f, 0.5f), DEFAULT_A));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_D, "Decay", NormalisableRange<float>(0.0f, 5.0f, 0.01f, 0.5f), DEFAULT_D));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_S, "Sustain", 0.0f, 1.0f, DEFAULT_S));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_R, "Release", NormalisableRange<float>(0.0f, 5.0f, 0.01f, 0.5f), DEFAULT_R));
		params.push_back(std::make_unique<AudioParameterInt>(NAME_TR, "Transpose", MIN_TR, MAX_TR, DEFAULT_TR));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_G, "Gain", -48.00f, 12.00f, DEFAULT_G));
		params.push_back(std::make_unique<AudioParameterInt>(NAME_GN, "Grain Number", 1, MAX_GN, DEFAULT_GN));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_GDMA, "Grain Dispersion Movement Amount", NormalisableRange<float>(0.000f, 1.000f, 0.001f, 0.5f), DEFAULT_GDMA));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_GDMF, "Grain Dispersion Movement Frequency", NormalisableRange<float>(POS_CTRL_RES, 20.0000f, POS_CTRL_RES, 0.3f), DEFAULT_GDMF));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_GDMWF, "Grain Dispersion Movement Waveform", StringArray{ "Sine", "Triangle", "Saw up", "Saw down", "Square", "Sample and hold" }, DEFAULT_GDMWF));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_RPMA, "Read Position Movement Amount", NormalisableRange<float>(0.000f, 1.000f, 0.001f, 0.5f), DEFAULT_RPMA));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_RPMF, "Read Position Movement Frequency", NormalisableRange<float>(POS_CTRL_RES, 20.0000f, POS_CTRL_RES, 0.3f), DEFAULT_RPMF));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_RPMWF, "Read Position Movement Waveform", StringArray{ "Sine", "Triangle", "Saw up", "Saw down", "Square", "Sample and hold" }, DEFAULT_RPMWF));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_GD, "Gain Dispersion", NormalisableRange<float>(0.00f, 1.00f, 0.01f), DEFAULT_GD));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_RPS, "Read Position Spread", NormalisableRange<float>(0.0000f, 1.0000f, POS_CTRL_RES), DEFAULT_RPS));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_SC, "Start Cut", NormalisableRange<float>(0.0000f, 1.0000f, POS_CTRL_RES), DEFAULT_SC));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_EC, "End Cut", NormalisableRange<float>(0.0000f, 1.0000f, POS_CTRL_RES), DEFAULT_EC));

		return { params.begin(), params.end() };
	}
}