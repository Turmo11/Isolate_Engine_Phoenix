#include "SDL/include/SDL_timer.h"

#include "Hourglass.h"


Hourglass::Hourglass(uint hours, uchar minutes, float seconds)
{
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;

	previous_ticks = 0;
}

void Hourglass::Update(uint ms)
{
	seconds += ms / 1000.0f;										// Returns the time that has elapsed since the start in seconds.

	if (seconds >= 60.0f)
	{
		++minutes;
		seconds = 0.0f;
	}

	if (minutes >= 60)
	{
		++hours;
		minutes = 0;
	}
}

std::string Hourglass::GetTimeAsString()
{
	std::string time_string = std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
	return time_string;
}