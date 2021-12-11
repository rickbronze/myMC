#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateStatus("Stopped");
    on_pbStartListening_clicked();
    on_pb_PlaySlideshow_clicked();
    QFileSystemModel *listModel = new QFileSystemModel(this);
    QString directory = ui->lePlayDirectory->text();
    listModel->sort(0, Qt::SortOrder::DescendingOrder);
    ui->lvRecordingSession->setModel(listModel);
    ui->lvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setModel(listModel);
    ui->tvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setMinimumWidth(250);
    ui->tabWidget->removeTab(2);
    QFont stopFont;
    stopFont.setBold(true);
    stopFont.setPointSize(28);
    ui->pbCancelPlay->setFont(stopFont);

}

MainWindow::~MainWindow()
{
    on_pbCancelPlay_clicked();
    delete ui;
}

void MainWindow::on_pbStartListening_clicked()
{
    if (mode != STOPPED) {
        QMessageBox msg(QMessageBox::Information,"Not Stopped", "The MIDI is currently performing an operation. \n Please hit the Stop button first and try again.", QMessageBox::Ok);
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        msg.setFont(font);
        msg.button(QMessageBox::Ok)->setFont(font);
        msg.exec();
        return;
    }
    mode = LISTENING;
    QString listenFilePrefix(ui->lePlayDirectory->text() + ui->leRecordPrefix->text());
    midiControls.startListening(listenFilePrefix);
    updateStatus("Listening for any piano play...\n " + ui->lePlayDirectory->text() + ui->leRecordPrefix->text());

}

void MainWindow::on_pbPlayDirectory_clicked()
{
    if (mode != STOPPED) {
        QMessageBox msg(QMessageBox::Information,"Not Stopped", "The MIDI is currently performing an operation. \n Please hit the Stop button first and try again.", QMessageBox::Ok);
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        msg.setFont(font);
        msg.button(QMessageBox::Ok)->setFont(font);
        msg.exec();
        return;
    }

    vector<string> filesToPlay;
    cancelFlag=false;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    mode = PLAYING;
    updateStatus("Playing Directory of files in " + dir);
    ui->lePlayDirectory->setText(dir);
    dir.append("/*.mid");
    filesToPlay = midiControls.utility.getDirectoryFiles(dir.toStdString());

    cout << "files to play size is " << filesToPlay.size() << endl;

//    for (unsigned int i = 0; i < filesToPlay.size();i++) {
    while(!filesToPlay.empty()){
       if (cancelFlag==false) {
           srand(time(NULL));
           unsigned int randFile = (rand() % filesToPlay.size());
           updateStatus("Playing Directory of files in " + dir + "\n" + filesToPlay.at(randFile).c_str());
           midiControls.playAMIDIFile(filesToPlay.at(randFile).c_str());
           midiControls.playProcess.waitForFinished(-1);
           filesToPlay.erase(filesToPlay.begin()+randFile);
       }
        else break;
    }
    cancelFlag = false;
    on_pbCancelPlay_clicked();
}


void MainWindow::on_pbPlayFile_clicked()
{

    if (mode != STOPPED) {
        QMessageBox msg(QMessageBox::Information,"Not Stopped", "The MIDI is currently performing an operation. \n Please hit the Stop button first and try again.", QMessageBox::Ok);
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        msg.setFont(font);
        msg.button(QMessageBox::Ok)->setFont(font);
        msg.exec();
        return;
    }

    QString fileToPlay = QFileDialog::getOpenFileName(this, "Open a MIDI file to play", ui->lePlayDirectory->text(), "MIDI (*.mid), All Files (*.*)");
    mode = PLAYING;
    updateStatus("Playing the file " + fileToPlay);
    midiControls.playAMIDIFile(fileToPlay);
    midiControls.playProcess.waitForFinished(-1);
    on_pbCancelPlay_clicked();
}

void MainWindow::updateStatus(QString newStatus)
{
    ui->lblStatus->setText(newStatus);
}


void MainWindow::on_pbCancelPlay_clicked()
{
    updateStatus("Stopped");
    mode = STOPPED;
    cancelFlag = true;
    midiControls.cancelPlay();
}


void MainWindow::updateFileModel(QString fileDirectory)
{
    QFileSystemModel *listModel = new QFileSystemModel(this);
    QString directory = fileDirectory;
    listModel->sort(0, Qt::SortOrder::DescendingOrder);
    ui->lvRecordingSession->setModel(listModel);
    ui->lvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setModel(listModel);
    ui->tvRecordingSession->setRootIndex(listModel->setRootPath(directory));

}

