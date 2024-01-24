#ifndef _RAY_INTERSECTION_NODE
#define _RAY_INTERSECTION_NODE
//
// File: RayIntersection.h
//
// Dependency Graph Node: rayIntersection
//
// Author: Benjamin H. Singleton
//

#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDistance.h>
#include <maya/MVector.h>
#include <maya/MFloatVector.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MTypeId.h> 
#include <maya/MGlobal.h>

 
class RayIntersection : public MPxNode
{

	public:

							RayIntersection();
		virtual				~RayIntersection();

		virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

		static  void*		creator();
		static  MStatus		initialize();

	public:

		static  MObject		inMesh;
		static  MObject		origin;
		static  MObject		originX;
		static  MObject		originY;
		static  MObject		originZ;
		static  MObject		direction;
		static  MObject		directionX;
		static  MObject		directionY;
		static  MObject		directionZ;
		static	MObject		testBothDirections;

		static  MObject		hit;
		static  MObject		hitFaceIndex;
		static  MObject		hitTriangleIndex;
		static  MObject		hitLocation;
		static  MObject		hitLocationX;
		static  MObject		hitLocationY;
		static  MObject		hitLocationZ;
		static  MObject		hitNormal;
		static  MObject		hitNormalX;
		static  MObject		hitNormalY;
		static  MObject		hitNormalZ;
		static  MObject		hitParameter;
		static  MObject		hitParameterU;
		static  MObject		hitParameterV;

		static	MString		inputCategory;
		static	MString		outputCategory;

		static	MTypeId		id;

};

#endif