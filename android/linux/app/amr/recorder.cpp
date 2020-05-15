#include <stdio.h>
#include <utils/Log.h>
#include <media/mediarecorder.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#undef  LOG_TAG
#define LOG_TAG "recorder"

using namespace android;

sp<MediaRecorder> Recorder = NULL;

class MRecorderListener : public MediaRecorderListener
{
	void notify(int msg, int ext1, int ext2)
	{
		switch (msg) {
		case MEDIA_RECORDER_EVENT_LIST_START:
			ALOGD("MEDIA_RECORDER_EVENT_LIST_START");
			break;
		case MEDIA_RECORDER_EVENT_INFO:
			ALOGD("MEDIA_RECORDER_EVENT_INFO");
			break;
		case MEDIA_RECORDER_EVENT_LIST_END:
			ALOGD("MEDIA_RECORDER_EVENT_LIST_END");
			
		default:
			break;
		}
	}

};

sp<MRecorderListener> mListener = new MRecorderListener();

extern int player(char * pAudioFile, float volume);
// # recorder /data/test.amr 
int main(int argc, char* argv[])
{
	if (argc < 3) 
		return -1;

	int mRecorderFd = -1;
	printf("recorder start ....\n");
	Recorder = new MediaRecorder;

	mRecorderFd = open(argv[1], O_WRONLY|O_CREAT, 0666);
	if (mRecorderFd <= 0) {
		printf("open path:%s error\n", argv[1]);
		return -1;
	}

	Recorder->setAudioSource(AUDIO_SOURCE_MIC);
	Recorder->setOutputFormat(OUTPUT_FORMAT_THREE_GPP);
	Recorder->setAudioEncoder(AUDIO_ENCODER_AMR_NB);
	Recorder->setListener(mListener);
	Recorder->setOutputFile(mRecorderFd, 0, 0); // invalid var2, var3 
	Recorder->prepare();
	Recorder->start();
	
	printf("recording ....\n");
	sleep(atoi(argv[2]));
	Recorder->stop();
	Recorder->release();
	close(mRecorderFd);
	printf("recorder stop ....\n");

	player(argv[1], 1.0);
	return 0;
}
