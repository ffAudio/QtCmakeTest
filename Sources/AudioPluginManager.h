

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class AudioPluginManager
{
public:
    AudioPluginManager();
    
    /**
     Register a factory to return an AudioProcessor instance from an identifierString. This is used for built in
     AudioProcessors.
     */
    void registerAudioProcessor (const juce::String& identifierString, std::function<std::unique_ptr<juce::AudioProcessor>()>);

    /**
     Creates an actual instance of an AudioPlugin. It will try the first the built in factories, and then the
     AudioPlugins that were found on the system.
     */
    std::unique_ptr<juce::AudioProcessor> createAudioPluginInstance (const juce::String& identifierString,
                                                                     double sampleRate,
                                                                     int blockSize,
                                                                     juce::String& error) const;

    juce::Array<juce::PluginDescription> getKnownPluginDescriptions() const;

    /**
     Set a file to save the results of plugin scanning to. It will read first, if the file exists.
     On success, it writes the plugin database to the file.
     */
    void setPluginDataFile (const juce::File& file);
    
private:
    class PluginScanJob : public juce::ThreadPoolJob
    {
    public:
        PluginScanJob (AudioPluginManager& owner, std::unique_ptr<juce::AudioPluginFormat>&& format);

        juce::ThreadPoolJob::JobStatus runJob() override;

    private:
        AudioPluginManager& owner;
        std::unique_ptr<juce::AudioPluginFormat> format;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginScanJob)
    };

    std::map<juce::String, std::function<std::unique_ptr<juce::AudioProcessor>()>> factories;

    juce::ThreadPool               scannerThreads { 1 };
    juce::KnownPluginList          knownPluginList;
    juce::AudioPluginFormatManager pluginManager;
    juce::File                     pluginDataFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginManager)
};
