#ifndef _Control_h_
#define _Control_h_
// Control.h:
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
* Copyright (c) 2005, Stanford University. All rights reserved. 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met: 
*  - Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer. 
*  - Redistributions in binary form must reproduce the above copyright 
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the distribution. 
*  - Neither the name of the Stanford University nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission. 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

/* Note: This code was originally developed by Realistic Dynamics Inc. 
 * Author: Frank C. Anderson 
 */


// INCLUDES
#include <OpenSim/Simulation/rdSimulationDLL.h>
#include <OpenSim/Tools/Object.h>
#include <OpenSim/Tools/Array.h>
#include <OpenSim/Tools/PropertyBool.h>
#include <OpenSim/Tools/PropertyDbl.h>


//=============================================================================
//=============================================================================
/**
 * A class that represents a control in a dynamic simulation.
 *
 * This class is intended to be the base class for different types of controls,
 * so many of its methods are virtual.
 *
 * In general, a control consists of a set of parameters.  These parameters
 * are used to reconstruct a control curve.  For example, a control may be
 * represented by a consant, a series of step functions, a set of linearly
 * interpolated values, a set of spline control points, coefficients in
 * a Fourier series, etc.
 *
 * Because there is not necessarily a 1-to-1 correspondence between the
 * parameters used to represent a control curve and the value of the
 * control curve, there are two basic ways to access the content of a control:
 * getParameter() gets the value of a parameter, and getValue() gets the
 * value at a particular time.
 *
 * A distinction is also made between controls that control a model and
 * controls that control some other aspect of a simulation.  For example,
 * a control for the excitation level of a muscle is a "model"
 * control.  The value of this type of control is querried during the
 * course of a simualtion.  On the other hand, a control for
 * the final time of a simulation is not usually a "model" control.
 * Nor is a control for the initial value of a state variable, even if that
 * state variable is the initial value of a muscle activation.  These
 * "non-model" controls are used to set things before a simulation ever
 * begins and are not querried during the cours of a simulation.  The
 * number of model controls can be querried by a call to Model::getNX().
 *
 * @author Frank C. Anderson
 * @version 1.0
 */
namespace OpenSim { 

class RDSIMULATION_API Control : public Object
{

//=============================================================================
// MEMBER VARIABLES
//=============================================================================
protected:
	// PROPERTIES
	/** Flag that specifies whether or not this control is a model control. */
	PropertyBool _propIsModelControl;
	/** Flag that specifies whether or not this control should use
	extrapolation for times outside the time range of the control parameters. */
	PropertyBool _propExtrapolate;
	/** Default parameter minimum. */
	PropertyDbl _propDefaultMin;
	/** Default parameter maximum. */
	PropertyDbl _propDefaultMax;

	// REFERENCES TO PROPERTY VALUES
	/** Reference to the value of the IsModelControl property. */
	bool &_isModelControl;
	/** Reference to the value of the Extrapolate flag. */
	bool &_extrapolate;
	/** Reference to the value of the DefaultMin property. */
	double &_defaultMin;
	/** Reference to the value of the DefaultMax property. */
	double &_defaultMax;


//=============================================================================
// METHODS
//=============================================================================
public:
	Control(const char *aName="UNKOWN");
	Control(DOMElement *aElement);
	Control(const Control &aControl);
	virtual ~Control();
private:
	void setNull();
protected:
	virtual void setupProperties();
	void copyData(const Control &aControl);

	//--------------------------------------------------------------------------
	// OPERATORS
	//--------------------------------------------------------------------------
public:
#ifndef SWIG
	Control& operator=(const Control &aControl);
#endif
	//--------------------------------------------------------------------------
	// GET AND SET
	//--------------------------------------------------------------------------
	void setIsModelControl(bool aTrueFalse);
	bool getIsModelControl() const;
	void setExtrapolate(bool aTrueFalse);
	bool getExtrapolate() const;
	// PARAMETERS
	// Number
	virtual int getNumParameters() const = 0;
	// Default Parameter Min
	void setDefaultParameterMin(double aMin);
	double getDefaultParameterMin() const;
	// Default Parameter Max
	void setDefaultParameterMax(double aMin);
	double getDefaultParameterMax() const;
	// Parameter Min
	virtual void setParameterMin(int aI,double aMin) = 0;
	virtual double getParameterMin(int aI) const = 0;
	// Parameter Max
	virtual void setParameterMax(int aI,double aMax) = 0;
	virtual double getParameterMax(int aI) const = 0;
	// Parameter Neighborhood
	virtual double
		getParameterTime(int aI) const = 0;
	virtual void
		getParameterNeighborhood(int aI,double &rTLower,double &rTUpper) const=0;
	// Parmeter List
	virtual int
		getParameterList(double aT,Array<int> &rList) = 0;
	virtual int
		getParameterList(double aTLower,double aTUpper,Array<int> &rList) = 0;
	// Parameter Value
	virtual double getParameterValue(int aI) const = 0;
	virtual void setParameterValue(int aI,double aP) = 0;
	// Control Value
	virtual double getControlValue(double aT=0.0) = 0;
	virtual void setControlValue(double aT,double aX) = 0;
	virtual double getControlValueMin(double aT=0.0) = 0;
	virtual void setControlValueMin(double aT,double aX) = 0;
	virtual double getControlValueMax(double aT=0.0) = 0;
	virtual void setControlValueMax(double aT,double aX) = 0;
	// Convenience methods to get first and last time.
	virtual const double getFirstTime() const;
	virtual const double getLastTime() const;

	// UTILITY
	virtual void
		simplify(const PropertySet &aProperties);
	virtual void filter(double aT);

//=============================================================================
};	// END of class Control

}; //namespace
//=============================================================================
//=============================================================================

#endif // __Control_h__
