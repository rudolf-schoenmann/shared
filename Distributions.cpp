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
#include "Distributions.h"
#include "GLApp/MathTools.h"
#include <vector>

std::vector<double> DistributionND::InterpolateY(const double & x, const bool & allowExtrapolate)
{
	return InterpolateVectorY(x, values, isLogLog, allowExtrapolate);
}

double DistributionND::InterpolateX(const double & y, const size_t & elementIndex, const bool & allowExtrapolate)
{
	return InterpolateVectorX(y, values, elementIndex, isLogLog, allowExtrapolate);
}

double Distribution2D::InterpolateY(const double &x, const bool& allowExtrapolate) const {
	return InterpolateXY(x, values, true, isLogLog, allowExtrapolate);
}

double Distribution2D::InterpolateX(const double &y, const bool& allowExtrapolate) const {
	return InterpolateXY(y, values, false, isLogLog, allowExtrapolate);
}

