#pragma once
#ifndef __CONFIG_H
#define __CONFIG_H
#include <string>
#include <boost\property_tree\ptree.hpp>

namespace Nostale
{
	namespace Files
	{
		class Config
		{
		public:
			Config(const std::string& path);
			~Config(void);

			// get settings key with template type
			template <typename Type> Type get(const std::string& key) const
			{
				if(key.empty())
					throw std::runtime_error("Read config key is empty");

				return _propertyTree.get<Type>(key);
			}

			std::string getPath(void) const;
		private:
			const std::string _path;
			boost::property_tree::ptree _propertyTree;
		};
	}
}

#endif
