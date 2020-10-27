/*
 * Copyright (c) 2013-2014 Chun-Ying Huang
 *
 * This file is part of GamingAnywhere (GA).
 *
 * GA is free software; you can redistribute it and/or modify it
 * under the terms of the 3-clause BSD License as published by the
 * Free Software Foundation: http://directory.fsf.org/wiki/License:BSD_3Clause
 *
 * GA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the 3-clause BSD License along with GA;
 * if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <pthread.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#include <map>

#include "vsource.h"
#include "dpipe.h"
#include "encoder-common.h"
#include "rtspconf.h"

#include "ga-common.h"

#ifdef WIN32
#include "ga-win32-common.h"
#ifdef D3D_CAPTURE
#include "ga-win32-d3d.h"
#elif defined DFM_CAPTURE
#include "ga-win32-dfm.h"
#else
#include "ga-win32-gdi.h"
#endif
#elif defined __APPLE__
#include "ga-osx.h"
#elif defined ANDROID
#include "ga-androidvideo.h"
#else
#include "ga-xwin.h"
#endif

#include "ga-avcodec.h"

#include "vsource-desktop.h"

#include <cstring>
#ifdef strncpy
#undef strncpy
#endif
#include "CImg.h"

#define	SOURCES			1
//#define	ENABLE_EMBED_COLORCODE	1	/* XXX: enabled at the filter, not here */

using namespace std;
using namespace cimg_library;

static struct gaRect croprect;
static struct gaRect *prect = NULL;
static int screenwidth, screenheight;

static struct gaImage realimage, *image = &realimage;

static int vsource_initialized = 0;
static int vsource_started = 0;
static pthread_t vsource_tid;

/* support reconfiguration of frame rate */
static int vsource_framerate_n = -1;
static int vsource_framerate_d = -1;
static int vsource_reconfigured = 0;

/* video source has to send images to video-# pipes */
/* the format is defined in VIDEO_SOURCE_PIPEFORMAT */

/*
 * vsource_init(void *arg)
 * arg is a pointer to a gaRect (if cropping is enabled)
 */
static int vsource_init(void *arg) {
	struct RTSPConf *rtspconf = rtspconf_global();
	struct gaRect *rect = (struct gaRect*) arg;	
	if(vsource_initialized != 0)
		return 0;
#ifdef ENABLE_EMBED_COLORCODE
	vsource_embed_colorcode_init(1/*RGBmode*/);
#endif
	if(rect != NULL) {
		if(ga_fillrect(&croprect, rect->left, rect->top, rect->right, rect->bottom) == NULL) {
			ga_error("video source: invalid rect (%d,%d)-(%d,%d)\n",
				rect->left, rect->top,
				rect->right, rect->bottom);
			return -1;
		}
		prect = &croprect;
	} else {
		prect = NULL;
	}
#ifdef WIN32
	// prevent GetSystemMetrics() get worng numbers
	// XXX: currently disabled
	//SetProcessDPIAware();
	#ifdef D3D_CAPTURE
	if(ga_win32_D3D_init(image) < 0) {
		ga_error("WIN32-D3D capture init failed.\n");
		return -1;
	}
	#elif defined DFM_CAPTURE
	if(ga_win32_DFM_init(image) < 0) {
		ga_error("WIN32-DFM capture init failed.\n");
		return -1;
	}
	#else
	if(ga_win32_GDI_init(image) < 0) {
		ga_error("WIN32-GDI capture init failed.\n");
		return -1;
	}
	#endif	/* D3D_CAPTURE */
#elif defined __APPLE__
	if(ga_osx_init(image) < 0) {
		ga_error("Mac OS X capture init failed.\n");
		return -1;
	}
#elif defined ANDROID
	if(ga_androidvideo_init(image) < 0) {
		ga_error("Android capture init failed.\n");
		return -1;
	}
#else
	if(ga_xwin_init(rtspconf->display, image) < 0) {
		ga_error("XWindow capture init failed.\n");
		return -1;
	}
#endif

	screenwidth = image->width;
	screenheight = image->height;

#ifdef SOURCES
	do {
		int i;
		vsource_config_t config[SOURCES];
		bzero(config, sizeof(config));
		for(i = 0; i < SOURCES; i++) {
			//config[i].rtp_id = i;
			config[i].curr_width = prect ? prect->width : image->width;
			config[i].curr_height = prect ? prect->height : image->height;
			config[i].curr_stride = prect ? prect->linesize : image->bytes_per_line;
		}
		if(video_source_setup_ex(config, SOURCES) < 0) {
			return -1;
		}
	} while(0);
#else
	if(video_source_setup(
			prect ? prect->width : image->width,
			prect ? prect->height : image->height,
			prect ? prect->linesize : image->bytes_per_line) < 0) {
		return -1;
	}
#endif
	vsource_initialized = 1;
	return 0;
}

