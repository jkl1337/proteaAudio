/**\mainpage

\section intro Overview
proteaAudio is a free cross-platform 2D audio library for C++ and Lua.

Due to its straightforward interface and minimal open-source code base, proteaAudio is both easy to maintain and use.

By default proteaAudio internally makes use of the excellent RtAudio low-level realtime audio input/output API, and therefore has no external dependencies apart from standard system libraries. Together with its liberal open-source licensing conditions (zlib style), this makes the integration of proteaAudio into both free and closed-source commercial software very easy. Alternatively, proteaAudio contains optional bindings for the highly portable SDL multimedia library and is therefore also usable on a plentitude of further platforms (e.g., iPhone, BEOS, FreeBSD).

Despite its minimalistic design, proteaAudio offers advanced features such as dynamic pitch, per-channel volume control, and user-definable time shifts between channels. proteaAudio is capable of handling normal .wav files as well as ogg/vorbis audio samples (via stb_vorbis). Additionally it offers a simple interface for integrating further custom audio format loaders (e.g., .mp3).

\section features Main features at a glance
- clean minimal C++ code base, portable, extendable class design
- supports playback of an arbitrary number of parallel mono/stereo sounds
- dynamic pitch shifts, panning between the stereo channels, user-definable disparity
- low CPU consumption, runs in its own dedicated thread
- regularly tested on MS Windows (MinGW/VisualStudio) and Linux (gcc)
- loaders for standard wav and ogg/vorbis audio samples included
- easily extensible to support further audio sample formats, e.g., mp3
- C++ and Lua API
- by default makes use of the RtAudio low-level cross-platform audio backend, supporting Windows (DirectSound, ASIO), Linux (ALSA, OSS, JACK), and MacOSX (CoreAudio, JACK)
- makes optionally use of libSDL as cross-platform audio backend, supporting various further platforms

\section download Download
- <a href="proteaAudio_src_090204.zip">proteaAudio source code release 2009-02-04</a>
- <a href="proteaAudio_lua_090204.zip">proteaAudio Lua Windows/Linux binary release 2009-02-04</a>

\section documentation Documentation
- <a href="annotated.html">C++ API</a>
- <a href="proteaaudiolua.html">Lua API</a>
- <a href="changelog.html">Changelog</a>

\section links Links

proteaAudio internally makes use of the following excellent open-source components:

- <a href="http://music.mcgill.ca/~gary/rtaudio">RtAudio</a> cross-platform low-level audio library (optional)
- <a href="http://www.libsdl.org">SDL</a> cross-platform multimedia layer (optional)
- <a href="http://nothings.org/stb_vorbis">stb_vorbis</a> Ogg Vorbis audio decoder
 

\section license License notice (zlib license):

 (c) 2009 by Gerald Franz, www.viremo.de

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the author be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  -# The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  -# Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  -# This notice may not be removed or altered from any source distribution.

*/
 