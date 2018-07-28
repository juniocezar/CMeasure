#ifndef MEASUREMENTS_HPP
#define MEASUREMENTS_HPP

#include "stdafx.h"
#include <assert.h>
#include <cstdlib>     /* exit, EXIT_FAILURE */
#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <windows.h>

#include "C:\Program Files (x86)\National Instruments 20\NI-DAQmx Base\Include\NIDAQmxBase.h"

using namespace std;
#define CHANNEL_BUFFER_SIZE 5000000L
#define VSUPPLY 5.0
#define SHUNT_GAIN 10.1112

/**--------SAMPLE RATE----------*/
#define SAMPLE_RATE 4800 // reduzido temporariamente devido a buffer overflow

/** NUMBER_OF_PULSES_TO_FINISH**/
#define CONTROL_VALUE 1

#define MAX_FILE_NAME_SIZE 100

#define NO_TRIGGER 0
#define TRIGGERED_ALL_POINTS 1
#define TRIGGERED_LAST_VALUE 2
#define TRIGGERED_TOTAL_ENERGY 3
#define TRIGGERED_AUTOMATIC 4

#define RESULT_VIEW_SCREEN 0
#define RESULT_VIEW_FILE 1

void log(std::string msg);


// handler for control C - Windows
static BOOL controlC(DWORD signal);

extern ofstream* GlobalFile;
extern double GlobalEnergy;
extern void* GlobalMeasurement;


class Channel  {
protected:
  float64 maxVoltage;
  string chName;
  string chId;
  vector<float64> samples;
  long ptSample;
  
public:
  float64 partial_sum;
  float64 partial_total_time;
  float64 partial_energy_HD;

  
public:
  Channel(string chName, string chId, float64 maxVoltage);
  
  void writeLastValue(char* fileName);
  
  int addSample(float64 sample);
  
  void writeOutputFile(string fileName, float64 sampleRate);

};


class ControlChannel : public Channel {
public:
  float64 startLevel;
  bool edge;
  bool active;

  void setState(float64 sample);
  int getBorder(float64 sample);
public:
  /**
   * Initialize the control channel
   **/
  ControlChannel(string chName, string chId,  float64 maxVoltage, 
     float64 startLevel, bool edge);

  bool trigger(float64 sample);

  bool untrigger(float64 sample);
};

#define SINGLE_READ_TIMEOUT 5.0
#define SINGLE_READ_BUFER_SIZE 70000L
// Calculate the address of the ith sample of the channel with index
// chIdx in a buffer with nSamples samples and nCh channels.
#define GET_SAMPLE(i,chIdx,nSamples) ( (((nSamples))*(chIdx))+(i) )

class Measurement {
public:
  typedef map< string, Channel* > ChannelList;
  typedef map< string, Channel* >::iterator ChannelListIt;
  ChannelList channels;
  ControlChannel* ctlCh;
  TaskHandle taskHandle;
  float64 sampleRatePerChannel;
  float64 partial_sum;
  int timeout_s;
  int ctlIndex;
  int numberChannels;

  void DAQmxErrChk(int32 error);

  void finishTasks();

public:
  Measurement(string deviceName, float64 sampleRatePerChannel, int timeout_s);

  void resetMeasurement(float64 sampleRatePerChannel, int timeout_s);

  void addChannel(string name, string id, float64 maxVoltage );

  void addChannel(string name, string id, float64 maxVoltage, float64 startLevel, bool edge);

  // measure all points
  char* acquireMeasurements(char* filename, int numer_of_measures = 1000);


  void startMeasure(int measurement_type ,  long numberOfSamples,  int result_view_mode, char* output_file_name = (char*)"output", std::string channel = "canalPrincipal");

  void writeOutputFile(char* filename, string channel) ;

  void writeLastValue(char* filename, string channel) ;

  void chooseName(char* original_filename,char* outfilename);
};



#endif