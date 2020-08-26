#ifndef _ZONE_TOOLS
#define _ZONE_TOOLS

#include <limits>

using namespace cv;

typedef std::vector<Vec3d> ZoneInfo;


// ----- HELPERS -------------------------------------------------

bool AreAlmostEqual(const Vec3d& v1, const Vec3d& v2, double epsilon = 1e-4)
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

bool IsSea(const Vec3d& colour, const ZoneInfo& zone_info, double epsilon = 1e-4)
{
	return AreAlmostEqual(colour, zone_info[0], epsilon);
}

bool IsLand(const Vec3d& colour, const ZoneInfo& zone_info, double epsilon = 1e-4)
{
	for (size_t i = 1; i < zone_info.size(); i++)
	{
		if (AreAlmostEqual(colour, zone_info[i], epsilon))
		{
			return true;
		}
	}
	   
	return false;
}

double EucDist(const double& x1, const double& y1, const double& x2, const double& y2)
{
	auto dx = x1 - x2;
	auto dy = y1 - y2;
	return sqrt((dx * dx) + (dy * dy));
}

double EucDist(const Vec3f& v1, const Vec3f& v2)
{
	auto dx = v1[0] - v2[0];
	auto dy = v1[1] - v2[1];
	auto dz = v1[2] - v2[2];
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}



// --------------------------------------------------------------------------

size_t FindZoneIdFromColour(const Vec3d& colour, const ZoneInfo& zone_info)
{
	size_t zone = 0;
	double min_dist = std::numeric_limits<double>::max();

	for (size_t zone_id = 1; zone_id < zone_info.size(); zone_id++)
	{
		auto d = EucDist(colour, zone_info[zone_id]);
		if (d < min_dist)
		{
			min_dist = d;
			zone = zone_id;
		}
	}

	return zone;
}


size_t FindZone(const cv::Mat& m, const int& row, const int& col, const double& width, const ZoneInfo& zone_info)
{
	int rows = m.rows;
	int cols = m.cols;

	double nearest_land_distance = std::numeric_limits<double>::max();
	int nearest_land_row = 0;
	int nearest_land_col = 0;

	int row_start = std::max<int>(row - (int)width, 0);
	int row_end	= std::min<int>(row + (int)width, rows-1);

	int col_start = std::max<int>(col - (int)width, 0);
	int col_end = std::min<int>(col + (int)width, cols-1);

	for (auto r = row_start; r <= row_end; r++)
	{
		for (auto c = col_start; c <= col_end; c++)
		{
			if (IsLand(m.at<Vec3d>(r, c), zone_info, 20))
			{
				auto d = EucDist(row, col, r, c);
				if (d < nearest_land_distance)
				{
					nearest_land_distance = d;
					nearest_land_row = r;
					nearest_land_col = c;
				}
			}
		}
	}

	if(nearest_land_distance > width)
	{
		return 0;
	}

	auto zone = FindZoneIdFromColour(m.at<Vec3d>(nearest_land_row, nearest_land_col), zone_info);
	return zone;
}


Mat CalcZones(const cv::Mat& m_in, const double& width, const ZoneInfo& zone_info) 
{ 
	Mat m;
	m_in.convertTo(m, CV_64F);
	Mat m_out(m_in);

	size_t rows = m_in.rows;
	size_t cols = m_in.cols;
	auto sea_colour = zone_info[0];
	auto num_zones = zone_info.size() - 1;

	for (auto row = 0; row < rows; row++)
	{
		for (auto col = 0; col < cols; col++)
		{
			auto colour = m.at<Vec3d>(row, col);
			if (IsSea(colour, zone_info, 20))
			{
				auto zone_id = FindZone(m, row, col, width, zone_info);
				if (zone_id > 0)
				{
					m_out.at<Vec3b>(row, col) = { 0xFF, (uchar)(zone_info[zone_id][1]), (uchar)(zone_info[zone_id][2]) };
				}
			}
		}
	}

	return m_out; 
}

#endif