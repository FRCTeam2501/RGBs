#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileinteract.h"
#include "rgbcode.h"
#include <QDebug>
#include <string>
#include <cstring>
#include <array>

static RGB rgb;
static fileInteract file;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->pushButton,SIGNAL(on_pushButton_clicked()),rgb,SLOT(rgb.SetAll(COLORS::GREEN)));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this, SLOT(Tick()));

    playListTimer = new QTimer(this);
    connect(playListTimer,SIGNAL(timeout()), this, SLOT(ProcessPlayListTimeOut()));
   // timer->start(2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("goodbye from jacob");
    cout<<"test"<<endl;
    rgb.SetAll(0xFF0000);
    rgb.Render();
}

void MainWindow::Test(){
    rgb.SetAll(0x00FF00);
    rgb.Render();
}

void MainWindow::on_pushButton_2_clicked()
{
    Clear();
   // uint8_t red, green, blue;
   // red = ui->lcdNumber->numDigits();
   // green = ui->lcdNumber_2->numDigits();
   // blue = ui->lcdNumber_3->numDigits();

  rgbTemporarySet.red = (uint8_t)ui->lcdNumber->intValue();
  rgbTemporarySet.green = (uint8_t)ui->lcdNumber_2->intValue();
  rgbTemporarySet.blue = (uint8_t)ui->lcdNumber_3->intValue();

    rgb.SetAll(rgbTemporarySet.color);
    cout<<rgbTemporarySet.red<<endl;
    rgb.Render();
    qDebug()<<"b";
}
void MainWindow::DoTicking(bool tickingActive){
    if(tickingActive){
        timer->start(50);
       // cout<<"timer Start"<<endl;
    }else{
        timer->stop();
    }
}

void MainWindow::Tick(){
    rgb.TickTracker();
   // cout<<"Tick"<<endl;
}

