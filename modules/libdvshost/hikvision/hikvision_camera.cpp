#include "hikvision_camera.h"
using namespace module::dvs;

HikvisionCamera::HikvisionCamera(
	const int idx /* = -1 */, 
	const CameraType ct/* = CameraType::CAMERA_TYPE_ANALOGY*/)
	: Camera(), cameraIndex{idx}, cameraType{ct}
{}

HikvisionCamera::~HikvisionCamera()
{}
