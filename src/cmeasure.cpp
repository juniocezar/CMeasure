#include "measurements.hpp"
#include <iostream>
#include <fstream>


int main(int argc, char* argv[]){

  // default values
  int codes_running = 33;
  int curr_code = 0;
  int measurement_type = TRIGGERED_ALL_POINTS;
  long numberOfSamples = 33;
  int result_view_mode = RESULT_VIEW_FILE;
  char output_file_name[MAX_FILE_NAME_SIZE];
  strcpy(output_file_name,"output");
  std::string channel = "canalPrincipal";

  // default mode being used is TRIGGERED_ALL_POINTS. So, the switch case
  // was removed
  
  switch(argc) {
    //no parameter
    case(1): { break; }
  }

  
  //initialize device
  Measurement test("Dev1",SAMPLE_RATE,60);
  test.addChannel("canalPrincipal","Dev1/ai0",1.0);
  test.addChannel("canalTrigger","Dev1/ai1",10.0,1.0,false);

  // Mostly the only one being used, verify later so
  // we can remove the other channels, increasing the final 
  // sampling rate of this channel
  test.addChannel("canalHD","Dev1/ai2",1.0);

  test.addChannel("canalnulo","Dev1/ai3",1.0);
           
  log("CMeasure Initiated! The output data will be written to the file: output.txt");

  // start the measure    
  test.startMeasure(measurement_type ,numberOfSamples, result_view_mode, output_file_name, channel );
      
  log("Finished.");

  return 0;
}
