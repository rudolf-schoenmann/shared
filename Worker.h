/*
Program:     MolFlow+ / Synrad+
Description: Monte Carlo simulator for ultra-high vacuum and synchrotron radiation
Authors:     Jean-Luc PONS / Roberto KERSEVAN / Marton ADY
Copyright:   E.S.R.F / CERN
Website:     https://cern.ch/molflow

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Full license text: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
*/
#pragma once

#include <string>
#include <vector>
#include "GLApp/GLTypes.h"
#include "Smp.h"
#include "Buffer_shared.h" //LEAK, HIT

class Geometry;
class GLProgress;
class LoadStatus;


#ifdef MOLFLOW
#include "Parameter.h"
#include "Vector.h" //moving parts
#include "MolflowTypes.h"

#define CDF_SIZE 100 //points in a cumulative distribution function

class MolflowGeometry;
#endif

#ifdef SYNRAD
#include "Region_full.h"

class SynradGeometry;
class Material;
#endif


class Worker
{

public:

  // Constructor
  Worker();
  ~Worker();

  // Return a handle to the currently loaded geometry
  Geometry *GetGeometry();

  
  void LoadGeometry(char *fileName, bool insert=false, bool newStr=false);// Loads or inserts a geometry (throws Error)
  void LoadTexturesSYN(FileReader* f,int version);  // Load a textures(throws Error)
  void RebuildTextures();
  
  // Save a geometry (throws Error)
  void SaveGeometry(char *fileName,GLProgress *prg,bool askConfirm=true,bool saveSelected=false,bool autoSave=false,bool crashSave=false);
  bool IsDpInitialized();
  
  // Export textures (throws Error)
  void ExportTextures(char *fileName,int grouping,int mode,bool askConfirm=true,bool saveSelected=false);
  //void ExportRegionPoints(char *fileName,GLProgress *prg,int regionId,int exportFrequency,bool doFullScan);
  //void ExportDesorption(char *fileName,bool selectedOnly,int mode,double eta0,double alpha,const Distribution2D &distr);

  std::vector<std::vector<std::string>> ImportCSV_string(FileReader *file);
  std::vector<std::vector<double>> ImportCSV_double(FileReader *file);

  void ExportBuffer(char *fileName);

  // Return/Set the current filename
  char *GetCurrentFileName();
  char *GetCurrentShortFileName();
  //char *GetShortFileName(char* longFileName);
  void  SetCurrentFileName(char *fileName);

  void SetProcNumber(size_t n);// Set number of processes [1..32] (throws Error)
  size_t GetProcNumber();  // Get number of processes
 // void SetMaxDesorption(llong max);// Set the number of maximum desorption
  DWORD GetPID(size_t prIdx);// Get PID
  void ResetStatsAndHits(float appTime);
  void Reload();    // Reload simulation (throws Error)
  void RealReload();
  std::ostringstream SerializeForLoader();
  void ChangeSimuParams();
  void Stop_Public();// Switch running/stopped
  //void Exit(); // Free all allocated resource
  void KillAll();// Kill all sub processes
  void Update(float appTime);// Get hit counts for sub process
  //void SendLeakCache(Dataport *dpHit); // From worker cache to dpHit shared memory
  //void SendHitCache(Dataport *dpHit);  // From worker cache to dpHit shared memory
  void GetProcStatus(size_t *states,std::vector<std::string>& statusStrings);// Get process status
  BYTE *GetHits(); // Access to dataport (HIT)
  std::tuple<size_t,ParticleLoggerItem*> GetLogBuff();
  void ReleaseLogBuff();
  void ReleaseHits();
 
