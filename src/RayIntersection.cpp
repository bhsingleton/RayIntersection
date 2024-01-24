//
// File: RayIntersection.cpp
//
// Dependency Graph Node: rayIntersection
//
// Author: Benjamin H. Singleton
//

#include "RayIntersection.h"

MObject		RayIntersection::inMesh;
MObject		RayIntersection::origin;
MObject		RayIntersection::originX;
MObject		RayIntersection::originY;
MObject		RayIntersection::originZ;
MObject		RayIntersection::direction;
MObject		RayIntersection::directionX;
MObject		RayIntersection::directionY;
MObject		RayIntersection::directionZ;
MObject		RayIntersection::testBothDirections;

MObject		RayIntersection::hit;
MObject		RayIntersection::hitFaceIndex;
MObject		RayIntersection::hitTriangleIndex;
MObject		RayIntersection::hitLocation;
MObject		RayIntersection::hitLocationX;
MObject		RayIntersection::hitLocationY;
MObject		RayIntersection::hitLocationZ;
MObject		RayIntersection::hitNormal;
MObject		RayIntersection::hitNormalX;
MObject		RayIntersection::hitNormalY;
MObject		RayIntersection::hitNormalZ;
MObject		RayIntersection::hitParameter;
MObject		RayIntersection::hitParameterU;
MObject		RayIntersection::hitParameterV;

MString     RayIntersection::inputCategory("Input");
MString     RayIntersection::outputCategory("Output");

MTypeId     RayIntersection::id(0x0013b1e7);


RayIntersection::RayIntersection() {}
RayIntersection::~RayIntersection() {}


