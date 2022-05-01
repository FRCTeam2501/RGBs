#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "rgbcode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void DoTicking(bool tickingActive);
     void Clear();
    void WaveInit();
    int BarNumberTracker = 0;

    union rgbTempSet{
        struct{
            uint8_t blue;
             uint8_t green;
             uint8_t red;
        };
          uint32_t color;
         };


    rgbTempSet rgbTemporarySet;
    rgbTempSet rgbTempSet2;
    rgbTempSet rgbTempSet3;


    struct barhold{
        union{
            struct{
                uint8_t blue;
                 uint8_t green;
                 uint8_t red;
            };
              uint32_t color;
            };
        int barLength;
    };

    int barNumber = 1;
    barhold barhold[500];


    struct TempRGBWaveSeed{
        union{
            struct{
                uint8_t targetBlue;
                uint8_t targetGreen;
                 uint8_t targetRed;
            };
            uint32_t colorTarget;
        };
        int tickInterval;

    };

    struct TempRGBWaveList{
        TempRGBWaveSeed waveSeedInfo;
        int ticksBeforeNextWave;
        int StartingPlace;
    };
    TempRGBWaveList WaveListTemp[300];
    int waveNumber = 0;

    int playListTempSlotNumber = 0;//this is the number that the playlist crafter will put the current one into the array.

    int playListPlayNumber = -1;//playList number we are currently playing
    int playListPlayDesign = -1;//design number we are on in the playlist

public slots:
    void Test();
    void Tick();
    void ProcessPlayListTimeOut();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_4_clicked();

    void on_PushButtonAddBar_clicked();

    void on_ButtonBarClearList_clicked();

    void on_WaveAddElement_clicked();

    void on_WaveClearList_clicked();

    void on_WaveRender_clicked();

    string WaveStringCraft(int saveID);

    string BarStringCraft(int saveID);

    string SolidSetFile(int saveID);

    void on_SolidSaveStateButton_clicked();

    void on_BarSaveStateButton_clicked();

    void on_WaveSaveStateButton_clicked();

    void on_SaveStateDeleteButton_clicked();

    void RenderSaveList();

    void on_SaveStateViewerUpdateButton_clicked();

    void on_SaveStateRenderButton_clicked();

    void SaveStateRender(int saveID);

    void on_PlayListAddButton_clicked();

    void on_PlayListClearButton_clicked();

    void on_PlayListAddPlayList_clicked();

    string PlayListStringCraft(int saveID);

    void on_PlayListViewerUpdateButton_clicked();

    void RenderPlayLists();

    void on_PlayListDeleteButton_clicked();

    void on_PlayListRenderButton_clicked();

    void PlayListInit(int playListInitID);

private:
    Ui::MainWindow *ui;
    QTimer *timer, *playListTimer;

};

#endif // MAINWINDOW_H