  void RemoveRegion(int index);
  void AddRegion(const char *fileName,int position=-1); //load region (position==-1: add as new region)
  void RecalcRegion(int regionId);
  void SaveRegion(char *fileName,int position,bool overwrite=false);
  bool CheckFilenameConflict(const std::string& newPath, const size_t& regionId, std::vector<std::string>& paths, std::vector<std::string>& fileNames, std::vector<size_t>& regionIds);

#ifdef MOLFLOW
  MolflowGeometry* GetMolflowGeometry();
  void ExportProfiles(char *fileName);
  void ExportAngleMaps(std::vector<size_t> faceList, std::string fileName);
  void AnalyzeSYNfile(char *fileName, size_t *nbFacet, size_t *nbTextured, size_t *nbDifferent);
  void ImportDesorption_SYN(char *fileName, const size_t &source, const double &time,
	  const size_t &mode, const double &eta0, const double &alpha, const double &cutoffdose,
	  const std::vector<std::pair<double, double>> &convDistr,
	  GLProgress *prg);
  void LoadTexturesGEO(FileReader *f, int version);
  void OneACStep();
  void StepAC(float appTime); // AC iteration single step
  void ComputeAC(float appTime); // Send Compute AC matrix order
  void PrepareToRun(); //Do calculations necessary before launching simulation
  int GetParamId(const std::string); //Get ID of parameter name
  void SendFacetHitCounts(Dataport * dpHit);
  int AddMoment(std::vector<double> newMoments); //Adds a time serie to moments and returns the number of elements
  std::vector<double> ParseMoment(std::string userInput); //Parses a user input and returns a vector of time moments
  void ResetMoments();
  double GetMoleculesPerTP(size_t moment);
  std::vector<std::pair<double, double>> Generate_ID(int paramId);
  int GenerateNewID(int paramId);
  std::vector<std::pair<double, double>> Generate_CDF(double gasTempKelvins, double gasMassGramsPerMol, size_t size);
  int GenerateNewCDF(double temperature);
  void CalcTotalOutgassing();
  int GetCDFId(double temperature);
  int GetIDId(int paramId);
  //Different signature:
  void SendToHitBuffer(bool skipFacetHits = false);// Send total and facet hit counts to subprocesses
  void StartStop(float appTime,size_t sMode);    // Switch running/stopped
#endif

#ifdef SYNRAD
  SynradGeometry* GetSynradGeometry();
  void AddMaterial(std::string *fileName);
  void ClearRegions();
  //Different signature:
  void SendToHitBuffer();// Send total and facet hit counts to subprocesses
  void StartStop(float appTime);    // Switch running/stopped
#endif

  // Global simulation parameters
  OntheflySimulationParams ontheflyParams;
  WorkerParams wp;
  GlobalHitBuffer globalHitCache;
  


  FacetHistogramBuffer globalHistogramCache;

  bool   isRunning;           // Started/Stopped state
  float  startTime;         // Start time
  float  stopTime;          // Stop time
  float  simuTime;          // Total simulation time

  char fullFileName[512]; // Current loaded file

  bool needsReload;
  bool abortRequested;

  bool calcAC; //Not used in Synrad, kept for ResetStatsAndHits function shared with Molflow

#ifdef MOLFLOW

  std::vector<Parameter> parameters;
  int displayedMoment;

  std::vector<std::vector<std::pair<double, double>>> CDFs; //cumulative distribution function for each temperature
  std::vector<std::vector<std::pair<double, double>>> IDs; //integrated distribution function for each time-dependent desorption type
  std::vector<double> temperatures; //keeping track of all temperatures that have a CDF already generated
  std::vector<double> moments;             //moments when a time-dependent simulation state is recorded
  std::vector<size_t> desorptionParameterIDs; //time-dependent parameters which are used as desorptions, therefore need to be integrated
  std::vector<std::string> userMoments;    //user-defined text values for defining time moments (can be time or time series)


  size_t    calcACprg;         // AC matrix progress
#endif

#ifdef SYNRAD
	double totalFlux;         // Total desorbed Flux
	double totalPower;        // Total desorbed power

	double no_scans;           // = nbDesorption/nbTrajPoints. Stored separately for saving/loading

	std::vector<Region_full> regions;
	std::vector<Material> materials;
	std::vector<std::vector<double>> psi_distro; //psi-energy map for full (par+ort) polarization
	std::vector<std::vector<double>> parallel_polarization; //ratio of parallel polarization for a given E/E_crit ratio and psi vertical angle
	std::vector<std::vector<std::vector<double>>> chi_distros; //3 psi-chi    maps for full/parallel/orthogonal polarizations
#endif
	/*
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(
			CEREAL_NVP(wp),
			CEREAL_NVP(ontheflyParams),
			CEREAL_NVP(CDFs),
			CEREAL_NVP(IDs),
			CEREAL_NVP(parameters),
			CEREAL_NVP(temperatures),
			CEREAL_NVP(moments),
			CEREAL_NVP(desorptionParameterIDs)
		);
	}
	*/
private:

  // Process management
  DWORD  pID[MAX_PROCESS];
  DWORD  pid;
  bool   allDone;

  // Dataport handles and names
  Dataport *dpControl;
  Dataport *dpHit;
  Dataport *dpLog;

  char      ctrlDpName[32];
  char      loadDpName[32];
  char      hitsDpName[32];
  char      logDpName[32];


  // Methods
  bool ExecuteAndWait(int command, size_t waitState, size_t param = 0);
  bool Wait(size_t waitState, LoadStatus *statusWindow);
  void ResetWorkerStats();
  void ClearHits(bool noReload);
  char *GetErrorDetails();
  void ThrowSubProcError(std::string message);
  void ThrowSubProcError(char *message = NULL);
  void Start();
  void Stop();
  void InnerStop(float appTime);

  // Geometry handle
#ifdef MOLFLOW
  MolflowGeometry *geom;
#endif
#ifdef SYNRAD
  SynradGeometry *geom;
  Dataport *dpMat;
  char      materialsDpName[32];
#endif
};