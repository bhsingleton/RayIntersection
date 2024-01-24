cmake_minimum_required(VERSION 3.21)
project(RayIntersection)
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(
	SOURCE_FILES
	"pluginMain.cpp"
	"RayIntersection.h"
	"RayIntersection.cpp"
)

set(
	LIBRARIES
	OpenMaya
	OpenMayaAnim
	Foundation
)

build_plugin()