void MainWindow::Clear(){
    rgb.DoTicking = false;
    rgb.doEvent = false;
    DoTicking(false);
    rgb.gradualPremited = false;
    for(int r = 0; r<COLORS::numberOfLeds;r++){
        rgb.LEDMap[r].doingGradual = false;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    rgb.switcher = 1;
    rgb.gradualPremited = false;
    for(int r = 0; r<COLORS::numberOfLeds;r++){
        rgb.LEDMap[r].doingGradual = false;
    }
    if(barhold[1].barLength != 0){
    int renderTracker = 0;//number of leds set
    int barTracker = 1;//what bar we are on
    int barNumberTracker = 1;//led number within bar that we are on

    //qDebug()<<"a";
    //usleep(300000);
    while(renderTracker < COLORS::numberOfLeds){
         qDebug()<<"a";
       //  usleep(300000);
        rgb.LEDMap[renderTracker].red = barhold[barTracker].red;
        rgb.LEDMap[renderTracker].green = barhold[barTracker].green;
        rgb.LEDMap[renderTracker].blue = barhold[barTracker].blue;

        renderTracker++;
        if(barNumberTracker == barhold[barTracker].barLength){
            barNumberTracker = 1;
             qDebug()<<"";
           //  usleep(300000);
             if(barhold[barTracker+1].barLength != 0){
                 barTracker++;
                  qDebug()<<"c";
                //  usleep(300000);
             }else{
                 barTracker = 1;
                  qDebug()<<"d";
                 // usleep(300000);
             }
        }else{
            barNumberTracker++;

        }
    }

    // qDebug()<<"b";
    // usleep(300000);
    rgb.DoTicking = true;
    rgb.doEvent = true;
    rgb.barMoveForward = ui->checkBox->isChecked();
    // qDebug()<<"c";
   //  usleep(300000);
    rgb.eventTickTime = ui->spinBox_4->value();
   //  qDebug()<<"d";
    // usleep(300000);
    rgb.barMoveJumbDist = ui->spinBox_3->value();
   //  qDebug()<<"e";
    // usleep(300000);
    rgb.RenderArray();
   //  qDebug()<<"f";
   //  usleep(300000);
    DoTicking(true);
   //  qDebug()<<"g";
   //  usleep(300000);
   // rgb.TickTracker();
     //qDebug()<<"h";
     //usleep(300000);
    qDebug()<<"done";
}
}


string MainWindow::SolidSetFile(int saveID){

    string saveString;
    //saveString += ":";
    saveString += to_string(saveID);
    file.saveRun[saveID].ID = saveID;
    saveString += ":";
    saveString += "'1'";
    file.saveRun[saveID].designType = 1;
    saveString += ".0.";
    file.saveRun[saveID].ticksBetweenMoves = 0;
    saveString += "|0|";
    file.saveRun[saveID].jumpDist = 0;
    saveString += "<0<";
    file.saveRun[saveID].forwardDirection = 0;

    saveString += "{";
    if(ui->lcdNumber->intValue() < 100){
        saveString += "0";
    }
    if(ui->lcdNumber->intValue() < 10){
        saveString += "0";
    }
    saveString += to_string(ui->lcdNumber->intValue());
    file.saveRun[saveID].colorStore[0].red = ui->lcdNumber->intValue();
    saveString += ",";

    if(ui->lcdNumber_2->intValue() < 100){
        saveString += "0";
    }
    if(ui->lcdNumber_2->intValue() < 10){
        saveString += "0";
    }

    saveString += to_string(ui->lcdNumber_2->intValue());
    file.saveRun[saveID].colorStore[0].green = ui->lcdNumber_2->intValue();
    saveString += ",";

    if(ui->lcdNumber_3->intValue() < 100){
        saveString += "0";
    }
    if(ui->lcdNumber_3->intValue() < 10){
        saveString += "0";
    }
    saveString += to_string(ui->lcdNumber_3->intValue());
    file.saveRun[saveID].colorStore[0].blue = ui->lcdNumber_3->intValue();

    saveString += "^0000*0000!0000}"; //^tickInterval*ticksBeforeAdded!waveStartNumber <- this is all for the wave, the stroe format will be difrent for the color block
    file.saveRun[saveID].colorStore[0].tiksBeforeAdded = NULL;
    file.saveRun[saveID].colorStore[0].waveStartNumber = NULL;
    file.saveRun[saveID].colorStore[0].tickChangeInterval = NULL;
    cout<<saveString<<endl;
    return saveString;
}

string MainWindow::BarStringCraft(int saveID){
    cout<<"BarStringCraft: "<<"A";
    string saveString;
    //saveString += ":";
    saveString += to_string(saveID);
    file.saveRun[saveID].ID = saveID;
    saveString += ":";//end of saveID
    saveString += "'2'";//desing ID
    file.saveRun[saveID].designType = 2;
    saveString += ".";//start of ticks between moves
    saveString += to_string(ui->spinBox_4->value());
    saveString += ".";
    file.saveRun[saveID].ticksBetweenMoves = ui->spinBox_4->value();
    saveString += "|";
    saveString += to_string(ui->spinBox_3->value());
    saveString += "|";
    file.saveRun[saveID].jumpDist = ui->spinBox_3->value();
    saveString += "<";
    saveString += to_string(ui->checkBox->isChecked());
    saveString += "<";
    file.saveRun[saveID].forwardDirection = ui->checkBox->isChecked();
    cout<<"B";

    int barNumberTracker = 1;

    while(barhold[barNumberTracker].barLength > 0){
    saveString += "{";
    if(barhold[barNumberTracker].red < 100){
        saveString += "0";
    }
    if(barhold[barNumberTracker].red < 10){
        saveString += "0";
    }
    saveString += to_string(barhold[barNumberTracker].red);
    file.saveRun[saveID].colorStore[barNumberTracker].red = barhold[barNumberTracker].red;
    saveString += ",";

    if(barhold[barNumberTracker].green < 100){
        saveString += "0";
    }
    if(barhold[barNumberTracker].green < 10){
        saveString += "0";
    }
    saveString += to_string(barhold[barNumberTracker].green);
    file.saveRun[saveID].colorStore[barNumberTracker].green = barhold[barNumberTracker].green;
    saveString += ",";

    if(barhold[barNumberTracker].blue < 100){
        saveString += "0";
    }
    if(barhold[barNumberTracker].blue < 10){
        saveString += "0";
    }
    saveString += to_string(barhold[barNumberTracker].blue);
    file.saveRun[saveID].colorStore[barNumberTracker].blue = barhold[barNumberTracker].blue;
    saveString += "^";

    if(barhold[barNumberTracker].barLength < 1000){
        saveString += "0";
    }
    if(barhold[barNumberTracker].barLength < 100){
        saveString += "0";
    }
    if(barhold[barNumberTracker].barLength < 10){
        saveString += "0";
    }
     saveString += to_string(barhold[barNumberTracker].barLength);
     file.saveRun[saveID].colorStore[barNumberTracker].tickChangeInterval = barhold[barNumberTracker].barLength;//tick ChangeInterval is also used to keep bar length

    saveString += "*0000!0000}"; //^tickInterval*ticksBeforeAdded!waveStartNumber <- this is all for the wave, the stroe format will be difrent for the color block
    file.saveRun[saveID].colorStore[0].waveStartNumber = NULL;
    file.saveRun[saveID].colorStore[0].tiksBeforeAdded = NULL;
    barNumberTracker++;
    }
    cout<<"C";

    return saveString;
}

string MainWindow::WaveStringCraft(int saveID){
    string saveString;
    //saveString += ":";
    saveString += to_string(saveID);
    file.saveRun[saveID].ID = saveID;
    saveString += ":";//end of saveID
    saveString += "'3'";//desing ID
    file.saveRun[saveID].designType = 3;
    saveString += ".";//start of ticks between moves
    saveString += to_string(ui->WaveTicksBetweenMove->value());
    saveString += ".";
    file.saveRun[saveID].ticksBetweenMoves = ui->WaveTicksBetweenMove->value();
    saveString += "|";
    saveString += to_string(ui->WaveJumpDist->value());
    saveString += "|";
    file.saveRun[saveID].jumpDist = ui->WaveJumpDist->value();
    saveString += "<";
    saveString += to_string(ui->WaveDirection->isChecked());
    saveString += "<";
    file.saveRun[saveID].forwardDirection = ui->WaveDirection->isChecked();

    int waveNumberTracker = 0;

    while(WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval > 0){
    saveString += "{";
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetRed < 100){
        saveString += "0";
    }
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetRed < 10){
        saveString += "0";
    }
    saveString += to_string(WaveListTemp[waveNumberTracker].waveSeedInfo.targetRed);
    file.saveRun[saveID].colorStore[waveNumberTracker].red = WaveListTemp[waveNumberTracker].waveSeedInfo.targetRed;
    saveString += ",";

    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetGreen < 100){
        saveString += "0";
    }
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetGreen < 10){
        saveString += "0";
    }
    saveString += to_string(WaveListTemp[waveNumberTracker].waveSeedInfo.targetGreen);
    file.saveRun[saveID].colorStore[waveNumberTracker].green = WaveListTemp[waveNumberTracker].waveSeedInfo.targetGreen;
    saveString += ",";

    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetBlue < 100){
        saveString += "0";
    }
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.targetBlue < 10){
        saveString += "0";
    }
    saveString += to_string(WaveListTemp[waveNumberTracker].waveSeedInfo.targetBlue);
    file.saveRun[saveID].colorStore[waveNumberTracker].blue = WaveListTemp[waveNumberTracker].waveSeedInfo.targetBlue;
    saveString += "^";

    if(WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval < 1000){
        saveString += "0";
    }
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval < 100){
        saveString += "0";
    }
    if(WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval < 10){
        saveString += "0";
    }
     saveString += to_string(WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval);
     file.saveRun[saveID].colorStore[waveNumberTracker].tickChangeInterval = WaveListTemp[waveNumberTracker].waveSeedInfo.tickInterval;
     saveString += "*";


     if(WaveListTemp[waveNumberTracker].ticksBeforeNextWave < 1000){
         saveString += "0";
     }
     if(WaveListTemp[waveNumberTracker].ticksBeforeNextWave < 100){
         saveString += "0";
     }
     if(WaveListTemp[waveNumberTracker].ticksBeforeNextWave < 10){
         saveString += "0";
     }
      saveString += to_string(WaveListTemp[waveNumberTracker].ticksBeforeNextWave);
      file.saveRun[saveID].colorStore[waveNumberTracker].tiksBeforeAdded = WaveListTemp[waveNumberTracker].ticksBeforeNextWave;
      saveString += "!";

      if(WaveListTemp[waveNumberTracker].StartingPlace < 1000){
          saveString += "0";
      }
      if(WaveListTemp[waveNumberTracker].StartingPlace < 100){
          saveString += "0";
      }
      if(WaveListTemp[waveNumberTracker].StartingPlace < 10){
          saveString += "0";
      }
       saveString += to_string(WaveListTemp[waveNumberTracker].StartingPlace);
       file.saveRun[saveID].colorStore[waveNumberTracker].waveStartNumber = WaveListTemp[waveNumberTracker].StartingPlace;
       saveString += "}";


    waveNumberTracker++;
    }

    return saveString;
}

