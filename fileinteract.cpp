#include "fileinteract.h"
#include <cstring>
#include <QDebug>
#include <QTimer>
#include <unistd.h>

fileInteract::fileInteract()
{

   // wrightText.open("RGBStoringSaveStates.txt", ofstream::out | ofstream::app);
   // readText.open()

}
int fileInteract::StringToInt(string str){
    int outputint;
    outputint = stoi(str);
    return outputint;
}

void fileInteract::ReadSaveStatesFile(){
    QFile readFile("RGBStoringSaveStates.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"saveStates file read failed!";
        return;
    }
    string line;
    int lineCounter = 1;
    while(!readFile.atEnd()){
        QString readLine = readFile.readLine();
        line = readLine.toStdString();
        //cout<<"Line:"<<line<<":"<<endl;
        if(readLine.size() != 1){
            SaveStatesStringDecipher(line);
        }
        if( line == "" && saveStateFirstLineOpen < 0){
            saveStateFirstLineOpen = lineCounter;
        }
        lineCounter++;

    }

   readFile.close();
}

void fileInteract::ReadPlaylistFile(){

    QFile readFile("RGBStoringPlaylists.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"PlayList file read failed!";
        return;
    }
    string line;
    int lineCounter = 1;
    while(!readFile.atEnd()){
        QString readLine = readFile.readLine();
        line = readLine.toStdString();

        if(readLine.size() > 1){
            PlaylistsStringDecipher(line);
        }
        if( readLine.size() == 1 && playListFirstLineOpen < 0){
            playListFirstLineOpen = lineCounter;
        }
        lineCounter++;
    }
   readFile.close();
}

void fileInteract::DeleteSaveStatesLineID(int IDDelete){
cout<<"deleteSaveStatesLineID: "<<endl<<"a"<<endl;
    //usleep(300000);
    int found;
    QFile readFile("RGBStoringSaveStates.txt");
    QFile writeFile("temp.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"saveStates file read failed!";
        return;
    }

    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
       // cout<<"saveStates file read failed!";
        return;
    }
    cout<<"b"<<endl;
    //usleep(300000);
    QTextStream out(&writeFile);
    string line;
    int lineTracker = 1;

    char tempCoppy[40];
cout<<"c"<<endl;
//usleep(300000);
    while(!readFile.atEnd()){
        cout<<"1"<<endl;
       // usleep(300000);
        QString readLine = readFile.readLine();
        //line = readLine.toStdString();
        cout<<"2"<<endl;
        //usleep(300000);
        if(readLine != ""){
            //found = line.find(":");
            cout<<"3"<<endl;
           // usleep(300000);
           // line.copy(tempCoppy,found - 1,0);
            if(IDDelete == lineTracker){
            readLine = "\n";
            cout<<"4"<<endl;
          //  usleep(300000);
        }
        }
        cout<<"5"<<endl;
       // usleep(300000);
        out<<readLine;
        lineTracker++;
        cout<<"6"<<endl;
      //  usleep(300000);
    }
    cout<<"d"<<endl;
   // usleep(300000);
    readFile.close();
    writeFile.close();
    QFile::remove("RGBStoringSaveStates.txt");
    QFile:rename("temp.txt", "RGBStoringSaveStates.txt");
    cout<<"e"<<endl;
    //usleep(300000);
}

void fileInteract::DeletePlayListID(int IDDelete){
    QFile readFile("RGBStoringPlaylists.txt");
    QFile writeFile("temp.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"saveStates file read failed!";
        return;
    }

    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
       // cout<<"saveStates file read failed!";
        return;
    }
    QTextStream out(&writeFile);
    string line;
    int lineTracker = 1;

    while(!readFile.atEnd()){
        QString readLine = readFile.readLine();

        if(readLine != ""){

            if(IDDelete == lineTracker){
            readLine = "\n";
        }
        }
        out<<readLine;
        lineTracker++;
    }
    readFile.close();
    writeFile.close();
    QFile::remove("RGBStoringPlaylists.txt");
    QFile:rename("temp.txt", "RGBStoringPlaylists.txt");
}

