#ifndef FILEINTERACT_H
#define FILEINTERACT_H
#include <string>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>

using namespace std;

class fileInteract
{
public:
   // ofstream wrightText;
//    ifstream readText;
    fileInteract();
    int StringToInt(string str);
    void SaveStatesStringDecipher(string stringLine);
    void PlaylistsStringDecipher(string stringLine);
    void ReadSaveStatesFile();
    void ReadPlaylistFile();
    void DeleteSaveStatesLineID(int IDDelete);
    void DeletePlayListID(int IDDelete);
    void AddSaveStateLine(int IDAdd, string addedString);
    void AddPlaylistLine(int IDAdd, string addedString);
    void FindNextOpenFilesSlot();//finds the first open line in both the savestate and playlist files

    struct RGBStore{
    union{
    struct{
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
    uint32_t color;
    };
    int tickChangeInterval;
    int tiksBeforeAdded;
    int waveStartNumber;
    };

    struct savestateRun{
         RGBStore colorStore [300];

        int ID;
        int designType;
        int jumpDist;
        int ticksBetweenMoves;
        bool forwardDirection;

    };
    savestateRun saveRun[500];

    struct playlistRun{
        int PlayID;
        int designID[300];
        int tickCount[300];
        bool loop;
    };
    playlistRun playRun[500];
    playlistRun playListTemp;

    int saveStateFirstLineOpen = -1;
    int playListFirstLineOpen = -1;
};

#endif // FILEINTERACT_H
