#pragma once
#ifndef __INPUTHANDLER_H
#define __INPUTHANDLER_H
#include <functional>
#include <vector>
#include <boost/thread.hpp>

typedef std::function<void(const std::vector<std::string>&)> con_callback;

class InputHandler : boost::noncopyable
{
public:
	InputHandler(void);
	~InputHandler(void);
	void start(void);
	void add(const std::string& cmd, std::size_t param_count, const con_callback& callback);
private:
	bool _running;
	boost::thread _threadInputHandler;

	struct Command
	{
		std::string cmd;
		std::size_t param_count;
		con_callback callback;
	};

	std::vector<Command> _commands;

	void InputHandlerThread(void);
};

#endif