#include "activatorNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj, "Alexei Gaidachev", "1.0", "Any");

	status = fnPlugin.registerNode(
		"activatorNode",
		activatorNode::typeId,
		&activatorNode::creator,
		&activatorNode::initialize
	);

	if (status != MS::kSuccess)
		status.perror("Could not register the activator node");

	return status;
}

MStatus uninitializePlugin(MObject object)
{
	MFnPlugin pluginFn;
	pluginFn.deregisterNode(activatorNode::typeId);
	return MS::kSuccess;
}