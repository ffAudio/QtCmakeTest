#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <juce_audio_devices/juce_audio_devices.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    juce::ToneGeneratorAudioSource source;
    juce::AudioSourcePlayer player;

    juce::AudioDeviceManager audioDeviceManager;
};
#endif // MAINWINDOW_H
