#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    juce::MessageManager::getInstance();

    auto ret = audioDeviceManager.initialiseWithDefaultDevices (0, 2);
    DBG ("Initialise: " << ret);
    auto setup = audioDeviceManager.getAudioDeviceSetup();
    DBG ("Output: " << setup.outputDeviceName);
    DBG ("SampleRate: " << setup.sampleRate);

    source.setFrequency (440.0);
    source.setAmplitude (0.7f);

    player.setSource (&source);
    
    audioDeviceManager.addAudioCallback (&player);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

