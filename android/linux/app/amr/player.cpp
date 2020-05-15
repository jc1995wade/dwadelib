/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "player"
#include <utils/Log.h>
#include <binder/ProcessState.h>
#include <media/mediaplayer.h>
#include <fcntl.h>
#include <media/AudioSystem.h>

using namespace android;

enum player_states {
    PLAYER_STATE_NONE        = 0,  
    PLAYER_STATE_PLAYING  =1,
    PLAYER_STATE_STOP  =2,    
};

sp<MediaPlayer> mPlayer = NULL;
player_states  mPlayState = PLAYER_STATE_NONE;
int  mPlayerFd = 0;
Mutex               mLock;
Condition           mSignal;

class MPlayerListener : public MediaPlayerListener
{
    void notify(int msg, int ext1, int ext2, const Parcel * /*obj*/)
    {
        switch (msg) {
        case MEDIA_ERROR:
            // Always log errors.
            ALOGE("error (%d, %d)", ext1, ext2);
            //fall through
        case MEDIA_PLAYBACK_COMPLETE:
            ALOGD("play media finished, quit");
            if (mPlayer != NULL) {
                mPlayer->stop();
                mPlayer->reset();
                mPlayer->setListener(0);
                mPlayer->disconnect();
                mPlayState = PLAYER_STATE_STOP;
				if (mPlayerFd != 0) {
					close(mPlayerFd);
					mPlayerFd = 0;
				}
            }
           mSignal.signal();
            break;
        default:
            break;
        }
    }
};


int player(char * pAudioFile, float volume)
{
	ProcessState::self()->startThreadPool();
	printf("player start ...\n");
	mPlayerFd = open(pAudioFile, O_RDONLY);
	if (mPlayerFd <= 0) {
		ALOGE("open file failed, fd: %d, path: %s", mPlayerFd, pAudioFile);
		return -1;
	} 
	
	mPlayer = new MediaPlayer();
	sp<MPlayerListener> mListener = new MPlayerListener();
	
	mPlayer->setListener(mListener);
	mPlayer->setDataSource(mPlayerFd, 0, 0x7ffffffffffffffLL);
	mPlayer->setAudioStreamType(AUDIO_STREAM_MUSIC);
	printf("player->prepare");
	mPlayer->prepare();
	printf("player->start()");
	mPlayer->start();
	printf("setStreamVolumeIndex");
	AudioSystem::setStreamVolumeIndex(AUDIO_STREAM_MUSIC, 15, AUDIO_DEVICE_OUT_SPEAKER);
	if (volume < 0.0 || volume > 1.0) {
		volume = 1.0;
	}
	mPlayer->setVolume(volume, volume);
	mPlayState = PLAYER_STATE_PLAYING;
	printf("playing ...\n");
	mSignal.wait(mLock);

	printf("play completed, ready to quit main process");
	return 0;
}

