#include "customHeader.h"

int main()
{
	// Azure Kinect releated variables
	k4a_device_configuration_t deviceConfig;
	k4a::device kinectDevice{};
	k4a::capture kinectCapture{};
	k4a::image depthImage{};

	// Time information related variables
	time_t currentTime{};
	struct tm tmCurrentTime{};
	clock_t msecTime{};
	errno_t error;

	// File I/O releated variables
	char fCharName[50]{};
	std::string instantFolderName{};
	std::string instantFileName{};
	
	// Device configuration
	deviceConfig.camera_fps = K4A_FRAMES_PER_SECOND_30;
	deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
	deviceConfig.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_720P;
	deviceConfig.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
	deviceConfig.subordinate_delay_off_master_usec = 0;
	deviceConfig.depth_delay_off_color_usec = 30000;

	// This is option for multu-camera setting
	//deviceConfig.synchronized_images_only = true;

	// if opening Azure Kinect device failed, throw a error
	try
	{
		// Open Azure Kinect device
		kinectDevice = k4a::device::open(0);
	}
	catch (std::exception e)
	{
		std::cout << "------------------- Error Message ----------------------" << std::endl;
		std::cout << "Device open failed! Please check the device connection." << std::endl;
		return -1;
	}
	
	// Azure Kinect start
	kinectDevice.start_cameras(&deviceConfig);

	// folder creation, name is today (month,day)
	currentTime = time(NULL);
	error = localtime_s(&tmCurrentTime, &currentTime);
	if (error != 0)
	{
		std::cout << "Getting current time information failed!" << std::endl;
	}
	instantFolderName = ".\\experimentRecord_" + std::to_string(tmCurrentTime.tm_mon) + std::to_string(tmCurrentTime.tm_mday);
	strcpy_s(fCharName, instantFolderName.c_str());

	// if folder exists, just pass
	if (_access(fCharName, 0) == -1)
	{
		CreateDirectory(fCharName, NULL);
	}
	else if (_access(fCharName, 0) == 0)
	{
		std::cout << "folder name " << instantFolderName << " already exists. Creating folder aborted. " << std::endl;
		std::cout << "Starting to create csv file..." << std::endl;
	}
	
	while (true)
	{
		// get capture data from camera
		// capture data has color image data and depth image data
		kinectDevice.get_capture(&kinectCapture);

		// get only depth image data from capture data
		depthImage = kinectCapture.get_depth_image();

		// get current time information for file name generation
		currentTime = time(NULL);
		error = localtime_s(&tmCurrentTime, &currentTime);
		if (error != 0)
		{
			std::cout << "Getting current time failed!" << std::endl;
		}

		// Getting miliseconds from program start point
		msecTime = clock();
		
		// File name is timeLine_(hour)(minute)(second)_(msec from start).csv
		// tm_mon indicates month has 0~11 range, tm_mday indicates day has 1~31 range, tm_hour indicates hour has 0~23 range, tm_min indicates minute has 0~59 range, tm_sec indicates seconds has 0~60 range.
		instantFileName = instantFolderName + "\\timeLine_" + std::to_string(tmCurrentTime.tm_hour) + std::to_string(tmCurrentTime.tm_min)
			+ std::to_string(tmCurrentTime.tm_sec) + "_" + std::to_string(msecTime) + ".csv";
		
		write_csv_file(instantFileName.c_str(), depthImage);

	}

	// After operation, camera need to be stopped and closed safely
	kinectDevice.stop_cameras();
	kinectDevice.close();
	
	return 0;
}