/*
Program:     ContaminationFlow
Description: Monte Carlo simulator for satellite contanimation studies
Authors:     Rudolf Sch�nmann / Hoai My Van
Copyright:   TU Munich
Forked from: Molflow (CERN) (https://cern.ch/molflow)

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

#include "GLApp/GLWindow.h"
class GLWindow;
class GLButton;
class GLTextField;
class GLLabel;

//#include "Geometry_shared.h"
//#include "Worker.h"
class Geometry;
class Worker;

class AddVertex : public GLWindow {

public:

  // Construction
  AddVertex(Geometry *geom,Worker *work);

  // Implementation
  void ProcessMessage(GLComponent *src,int message);

private:

  Geometry     *geom;
  Worker	   *work;

  GLButton    *addButton;
  GLButton    *facetCenterButton;
  GLButton    *facetUButton;
  GLButton    *facetVButton;
  GLButton    *facetNormalButton;
  GLTextField *x;
  GLTextField *y;
  GLTextField *z;

};