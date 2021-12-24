#include "elimidi.h"

EliMIDI::EliMIDI()
{

}
EliMIDI::~EliMIDI()
{
    cancelPlay();
}
void EliMIDI::startListening(QString filePrefix)
{
    QString midiCommand("/home/pi/qt/midibin/");
    QStringList midiCommandAurguments;
    midiCommand.append("brainstorm --in 1 --prefix ");

    midiCommand.append(filePrefix);

    cout << "midi command is " << midiCommand.toStdString() << endl;
    playProcess.start(midiCommand);
    playProcessID = playProcess.processId();
    cout << "listening process pid is " << playProcess.processId() << endl;

    bool done = false;
     done = playProcess.waitForFinished(1000);
//            cout << "midi output flag output is " << done << endl;

}

void EliMIDI::playAMIDIFile(QString midiFile)
{
        QString midiCommand("/home/pi/qt/midibin/");
 //       QStringList midiCommandAurguments;
         midiCommand.append("playsmf --out 1 ");

         midiCommand.append(midiFile);
        cout << "midi command is " << midiCommand.toStdString() << endl;
//        char *midiCommandOutput = utility.exec(midiCommand.toStdString().c_str());
        playProcess.start(midiCommand);
        playProcessID = playProcess.processId();
 //       QThread::sleep(3000);
        cout << "process pid is " << playProcess.processId() << endl;

        bool done = false;
    while(!done){
        done = playProcess.waitForFinished(1000);
//        cout << "midi output flag output is " << done << endl;
//        done = cancelFlag;
         QApplication::processEvents();
    }
//        playProcess.waitForFinished(-1);
    playProcess.close();
    cout << "midi output flag output is " << done << endl;
}

void EliMIDI::resetMIDI()
{

        QProcess resetMIDIProcess;
        QString midiCommand("/home/pi/qt/midibin/");
 //       QStringList midiCommandAurguments;
         midiCommand.append("playsmf --out 1 /home/pi/qt/midibin/SCCRESET.MID");
         resetMIDIProcess.startDetached(midiCommand);
        cout << "midi command is " << midiCommand.toStdString() << endl;
         QApplication::processEvents();
}

void EliMIDI::cancelPlay() {
//  playProcess.kill();
  cout << "tried to kill process " << endl;
  QString killCommand("kill -9 ");
  killCommand.append(QString::number(playProcessID));
  QProcess killProcess;
  cout << "kill process is " << killCommand.toStdString() << endl;
  killProcess.startDetached(killCommand);
  QString killAllCommand("killall -KILL playsmf");
  killProcess.startDetached(killAllCommand);
  cout << "kill all process is " << killAllCommand.toStdString() << endl;
  QApplication::processEvents();
  QString killAllCommand2("killall -KILL brainstorm");
  killProcess.startDetached(killAllCommand2);
  QApplication::processEvents();
  resetMIDI();
  playProcess.close();
  killProcess.close();
  cancelFlag=true;
}

void EliMIDI::playNext() {
    cout << "tried to kill process " << endl;
    QString killCommand("kill -9 ");
    killCommand.append(QString::number(playProcessID));
    QProcess killProcess;
    cout << "kill process is " << killCommand.toStdString() << endl;
    killProcess.startDetached(killCommand);
    killProcess.waitForFinished(-1);
    playProcess.kill();
    resetMIDI();
    cancelFlag=false;
    QApplication::processEvents();
}