void MainWindow::InsertRecordFileList(QStringList newRecordFiles)
{
    QFileSystemModel *listModel = new QFileSystemModel(this);
    QString directory = "/home/pi/midi";
    listModel->sort(0, Qt::SortOrder::DescendingOrder);
    ui->lvRecordingSession->setModel(listModel);
    ui->lvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setModel(listModel);
    ui->tvRecordingSession->setRootIndex(listModel->setRootPath(directory));
}

void MainWindow::on_lvRecordingSession_doubleClicked(const QModelIndex &index)
{
    Utils utility;
    updateFileModel(ui->lePlayDirectory->text());

}


void MainWindow::on_pbPlayRecording_clicked()
{
    bool wasListening = false;
    if (mode == LISTENING) {
        wasListening = true;
        on_pbCancelPlay_clicked();
        QApplication::processEvents();
    }
    else if (mode != STOPPED) {
        QMessageBox msg(QMessageBox::Information,"Not Stopped", "The MIDI is currently performing an operation. \n Please hit the Stop button first and try again.", QMessageBox::Ok);
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        msg.setFont(font);
        msg.button(QMessageBox::Ok)->setFont(font);
        msg.exec();
        return;
    }
    QString fileNameWithPath(ui->lePlayDirectory->text());
    fileNameWithPath.append("/");
    QModelIndexList templatelist = ui->lvRecordingSession->selectionModel()->selectedIndexes();
       foreach (const QModelIndex &indexLoop, templatelist){
       fileNameWithPath.append(indexLoop.data(Qt::DisplayRole).toString());
       updateStatus("Playing the file " + fileNameWithPath);
       midiControls.playAMIDIFile(fileNameWithPath);
       midiControls.playProcess.waitForFinished(-1);
       updateStatus("Stopped");
       }
       if(wasListening){
           on_pbStartListening_clicked();
       }

}

void MainWindow::on_pbDeleteRecording_clicked()
{
    QString fileNameWithPath(ui->lePlayDirectory->text());
    fileNameWithPath.append("/");
    QModelIndexList templatelist = ui->lvRecordingSession->selectionModel()->selectedIndexes();


    foreach (const QModelIndex &indexLoop, templatelist){
        fileNameWithPath.append(indexLoop.data(Qt::DisplayRole).toString());
        if (templatelist.isEmpty()) // check if has selection
        {
          QMessageBox::warning(this, "Nothing Selected",
                               "A file must be selected first for this operations.  "
                               "\n Please select a file from the List of files.");
        } else {
          QMessageBox::StandardButton reply;
          reply = QMessageBox::question(
              this, "Deleting File",
              "Deleting file " +
                  fileNameWithPath +
                  "?",
              QMessageBox::Yes | QMessageBox::No);
          if (reply == QMessageBox::Yes) {
       QString rmCommand("rm -r  ");
       rmCommand.append(fileNameWithPath);
       QProcess rmProcess;
       cout << "rm command is " << rmCommand.toStdString() << endl;
       rmProcess.startDetached(rmCommand);
          }}
}       updateFileModel(ui->lePlayDirectory->text());

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    updateFileModel(ui->lePlayDirectory->text());
}

void MainWindow::on_pbChooseDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lePlayDirectory->setText(dir);
    updateFileModel(ui->lePlayDirectory->text());
}

void MainWindow::on_lePlayDirectory_textChanged(const QString &arg1)
{
    if (!ui->lePlayDirectory->text().endsWith("/")) {
        ui->lePlayDirectory->setText(ui->lePlayDirectory->text() + "/");
    }

    ui->lePlayDirectory_2->setText(ui->lePlayDirectory->text());
}

void MainWindow::on_pbChooseDirectory_2_clicked()
{
    on_pbChooseDirectory_clicked();
}

void MainWindow::on_MainWindow_destroyed()
{
    midiControls.cancelPlay();
}

void MainWindow::on_pbChooseDirectory_3_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory_3->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lePlayDirectory_3->setText(dir);

}

void MainWindow::on_pb_PlaySlideshow_clicked()
{
    Utils utility;
    QString slideShowCommand("/bin/sh /home/pi/runslideshow.sh ");
    slideShowCommand.append(ui->lePlayDirectory_3->text());
    utility.exec(slideShowCommand.toStdString().c_str());
}
