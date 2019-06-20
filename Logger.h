
#ifndef LOGGER_H
#define LOGGER_H
class Logger
{
public:
	void logg_error(string ip, string msg, int stat);
	void logg(string ip, string filename, int i);
};