void fileInteract::PlaylistsStringDecipher(string stringLine){
    char tempCoppy[40];
   // stringLine +="789:";

    int found = stringLine.find(":");
    stringLine.copy(tempCoppy,found,0);
    int ID = stoi(tempCoppy);

    playRun[ID].PlayID = ID;
    //cout<<ID<<endl;
    found = stringLine.find("'");
    memset(tempCoppy, 0, sizeof(tempCoppy));
    stringLine.copy(tempCoppy,1,found+1);

    QString qstringTemp = tempCoppy;
    //double check to see if it works or not
    if(qstringTemp == "T"){
        playRun[ID].loop = true;
    }else{
        playRun[ID].loop = false;
    }

    memset(tempCoppy, 0, sizeof(tempCoppy));
    bool doLoop = true;
    int loopTrack = -1;

    int found2 = 0;
    while(doLoop){
        loopTrack ++;
        cout<<"did a loop"<<endl;
        memset(tempCoppy, 0, sizeof(tempCoppy));
        found = stringLine.find("[",found +1);
        found2 = stringLine.find("]",found2 +1);
                cout<<"found: "<<found<<" found2: "<<found2<<endl;
        stringLine.copy(tempCoppy,found2 - found - 1,found+=1);
        playRun[ID].designID[loopTrack] = stoi(tempCoppy);

        memset(tempCoppy, 0, sizeof(tempCoppy));
        found = stringLine.find("{",found +1);
        found2 = stringLine.find("}",found2 +1);
        cout<<"found: "<<found<<" found2: "<<found2<<endl;
        stringLine.copy(tempCoppy,found2 - found - 1,found+=1);
        playRun[ID].tickCount[loopTrack] = stoi(tempCoppy);

        if(stringLine.find("[",found) == (found2 +1)){
            doLoop = true;
        }else{
           doLoop = false;
        }
    }
}

void fileInteract::SaveStatesStringDecipher(string stringLine){

    char tempCoppy[40];
   // stringLine +="789:";
   // cout<<"stringDesipher"<<endl<<"a"<<endl;
   // usleep(600000);
    int found = stringLine.find(":");
    stringLine.copy(tempCoppy,found,0);
    int ID = stoi(tempCoppy);
    memset(saveRun[ID].colorStore,NULL,sizeof(saveRun[ID].colorStore));
    saveRun[ID].ID = ID;
   // cout<<"bb"<<endl;
   // usleep(600000);
    //cout<<ID<<endl;
    found = stringLine.find("'");
    memset(tempCoppy, 0, sizeof(tempCoppy));
    stringLine.copy(tempCoppy,1,found+1);
    int design = stoi(tempCoppy);
    saveRun[ID].designType = design;
   // cout<<"b"<<endl;
   // usleep(600000);
    memset(tempCoppy, 0, sizeof(tempCoppy));
    found = stringLine.find(".",found);
    int found2 = stringLine.find(".",found +1);
    stringLine.copy(tempCoppy,found2 - found - 1,found+=1);
    saveRun[ID].ticksBetweenMoves = stoi(tempCoppy);
  //  cout<<"c"<<endl;
//    usleep(600000);
    memset(tempCoppy, 0, sizeof(tempCoppy));
    found = stringLine.find("|",found);
    found2 = stringLine.find("|",found +1);
    stringLine.copy(tempCoppy,found2 - found - 1,found+=1);
    saveRun[ID].jumpDist = stoi(tempCoppy);
   // cout<<"d"<<endl;
//    usleep(600000);
    memset(tempCoppy, 0, sizeof(tempCoppy));
    found = stringLine.find("<",found);
    found2 = stringLine.find("<",found +1);
    stringLine.copy(tempCoppy,found2 - found - 1,found+=1);
    saveRun[ID].forwardDirection = stoi(tempCoppy);

  //  cout<<"e"<<endl;
   // usleep(600000);

    bool doLoop = true;
    int loopTrack = 0;
    found = stringLine.find("{");
    while(doLoop){
        loopTrack ++;
        cout<<"did a loop"<<endl;

        stringLine.copy(tempCoppy,3,found+=1);
        saveRun[ID].colorStore[loopTrack].red = stoi(tempCoppy);

        found += 4;
        memset(tempCoppy, NULL, sizeof(tempCoppy));
        stringLine.copy(tempCoppy,3,found);
        saveRun[ID].colorStore[loopTrack].green = stoi(tempCoppy);

        memset(tempCoppy, NULL, sizeof(tempCoppy));
        found += 4;
        stringLine.copy(tempCoppy,3,found);
        saveRun[ID].colorStore[loopTrack].blue = stoi(tempCoppy);

        memset(tempCoppy, NULL, sizeof(tempCoppy));
        found += 4;
        stringLine.copy(tempCoppy,4,found);
        saveRun[ID].colorStore[loopTrack].tickChangeInterval = stoi(tempCoppy);
        cout<<"tickChangeInterval: "<<ID<<"loopTrac: "<<loopTrack<<" tickChangeInterval: "<<saveRun[ID].colorStore[loopTrack].tickChangeInterval<<endl;
        memset(tempCoppy, NULL, sizeof(tempCoppy));
        found += 5;
        stringLine.copy(tempCoppy,4,found);
        saveRun[ID].colorStore[loopTrack].tiksBeforeAdded = stoi(tempCoppy);

        memset(tempCoppy, NULL, sizeof(tempCoppy));
        found += 5;
        stringLine.copy(tempCoppy,4,found);
        saveRun[ID].colorStore[loopTrack].waveStartNumber = stoi(tempCoppy);

        memset(tempCoppy, NULL, sizeof(tempCoppy));
        found +=5;
        stringLine.copy(tempCoppy,1,found);
            if(stringLine.find("{",found) == found){
                cout<<"string decipher true"<<endl;
                doLoop = true;
            }else{
               doLoop = false;
               cout<<"string decipher false"<<endl;
            }

        }
   // cout<<"f"<<endl;
   // usleep(600000);
}