string MainWindow::PlayListStringCraft(int saveID){

    string saveString;
    //saveString += ":";
    saveString += to_string(saveID);
    file.playRun[saveID].PlayID = saveID;
    saveString += ":";//end of saveID

    if(ui->PlayListLoopCkeck->isChecked()){
        saveString += "'T'";
        file.playRun[saveID].loop = true;
    }else{
       saveString += "'F'";
       file.playRun[saveID].loop = false;
    }

    int playCounter = 0;

    while(file.playListTemp.tickCount[playCounter] > 0){
    saveString += "[";
    saveString += to_string(file.playListTemp.designID[playCounter]);
    file.playRun[saveID].designID[playCounter] = file.playListTemp.designID[playCounter];
    saveString += "]";
    saveString += "{";
    saveString += to_string(file.playListTemp.tickCount[playCounter]);
    file.playRun[saveID].tickCount[playCounter] = file.playListTemp.tickCount[playCounter];
    saveString += "}";
    playCounter++;
    }
    return saveString;
}


void MainWindow::on_pushButton_4_clicked(){

}

void MainWindow::on_PushButtonAddBar_clicked(){
    QString str;

    str += "R: ";
    str += QString::number(ui->lcdNumber_4->intValue());
    str += " G: ";
    str += QString::number(ui->lcdNumber_5->intValue());
    str += " B: ";
    str += QString::number(ui->lcdNumber_6->intValue());
    str += " L: ";
    str += QString::number(ui->spinBox->value());
    ui->BarListWidget->addItem(str);
    barhold[barNumber].red = ui->lcdNumber_4->intValue();
    barhold[barNumber].green = ui->lcdNumber_5->intValue();
    barhold[barNumber].blue = ui->lcdNumber_6->intValue();
    barhold[barNumber].barLength = ui->spinBox->value();

    barNumber ++;
}

