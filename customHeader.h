#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <io.h>

// Azure Kinect SDK c++ header
#include "k4a/k4a.hpp"

// OpenGL graphics library
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

// GUI programming library, based on OpenGL
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

// CSV file write function
void write_csv_file(const char* file_name, const k4a::image src)
{
	FILE* fp = 0;
#ifdef _MSC_VER
	fopen_s(&fp, file_name, "w");
#else
	fp = fopen(file_name, "w");
#endif

	int width = src.get_width_pixels();
	int height = src.get_height_pixels();
	uint16_t* src_data = (uint16_t*)(void*)src.get_buffer();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			fprintf(fp, "%d", src_data[y * width + x]);
			if (x < width - 1)
			{
				fprintf(fp, ",");
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}