/*
 * vsource_threadproc accepts no arguments
 */
static void * vsource_threadproc(void *arg) {
	int i;
	int token;
	int frame_interval;
	struct timeval tv;
	dpipe_buffer_t *data;
	vsource_frame_t *frame;
	dpipe_t *pipe[SOURCES];
	struct timeval initialTv, lastTv, captureTv;
	struct RTSPConf *rtspconf = rtspconf_global();
	// reset framerate setup
	vsource_framerate_n = rtspconf->video_fps;
	vsource_framerate_d = 1;
	vsource_reconfigured = 0;
	frame_interval = 1000000/rtspconf->video_fps;	// in the unif of us
	frame_interval++;
#ifdef ENABLE_EMBED_COLORCODE
	vsource_embed_colorcode_reset();
#endif
	for(i = 0; i < SOURCES; i++) {
		char pipename[64];
		snprintf(pipename, sizeof(pipename), VIDEO_SOURCE_PIPEFORMAT, i);
		if((pipe[i] = dpipe_lookup(pipename)) == NULL) {
			ga_error("video source: cannot find pipeline '%s'\n", pipename);
			exit(-1);
		}
	}

	ga_error("video source thread started: thread id=%ld\n", ga_gettid());
	gettimeofday(&initialTv, NULL);
	lastTv = initialTv;
	token = frame_interval;
	static unsigned int  saveNum = 0;
	while(vsource_started != 0) {
		// encoder has not launched?
		if(encoder_running() == 0) {
#ifdef WIN32
			Sleep(1);
#else
			usleep(1000);
#endif
			gettimeofday(&lastTv, NULL);
			token = frame_interval;
			continue;
		}
		// token bucket based capturing
		gettimeofday(&captureTv, NULL);
		token += tvdiff_us(&captureTv, &lastTv);
		if(token > (frame_interval<<1)) {
			token = (frame_interval<<1);
		}
		lastTv = captureTv;
		if(token < frame_interval) {
#ifdef WIN32
			Sleep(1);
#else
			usleep(1000);
#endif
			continue;
		}
		token -= frame_interval;
		// copy image 
		data = dpipe_get(pipe[0]);
		frame = (vsource_frame_t*) data->pointer;
#ifdef __APPLE__
		frame->pixelformat = AV_PIX_FMT_RGBA;
#else
		frame->pixelformat = AV_PIX_FMT_BGRA;
#endif
		if(prect == NULL) {
			frame->realwidth = screenwidth;
			frame->realheight = screenheight;
			frame->realstride = screenwidth<<2;
			frame->realsize = screenheight * frame->realstride;
		} else {
			frame->realwidth = prect->width;
			frame->realheight = prect->height;
			frame->realstride = prect->width<<2;
			frame->realsize = prect->height * frame->realstride;
		}
		frame->linesize[0] = frame->realstride/*frame->stride*/;
		/*saveNum++;
		if (saveNum == 100){
		static const unsigned char pal[] = {
		217, 62, 88, 75, 1, 237, 240, 12, 56, 160, 165, 116, 1, 1, 204, 2, 15, 248, 148, 185, 133, 141, 46, 246, 222, 116, 16, 5, 207, 226,
		17, 114, 247, 1, 214, 53, 238, 0, 95, 55, 233, 235, 109, 0, 17, 54, 33, 0, 90, 30, 3, 0, 94, 27, 19, 0, 68, 212, 166, 130, 0, 15, 7, 119,
		238, 2, 246, 198, 0, 3, 16, 10, 13, 2, 25, 28, 12, 6, 2, 99, 18, 141, 30, 4, 3, 140, 12, 4, 30, 233, 7, 10, 0, 136, 35, 160, 168, 184, 20,
		233, 0, 1, 242, 83, 90, 56, 180, 44, 41, 0, 6, 19, 207, 5, 31, 214, 4, 35, 153, 180, 75, 21, 76, 16, 202, 218, 22, 17, 2, 136, 71, 74,
		81, 251, 244, 148, 222, 17, 0, 234, 24, 0, 200, 16, 239, 15, 225, 102, 230, 186, 58, 230, 110, 12, 0, 7, 129, 249, 22, 241, 37, 219,
		1, 3, 254, 210, 3, 212, 113, 131, 197, 162, 123, 252, 90, 96, 209, 60, 0, 17, 0, 180, 249, 12, 112, 165, 43, 27, 229, 77, 40, 195, 12,
		87, 1, 210, 148, 47, 80, 5, 9, 1, 137, 2, 40, 57, 205, 244, 40, 8, 252, 98, 0, 40, 43, 206, 31, 187, 0, 180, 1, 69, 70, 227, 131, 108, 0,
		223, 94, 228, 35, 248, 243, 4, 16, 0, 34, 24, 2, 9, 35, 73, 91, 12, 199, 51, 1, 249, 12, 103, 131, 20, 224, 2, 70, 32,
		233, 1, 165, 3, 8, 154, 246, 233, 196, 5, 0, 6, 183, 227, 247, 195, 208, 36, 0, 0, 226, 160, 210, 198, 69, 153, 210, 1, 23, 8, 192, 2, 4,
		137, 1, 0, 52, 2, 249, 241, 129, 0, 0, 234, 7, 238, 71, 7, 32, 15, 157, 157, 252, 158, 2, 250, 6, 13, 30, 11, 162, 0, 199, 21, 11, 27, 224,
		4, 157, 20, 181, 111, 187, 218, 3, 0, 11, 158, 230, 196, 34, 223, 22, 248, 135, 254, 210, 157, 219, 0, 117, 239, 3, 255, 4, 227, 5, 247,
		11, 4, 3, 188, 111, 11, 105, 195, 2, 0, 14, 1, 21, 219, 192, 0, 183, 191, 113, 241, 1, 12, 17, 248, 0, 48, 7, 19, 1, 254, 212, 0, 239, 246,
		0, 23, 0, 250, 165, 194, 194, 17, 3, 253, 0, 24, 6, 0, 141, 167, 221, 24, 212, 2, 235, 243, 0, 0, 205, 1, 251, 133, 204, 28, 4, 6, 1, 10,
		141, 21, 74, 12, 236, 254, 228, 19, 1, 0, 214, 1, 186, 13, 13, 6, 13, 16, 27, 209, 6, 216, 11, 207, 251, 59, 32, 9, 155, 23, 19, 235, 143,
		116, 6, 213, 6, 75, 159, 23, 6, 0, 228, 4, 10, 245, 249, 1, 7, 44, 234, 4, 102, 174, 0, 19, 239, 103, 16, 15, 18, 8, 214, 22, 4, 47, 244,
		255, 8, 0, 251, 173, 1, 212, 252, 250, 251, 252, 6, 0, 29, 29, 222, 233, 246, 5, 149, 0, 182, 180, 13, 151, 0, 203, 183, 0, 35, 149, 0,
		235, 246, 254, 78, 9, 17, 203, 73, 11, 195, 0, 3, 5, 44, 0, 0, 237, 5, 106, 6, 130, 16, 214, 20, 168, 247, 168, 4, 207, 11, 5, 1, 232, 251,
		129, 210, 116, 231, 217, 223, 214, 27, 45, 38, 4, 177, 186, 249, 7, 215, 172, 16, 214, 27, 249, 230, 236, 2, 34, 216, 217, 0, 175, 30,
		243, 225, 244, 182, 20, 212, 2, 226, 21, 255, 20, 0, 2, 13, 62, 13, 191, 14, 76, 64, 20, 121, 4, 118, 0, 216, 1, 147, 0, 2, 210, 1, 215,
		95, 210, 236, 225, 184, 46, 0, 248, 24, 11, 1, 9, 141, 250, 243, 9, 221, 233, 160, 11, 147, 2, 55, 8, 23, 12, 253, 9, 0, 54, 0, 231, 6, 3,
		141, 8, 2, 246, 9, 180, 5, 11, 8, 227, 8, 43, 110, 242, 1, 130, 5, 97, 36, 10, 6, 219, 86, 133, 11, 108, 6, 1, 5, 244, 67, 19, 28, 0, 174,
		154, 16, 127, 149, 252, 188, 196, 196, 228, 244, 9, 249, 0, 0, 0, 37, 170, 32, 250, 0, 73, 255, 23, 3, 224, 234, 38, 195, 198, 0, 255, 87,
		33, 221, 174, 31, 3, 0, 189, 228, 6, 153, 14, 144, 14, 108, 197, 0, 9, 206, 245, 254, 3, 16, 253, 178, 248, 0, 95, 125, 8, 0, 3, 168, 21,
		23, 168, 19, 50, 240, 244, 185, 0, 1, 144, 10, 168, 31, 82, 1, 13
		};
		static const CImg<unsigned char> colormap(pal, 16, 16, 1, 3, false);
		colormap.save("frame1.jpg");
		}*/
#ifdef WIN32
	#ifdef D3D_CAPTURE
		ga_win32_D3D_capture((char*) frame->imgbuf, frame->imgbufsize, prect);
	#elif defined DFM_CAPTURE
		ga_win32_DFM_capture((char*) frame->imgbuf, frame->imgbufsize, prect);
	#else
		ga_win32_GDI_capture((char*) frame->imgbuf, frame->imgbufsize, prect);
	#endif
#elif defined __APPLE__
		ga_osx_capture((char*) frame->imgbuf, frame->imgbufsize, prect);
#elif defined ANDROID
		ga_androidvideo_capture((char*) frame->imgbuf, frame->imgbufsize);
#else // X11
		ga_xwin_capture((char*) frame->imgbuf, frame->imgbufsize, prect);
#endif
		saveNum++;
		if (saveNum == 100){
			//CImg<unsigned char> colormap(frame->imgbuf, frame->realwidth, frame->realheight, 1, 3, false);
			//colormap.permute_axes("YZCX");
			//CImg<unsigned char> colormap(frame->imgbuf, frame->realwidth, frame->realheight, 1, 4, true);
			//CImg<unsigned char> colormap(frame->imgbuf, frame->realwidth, frame->realheight, 1, 4, false);
			/*CImg<unsigned char> colormap(frame->imgbuf, frame->realwidth, frame->realheight, 1, 4, false);
			colormap.save("frame.jpg");*/
			//CImg<unsigned char> colormap(frame->imgbuf, 4, frame->realwidth, frame->realheight);
			CImg<unsigned char> colormap(frame->imgbuf, 4, frame->realwidth, frame->realheight);
			//colormap.permute_axes("YZCX");
			colormap.permute_axes("yzcx");
			//colormap.permute_axes("zyxc"); //AV_PIX_ FMT_ BGRA  //AV_PIX_FMT_RGBA --apple
			//colormap.permute_axes("yczx");  //czyx
			//colormap.permute_axes("cxyz");
			colormap.save("frame.jpg");
		}
		// draw cursor
#ifdef WIN32
		ga_win32_draw_system_cursor(frame);			
#endif
		//gImgPts++;
		frame->imgpts = tvdiff_us(&captureTv, &initialTv)/frame_interval;
		frame->timestamp = captureTv;
		// embed color code?
#ifdef ENABLE_EMBED_COLORCODE
		vsource_embed_colorcode_inc(frame);
#endif
		// duplicate from channel 0 to other channels
		for(i = 1; i < SOURCES; i++) {
			dpipe_buffer_t *dupdata;
			vsource_frame_t *dupframe;
			dupdata = dpipe_get(pipe[i]);
			dupframe = (vsource_frame_t*)dupdata->pointer;
			vsource_dup_frame(frame, dupframe);
			dpipe_store(pipe[i], dupdata);
		}
		dpipe_store(pipe[0], data);
		// reconfigured?
		if(vsource_reconfigured != 0) {
			frame_interval = (int) (1000000.0 * vsource_framerate_d / vsource_framerate_n);
			frame_interval++;
			vsource_reconfigured = 0;
			ga_error("video source: reconfigured - framerate=%d/%d (interval=%d)\n",
				vsource_framerate_n, vsource_framerate_d, frame_interval);
		}
	}

	ga_error("video source: thread terminated.\n");
	return NULL;
}

