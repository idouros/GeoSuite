#ifndef _ZONE_TOOLS
#define _ZONE_TOOLS

using namespace cv;

typedef std::vector<Vec3f> ZoneInfo;

bool AreAlmostEqual(const Vec3f& v1, const Vec3f& v2, float epsilon = 1e-4)
{
	for (auto i = 0; i < 3; i++)
	{
		if (fabs(v1[i] - v2[i]) > epsilon)
		{
			return false;
		}
	}
	return true;
}




Mat CalcZones(const cv::Mat& m_in, const size_t& width, const ZoneInfo& zone_info) 
{ 
	Mat m;
	m_in.convertTo(m, CV_32F);
	Mat m_out(m_in);

	size_t rows = m_in.rows;
	size_t cols = m_in.cols;
	auto sea_colour = zone_info[0];
	auto num_zones = zone_info.size() - 1;

	for (auto row = 0; row < rows; row++)
	{
		for (auto col = 0; col < cols; col++)
		{
			auto colour = m.at<Vec3f>(row, col);
			if (AreAlmostEqual(colour, sea_colour, 150))
			{
				m_out.at<Vec3b>(row, col) = { '0', 'K', '0' };
			}
		}
	}

	return m_out; 
}

#endif