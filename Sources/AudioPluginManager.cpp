

#include "AudioPluginManager.h"


AudioPluginManager::AudioPluginManager ()
{
    pluginManager.addDefaultFormats();
}

void AudioPluginManager::registerAudioProcessor (const juce::String& identifierString, std::function<std::unique_ptr<juce::AudioProcessor>()> factory)
{
    factories [identifierString] = std::move (factory);
}

std::unique_ptr<juce::AudioProcessor> AudioPluginManager::createAudioPluginInstance (const juce::String& identifierString,
                                                                                     double sampleRate,
                                                                                     int blockSize,
                                                                                     juce::String& error) const
{
    auto factory = factories.find (identifierString);
    if (factory != factories.cend())
        return factory->second();

    auto description = knownPluginList.getTypeForIdentifierString (identifierString);
    if (description.get() == nullptr)
    {
        error = NEEDS_TRANS ("Plugin not known");
        return {};
    }

    std::unique_ptr<juce::AudioPluginInstance> plugin (pluginManager.createPluginInstance (*description, sampleRate, blockSize, error));
    return plugin;
}

juce::Array<juce::PluginDescription> AudioPluginManager::getKnownPluginDescriptions() const
{
    return knownPluginList.getTypes();
}

void AudioPluginManager::setPluginDataFile (const juce::File& file)
{
    pluginDataFile = file;

    auto xml = juce::XmlDocument::parse (pluginDataFile);
    if (xml.get() != nullptr)
    {
        knownPluginList.recreateFromXml (*xml);
    }

#if JUCE_PLUGINHOST_AU && (JUCE_MAC || JUCE_IOS)
    scannerThreads.addJob (new PluginScanJob (*this,
                                              std::make_unique<juce::AudioUnitPluginFormat>()),
                           true);
#endif

#if JUCE_PLUGINHOST_LADSPA && JUCE_LINUX
    scannerThreads.addJob (new PluginScanJob (*this,
                                              std::make_unique<juce::LADSPAPluginFormat>()),
                           true);
#endif

#if JUCE_PLUGINHOST_VST3 && (JUCE_MAC || JUCE_WINDOWS)
    scannerThreads.addJob (new PluginScanJob (*this,
                                              std::make_unique<juce::VST3PluginFormat>()),
                           true);
#endif
}

//==============================================================================

AudioPluginManager::PluginScanJob::PluginScanJob (AudioPluginManager& ownerToUse,
                                                  std::unique_ptr<juce::AudioPluginFormat>&& formatToUse)
  : juce::ThreadPoolJob ("Plugin Scanner"),
    owner (ownerToUse),
    format (std::move (formatToUse))
{
}

juce::ThreadPoolJob::JobStatus AudioPluginManager::PluginScanJob::runJob()
{
    auto deadMansPedal = juce::File::getSpecialLocation (juce::File::tempDirectory).getChildFile (format->getName() + "ScanPlugins.tmp");
    juce::PluginDirectoryScanner scanner (owner.knownPluginList,
                                          *format,
                                          format->getDefaultLocationsToSearch(),
                                          true,
                                          deadMansPedal);

    juce::String name;
    while (scanner.scanNextFile (true, name))
    {
        DBG ("Scanned " + format->getName() + ": " + name);

        auto xml = owner.knownPluginList.createXml();
        if (xml.get() != nullptr)
            xml->writeTo (owner.pluginDataFile);
    }

    return juce::ThreadPoolJob::jobHasFinished;
}
