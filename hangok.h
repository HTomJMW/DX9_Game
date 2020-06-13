#pragma once


#include <dshow.h>
#include "struct.h"

class Hangok
{
	public:
		Hangok();
		~Hangok();

		LONGLONG start_sec = 0 * 10000000;
		IGraphBuilder *s_graph;
		IMediaControl *s_mediacontrol;
		IMediaEvent	*s_event;
		IMediaSeeking *s_seeking;

		void init_sound(void);
		void start(void);
		void stop(void);
		void cleanup(void);
};