void MainWindow::on_ButtonBarClearList_clicked()
{
    //memset(barhold,sizeof(barhold));
    //fill_n(barhold->red,500,NULL);
    for(int i = 0;i<500;i++){
        barhold[i].red = NULL;
        barhold[i].green = NULL;
        barhold[i].blue = NULL;
        barhold[i].barLength = NULL;
    }
    ui->BarListWidget->clear();
    barNumber = 1;
}

void MainWindow::WaveInit(){
//as of right now this function is never called
     qDebug()<<"TESSSSTTTTINGGGGGGGGA::::"<<rgb.gradualPremited;
    for(int y; y<300;y++){
       rgb.waveSeedList[y].waveSeedInfo.colorTarget = WaveListTemp[y].waveSeedInfo.colorTarget;
       rgb.waveSeedList[y].waveSeedInfo.tickInterval = WaveListTemp[y].waveSeedInfo.tickInterval;
       rgb.waveSeedList[y].StartingPlace = WaveListTemp[y].StartingPlace;
       rgb.waveSeedList[y].ticksBeforeNextWave = WaveListTemp[y].ticksBeforeNextWave;
    }
    rgb.waveMoveForward = ui->WaveDirection->isChecked();;//set to check box output
    rgb.waveMoveDistance = ui->WaveJumpDist->value();//set to ticker box
    rgb.eventTickTime = ui->WaveTicksBetweenMove->value();
    rgb.switcher = 2;

}

void MainWindow::on_WaveAddElement_clicked()
{
    QString str;

    str += "R: ";
    str += QString::number(ui->WaveRedDisplay->intValue());
    str += " G: ";
    str += QString::number(ui->WaveGreenDisplay->intValue());
    str += " B: ";
    str += QString::number(ui->WaveBlueDisplay->intValue());
    str += " T: ";
    str += QString::number(ui->WaveTickChange->value());
    str += " TTNW: ";
    str += QString::number(ui->WaveTicksToNextAdd->value());
    str += " S: ";
    str += QString::number(ui->WaveStartPlace->value());
    ui->WaveListWidget->addItem(str);
    WaveListTemp[waveNumber].waveSeedInfo.targetRed = ui->WaveRedDisplay->intValue();
    WaveListTemp[waveNumber].waveSeedInfo.targetGreen = ui->WaveGreenDisplay->intValue();
    WaveListTemp[waveNumber].waveSeedInfo.targetBlue = ui->WaveBlueDisplay->intValue();
    WaveListTemp[waveNumber].StartingPlace = ui->WaveStartPlace->value();
    WaveListTemp[waveNumber].waveSeedInfo.tickInterval = ui->WaveTickChange->value();
    WaveListTemp[waveNumber].ticksBeforeNextWave = ui->WaveTicksToNextAdd->value();

    waveNumber++;
}

void MainWindow::on_WaveClearList_clicked()
{
    for(int i = 0;i<300;i++){
        WaveListTemp[i].waveSeedInfo.targetRed = NULL;
        WaveListTemp[i].waveSeedInfo.targetGreen = NULL;
        WaveListTemp[i].waveSeedInfo.targetBlue = NULL;
        WaveListTemp[i].StartingPlace = NULL;
        WaveListTemp[i].ticksBeforeNextWave = NULL;
    }
    ui->WaveListWidget->clear();
    waveNumber = 0;
}

