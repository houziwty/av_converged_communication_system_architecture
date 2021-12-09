// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for playback stream.
//

#ifndef PLAYBACK_STREAM_MEDIA_GRAPH_H
#define PLAYBACK_STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class PlaybackStreamMediaGraph : public MediaGraph
{
public:
	PlaybackStreamMediaGraph(void);
	virtual ~PlaybackStreamMediaGraph(void);

protected:
	int createNewVideoDecoderFilter(void);
	int createNewAudioDecoderFilter(void);
	int createNewVideoRendererFilter(void* hwnd = NULL);
	int createNewAudioRendererFilter(void);
};//class PlaybackStreamMediaGraph

NS_END

#endif//PLAYBACK_STREAM_MEDIA_GRAPH_H
