#include "measurements.hpp"

ofstream* GlobalFile;
double GlobalEnergy;
void* GlobalMeasurement;


void log(std::string msg) {
  std::cout << msg << std::endl;
}

// handler for control C - Windows
  static BOOL controlC(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
      cout << "Finishing tasks" << endl;
      GlobalFile->close();
      cout << "Energy: " << GlobalEnergy << endl;
      Measurement* gm = (Measurement*)GlobalMeasurement;
      gm->finishTasks();
      exit(EXIT_SUCCESS);
    }
    return FALSE;
  }


Channel::Channel(string chName, string chId, float64 maxVoltage) : samples(CHANNEL_BUFFER_SIZE) {
    this->maxVoltage = maxVoltage;
    this->chName = chName;
    this->chId = chId;
    ptSample = 0;
    partial_sum = 0.0;
    partial_total_time =0.0;
    partial_energy_HD = 0.0;
    
  }


  void Channel::writeLastValue(char* fileName){
    FILE* output = fopen(fileName,"a+"); 
    fprintf(output,"%f %f\n",partial_total_time,partial_sum);
    fclose(output);
  }

  
  int Channel::addSample(float64 sample) {
    samples[ptSample] = sample;
    
    ptSample = ptSample + 1;
    // Buffer overflow
    if(ptSample == (CHANNEL_BUFFER_SIZE -1)){
      printf("Warning: Channel %s buffer overflow\n ---Just for testing purpose---\n", chName.c_str());
      ptSample = 0;
      return (1); //houve estouro
    }

    return (0); //funcionamento normal
  }

  void Channel::writeOutputFile(string fileName, float64 sampleRate){
    ofstream output;

    output.open(fileName);

    for(long i = 0; i != ptSample; i++){ 
      output << (ptSample*(1/sampleRate)) << "  " << samples[i] << endl; 
    }

    output.close();
  }



  void ControlChannel::setState(float64 sample){
    active = (sample > startLevel)? edge : !edge;
  }

  int ControlChannel::getBorder(float64 sample){
    bool lastState = active;
    setState(sample);
    // Positive border
    if(this->edge){
      // It is in level H
      if(lastState ) return (sample >startLevel)? 0 : -1;
      // It is in level L
      else return (sample < startLevel)? 0 : 1;
    // Neg border
    } else {
      // It is in level L
      if(lastState) return (sample < startLevel)? 0 : 1;
      // It is in level H
      else return (sample > startLevel)? 0 : -1;
    }

  }

  /**
   * Initialize the control channel
   **/
  ControlChannel::ControlChannel(string chName, string chId,  float64 maxVoltage, 
     float64 startLevel, bool edge) : Channel (chName, chId,  maxVoltage) {
    this->startLevel = startLevel;
    this->active = false;
    this->edge = edge;
  }



  bool ControlChannel::trigger(float64 sample){
    int borda = getBorder(sample);
    if ( (edge && (borda == 1)) ||
              (!edge && (borda == -1)) ) return true;
    else return false;
  }

  bool ControlChannel::untrigger(float64 sample){
    int borda = getBorder(sample);
    return  ( (edge && (borda == -1)) ||
              (!edge &&(borda == 1)) );
  }

  void Measurement::DAQmxErrChk(int32 error) {
    char        errBuff[4096] = { '\0' };

    if (DAQmxFailed(error)) {
      cout << error << endl;
      finishTasks();
      DAQmxBaseGetExtendedErrorInfo(errBuff, 2048);
      cout << "DAQmx Error: " << errBuff << endl;;
      cout << "End of program, press Enter key to quit" << endl;
      getchar();

      exit(EXIT_FAILURE);
    }

  }

  void Measurement::finishTasks() {
    if (taskHandle != 0) {
      /*********************************************/
      // DAQmx Stop Code
      /*********************************************/
      DAQmxBaseStopTask(taskHandle);
      DAQmxBaseClearTask(taskHandle);

    }
  }

  


  Measurement::Measurement(string deviceName, float64 sampleRatePerChannel, int timeout_s){
    this->sampleRatePerChannel = sampleRatePerChannel;
    this->timeout_s = timeout_s;
    this->numberChannels = 0;
    this->partial_sum = 0.0;
    DAQmxErrChk(DAQmxBaseResetDevice(deviceName.c_str()));
    DAQmxErrChk(DAQmxBaseCreateTask("",&taskHandle));

  }

  void Measurement::resetMeasurement(float64 sampleRatePerChannel, int timeout_s)
  {
    this->sampleRatePerChannel = sampleRatePerChannel;
    this->timeout_s = timeout_s;
    this->partial_sum = 0.0;
  }

  
  void Measurement::addChannel(string name, string id, float64 maxVoltage ){
    this->channels[name] = new Channel(name, id, maxVoltage);
    this->numberChannels++;
    // Init the DAQmx Channel
    DAQmxErrChk(DAQmxBaseCreateAIVoltageChan(taskHandle,id.c_str(),name.c_str(),DAQmx_Val_Diff,
            -maxVoltage,maxVoltage,DAQmx_Val_Volts,NULL));
    this->sampleRatePerChannel = SAMPLE_RATE/numberChannels;

  }

  void Measurement::addChannel(string name, string id, float64 maxVoltage, float64 startLevel, bool edge){    
    ctlCh = new ControlChannel(name,id,maxVoltage, startLevel,edge);
    assert(ctlCh != NULL);

    this->channels[name] = ctlCh;
    ctlIndex = numberChannels;
    this->numberChannels++;
    // Init the DAQmx Channel
    DAQmxErrChk(DAQmxBaseCreateAIVoltageChan(taskHandle,id.c_str(),name.c_str(),DAQmx_Val_Diff,
            -maxVoltage,maxVoltage,DAQmx_Val_Volts,NULL));

  };



  // measure all points
  char* Measurement::acquireMeasurements(char* filename, int numer_of_measures){
    DAQmxBaseResetDevice("Dev1");
    char out[MAX_FILE_NAME_SIZE];

    for(int i = 0; i<numer_of_measures ;i++)
    {
      float64 buffer[SINGLE_READ_BUFER_SIZE];
      double energy = 0.0;
      int32 samplesRead=1;
      bool started = false;
      bool firstSample = true;
      bool finished = false;
      int currSample;

      // Config the timing
      DAQmxErrChk(DAQmxBaseCfgSampClkTiming(taskHandle,"",this->sampleRatePerChannel,
                DAQmx_Val_Rising,DAQmx_Val_ContSamps,SINGLE_READ_BUFER_SIZE));

      ofstream output;
      chooseName(filename,out);
      output.open(out);

      // used for properly closing file in case of control c interrupt
      GlobalFile = &output;


      DAQmxErrChk (DAQmxBaseStartTask(taskHandle));
     // Wait for trigger
      while(!started){
        DAQmxErrChk(DAQmxBaseReadAnalogF64(taskHandle,-1,SINGLE_READ_TIMEOUT,DAQmx_Val_GroupByChannel,
               buffer,SINGLE_READ_BUFER_SIZE/this->numberChannels,&samplesRead,NULL));

        for(currSample= 0; currSample < samplesRead; currSample++){
          // Check if control channel has been triggered       
          if(firstSample){
            ctlCh->setState(buffer[GET_SAMPLE(currSample,ctlIndex,samplesRead)]);
            firstSample = false;
            started = true;            
          }          

          if( ctlCh->trigger(buffer[GET_SAMPLE(currSample,ctlIndex,samplesRead)]) == true ){
            started = true;
              cout << "Triggered" << endl;
          }
        }

        break;
      }

      cout<< "Start Acquiring Data" << endl;

      cout << "Sample Rate: " << sampleRatePerChannel << endl;
      cout << "Sample Read: " << samplesRead << endl;
      
      // Sample channels 
      long savedSample = 0;
      int ret = 0;
      currSample--;
      while(!finished){
        // currSample reads loops through the number of samples taken for a single channel
        // It is not set to 0 here because a trigger may have happened in the middle of a read
        for(; currSample < samplesRead; currSample++){
          int i=0;
          for(ChannelListIt currCh = channels.begin(); currCh != channels.end(); currCh++,i++){
            currCh->second->addSample(buffer[GET_SAMPLE(currSample,i,samplesRead)]);
            currCh->second->partial_total_time += (1/sampleRatePerChannel); //sum time
            if (currCh == channels.begin()) 
            {
              energy += VSUPPLY*SHUNT_GAIN*buffer[currSample]/sampleRatePerChannel;
              output << currCh->second->partial_total_time << "  " << VSUPPLY*SHUNT_GAIN*buffer[currSample] << endl;
            }
              
          } 

          savedSample++;
          if( ctlCh->untrigger(buffer[GET_SAMPLE(currSample,ctlIndex,samplesRead)]) ){
            finished = true;
              cout << "Untriggered" << endl;
            break;
          }    
        }

        DAQmxErrChk(DAQmxBaseReadAnalogF64(taskHandle,-1,SINGLE_READ_TIMEOUT,DAQmx_Val_GroupByChannel,
               buffer,SINGLE_READ_BUFER_SIZE,&samplesRead,NULL));
        currSample = 0;

        GlobalEnergy = energy;
      } 

      cout << i << ":  " << energy << endl;      

      output.close();
      finishTasks();
    }

    // bug: its a local variable, why returning it?
    return out;
  }


  void Measurement::startMeasure(int measurement_type ,  long numberOfSamples,  int result_view_mode, char* output_file_name, std::string channel)
  {
    // Handling control C interrupts. 
    // Finish properly and report energy spent
    GlobalMeasurement = (void*)this;
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)controlC, TRUE)) {
      std::cerr << "Error while handling control+c" << endl;
      return;
    }

    acquireMeasurements(output_file_name, numberOfSamples);
  }

  void Measurement::writeOutputFile(char* filename, string channel){
    channels[channel]->writeOutputFile(filename,sampleRatePerChannel);
  }

  void Measurement::writeLastValue(char* filename, string channel){
    channels[channel]->writeLastValue(filename);
  }



  void Measurement::chooseName(char* original_filename,char* outfilename)
  {
    FILE* in;
    char curr_filename[MAX_FILE_NAME_SIZE];
    strcpy(curr_filename,original_filename);
    char buff[MAX_FILE_NAME_SIZE];
    char buff_opening[MAX_FILE_NAME_SIZE];
    for(int i =1;i<100;i++)
    {
      strcpy(buff_opening,curr_filename);
      strcat(buff_opening,".txt");
      in = fopen(buff_opening,"r");

      if( in == NULL) { strcpy(outfilename,curr_filename); strcat(outfilename,".txt"); return; }

      strcpy(curr_filename,original_filename);
      
      itoa(i,buff,10);
      strcat(curr_filename,buff);
      fclose(in);
    }
  }
