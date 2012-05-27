#if defined __WIN32__ || defined WIN32
# define WINDOWS_LEAN_AND_MEAN
# include <windows.h>
# define _EXPORT __declspec(dllexport)
#else
#  include <unistd.h>
# define _EXPORT
#endif

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};

#include "proAudioRt.h"
#include <cstdio>
#include <climits>

using namespace std;

static int create(lua_State *L) {
	unsigned int nTracks = lua_isnumber(L,1) ? lua_tointeger(L,1) : 8;
	unsigned int frequency = lua_isnumber(L,2) ? lua_tointeger(L,2) : 22050;
	unsigned int chunkSize = lua_isnumber(L,3) ? lua_tointeger(L,3) : 1024;
	DeviceAudio* pAudio = DeviceAudioRt::create(nTracks, frequency, chunkSize);
	lua_pushboolean(L, pAudio ? 1 : 0);
	return 1;
}

static int destroy(lua_State *L) {
	DeviceAudio::destroy();
	return 0;
}

static int loaderAvailable(lua_State *L) {
	const char * fname = luaL_checkstring(L,1);
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	lua_pushboolean(L, audio.loaderAvailable(fname));
	return 1;
}

static int volume(lua_State *L) {
	int argc = lua_gettop(L);
	if(!argc) return 0;
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	if(argc==1) audio.volume(luaL_checknumber(L,1));
	else  audio.volume(luaL_checknumber(L,1),luaL_checknumber(L,2));
	return 0;
}

static int sleep(lua_State*L) {
	double secs = luaL_checknumber(L,1);
#if defined __WIN32__ || defined WIN32
	Sleep( (DWORD)(secs*1000.0) ) ;
#else // Unix variants
	usleep( (unsigned long) (secs * 1000000.0) );
#endif
	return 0;
}

static int sampleFromFile(lua_State *L) {
	const char * fname = luaL_checkstring(L,1);
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	int argc = lua_gettop(L);
	float volume = argc>1 ? luaL_checknumber(L,2) : 1.0f;
	int ret = (int)audio.sampleFromFile(fname, volume);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int sampleFromMemory(lua_State *L) {
	int argc = lua_gettop(L);
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio||argc<2||!lua_istable(L,1)) return 0;

	size_t len = lua_objlen(L,1);
	if(!len) return 0;
	int sampleRate = luaL_checkinteger(L,2);
	signed short *data = new signed short[len];
	for(size_t i = 0; i<len; ++i) {
		lua_rawgeti(L, 1, i+1);
		if(lua_isnumber(L, -1)) {
			float value = lua_tonumber(L,-1);
			if(value<-1.0f) data[i] = SHRT_MIN;
			else if(value>=1.0f) data[i] = SHRT_MAX;
			else data[i] = (signed short)(value*SHRT_MAX);
		}
		lua_pop(L,1);
	}
	AudioSample sample((unsigned char*)data, len*sizeof(signed short), 1, sampleRate, 16); 
	int ret = (int)audio.sampleFromMemory(sample);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int sampleDestroy(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	lua_pushboolean(L, audio.sampleDestroy(luaL_checkinteger(L,1)));
	return 1;
}

static int sampleProperties(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	const AudioSample *pSample = audio.sample(luaL_checkinteger(L,1));
	if(!pSample) return 0;
	lua_pushnumber(L, (double)pSample->frames() / (double)pSample->sampleRate());
	lua_pushinteger(L, pSample->channels());
	lua_pushinteger(L, pSample->sampleRate());
	lua_pushinteger(L, pSample->bitsPerSample());
	return 4;
}

static int soundPlay(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	unsigned int sample = luaL_checkinteger(L,1);
	float volL= lua_isnumber(L,2) ? lua_tonumber(L,2) : 1.0f;
	float volR= lua_isnumber(L,3) ? lua_tonumber(L,3) : 1.0f;
	float disparity= lua_isnumber(L,4) ? lua_tonumber(L,4) : 0.0f;
	float pitch = lua_isnumber(L,5) ? lua_tonumber(L,5) : 1.0f;
	int ret = (int)audio.soundPlay(sample, volL,volR,disparity,pitch);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int soundLoop(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	unsigned int sample = luaL_checkinteger(L,1);
	float volL= lua_isnumber(L,2) ? lua_tonumber(L,2) : 1.0f;
	float volR= lua_isnumber(L,3) ? lua_tonumber(L,3) : 1.0f;
	float disparity= lua_isnumber(L,4) ? lua_tonumber(L,4) : 0.0f;
	float pitch = lua_isnumber(L,5) ? lua_tonumber(L,5) : 1.0f;
	int ret = (int)audio.soundLoop(sample, volL,volR,disparity,pitch);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int soundUpdate(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	unsigned int sound = luaL_checkinteger(L,1);
	float volL= lua_isnumber(L,2) ? lua_tonumber(L,2) : 1.0f;
	float volR= lua_isnumber(L,3) ? lua_tonumber(L,3) : 1.0f;
	float disparity= lua_isnumber(L,4) ? lua_tonumber(L,4) : 0.0f;
	float pitch = lua_isnumber(L,5) ? lua_tonumber(L,5) : 1.0f;
	int ret = (int)audio.soundUpdate(sound, volL,volR,disparity,pitch);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int soundStop(lua_State *L) {
	int argc = lua_gettop(L);
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	if(!argc) {
		audio.soundStop();
		return 0;
	}
	unsigned int sound = luaL_checkinteger(L,1);
	int ret = (int)audio.soundStop(sound);
	if(ret) {
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int soundActive(lua_State *L) {
	DeviceAudio & audio = DeviceAudio::singleton();
	if(!&audio) return 0;
	lua_pushinteger(L, (int)audio.soundActive());
	return 1;
}

extern "C" int _EXPORT luaopen_proAudioRt (lua_State* L) {
    static const struct luaL_Reg funcs[] = {
        {"create", create },
        {"destroy", destroy },
		{"loaderAvailable", loaderAvailable },
        {"volume", volume },
        {"sleep", sleep },

        {"sampleFromFile", sampleFromFile },
        {"sampleFromMemory", sampleFromMemory },
        {"sampleDestroy", sampleDestroy },
        {"sampleProperties", sampleProperties },

        {"soundPlay", soundPlay },
        {"soundLoop", soundLoop },
        {"soundUpdate", soundUpdate },
        {"soundStop", soundStop },
        {"soundActive", soundActive },
        {NULL, NULL}
    };
    luaL_register(L, "proAudio", funcs);
    return 1;
}
