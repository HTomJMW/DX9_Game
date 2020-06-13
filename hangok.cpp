#include "hangok.h"

Hangok::Hangok()
{

}

Hangok::~Hangok()
{

}

void Hangok::init_sound(void)
{
	// Initialize COM
	CoInitialize(NULL);
	// Create the graph
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&s_graph);
	// Query for interface objects
	s_graph->QueryInterface(IID_IMediaControl, (void **)&s_mediacontrol);
	s_graph->QueryInterface(IID_IMediaEvent, (void **)&s_event);
	s_graph->QueryInterface(IID_IMediaSeeking, (void **)&s_seeking);
	// Load the sound
	s_graph->RenderFile(L"Audio\\action.wav", NULL);
	// Set the playback rate
	s_seeking->SetRate(1);
	// Set position
	//s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	// Start sound
	//s_mediacontrol->Run();

	return;
}

void Hangok::start(void)
{
	s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	s_mediacontrol->Run();

	return;
}

void Hangok::stop(void)
{
	s_mediacontrol->Stop();
	s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

	return;
}

void Hangok::cleanup(void)
{
	s_graph->Release();
	s_mediacontrol->Release();
	s_event->Release();
	s_seeking->Release();

	return;
}