void MainWindow::on_WaveRender_clicked()
{
    rgb.gradualPremited = true;
    qDebug()<<"a";
  //  usleep(300000);
    for(int y = 0; y<300;y++){

       rgb.waveSeedList[y].waveSeedInfo.targetRed = WaveListTemp[y].waveSeedInfo.targetRed;
       //qDebug()<<"c";
       //usleep(300000);
       rgb.waveSeedList[y].waveSeedInfo.targetGreen = WaveListTemp[y].waveSeedInfo.targetGreen;
       rgb.waveSeedList[y].waveSeedInfo.targetBlue = WaveListTemp[y].waveSeedInfo.targetBlue;
       rgb.waveSeedList[y].waveSeedInfo.tickInterval = WaveListTemp[y].waveSeedInfo.tickInterval;
       rgb.waveSeedList[y].StartingPlace = WaveListTemp[y].StartingPlace;
       rgb.waveSeedList[y].ticksBeforeNextWave = WaveListTemp[y].ticksBeforeNextWave;
    }
      qDebug()<<"b";
   //   usleep(800000);
    rgb.waveMoveForward = ui->WaveDirection->isChecked();//set to check box output
    rgb.waveMoveDistance = ui->WaveJumpDist->value();//set to ticker box
    rgb.eventTickTime = ui->WaveTicksBetweenMove->value();
    rgb.switcher = 2;
    rgb.DoTicking = true;
    rgb.doEvent = false;


  //  rgb.RenderArray();
    qDebug()<<"c";
   // usleep(800000);
    DoTicking(true);

    qDebug()<<"d";
   // usleep(800000);

   // rgb.TickTracker();
    qDebug()<<"e";
  //  usleep(800000);
    DoTicking(true);
}

void MainWindow::on_SolidSaveStateButton_clicked()
{
    file.FindNextOpenFilesSlot();
    cout<<"first open:"<<file.saveStateFirstLineOpen<<endl;
    file.AddSaveStateLine(file.saveStateFirstLineOpen,SolidSetFile(file.saveStateFirstLineOpen));
    //file.FindNextOpenFilesSlot();
}

void MainWindow::on_BarSaveStateButton_clicked()
{
    file.FindNextOpenFilesSlot();
    cout<<"first open:"<<file.saveStateFirstLineOpen<<endl;
    file.AddSaveStateLine(file.saveStateFirstLineOpen,BarStringCraft(file.saveStateFirstLineOpen));
    //file.FindNextOpenFilesSlot();
}


void MainWindow::on_WaveSaveStateButton_clicked()
{
    file.FindNextOpenFilesSlot();
    cout<<"first open:"<<file.saveStateFirstLineOpen<<endl;
    file.AddSaveStateLine(file.saveStateFirstLineOpen,WaveStringCraft(file.saveStateFirstLineOpen));
}

void MainWindow::on_SaveStateDeleteButton_clicked()
{
    int deleteID = ui->SaveStateDeleteSpin->value();

    file.saveRun[deleteID].ID = NULL;
    file.saveRun[deleteID].jumpDist = NULL;
    file.saveRun[deleteID].forwardDirection = NULL;
    file.saveRun[deleteID].designType = NULL;
    file.saveRun[deleteID].ticksBetweenMoves = NULL;
    for(int t = 0; t< 300; t++){
        file.saveRun[deleteID].colorStore[t].red = NULL;
        file.saveRun[deleteID].colorStore[t].green = NULL;
        file.saveRun[deleteID].colorStore[t].blue = NULL;
    }
    file.DeleteSaveStatesLineID(deleteID);
}

