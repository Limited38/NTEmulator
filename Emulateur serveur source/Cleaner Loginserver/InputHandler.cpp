#include "InputHandler.h"
#include <boost/algorithm/string.hpp>
#include "Log.h"

InputHandler::InputHandler(void)
	: _running(false)
{
}

InputHandler::~InputHandler(void)
{
	if(_running)
	{
		_running = false;
		_threadInputHandler.join();
	}
}

void InputHandler::start(void)
{
	if(!_running)
	{
		// create console input handler thread
		_running = true;
		_threadInputHandler = boost::thread(&InputHandler::InputHandlerThread, this);
	} 
}

void InputHandler::add(const std::string& cmd, std::size_t param_count, const con_callback& callback)
{
	if(!cmd.empty() && !callback._Empty())
	{
		// add command to console input handler
		Command command = {std::move(cmd), param_count, std::move(callback)};
		_commands.push_back(command);
	}
}

void InputHandler::InputHandlerThread(void)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");

	std::vector<std::string> input_params;
	std::string input;
	while(_running)
	{
		// detect console input
		if(std::getline(std::cin, input) && !input.empty() && input.at(0) == '/')
		{
			boost::algorithm::split(input_params, input, std::bind2nd(std::equal_to<char>(), (' ')));
			
			// search for valid input and execute command
			for(auto it = _commands.begin(); it != _commands.end(); ++it)
			{
				if(it->cmd == input_params.at(0) && it->param_count == input_params.size())
				{
					BOOST_LOG(globalLog::get()) << "Command ('" << it->cmd << "') executed.";
					it->callback(input_params);
					break;
				}
			}

			// cleanup buffers
			input.clear();
			input_params.clear();
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}