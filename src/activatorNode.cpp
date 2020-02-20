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
MObject activatorNode::outputBarycentric;
MObject activatorNode::maximumDistance;

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

	// define the maximum distance
	maximumDistance = numFn.create("maximumDistance", "maxd", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(true);
	numFn.setKeyable(true);
	numFn.setWritable(true);
	addAttribute(maximumDistance);

 	// define the output distance
 	outputDistance = numFn.create("outputDistance", "od", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(false);
	numFn.setKeyable(false);
	numFn.setWritable(false);
	addAttribute(outputDistance);

 	// define the output distance
 	outputBarycentric = numFn.create("outputBarycentric", "ob", MFnNumericData::kDouble, 0.0);
	numFn.setStorable(false);
	numFn.setKeyable(false);
	numFn.setWritable(false);
	addAttribute(outputBarycentric);

	// make attribute connections
	attributeAffects(inputOrigin, output);
	attributeAffects(inputVector, output);
	attributeAffects(inputVector, outputDistance);
	attributeAffects(inputOrigin, outputDistance);
	attributeAffects(maximumDistance, outputBarycentric);
	attributeAffects(inputVector, outputBarycentric);
	attributeAffects(inputOrigin, outputBarycentric);
	return MS::kSuccess;
}


MStatus activatorNode::compute(const MPlug& plug, MDataBlock& dataBlock)
{
	// get data
	if ((plug == output) || (plug == outputX) || (plug == outputY) || (plug == outputZ) || (plug == outputDistance) || (plug == outputBarycentric))
	{
		// initialize stored variables
		double outputBaryUnit;
		// double percent;
		double array[3] = {};
		int i;
		double sum = 0;

		// extract input data
		MVector inOriginVector = dataBlock.inputValue(inputOrigin).asVector();
		MVector inVector = dataBlock.inputValue(inputVector).asVector();
		double maxDistance = dataBlock.inputValue(maximumDistance).asDouble();

		// compute needed vectors and normalize
		MVector outputResultVector = inOriginVector - inVector;

		// compute distance between the origin and target vector using trigonometry
		array[0] = pow(outputResultVector.x, 2);
		array[1] = pow(outputResultVector.y, 2);
		array[2] = pow(outputResultVector.z, 2);

		// calculate the sum
		for(i = 0; i < 3; i++){
			sum += array[i];
		}

		// square root the sum
		double distance = pow(sum, 0.5);

		// calculate the barycentric delta: (start * percent) + (end * (1-percent))
		if (maxDistance > 0) {
			if (distance >= maxDistance) {
				distance = maxDistance;
			}
			outputBaryUnit = 1 - (distance / maxDistance);
			// outputBaryUnit = 0.0 + (maxDistance * (1.0 - percent));
		}


		// set output values
		dataBlock.outputValue(output).set(outputResultVector.x, outputResultVector.y, outputResultVector.z);
		dataBlock.outputValue(outputDistance).set(distance);
		dataBlock.outputValue(outputBarycentric).set(outputBaryUnit);

		dataBlock.outputValue(output).setClean();
		dataBlock.outputValue(outputDistance).setClean();
		dataBlock.setClean(plug);
	};

	return MS::kSuccess;
}