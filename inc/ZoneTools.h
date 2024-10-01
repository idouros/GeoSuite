#ifndef _ZONE_TOOLS
#define _ZONE_TOOLS

#include <limits>
#include <filesystem>
#include <boost/property_tree/ptree.hpp>

enum RETURN_CODE
{
	OK = 0,
	ERR_FILE_NOT_FOUND = -1,
	ERR_INVALID_ZONE_INFO = -2
};

std::string SEA_ZONE_NAME = "Sea";

using namespace cv;

typedef boost::property_tree::ptree ConfigParams;
typedef boost::property_tree::ptree ZoneInfo;
typedef std::vector<Vec3d> Territories;
typedef std::vector<Vec3d> Waters;

// ----- HELPERS -------------------------------------------------

std::string getFileParameter(const std::filesystem::path& configFilePath,
	const boost::property_tree::ptree& configParams,
	const std::string& key)
{
	auto configFileFolder = configFilePath.parent_path();
	return configFileFolder.append(configParams.get<std::string>(key)).string();
	// TODO: handle absolute paths (the above only deals with relatives to the config file location)
}

Vec3d PtreeToVec3d(const boost::property_tree::ptree& V)
{
	Vec3d result(0, 0, 0);
	int i = 0;
	for (auto it = V.begin(); it != V.end(); ++it) {
		result[i] = std::stof(it->second.data());
		i++;
	}
	return result;
}

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

bool IsSea(const Vec3d& colour, const Vec3d& sea_colour, double epsilon = 1e-4)
{
	return AreAlmostEqual(colour, sea_colour, epsilon);
}

bool IsLand(const Vec3d& colour, const Territories& territory_colours, double epsilon = 1e-4)
{

	for (size_t i = 1; i < territory_colours.size(); i++)
	{
		if (AreAlmostEqual(colour, territory_colours[i], epsilon))
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

size_t FindZoneIdFromColour(const Vec3d& colour, const Territories& territory_colours)
{
	size_t zone = 0;
	double min_dist = std::numeric_limits<double>::max();

	for (size_t zone_id = 1; zone_id < territory_colours.size(); zone_id++)
	{
		auto d = EucDist(colour, territory_colours[zone_id]);
		if (d < min_dist)
		{
			min_dist = d;
			zone = zone_id;
		}
	}

	return zone;
}


size_t FindZone(const cv::Mat& m, const int& row, const int& col, const double& width, const double& colour_epsilon, const Territories& territory_colours)
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
			if (IsLand(m.at<Vec3d>(r, c), territory_colours, colour_epsilon))
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

	auto zone = FindZoneIdFromColour(m.at<Vec3d>(nearest_land_row, nearest_land_col), territory_colours);
	return zone;
}


Mat CalcZones(const cv::Mat& m_in, const ConfigParams& configParams, const ZoneInfo& zone_info) 
{ 
	Mat m;
	m_in.convertTo(m, CV_64F);
	Mat m_out(m_in);

	size_t rows = m_in.rows;
	size_t cols = m_in.cols;

	Territories territories;
	Waters waters;
	Vec3d sea_colour;
	
	auto width = configParams.get<double>("zones.width");
	auto colour_epsilon = configParams.get<double>("zones.colour_epsilon");

	auto &zones = zone_info.get_child("Zones");
	size_t i = 0;
	for (auto zone = zones.begin(); zone != zones.end(); ++zone)
	{
		std::string zone_name = zone->first.data();
		auto zone_colours = zone->second;
		if (i == 0)
		{
			if (zone_name == SEA_ZONE_NAME) 
			{
				sea_colour = PtreeToVec3d(zone_colours.get_child("Waters"));
				territories.push_back(sea_colour);
				waters.push_back(sea_colour);
			}
			else
			{
				std::cout << "Error in zone info json file. First zone must be called \"" + SEA_ZONE_NAME + "\" and contain the sea water colour" << std::endl;
				exit(RETURN_CODE::ERR_INVALID_ZONE_INFO);
			}
		}
		else
		{
			territories.push_back(PtreeToVec3d(zone_colours.get_child("Territory")));
			waters.push_back(PtreeToVec3d(zone_colours.get_child("Waters")));
		}
		i++;
	}
	auto num_zones = zones.size() - 1;

	for (auto row = 0; row < rows; row++)
	{
		for (auto col = 0; col < cols; col++)
		{
			auto colour = m.at<Vec3d>(row, col);
			if (IsSea(colour, sea_colour, colour_epsilon))
			{
				auto zone_id = FindZone(m, row, col, width, colour_epsilon, territories);
				if (zone_id > 0)
				{
					m_out.at<Vec3b>(row, col) = { (uchar)(waters[zone_id][0]), (uchar)(waters[zone_id][1]), (uchar)(waters[zone_id][2]) };
				}
			}
		}
	}

	return m_out; 
}

#endif