void MainWindow::RenderSaveList(){
    QString lineString;

    for(int u = 0; u < 500; u ++){
        if(file.saveRun[u].ID > 0){
            lineString.clear();

            if(u < 100){
                lineString += "0";
            }
            if(u < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(u));
            lineString += ": ";
            lineString += "ID: ";
            lineString += QString::fromStdString(to_string(file.saveRun[u].designType));
            lineString += " TBM: ";
            lineString += QString::fromStdString(to_string(file.saveRun[u].ticksBetweenMoves));
            lineString += " JD: ";
            lineString += QString::fromStdString(to_string(file.saveRun[u].jumpDist));
            lineString += " DIR: ";
            if(file.saveRun[u].forwardDirection){
                lineString += "F ";
            }else{
                lineString += "R ";
            }

            if(file.saveRun[u].designType == 1){
            lineString += "{";

            if(file.saveRun[u].colorStore[0].red < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[0].red < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[0].red));
            lineString += ",";

            if(file.saveRun[u].colorStore[0].green < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[0].green < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[0].green));
            lineString += ",";
            if(file.saveRun[u].colorStore[0].blue < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[0].blue < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[0].blue));
            lineString += "}";
            }
            int saveNumberTracker;

         if(file.saveRun[u].designType == 2 || file.saveRun[u].designType == 3){
             cout<<"type 2 or 3"<<endl;
             if(file.saveRun[u].designType == 2){//becasue I wrote the bar code to start the array at one because I dumb
                saveNumberTracker = 1;
             }else{
                saveNumberTracker = 1;
             }
             cout<<file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval<<endl;
             cout<<u<<endl;
             cout<<file.saveRun[u].colorStore[0].tickChangeInterval<<endl;
            while(file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval > 0){
                cout<<"loop list"<<endl;
            lineString += "{";

            if(file.saveRun[u].colorStore[saveNumberTracker].red < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[saveNumberTracker].red < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].red));
            lineString += ", ";

            if(file.saveRun[u].colorStore[saveNumberTracker].green < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[saveNumberTracker].green < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].green));
            lineString += ", ";

            if(file.saveRun[u].colorStore[saveNumberTracker].blue < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[saveNumberTracker].blue < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].blue));

            if(file.saveRun[u].designType == 2){
                lineString += " L: ";
            }else{
                lineString += " TI: ";
            }

            if(file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval < 1000){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval < 100){
                lineString += "0";
            }
            if(file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].tickChangeInterval));

            if(file.saveRun[u].designType == 3){
                lineString += " TBA: ";

                if(file.saveRun[u].colorStore[saveNumberTracker].tiksBeforeAdded < 1000){
                    lineString += "0";
                }
                if(file.saveRun[u].colorStore[saveNumberTracker].tiksBeforeAdded < 100){
                    lineString += "0";
                }
                if(file.saveRun[u].colorStore[saveNumberTracker].tiksBeforeAdded < 10){
                    lineString += "0";
                }
                lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].tiksBeforeAdded));
                lineString += " WS: ";

                if(file.saveRun[u].colorStore[saveNumberTracker].waveStartNumber < 1000){
                    lineString += "0";
                }
                if(file.saveRun[u].colorStore[saveNumberTracker].waveStartNumber < 100){
                    lineString += "0";
                }
                if(file.saveRun[u].colorStore[saveNumberTracker].waveStartNumber < 10){
                    lineString += "0";
                }
                lineString += QString::fromStdString(to_string(file.saveRun[u].colorStore[saveNumberTracker].waveStartNumber));

        }
            lineString += "}";
            saveNumberTracker++;

            }
        }
         ui->SaveStateListView->addItem(lineString);
         ui->PlaySaveOptionsList->addItem(lineString);
        }
        }
    }

void MainWindow::RenderPlayLists(){
    QString lineString;

    for(int u = 0; u < 500; u ++){
        if(file.playRun[u].tickCount[0] > 0){

            lineString.clear();

            if(u < 100){
                lineString += "0";
            }
            if(u < 10){
                lineString += "0";
            }
            lineString += QString::fromStdString(to_string(u));
            lineString += ": ";

            if(file.playRun[u].loop){
                lineString += "'T'";
            }else{
                lineString += "'F'";
            }

            for(int i = 0; i<300;i++){
                if(file.playRun[u].tickCount[i] > 0){
                    cout<<"U: "<<u<<" I: "<<i<<" design: "<<file.playRun[u].designID[i]<<" tick count: "<<file.playRun[u].tickCount[i]<<endl;
                    lineString += " (SID: ";
                    lineString += QString::fromStdString(to_string(file.playRun[u].designID[i]));
                    lineString += " TC: ";
                    lineString += QString::fromStdString(to_string(file.playRun[u].tickCount[i]));
                    lineString += ")";
                }
        }
            ui->PlayListListView->addItem(lineString);
        }
}
}

void MainWindow::on_SaveStateViewerUpdateButton_clicked()
{
    file.ReadSaveStatesFile();
    ui->SaveStateListView->clear();
    RenderSaveList();
}

