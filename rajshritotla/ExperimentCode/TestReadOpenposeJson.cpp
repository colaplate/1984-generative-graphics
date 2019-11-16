/*
    It is printing the JSON output as it is generated by the application.

    Steps to run:

    1. In the main function: Set the location of openpose application and output directory

    2. Run openpose application using "output" as the folder name

    3. Run this program/appliction in parallel

    Command to compile : g++ ReadOpenPoseJson.cpp 

    ** Currently stopping condition of reading depends on the variable generateThisTimes, 
    this will read till 50 but this can be removed and the loop can run infinite time 
    as we require to continuosly read JSON

*/

#include <conio.h>
#include <fstream>

#include <ghc/filesystem.hpp>       // alternative to C++ filesystem
namespace fs = ghc::filesystem;

#include <iostream>
#include <math.h>

#include <nlohmann/json.hpp>        // to read json
using json = nlohmann::json;

#include <string.h>
#include <unistd.h>                 // for sleep()

using namespace std;

void generateFileName(int frameTime, string outputFolderLocation);          // value in microseconds
void parseJSON(string filename);
void startOpenPoseApplication(string openPoseAppLocation);


void generateFileName(int frameTime, string outputFolderLocation){          //frameTime- micorseconds [...SET THIS...]

    // for testing purpose how many filenames required to be print
    int generateThisTimes = 50;                     
    
    /* FILENAME FORMAT VARIABLES */ 
    
    // left digits count initially, 11 because 12th position is occupied already with 0 and will update 1,2...
    long appendZeroLeft = 11;   
    
    // number updating with every frame change
    long frameNumber = 0;   

    // fixed string 
    string str = "_keypoints.json"; 
    
    // file name that will be generated by application
    string filename; 
    

    // start generating filenames ; this condition can be left empty for loop to run infintely
    while ( frameNumber < generateThisTimes ) {
        filename = string( appendZeroLeft, '0').append(to_string(frameNumber)+str);
        
        if(fs::exists(outputFolderLocation+'/'+filename)){
            parseJSON(outputFolderLocation+'/'+filename);
            
            // update as per time interval
            usleep(frameTime);

            // incrementing with frames   
            frameNumber++;  

            /** Source: https://www.geeksforgeeks.org/program-count-digits-integer-3-different-methods/ */
            // floor(log10(frameNumber) + 1) -> Counts the number of digits in frame number
    
            appendZeroLeft = 12 - floor(log10(frameNumber) + 1);
        }
        else {
            cout<<endl<<"...waiting for file...."<<endl;
            continue;
        }   
    }
}


void parseJSON(string fileName) {
    ifstream i(fileName);
    json j;
    i >> j;
    // print JSON output
    cout << endl <<"filename: " << fileName << endl;
    cout << j;
    cout << endl;
}


void startOpenPoseApplication(string openPoseAppLocation){
    string appLocation = "cd "+openPoseAppLocation+" && ";
    string openPoseCmd = "OpenPoseDemo.exe --model_pose COCO -write_json output/ --disable_blending";
    string runCmd = appLocation+openPoseCmd;
    cout << "OpenPose App Path"<< runCmd;
    system(runCmd.c_str()); 
    
    /** c_str() - system arg are *char type, Source: http://www.cplusplus.com/reference/string/string/c_str/ */
}


int main () {
    string openPoseAppLocation = "E:/Practice Projects/openpose"; // SET THIS
    string outputFolderLocation = openPoseAppLocation+"/output";
    int frameTimeInterval = 1000000;    // value in microseconds
    //startOpenPoseApplication(openPoseAppLocation);
    generateFileName(frameTimeInterval,outputFolderLocation); 
    getch();
    return 0;
}