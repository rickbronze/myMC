#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lePlayFilenameWithPath->hide();
    this->setStyleSheet("background-color: rgb(100, 150, 255);");
    updateStatus("Stopped");
    on_pbStartListening_clicked();
    on_pb_PlaySlideshow_clicked();
    QFileSystemModel *listModel = new QFileSystemModel(this);
    QString directory = ui->lePlayDirectory->text();
    QStringList filters;
    filters <<"*.mid";
    listModel->setNameFilters(filters);
    listModel->setFilter( QDir::NoDotAndDotDot | QDir::Files )  ;
    listModel->sort(0, Qt::SortOrder::DescendingOrder);
    ui->lvRecordingSession->setModel(listModel);
    ui->lvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setModel(listModel);
    ui->tvRecordingSession->setRootIndex(listModel->setRootPath(directory));
    ui->tvRecordingSession->setMinimumWidth(250);
    ui->tabWidget->removeTab(2);
    ui->pbNext->hide();
    ui->pbDelete->hide();
    ui->pb_PlaySlideshow_2->hide();
    ui->pbNextRecording->hide();
    ui->pbStartListening->setVisible(false);
    QFont stopFont;
    stopFont.setBold(true);
    stopFont.setPointSize(28);
    ui->pbCancelPlay->setFont(stopFont);
    setAttribute(Qt::WA_DeleteOnClose, true);
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
    if (!ui->lePlayDirectory_2->text().endsWith("/")) {
        ui->lePlayDirectory_2->setText(ui->lePlayDirectory_2->text() + "/");
    }

    QString listenFilePrefix(ui->lePlayDirectory_2->text() + ui->leRecordPrefix->text());
    midiControls.startListening(listenFilePrefix);
    updateStatus("Listening for any piano play...\n " + ui->lePlayDirectory_2->text() + ui->leRecordPrefix->text());

}

void MainWindow::on_pbPlayDirectory_clicked()
{
    bool wasListening = false;
    if (mode == LISTENING) {
        wasListening = true;
        midiControls.cancelPlay();
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
    vector<string> filesToPlay;
    cancelFlag=false;
    this->setStyleSheet("background-color: rgb(220, 220, 220);");
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks
                                                 | QFileDialog::ReadOnly);
    this->setStyleSheet("background-color: rgb(50, 100, 255);");
    mode = PLAYING;
    updateStatus("Playing Directory of files in " + dir);
    ui->lePlayDirectory->setText(dir);
    dir.append("/*.mid");
    filesToPlay = midiControls.utility.getDirectoryFiles(dir.toStdString());

    cout << "files to play size is " << filesToPlay.size() << endl;

    while(!filesToPlay.empty()){
        ui->pbNext->show();
        ui->pbDelete->show();
        ui->pb_PlaySlideshow_2->show();
       if (cancelFlag==false) {
           srand(time(NULL));
           unsigned int randFile = (rand() % filesToPlay.size());
           string fileStatus("Playing Directory of files\n");
           fileStatus.append(midiControls.utility.split_string(filesToPlay.at(randFile).c_str(), "/").at(midiControls.utility.split_string(filesToPlay.at(randFile).c_str(), "/").size()-1));
           ui->lePlayFilenameWithPath->clear();
           ui->lePlayFilenameWithPath->setText(filesToPlay.at(randFile).c_str());
           updateStatus(fileStatus.c_str());
           midiControls.playAMIDIFile(filesToPlay.at(randFile).c_str());
           midiControls.playProcess.waitForFinished(-1);
           midiControls.resetMIDI();
           filesToPlay.erase(filesToPlay.begin()+randFile);
       }
        else break;
    }
    cancelFlag = false;
    on_pbCancelPlay_clicked();
    if(wasListening){
        mode = STOPPED;
        on_pbStartListening_clicked();
    }

}