void MainWindow::SaveStateRender(int saveID){

    if(file.saveRun[saveID].designType == 1){
        Clear();

      rgbTemporarySet.red = file.saveRun[saveID].colorStore[0].red;
      rgbTemporarySet.green = file.saveRun[saveID].colorStore[0].green;
      rgbTemporarySet.blue = file.saveRun[saveID].colorStore[0].blue;

        rgb.SetAll(rgbTemporarySet.color);
       // cout<<rgbTemporarySet.red<<endl;
        rgb.Render();
    }

    if(file.saveRun[saveID].designType == 2){//you should probably put this in a function
    rgb.switcher = 1;
    rgb.gradualPremited = false;
    for(int r = 0; r<COLORS::numberOfLeds;r++){
        rgb.LEDMap[r].doingGradual = false;
    }
    if(file.saveRun[saveID].colorStore[1].tickChangeInterval != 0){
    int renderTracker = 0;//number of leds set
    int barTracker = 1;//what bar we are on
    int barNumberTracker = 1;//led number within bar that we are on

    while(renderTracker < COLORS::numberOfLeds){
         qDebug()<<"a";
         //usleep(300000);
        rgb.LEDMap[renderTracker].red = file.saveRun[saveID].colorStore[barTracker].red;
        rgb.LEDMap[renderTracker].green = file.saveRun[saveID].colorStore[barTracker].green;
        rgb.LEDMap[renderTracker].blue = file.saveRun[saveID].colorStore[barTracker].blue;

        renderTracker++;
        cout<<"bar Length: "<<file.saveRun[saveID].colorStore[barTracker].tickChangeInterval<<endl;
        if(barNumberTracker == file.saveRun[saveID].colorStore[barTracker].tickChangeInterval){//tick change interval is used as the bar length for the bar design
            barNumberTracker = 1;
             qDebug()<<"b";
             //usleep(300000);
             if(file.saveRun[saveID].colorStore[barTracker+1].tickChangeInterval != 0){
                 barTracker++;
                  qDebug()<<"c";
                //  usleep(300000);
             }else{
                 barTracker = 1;
                  qDebug()<<"d";
                 // usleep(300000);
             }
        }else{
            qDebug()<<"e";
            cout<<barNumberTracker<<endl;
            barNumberTracker++;
        }
    }

    rgb.DoTicking = true;
    rgb.doEvent = true;
    rgb.barMoveForward = file.saveRun[saveID].forwardDirection;
    rgb.eventTickTime = file.saveRun[saveID].ticksBetweenMoves;
    rgb.barMoveJumbDist = file.saveRun[saveID].jumpDist;
    rgb.RenderArray();
    DoTicking(true);
    }
    }

    if(file.saveRun[saveID].designType == 3){
        rgb.gradualPremited = true;
        qDebug()<<"a";
      //  usleep(300000);
        for(int y = 0; y<300;y++){

           rgb.waveSeedList[y].waveSeedInfo.targetRed = file.saveRun[saveID].colorStore[y].red;
           //qDebug()<<"c";
           //usleep(300000);
           rgb.waveSeedList[y].waveSeedInfo.targetGreen = file.saveRun[saveID].colorStore[y].green;
           rgb.waveSeedList[y].waveSeedInfo.targetBlue = file.saveRun[saveID].colorStore[y].blue;
           rgb.waveSeedList[y].waveSeedInfo.tickInterval = file.saveRun[saveID].colorStore[y].tickChangeInterval;
           rgb.waveSeedList[y].StartingPlace = file.saveRun[saveID].colorStore[y].waveStartNumber;
           rgb.waveSeedList[y].ticksBeforeNextWave = file.saveRun[saveID].colorStore[y].tiksBeforeAdded;
        }
          qDebug()<<"b";
       //   usleep(800000);
        rgb.waveMoveForward = file.saveRun[saveID].forwardDirection;
        rgb.waveMoveDistance = file.saveRun[saveID].jumpDist;//set to ticker box
        rgb.eventTickTime = file.saveRun[saveID].ticksBetweenMoves;
        rgb.switcher = 2;
        rgb.DoTicking = true;
        rgb.doEvent = false;


      //  rgb.RenderArray();
        qDebug()<<"c";
       // usleep(800000);
        DoTicking(true);

        qDebug()<<"d";
       // usleep(800000);

       // rgb.TickTracker();
        qDebug()<<"e";
      //  usleep(800000);
        DoTicking(true);
    }
}

void MainWindow::on_SaveStateRenderButton_clicked()
{
    int saveID = ui->SaveStateRenderSpin->value();
    SaveStateRender(saveID);
}

void MainWindow::PlayListInit(int playListInitID){
    cout<<"test"<<endl;
    if(file.playRun[playListInitID].tickCount[0] > 0){
        cout<<"play init"<<endl;
        playListPlayNumber = playListInitID;
        playListPlayDesign = 0;
        cout<<"playNumber: "<<playListPlayNumber<<endl;
        cout<<"play desigh: "<<playListPlayDesign<<endl;

        SaveStateRender(file.playRun[playListPlayNumber].designID[playListPlayDesign]);
       playListTimer->start(file.playRun[playListPlayNumber].tickCount[playListPlayDesign] * 50);

    }
}

