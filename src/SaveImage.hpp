#pragma once
#include <vector>
#include <cstdint>
#include <opencv2/opencv.hpp>


class SaveImage
{
	struct RGBColor {
		uint8_t R;
		uint8_t G;	
		uint8_t B;

		RGBColor(uint8_t r_, uint8_t g_, uint8_t b_)
			: R(r_)
			, G(g_)
			, B(b_)
		{
		}
	};

public:
  SaveImage()
  {
		int r, g, b;
		std::string line;

		std::ifstream inFile("..\\..\\..\\support\\Rainbow.col");
		std::getline(inFile, line);
		
		while (inFile >> r >> g >> b) {
			m_colors.emplace_back(r, g, b);
		}

		inFile.close();

		int width{ 200 };
		int height{ 100 };
		cv::Mat mat(height, width, CV_8UC3);

		unsigned char maxRbgVal = 255;

		for (int y{}; y < height; y++) {
			for (int x{}; x < width; x++) {
				uint8_t normalizedValue(0);

				RGBColor color = m_colors[normalizedValue];
				cv::Vec3b& bgra = mat.at<cv::Vec3b>(y, x);
				bgra[0] = color.B;
				bgra[1] = color.G;
				bgra[2] = color.R;
			}
		}

		std::vector<int> compressionParams;
		compressionParams.push_back(cv::IMWRITE_PNG_COMPRESSION);
		compressionParams.push_back(9);

		auto result = imwrite("output.png", mat, compressionParams);
  }

  ~SaveImage() = default;
  
private:
	std::vector<RGBColor> m_colors;
};