MStatus RayIntersection::compute(const MPlug& plug, MDataBlock& data)
/**
This method should be overridden in user defined nodes.
Recompute the given output based on the nodes inputs.
The plug represents the data value that needs to be recomputed, and the data block holds the storage for all of the node's attributes.
The MDataBlock will provide smart handles for reading and writing this node's attribute values.
Only these values should be used when performing computations!

@param plug: Plug representing the attribute that needs to be recomputed.
@param data: Data block containing storage for the node's attributes.
@return: Return status.
*/
{

	MStatus status;
	
	// Check requested attribute
	//
	MObject attribute = plug.attribute(&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MFnAttribute fnAttribute(attribute, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (fnAttribute.hasCategory(RayIntersection::outputCategory))
	{

		// Get input handles
		//
		MDataHandle inMeshHandle = data.inputValue(RayIntersection::inMesh, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle originHandle = data.inputValue(RayIntersection::origin, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle originXHandle = originHandle.child(RayIntersection::originX);
		MDataHandle originYHandle = originHandle.child(RayIntersection::originY);
		MDataHandle originZHandle = originHandle.child(RayIntersection::originZ);

		MDataHandle directionHandle = data.inputValue(RayIntersection::direction, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle directionXHandle = directionHandle.child(RayIntersection::directionX);
		MDataHandle directionYHandle = directionHandle.child(RayIntersection::directionY);
		MDataHandle directionZHandle = directionHandle.child(RayIntersection::directionZ);

		MDataHandle testBothDirectionsHandle = data.inputValue(RayIntersection::testBothDirections, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Get input values
		//
		MDistance::Unit distanceUnit = MDistance::internalUnit();

		MObject inMesh = inMeshHandle.asMesh();

		MPoint origin = MPoint(
			originXHandle.asDistance().asUnits(distanceUnit),
			originYHandle.asDistance().asUnits(distanceUnit),
			originZHandle.asDistance().asUnits(distanceUnit)
		);

		MVector direction = MVector(
			directionXHandle.asDouble(), 
			directionYHandle.asDouble(), 
			directionZHandle.asDouble()
		);

		bool testBothDirections = testBothDirectionsHandle.asBool();

		// Initialize mesh function set and accelerator
		//
		MFnMesh fnMesh(inMesh, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MMeshIsectAccelParams intersectAccel;
		intersectAccel = fnMesh.autoUniformGridParams();

		// Perform intersection
		//
		MFloatPoint raySource(origin);
		MFloatVector rayDirection(direction);

		MFloatPoint hitLocation = MFloatPoint::origin;
		MVector hitNormal = MFloatVector::zero;
		float2 hitParameter = {0.0f, 0.0f};

		int hitFaceIndex = 0;
		int hitTriangleIndex = 0;

		bool hit = fnMesh.closestIntersection(
			raySource, 
			rayDirection, 
			NULL,
			NULL, 
			true, 
			MSpace::kWorld, 
			1.0f, 
			testBothDirections,
			&intersectAccel, 
			hitLocation,
			NULL, 
			&hitFaceIndex,
			&hitTriangleIndex,
			NULL, 
			NULL, 
			1e-6f, 
			&status 
		);

		CHECK_MSTATUS_AND_RETURN_IT( status );

		// Evaluate results
		//
		if (hit)
		{

			// Get current UV set
			//
			MString currentUVSet = fnMesh.currentUVSetName(&status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			// Get hit normal
			//
			status = fnMesh.getPolygonNormal(hitFaceIndex, hitNormal, MSpace::kWorld);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			// Get hit UV parameters
			//
			status = fnMesh.getUVAtPoint(MPoint(hitLocation), hitParameter, MSpace::kWorld, &currentUVSet, &hitFaceIndex);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}

		// Get output handles
		//
		MDataHandle hitHandle = data.outputValue(RayIntersection::hit, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitFaceIndexHandle = data.outputValue(RayIntersection::hitFaceIndex, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitTriangleIndexHandle = data.outputValue(RayIntersection::hitTriangleIndex, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitLocationXHandle = data.outputValue(RayIntersection::hitLocationX, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitLocationYHandle = data.outputValue(RayIntersection::hitLocationY, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitLocationZHandle = data.outputValue(RayIntersection::hitLocationZ, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitNormalXHandle = data.outputValue(RayIntersection::hitNormalX, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitNormalYHandle = data.outputValue(RayIntersection::hitNormalY, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitNormalZHandle = data.outputValue(RayIntersection::hitNormalZ, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitParameterUHandle = data.outputValue(RayIntersection::hitParameterU, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MDataHandle hitParameterVHandle = data.outputValue(RayIntersection::hitParameterV, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Update data handles
		//
		hitHandle.setBool(hit);
		hitHandle.setClean();

		hitFaceIndexHandle.setInt(hitFaceIndex);
		hitFaceIndexHandle.setClean();

		hitTriangleIndexHandle.setInt(hitTriangleIndex);
		hitTriangleIndexHandle.setClean();

		hitLocationXHandle.setMDistance(MDistance(hitLocation.x, distanceUnit));
		hitLocationXHandle.setClean();

		hitLocationYHandle.setMDistance(MDistance(hitLocation.y, distanceUnit));
		hitLocationYHandle.setClean();

		hitLocationZHandle.setMDistance(MDistance(hitLocation.z, distanceUnit));
		hitLocationZHandle.setClean();

		hitNormalXHandle.setDouble(hitNormal.x);
		hitNormalXHandle.setClean();

		hitNormalYHandle.setDouble(hitNormal.y);
		hitNormalYHandle.setClean();

		hitNormalZHandle.setDouble(hitNormal.z);
		hitNormalZHandle.setClean();

		hitParameterUHandle.setFloat(hitParameter[0]);
		hitParameterUHandle.setClean();

		hitParameterVHandle.setFloat(hitParameter[1]);
		hitParameterVHandle.setClean();

		// Mark plug as clean
		//
		status = data.setClean(plug);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return MS::kSuccess;

	} 
	else 
	{

		return MS::kUnknownParameter;

	}

}



void* RayIntersection::creator()
/**
This function is called by Maya when a new instance is requested.
See pluginMain.cpp for details.

@return: RayIntersection
*/
{

	return new RayIntersection();

}


MStatus RayIntersection::initialize()
/**
This function is called by Maya after a plugin has been loaded.
Use this function to define any static attributes.

@return
*/
{

	MStatus status;

	// Initialize function sets
	//
	MFnNumericAttribute fnNumericAttr;
	MFnUnitAttribute fnUnitAttr;
	MFnTypedAttribute fnTypedAttr;

	// Input attributes:
	// ".inMesh" attribute
	// 
	RayIntersection::inMesh = fnTypedAttr.create("inMesh", "im", MFnData::kMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnTypedAttr.addToCategory(RayIntersection::inputCategory));

	// ".originX" attribute
	//
	RayIntersection::originX = fnUnitAttr.create("originX", "ox", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setKeyable(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::inputCategory));

	// ".originY" attribute
	//
	RayIntersection::originY = fnUnitAttr.create("originY", "oy", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setKeyable(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::inputCategory));

	// ".originZ" attribute
	//
	RayIntersection::originZ = fnUnitAttr.create("originZ", "oz", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setKeyable(true));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::inputCategory));

	// ".origin" attribute
	//
	RayIntersection::origin = fnNumericAttr.create("origin", "o", RayIntersection::originX, RayIntersection::originY, RayIntersection::originZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// ".directionX" attribute
	//
	RayIntersection::directionX = fnNumericAttr.create("directionX", "dx", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// ".directionY" attribute
	//
	RayIntersection::directionY = fnNumericAttr.create("directionY", "dy", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// ".directionZ" attribute
	//
	RayIntersection::directionZ = fnNumericAttr.create("directionZ", "dz", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// ".direction" attribute
	//
	RayIntersection::direction = fnNumericAttr.create("direction", "d", RayIntersection::directionX, RayIntersection::directionY, RayIntersection::directionZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setKeyable(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// ".testBothDirections" attribute
	//
	RayIntersection::testBothDirections = fnNumericAttr.create("testBothDirections", "tbd", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setChannelBox(true));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::inputCategory));

	// Output attributes:
	// ".hit" attribute
	//
	RayIntersection::hit = fnNumericAttr.create("hit", "h", MFnNumericData::kBoolean, false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitFaceIndex" attribute
	//
	RayIntersection::hitFaceIndex = fnNumericAttr.create("hitFaceIndex", "hfi", MFnNumericData::kLong, 0u, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitTriangleIndex" attribute
	//
	RayIntersection::hitTriangleIndex = fnNumericAttr.create("hitTriangleIndex", "hti", MFnNumericData::kLong, 0u, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitLocationX" attribute
	//
	RayIntersection::hitLocationX = fnUnitAttr.create("hitLocationX", "hlx", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitLocationY" attribute
	//
	RayIntersection::hitLocationY = fnUnitAttr.create("hitLocationY", "hly", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitLocationZ" attribute
	//
	RayIntersection::hitLocationZ = fnUnitAttr.create("hitLocationZ", "hlz", MFnUnitAttribute::kDistance, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnUnitAttr.setWritable(false));
	CHECK_MSTATUS(fnUnitAttr.setStorable(false));
	CHECK_MSTATUS(fnUnitAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitLocation" attribute
	//
	RayIntersection::hitLocation = fnNumericAttr.create("hitLocation", "hl", RayIntersection::hitLocationX, RayIntersection::hitLocationY, RayIntersection::hitLocationZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitNormalX" attribute
	//
	RayIntersection::hitNormalX = fnNumericAttr.create("hitNormalX", "hnx", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitNormalY" attribute
	//
	RayIntersection::hitNormalY = fnNumericAttr.create("hitNormalY", "hny", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitNormalZ" attribute
	//
	RayIntersection::hitNormalZ = fnNumericAttr.create("hitNormalZ", "hnz", MFnNumericData::kDouble, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitNormal" attribute
	//
	RayIntersection::hitNormal = fnNumericAttr.create("hitNormal", "hn", RayIntersection::hitNormalX, RayIntersection::hitNormalY, RayIntersection::hitNormalZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitParameterU" attribute
	//
	RayIntersection::hitParameterU = fnNumericAttr.create("hitParameterU", "hpu", MFnNumericData::kFloat, 0.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitParameterV" attribute
	//
	RayIntersection::hitParameterV = fnNumericAttr.create("hitParameterV", "hpv", MFnNumericData::kFloat, 0.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// ".hitParameter" attribute
	//
	RayIntersection::hitParameter = fnNumericAttr.create("hitParameter", "hp", RayIntersection::hitParameterU, RayIntersection::hitParameterV, MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	CHECK_MSTATUS(fnNumericAttr.setWritable(false));
	CHECK_MSTATUS(fnNumericAttr.setStorable(false));
	CHECK_MSTATUS(fnNumericAttr.addToCategory(RayIntersection::outputCategory));

	// Add attributes to node
	//
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::inMesh));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::origin));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::direction));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::testBothDirections));

	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hit));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hitFaceIndex));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hitTriangleIndex));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hitLocation));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hitNormal));
	CHECK_MSTATUS(RayIntersection::addAttribute(RayIntersection::hitParameter));

	// Define attribute relationships
	//
	MObject attributes[6] = { RayIntersection::hit, RayIntersection::hitFaceIndex, RayIntersection::hitTriangleIndex, RayIntersection::hitLocation, RayIntersection::hitNormal, RayIntersection::hitParameter };

	for (MObject attribute : attributes)
	{

		CHECK_MSTATUS(RayIntersection::attributeAffects(RayIntersection::inMesh, attribute));
		CHECK_MSTATUS(RayIntersection::attributeAffects(RayIntersection::origin, attribute));
		CHECK_MSTATUS(RayIntersection::attributeAffects(RayIntersection::direction, attribute));
		CHECK_MSTATUS(RayIntersection::attributeAffects(RayIntersection::testBothDirections, attribute));

	}

	return status;

}