void MainWindow::on_pbPlayFile_clicked()
{

    bool wasListening = false;
    if (mode == LISTENING) {
        wasListening = true;
        midiControls.cancelPlay();
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
    this->setStyleSheet("background-color: rgb(220, 220, 220);");
    QString fileToPlay = QFileDialog::getOpenFileName(this, "Open a MIDI file to play", ui->lePlayDirectory->text(), "MIDI (*.mid), All Files (*.*)");
    this->setStyleSheet("background-color: rgb(50, 100, 255);");
    mode = PLAYING;
    updateStatus("Playing the file:\n" + fileToPlay);
    midiControls.playAMIDIFile(fileToPlay);
    midiControls.playProcess.waitForFinished(-1);
    on_pbCancelPlay_clicked();
    if(wasListening){
        mode = STOPPED;
        on_pbStartListening_clicked();
    }
}

void MainWindow::updateStatus(QString newStatus)
{
    ui->lblStatus->setText(newStatus);
    QApplication::processEvents();

}


void MainWindow::on_pbCancelPlay_clicked()
{
    updateStatus("Stopped");
    mode = STOPPED;
    cancelFlag = true;
    midiControls.cancelPlay();
    on_pbStartListening_clicked();
    ui->pbNext->hide();
    ui->pbDelete->hide();
    ui->pb_PlaySlideshow_2->hide();

}


void MainWindow::updateFileModel(QString fileDirectory)
{
    QFileSystemModel *listModel = new QFileSystemModel(this);
    QString directory = fileDirectory;
    QStringList filters;
    filters <<"*.mid";
    listModel->setNameFilters(filters);
    listModel->setFilter( QDir::NoDotAndDotDot | QDir::Files )  ;
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
//    listModel->setNameFilterDisables(true);
    QStringList filters;
    filters <<"*.mid";
    listModel->setNameFilters(filters);
    listModel->setFilter( QDir::NoDotAndDotDot | QDir::Files )  ;
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
        midiControls.cancelPlay();
        QApplication::processEvents();
    }
    else if (mode != STOPPED) {
        QMessageBox msg(QMessageBox::Information,"Not Stopped", "The MIDI is currently performing an operation. \nPlease hit the Stop button first and try again.", QMessageBox::Ok);
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        msg.setFont(font);
        msg.button(QMessageBox::Ok)->setFont(font);
        msg.exec();
        return;
    }
    QString fileNameWithPath(ui->lePlayDirectory_2->text());
    fileNameWithPath.append("/");
    QModelIndexList templatelist = ui->lvRecordingSession->selectionModel()->selectedIndexes();
    if (templatelist.isEmpty()) // check if has selection
    {
      QMessageBox::warning(this, "Nothing Selected",
                           "A file must be selected first for this operation.  "
                           "\nPlease select a file from the List of files.");
    } else {
       foreach (const QModelIndex &indexLoop, templatelist){
       fileNameWithPath.append(indexLoop.data(Qt::DisplayRole).toString());
       updateStatus("Playing the file " + fileNameWithPath);
       ui->lePlayFilenameWithPath->clear();
       ui->lePlayFilenameWithPath->setText(fileNameWithPath);
       midiControls.playAMIDIFile(fileNameWithPath);
       midiControls.playProcess.waitForFinished(-1);
       updateStatus("Stopped");
       mode = STOPPED;
       }
       if(wasListening){
           on_pbStartListening_clicked();
       }
}
}

void MainWindow::on_pbDeleteRecording_clicked()
{
    QString fileNameWithPath(ui->lePlayDirectory_2->text());
    fileNameWithPath.append("/");
    QModelIndexList templatelist = ui->lvRecordingSession->selectionModel()->selectedIndexes();

    if (templatelist.isEmpty()) // check if has selection
    {
      QMessageBox::warning(this, "Nothing Selected",
                           "A file must be selected first for this operation.  "
                           "\nPlease select a file from the List of files.");
    } else {

    foreach (const QModelIndex &indexLoop, templatelist){
        fileNameWithPath.append(indexLoop.data(Qt::DisplayRole).toString());
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
}       updateFileModel(ui->lePlayDirectory_2->text());

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    updateFileModel(ui->lePlayDirectory_2->text());
}

void MainWindow::on_pbChooseDirectory_clicked()
{
    this->setStyleSheet("background-color: rgb(220, 220, 220);");
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks
                                                    | QFileDialog::ReadOnly);
    this->setStyleSheet("background-color: rgb(50, 100, 255);");
    ui->lePlayDirectory->setText(dir);
}

void MainWindow::on_lePlayDirectory_textChanged(const QString &arg1)
{
    if (!ui->lePlayDirectory->text().endsWith("/")) {
        ui->lePlayDirectory->setText(ui->lePlayDirectory->text() + "/");
    }

//    ui->lePlayDirectory_2->setText(ui->lePlayDirectory->text());
}

void MainWindow::on_pbChooseDirectory_2_clicked()
{
    this->setStyleSheet("background-color: rgb(220, 220, 220);");
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks
                                                    | QFileDialog::ReadOnly);
    this->setStyleSheet("background-color: rgb(50, 100, 255);");
    ui->lePlayDirectory_2->setText(dir);
    updateFileModel(ui->lePlayDirectory_2->text());
}

void MainWindow::on_MainWindow_destroyed()
{
    midiControls.cancelFlag = true;
    midiControls.cancelPlay();
}

void MainWindow::on_pbChooseDirectory_3_clicked()
{
    this->setStyleSheet("background-color: rgb(220, 220, 220);");
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lePlayDirectory_3->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks
                                                    | QFileDialog::ReadOnly);

    this->setStyleSheet("background-color: rgb(50, 100, 255);");
    ui->lePlayDirectory_3->setText(dir);

}