static int vsource_deinit(void *arg) {
	if(vsource_initialized == 0)
		return 0;
#ifdef WIN32
	#ifdef D3D_CAPTURE
	ga_win32_D3D_deinit();
	#elif defined DFM_CAPTURE
	ga_win32_DFM_deinit();
	#else
	ga_win32_GDI_deinit();
	#endif /* D3D_CAPTURE */
	CoUninitialize();
#elif defined __APPLE__
	ga_osx_deinit();
#elif defined ANDROID
	ga_androidvideo_deinit();
#else
	//ga_xwin_deinit(display, image);
	ga_xwin_deinit();
#endif
	vsource_initialized = 0;
	return 0;
}

static int vsource_start(void *arg) {
	if(vsource_started != 0)
		return 0;
	vsource_started = 1;
	if(pthread_create(&vsource_tid, NULL, vsource_threadproc, arg) != 0) {
		vsource_started = 0;
		ga_error("video source: create thread failed.\n");
		return -1;
	}
	pthread_detach(vsource_tid);
	return 0;
}

static int vsource_stop(void *arg) {
	if(vsource_started == 0)
		return 0;

	vsource_started = 0;
	pthread_cancel(vsource_tid);
	return 0;
}

static int vsource_ioctl(int command, int argsize, void *arg) {
	int ret = 0;
	ga_ioctl_reconfigure_t *reconf = (ga_ioctl_reconfigure_t*) arg;	
	if(vsource_initialized == 0)
		return GA_IOCTL_ERR_NOTINITIALIZED;

	switch(command) {
	case GA_IOCTL_RECONFIGURE:
		if(argsize != sizeof(ga_ioctl_reconfigure_t))
			return GA_IOCTL_ERR_INVALID_ARGUMENT;
		if(reconf->framerate_n > 0 && reconf->framerate_d > 0) {
			double framerate;
			if(vsource_framerate_n == reconf->framerate_n
			&& vsource_framerate_d == reconf->framerate_d)
				break;
			framerate = 1.0 * reconf->framerate_n / reconf->framerate_d;
			if(framerate < 2 || framerate > 120) {
				return GA_IOCTL_ERR_INVALID_ARGUMENT;
			}
			vsource_framerate_n = reconf->framerate_n;
			vsource_framerate_d = reconf->framerate_d;
			vsource_reconfigured = 1;
		}
		break;
	default:
		ret = GA_IOCTL_ERR_NOTSUPPORTED;
		break;
	}
	return ret;
}

ga_module_t * module_load() {
	static ga_module_t m;
	bzero(&m, sizeof(m));
	m.type = GA_MODULE_TYPE_VSOURCE;
	m.name = strdup("vsource-desktop");
	m.init = vsource_init;
	m.start = vsource_start;
	m.stop = vsource_stop;
	m.deinit = vsource_deinit;
	m.ioctl = vsource_ioctl;
	return &m;
}
