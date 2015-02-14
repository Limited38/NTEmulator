#include "Tools.h"
#include <boost\date_time.hpp>

namespace Nostale
{
	namespace Tools
	{
		namespace btime = boost::posix_time;

		std::string getCurrentDateTime(void)
		{
			// get current utc time
			btime::ptime time = btime::second_clock::universal_time();
			return btime::to_simple_string(time);
		}

		__int64 getUnixTime(void)
		{
			// get total seconds between utc time and 1970.1.1
			btime::time_duration duration = btime::microsec_clock::universal_time() - btime::ptime(boost::gregorian::date(1970, 1, 1));
			return duration.total_seconds();
		}

		std::string getDateTimeFromUnix(__int64 unixtime)
		{
			// convert unix time to standart time
			btime::ptime now = btime::from_time_t(unixtime);
			return btime::to_simple_string(now);
		}
	}
}