void fileInteract::AddSaveStateLine(int IDAdd, string addedString){
int lineTracker = 1;
        QFile readFile("RGBStoringSaveStates.txt");
        QFile writeFile("temp.txt");
        if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            cout<<"saveStates file read failed!";
            return;
        }

        if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
           // cout<<"saveStates file read failed!";
            return;
        }
        QTextStream out(&writeFile);
        string line;

        QString readLine;
        while(!readFile.atEnd()){
             readLine = readFile.readLine();

           // cout<<lineTracker<<endl;
                    if(IDAdd == lineTracker){

                        readLine = QString::fromStdString(addedString);
                    }
                    out<<readLine;
                    lineTracker++;
        }
        if(IDAdd == lineTracker){

            readLine = QString::fromStdString(addedString);
        }
         out<<readLine;

     //   out<<"Hey this finaly worked I am soooooo happy"<<endl;
       readFile.close();
       writeFile.close();
       QFile::remove("RGBStoringSaveStates.txt");
       QFile::rename("temp.txt", "RGBStoringSaveStates.txt");
}

void fileInteract::AddPlaylistLine(int IDAdd, string addedString){
    QString readLine;
    QFile readFile("RGBStoringPlaylists.txt");
    QFile writeFile("temp.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"Playlists file read failed!";
        return;
    }

    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
       // cout<<"saveStates file read failed!";
        return;
    }
    QTextStream out(&writeFile);
    string line;
    int lineTracker = 1;
    while(!readFile.atEnd()){
        readLine = readFile.readLine();

                if(IDAdd == lineTracker){
                    cout<<lineTracker<<endl;
                    readLine = QString::fromStdString(addedString);
                }
                out<<readLine;
                lineTracker++;
    }

    if(IDAdd == lineTracker){

        readLine = QString::fromStdString(addedString);
    }
     out<<readLine;

   readFile.close();
   writeFile.close();
   QFile::remove("RGBStoringPlaylists.txt");
   QFile:rename("temp.txt", "RGBStoringPlaylists.txt");
}

void fileInteract::FindNextOpenFilesSlot(){

    QFile readFile("RGBStoringPlaylists.txt");
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"Playlists file read failed!";
        return;
    }

    string line;


    bool doLoop = true;
    int lineTracker = 1;
    while(!readFile.atEnd() && doLoop){
        QString readLine = readFile.readLine();
        if(readLine.size() == 1){
            doLoop = false;
            playListFirstLineOpen = lineTracker;
        }

        lineTracker++;
    }
    if(doLoop){
        doLoop = false;
        playListFirstLineOpen = lineTracker;
    }
    readFile.close();

    QFile readingFile("RGBStoringSaveStates.txt");
    if(!readingFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"saveStates file read failed!"<<endl;
        return;
    }
    line.clear();
     doLoop = true;
     lineTracker = 1;
    // cout<<"loopstart"<<endl;
     while(!readingFile.atEnd() && doLoop){
    //     cout<<"doLoop"<<endl;

         QString readLine = readingFile.readLine();
         //cout<<readLine.size()<<endl;
         if(readLine.size() == 1){
             doLoop = false;
             //cout<<"isEMPTYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"<<endl;
             saveStateFirstLineOpen = lineTracker;
         }

         lineTracker++;
     }
     if(doLoop){
         doLoop = false;
         saveStateFirstLineOpen = lineTracker;
     }
     readingFile.close();
}
