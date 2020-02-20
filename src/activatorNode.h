#pragma once
/*

@info:
activator: This is an activator node plugin.
@author: Alexei Gaidachev
@date: February 15, 2017
@versions:
1.0.0 - February 15, 2017:
initial commit.

*/

// classify header
#ifndef activatorNode_H
#define activatorNode_H

// import modules
#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

// define constructor
class activatorNode : public MPxNode
{
public:
	static void* creator();
	activatorNode();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& dataBlock);

public:
	static MTypeId typeId;
	// define the distance float value
	static MObject outputDistance;
	static MObject outputBarycentric;
	static MObject maximumDistance;

	// define translates values
	static MObject inputVectorX;
	static MObject inputVectorY;
	static MObject inputVectorZ;
	static MObject inputVector;

	// define origin vector
	static MObject inputOriginX;
	static MObject inputOriginY;
	static MObject inputOriginZ;
	static MObject inputOrigin;

	// output vector
	static MObject outputX;
	static MObject outputY;
	static MObject outputZ;
	static MObject output;
};

#endif