void MainWindow::on_pb_PlaySlideshow_clicked()
{
    QThread screenSaverThread;
    Utils utility;
    QString slideShowCommand("/bin/sh /home/pi/runslideshow.sh ");
    slideShowCommand.append(ui->lePlayDirectory_3->text());
    slideShowCommand.append(" &");
    utility.exec(slideShowCommand.toStdString().c_str());
}

void MainWindow::on_pbNext_clicked()
{
    midiControls.playProcess.kill();
    //midiControls.playNext();

}

void MainWindow::on_pbRenameRecording_clicked()
{
    QString fileNameWithPath(ui->lePlayDirectory_2->text());
    fileNameWithPath.append("/");
    QString newFileNameWithPath(ui->lePlayDirectory_2->text());
    newFileNameWithPath.append("/");
    QModelIndexList templatelist = ui->lvRecordingSession->selectionModel()->selectedIndexes();

    if (templatelist.isEmpty()) // check if has selection
    {
      QMessageBox::warning(this, "Nothing Selected",
                           "A file must be selected first for this operation.  "
                           "\nPlease select a file from the List of files.");
    } else {

    foreach (const QModelIndex &indexLoop, templatelist){
        fileNameWithPath.append(indexLoop.data(Qt::DisplayRole).toString());
          QMessageBox::StandardButton reply;
          reply = QMessageBox::question(
              this, "Renaming File",
              "Renaming file " +
                  fileNameWithPath +
                  "?",
              QMessageBox::Yes | QMessageBox::No);
          if (reply == QMessageBox::Yes) {
              QString filter("MIDI (*.mid)");
              this->setStyleSheet("background-color: rgb(220, 220, 220);");
              QString saveFileName = QFileDialog::getSaveFileName(this, "Rename file to", ui->lePlayDirectory_2->text(), filter, &filter);
              if (!saveFileName.endsWith(".mid"))
                  saveFileName += ".mid";
              this->setStyleSheet("background-color: rgb(50, 100, 255);");
              QString mvCommand("mv ");
       mvCommand.append(fileNameWithPath);
       mvCommand.append(" ");
       mvCommand.append(saveFileName);
       QProcess mvProcess;
       cout << "mv command is " << mvCommand.toStdString() << endl;
       mvProcess.startDetached(mvCommand);
          }}
}       updateFileModel(ui->lePlayDirectory_2->text());


}

void MainWindow::on_lePlayDirectory_2_textChanged(const QString &arg1)
{
    if (!ui->lePlayDirectory_2->text().endsWith("/")) {
        ui->lePlayDirectory_2->setText(ui->lePlayDirectory_2->text() + "/");
    }
    on_pbCancelPlay_clicked();

}

void MainWindow::on_pbExit_clicked()
{
    on_pbCancelPlay_clicked();
    exit(0);
}

void MainWindow::on_pbNextRecording_clicked()
{
//    on_pbCancelPlay_clicked();
selectNextRow(ui->lvRecordingSession);
    //  ui->lvRecordingSession ui->lvRecordingSession->currentIndex().row()+1;
}
void MainWindow::selectNextRow( QListView *view )
{
    QItemSelectionModel *selectionModel = view->selectionModel();
    int row = -1;
    if ( selectionModel->hasSelection() )
        row = selectionModel->selection().first().indexes().first().row();
    int rowcount = view->model()->rowCount();
    row = (row + 1 ) % rowcount;
    QModelIndex newIndex = view->model()->index(row, 0);
    selectionModel->select( newIndex, QItemSelectionModel::ClearAndSelect );
}

void MainWindow::on_pb_PlaySlideshow_2_clicked()
{
    on_pb_PlaySlideshow_clicked();
}

void MainWindow::on_pbDelete_clicked()
{
    QString fileNameWithPath;
    fileNameWithPath.append(ui->lePlayFilenameWithPath->text());
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
   midiControls.playProcess.kill();
      }
}

void MainWindow::on_pbNetworkPics_clicked()
{
QString mountCommand("sudo mount -t cifs //nas-pi/SimanTovPictures /mnt/spics -o username=anonymous,password=,file_mode=0775,dir_mode=0775");
QProcess mountProcess;
cout << "mount command is " << mountCommand.toStdString() << endl;
mountProcess.start(mountCommand);
if (ui->cbPictureSetList->currentIndex()==0){
        ui->lePlayDirectory_3->setText( "/mnt/spics/0Sand Art/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==1)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/2025 Family Pics/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==2)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/2025 Family Pics/2025GreeceTrip");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==3)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/0Simi/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==4)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/0-JeffKirsh/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==5)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/2024 Family Pics/2024ThailandTrip/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}
else if (ui->cbPictureSetList->currentIndex()==6)
{
        ui->lePlayDirectory_3->setText( "/mnt/spics/2024 Family Pics/2024HotAirBalloon/");
        ui->lePlayDirectory_3->setText('"' + ui->lePlayDirectory_3->text() + '"');
        ui->lePlayDirectory_3->text().append("""");
}

on_pb_PlaySlideshow_clicked();

}
