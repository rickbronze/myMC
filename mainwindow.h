#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QThread>
#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QStringList>
#include <QFileSystemModel>
#include <QListView>
#include "elimidi.h"
#define STOPPED 0
#define PLAYING 1
#define LISTENING 2
#define INTERACTIVE 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    EliMIDI midiControls;
    bool cancelFlag = false;
    bool nextFlag = false;
    void updateStatus(QString newStatus);
    void updateFileModel(QString newRecordFile);
    void InsertRecordFileList(QStringList newRecordFiles);
    int mode=0;

    void selectNextRow(QListView *view);
private slots:
    void on_pbStartListening_clicked();

    void on_pbPlayDirectory_clicked();

    void on_pbPlayFile_clicked();


    void on_pbCancelPlay_clicked();

    void on_lvRecordingSession_doubleClicked(const QModelIndex &index);

    void on_pbPlayRecording_clicked();

    void on_pbDeleteRecording_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pbChooseDirectory_clicked();

    void on_lePlayDirectory_textChanged(const QString &arg1);

    void on_pbChooseDirectory_2_clicked();

    void on_MainWindow_destroyed();

    void on_pbChooseDirectory_3_clicked();

    void on_pb_PlaySlideshow_clicked();

    void on_pbNext_clicked();

    void on_pbRenameRecording_clicked();

    void on_lePlayDirectory_2_textChanged(const QString &arg1);

    void on_pbExit_clicked();

    void on_pbNextRecording_clicked();

    void on_pb_PlaySlideshow_2_clicked();

    void on_pbDelete_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
