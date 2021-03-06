/*
 *
 *   IOC for He3 voltage logger, built by Sam de Jong
 *
 */

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/time.h>
#include <sstream>
#include <string>
#include <cstring>

#include <dirent.h>
#include <errno.h>

#include <subRecord.h>
#include <aSubRecord.h>
#include "stringoutRecord.h"

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"
#include "epicsMessageQueue.h"

#include <registryFunction.h>
#include <epicsExport.h>

//root
#include "TFile.h"
#include "TTree.h" 

#include "DI-2108-P.cpp"

DI_2108_P logger;

epicsMessageQueue *mq1;
epicsThreadId myThread=0;
epicsMutex *plock;
int message;

struct timeval timeMark;

//int pause=false;

vector<double> ts;
vector< vector<double> > voltages;


std::string path = "./";
int NTRIGS=3600;


//returns the time of the call.
double markTime() {
  gettimeofday(&timeMark,NULL);
  return (double)timeMark.tv_sec + (double)timeMark.tv_usec/1000000.; 
}


void writeNtuple(){
  //pause=true;

 

  double fileStartName = ts[0];

  std::string filename = path + Form("%s_%F.root", "He3VoltageLogger", fileStartName);

  cout<<"\nWriting "<<ts.size()<<" events to "<<filename<<endl<<"epics> ";



  TString a = filename;
  TFile *outfile = new TFile(a, "RECREATE");

  double time;
  vector<double> v;
  
  TTree *outtree = new TTree("tout", "tout");
  outtree->Branch("ts", &time, "ts/D");
  outtree->Branch("voltage", &v);

  for(int i=0; i<(int)ts.size(); i++){
    time=ts[i];
    v=voltages[i];
    
    outtree->Fill();
  }
  
  ts.clear();
  voltages.clear();


  outtree->Write();
  outfile->Close();
  

}




/****************************************************************************************************************/
/*				  	        EPICS code							*/
/*														*/
/****************************************************************************************************************/


static long initialize(struct subRecord *psub) {
  
  NTRIGS=psub->k;
  //logger.reset();
  if(psub->a==1){
    cout<<"recording...\n";
    
    logger.Initialize();

    if(!logger.isInitialized()){
      psub->l=0;
      return 0;
    }else{
      psub->l=1;
    }
     
    logger.setNChannels(psub->b);
    
    logger.setRange(0, psub->c);
    logger.setRange(1, psub->d); 
    logger.setRange(2, psub->e);
    logger.setRange(3, psub->f);
    logger.setRange(4, psub->g);
    logger.setRange(5, psub->h);
    logger.setRange(6, psub->i);
    logger.setRange(7, psub->j);
    
    cout<<"starting scan\n";

    logger.startScan();
    cout<<"scan started\n";

  }else if(psub->a==0){
    
    if(logger.isInitialized()){ 
      cout<<"stopping recording...\n";
      while(logger.isRec()) sleep(0.1);
      logger.stopScan();
      logger.softReset();
      logger.close();
      psub->l=0;
	
    }
    //logger.reset();
  }

  return(0);
} 
  
 


static long setPath(struct subRecord *psub) {
  path=psub->desc;
  cout<<"Data path set to "<<path<<"\n";

 
  return 0; 
}


static long getVoltage(struct subRecord *psub) {


  if(logger.isRunning()){
  
    logger.getReadings();
    logger.getReadings();
    logger.getReadings();
    
    

    vector<double> v = logger.getReadings();
    double time = markTime();

    if(psub->j==1) cout<<v[0]<<endl;
    
    ts.push_back(time); 
    voltages.push_back(v);   
     
    if((int)ts.size()==NTRIGS) writeNtuple();
    
  
    psub->a=v[0]; 
    psub->b=v[1];
    psub->c=v[2];
    psub->d=v[3];   
    psub->e=v[4];
    psub->f=v[5];
    psub->g=v[6];
    psub->h=v[7];

  }



  return 0;

}

static long saveData(struct subRecord *psub) {
  
  while(logger.isRec()) sleep(0.5);
  writeNtuple();
  
  return 0;
}
 


//register routines
epicsRegisterFunction(initialize);  
epicsRegisterFunction(setPath);
epicsRegisterFunction(getVoltage);
epicsRegisterFunction(saveData);

//note that these routines must also be registered in voltageLoggerApp/src/voltageLoggercroutines.dbd


//main method, self explanatory
int main(int argc,char *argv[])
{
 
  plock = new epicsMutex;            //initalize mutex.
  if(plock==0){
    printf(" plock is null :(\n");
    return 1;
  }


  if(argc>=2) {    
    iocsh(argv[1]);
    epicsThreadSleep(.2);
  }
 
  
  iocsh(NULL);

   if(logger.isInitialized()){ 
      cout<<"stopping recording...\n";
      while(logger.isRec()) sleep(0.1);
      logger.stopScan();
      logger.softReset();
      logger.close();
	
    }


  if(ts.size()!=0) writeNtuple();


  epicsExit(0);

 
  return(0);


} 
 
 