void MainWindow::ProcessPlayListTimeOut(){
    cout<<"playlist Timeout"<<endl;
    bool doLoop = true;
    while(doLoop){
    if(file.playRun[playListPlayNumber].tickCount[playListPlayDesign + 1] > 0){
        cout<<"a ";
        playListTimer->stop();

        playListPlayDesign ++;
        cout<<"b ";


        if(file.saveRun[file.playRun[playListPlayNumber].designID[playListPlayDesign]].designType != 0){
            cout<<"c ";
            SaveStateRender(file.playRun[playListPlayNumber].designID[playListPlayDesign]);
           playListTimer->start(file.playRun[playListPlayNumber].tickCount[playListPlayDesign] * 50);//chenck to see what wnit of time this takes
           doLoop = false;
        }else{
            doLoop = true;
        }

        //cancel old design >done
        //play new design >done
        //set timer to new timeout >done

    }else if(file.playRun[playListPlayNumber].loop == true){
cout<<"d ";
        playListTimer->stop();
        //cancel old design
        playListPlayDesign = 0;
        if(file.saveRun[file.playRun[playListPlayNumber].designID[playListPlayDesign]].designType != 0){
            cout<<"c ";
            SaveStateRender(file.playRun[playListPlayNumber].designID[playListPlayDesign]);
           playListTimer->start(file.playRun[playListPlayNumber].tickCount[playListPlayDesign] * 50);//chenck to see what wnit of time this takes
           doLoop = false;
        }else{
            doLoop = true;
        }
        cout<<"e ";
        //play first design
    }else{
        //cancel design
        Clear();
        playListTimer->stop();
        cout<<"f ";
        doLoop = false;

    }
    cout<<endl;
    }
}

void MainWindow::on_PlayListAddButton_clicked()
{
    file.playListTemp.designID[playListTempSlotNumber] = ui->PlayListAddSpinBox->value();
    file.playListTemp.tickCount[playListTempSlotNumber] = ((ui->PlayListHourSpinBox->value())*72000 + (ui->PlayListMinSpinBox->value())*1200 + (ui->PlayListSecSpinBox->value())*20);
    cout<<"tickCount: "<<file.playListTemp.tickCount[playListTempSlotNumber]<<endl;
    string playLineAdd;

    playLineAdd += to_string(file.playListTemp.designID[playListTempSlotNumber]);
    playLineAdd += ": ";
    playLineAdd += "H: ";
    playLineAdd += to_string(ui->PlayListHourSpinBox->value());
    playLineAdd += " M: ";
    playLineAdd += to_string(ui->PlayListMinSpinBox->value());
    playLineAdd += " S: ";
    playLineAdd += to_string(ui->PlayListSecSpinBox->value());
    ui->PlayListMakeList->addItem(QString::fromStdString(playLineAdd));
    playListTempSlotNumber++;
}

void MainWindow::on_PlayListClearButton_clicked()
{
    file.playListTemp.loop = NULL;
    file.playListTemp.PlayID = NULL;
    for(int y = 0; y<300;y++){
        file.playListTemp.designID[y] = NULL;
        file.playListTemp.tickCount[y] = NULL;
    }
    ui->PlayListMakeList->clear();
    playListTempSlotNumber = 0;
}


void MainWindow::on_PlayListAddPlayList_clicked()
{
    file.FindNextOpenFilesSlot();
    file.AddPlaylistLine(file.playListFirstLineOpen, PlayListStringCraft(file.playListFirstLineOpen));
}

void MainWindow::on_PlayListViewerUpdateButton_clicked()
{
    file.ReadPlaylistFile();
    ui->PlayListListView->clear();
    RenderPlayLists();
}

void MainWindow::on_PlayListDeleteButton_clicked()
{
    int deleteID = ui->PlayListDeleteSpin->value();

    file.playRun[deleteID].PlayID = NULL;
    file.playRun[deleteID].loop = false;

    for(int t = 0; t< 300; t++){
        file.playRun[deleteID].designID[t] = NULL;
        file.playRun[deleteID].tickCount[t] = NULL;
    }
    file.DeletePlayListID(deleteID);
}

void MainWindow::on_PlayListRenderButton_clicked()
{
    cout<<"test 1"<<endl;
    PlayListInit(ui->PlayListRenderSpin->value());
}
