/*

@info:
	A simple test plugin describing a vector.

*/
#include "activatorNode.h"
#include <maya/MGlobal.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFloatVector.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MDataHandle.h>
#include <maya/MPoint.h>
#include <math.h>

using namespace std;

MTypeId activatorNode::typeId(0x80001);

// declare objects
MObject activatorNode::inputVector;
MObject activatorNode::inputVectorX;
MObject activatorNode::inputVectorY;
MObject activatorNode::inputVectorZ;
MObject activatorNode::inputOrigin;
MObject activatorNode::inputOriginX;
MObject activatorNode::inputOriginY;
MObject activatorNode::inputOriginZ;
MObject activatorNode::output;
MObject activatorNode::outputX;
MObject activatorNode::outputY;
MObject activatorNode::outputZ;
MObject activatorNode::outputDistance;

// class constructor
// useful when you want add variables and initialize them
activatorNode::activatorNode()
{
}

// change the name of the class we are creating
void* activatorNode::creator()
{
	return new activatorNode();
}


MStatus activatorNode::initialize()
{
	// initialize Maya attributes
	MFnNumericAttribute numFn;
	MFnTypedAttribute typedFn;
	MFnCompoundAttribute compound;
 	MFnUnitAttribute uAttr;

	// define input translates
	inputVectorX = numFn.create("inputVectorX", "ivx", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputVectorX);

	inputVectorY = numFn.create("inputVectorY", "ivy", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputVectorY);

	inputVectorZ = numFn.create("inputVectorZ", "ivz", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputVectorZ);

	inputVector = compound.create("inputVector", "inv");
	compound.addChild(inputVectorX);
	compound.addChild(inputVectorY);
	compound.addChild(inputVectorZ);
	compound.setStorable(true);
	compound.setKeyable(true);
	compound.setWritable(true);
	addAttribute(inputVector);

	// define input origin
	inputOriginX = numFn.create("inputOriginX", "inox", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputOriginX);

	inputOriginY = numFn.create("inputOriginY", "inoy", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputOriginY);

	inputOriginZ = numFn.create("inputOriginZ", "inoz", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(inputOriginZ);

	inputOrigin = compound.create("inputOrigin", "ino");
	compound.addChild(inputOriginX);
	compound.addChild(inputOriginY);
	compound.addChild(inputOriginZ);
	compound.setStorable(true);
	compound.setKeyable(true);
	compound.setWritable(true);
	addAttribute(inputOrigin);

	// define output vector
	outputX = numFn.create("outputX", "outx", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(false);
	numFn.setKeyable(false);
	numFn.setWritable(false);
	addAttribute(outputX);

	outputY = numFn.create("outputY", "outy", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(false);
	numFn.setKeyable(false);
	numFn.setWritable(false);
	addAttribute(outputY);

	outputZ = numFn.create("outputZ", "outz", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(false);
	numFn.setKeyable(false);
	numFn.setWritable(false);
	addAttribute(outputZ);

	output = compound.create("outputVector", "outv");
	compound.addChild(outputX);
	compound.addChild(outputY);
	compound.addChild(outputZ);
	compound.setStorable(false);
	compound.setKeyable(false);
	compound.setWritable(false);
	addAttribute(output);

 	// define the output distance
 	outputDistance = numFn.create("outputDistance", "od", MFnNumericData::kDouble, 0.0);
	uAttr.setStorable(false);
	uAttr.setKeyable(false);
	uAttr.setWritable(false);
	addAttribute(outputDistance);

	// make attribute connections
	attributeAffects(inputOrigin, output);
	attributeAffects(inputVector, output);
	attributeAffects(inputVector, outputDistance);
	return MS::kSuccess;
}


MStatus activatorNode::compute(const MPlug& plug, MDataBlock& dataBlock)
{
	// get data
	if ((plug == output) || (plug == outputX) || (plug == outputY) || (plug == outputZ) || (plug == outputDistance))
	{
		// extract input data
		MVector inOriginVector = dataBlock.inputValue(inputOrigin).asVector();
		MVector inVector = dataBlock.inputValue(inputVector).asVector();

		// compute needed vectors and normalize
		MVector outputResultVector = inVector - inOriginVector;
		outputResultVector += inOriginVector;
		// outputResultVector.normalize();
		// compute distance between the origin and target vector
		double distance;
		double array[3] = {};
		array[0] = pow(outputResultVector.x, 2);
		array[1] = pow(outputResultVector.y, 2);
		array[2] = pow(outputResultVector.z, 2);

		// calculate the sum
		int i;
		double sum = 0;
		for(i=0; i<3; i++){
			sum += array[i];
		}
		distance = pow(sum, 0.5);

		// set output values
		dataBlock.outputValue(output).set(outputResultVector.x, outputResultVector.y, outputResultVector.z);
		dataBlock.outputValue(outputDistance).set(distance);

		dataBlock.outputValue(output).setClean();
		dataBlock.outputValue(outputDistance).setClean();
		dataBlock.setClean(plug);
	};

	return MS::kSuccess;
}