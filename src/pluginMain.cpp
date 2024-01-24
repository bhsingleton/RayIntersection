//
// File: pluginMain.cpp
//
// Author: Benjamin H. Singleton
//

#include "RayIntersection.h"
#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj)
{

	MStatus status;

	MFnPlugin plugin(obj, "Ben Singleton", "2017", "Any");
	status = plugin.registerNode("rayIntersection", RayIntersection::id, RayIntersection::creator, RayIntersection::initialize);

	if (!status)
	{

		status.perror("registerNode");
		return status;

	}

	return status;

};


MStatus uninitializePlugin(MObject obj)
{

	MStatus status;

	MFnPlugin plugin(obj);
	status = plugin.deregisterNode(RayIntersection::id);

	if (!status)
	{

		status.perror("deregisterNode");
		return status;

	}

	return status;

};