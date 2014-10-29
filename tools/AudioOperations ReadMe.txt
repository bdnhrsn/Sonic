Audio Operations
8 October 2014
Matt Erickson

AudioOperations is a static C++ class that is used to carry out basic operations on user-provided audio files.

--

Currently, the available functions of the class, as well as their descriptions, are as follows:

static void AudioOperations::printInfo(const char* fileName);

//Prints all relevant information about the audio file. This includes: file size (in bytes), audio format (1 = PCM), channels (1 = mono, 2 = stereo, etc.), sample rate (Hz), byte rate (bytes/sec), bytes per block (bytes per multi-channel sample), bits per sample (usually 16), and number of samples (counting channels seperately).

static const char* AudioOperations::trim(const char* fileName, double startInSeconds, double lengthInSeconds)

//Creates a new wave file containing only a portion of the original file. Provide where in the original file you would like the new file to begin (in seconds), and provide how long you want the new file to carry on from it's start (in seconds). If you provide a length that exceeds the end of the original file, the new file will only last until that point.

static const char* AudioOperations::trimSamples(const char* fileName, int startInSamples, int lengthInSamples);

//Creates a new wave file containing only a portion of the original file. Provide where in the original file you would like the new file to begin (in samples), and provide how long you want the new file to carry on from it's start (in samples). If you provide a length that exceeds the  end of the original file, the new file will only last until that point.

static const char* AudioOperations::zeroPad(const char* fileName, double startPadInSeconds, double endPadInSeconds);

//Creates a new wave file with silence added at the beginning and/or the end of the original file.

static const char* AudioOperations::concatenate(const char* fileNameA, const char* fileNameB);

//Creates a new wave file by linking the two original files with one another in succession. Parameter fileNameA will specify the file that will come first, and fileNameB will specify the file that will come first second.

static const char* AudioOperations::amplify(const char* fileName, double ratio);
	
//Creates a new wave file wherein the volume of the audio is changed by the coefficient specified in the parameter ratio (must be at least zero). If you provide a ratio that too high and will cause the audio to clip, a new  file will not be created and the maximum ratio will be printed.

static const char* AudioOperations::switchStereoChannels(const char* fileName);

//Creates a new wave file from a given stereo file wherein the stereo channels are switched.

static const char* AudioOperations::flipPolarity(const char* fileName);
	
//Creates a new wave file from a given stereo file wherein the polarity of the waveform is flipped. Note that this process yields no immediately audible change.

static const char* AudioOperations::reverse(const char* fileName);

//Creates a new wave file wherein the audio data is reversed 

static const char* AudioOperations::fadeIn(const char* fileName, double fadeSeconds);
	
//Creates a new wave file wherein the audio volume fades from zero to one at the beginning of the track over the amount of time specified in the parameter fadeSeconds.
	
static const char* AudioOperations::fadeOut(const char* fileName, double fadeSeconds);
	
//Creates a new wave file wherein the audio volume fades from one to zero at the end of the track over the amount of time specified in the parameter fadeSeconds.

static const char* AudioOperations::fadeInSamples(const char* fileName, int fadeSamples);
	
//Creates a new wave file wherein the audio volume fades from zero to one at the beginning of the track over the amount of samples specified in the parameter fadeSamples

static const char* AudioOperations::fadeOutSamples(const char* fileName, int fadeSamples);
	
//Creates a new wave file wherein the audio volume fades from one to zero at the end of the track over the amount of samples specified in the parameter fadeSamples

static const char* AudioOperations::scale(const char* fileName, double scale);

//Creates a new wave file wherein the audio's time scale is scaled by the coefficient specified in the parameter scale. This shortens/lengthens the audio track, as well as alters the audio's pitch.

static const char** AudioOperations::split(const char* fileName);
	
//Creates two new mono wave files from the stereo wave file. Only works on stereo tracks.

static const char* AudioOperations::merge(const char* fileNameL, const char* fileNameR, bool loop);

//Creates one stereo wave file from two mono wave files. If loop = false, the longer file will be trimmed to be the  same length as the shorter file. If loop = true, the shorter file will be looped until the end of the longer file. Audio format, sample rate, and bit depth must be equal. Only works with mono files.