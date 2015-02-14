#include "Config.h"
#include <boost\property_tree\ini_parser.hpp>

namespace Nostale
{
	namespace Files
	{
		Config::Config(const std::string& path)
			: _path(path)
		{
			if(_path.empty())
				throw std::runtime_error("Config path is empty");

			try
			{
				// parse settings file
				boost::property_tree::ini_parser::read_ini(_path, _propertyTree);
			} catch(...)
			{
				throw;
			}
		}

		Config::~Config(void)
		{
		}

		std::string Config::getPath(void) const
		{
			return _path;
		}
	}
}