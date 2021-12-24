#ifndef ELIMIDI_H
#define ELIMIDI_H
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"
#include <QProcess>
#include <QApplication>
#include <QMessageBox>

    using namespace std;

class EliMIDI
{
public:
    EliMIDI();
    Utils utility;
    QProcess playProcess;
    qint64 playProcessID;
    bool cancelFlag=false;
 long start_time_msecs;

 void cancelPlay();
 void playAMIDIFile(QString midiFile);
 void startListening(QString filePrefix);
 ~EliMIDI();
 void playNext();
 void resetMIDI();
};

#endif // ELIMIDI_H
