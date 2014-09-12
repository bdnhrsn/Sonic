#include <math.h>
#include "../include/Mixer3D.h"
#include "../include/World.h"
#include "../include/fft.h"
#include "../include/mit_hrtf_lib.h"

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w) :
myWorld(w), bufferSize(bufSize), sampleRate(smpRate), bitDepth(bitD), player(myWorld->getPlayer())
{
    inputAO = new complex[2*bufferSize];
    overlapInput = new complex[2 * bufferSize];
    fInput = new complex[2 * bufferSize];
    fFilter = new complex[2 * bufferSize];
    
    azimuths = new int[World::MAX_OBJ];
    elevations = new int[World::MAX_OBJ];
    prevAzimuths = new int[World::MAX_OBJ];
    prevElevations = new int[World::MAX_OBJ];

    updateAngles(); // initialize azimuths and elevations

    outputLeft = new complex*[World::MAX_OBJ];
    outputRight = new complex*[World::MAX_OBJ];
    overlapLeft = new complex*[World::MAX_OBJ];
    overlapRight = new complex*[World::MAX_OBJ];

    leftFilter = new short[2 * bufferSize];
    rightFilter = new short[2 * bufferSize];
    complexLeftFilter = new complex*[World::MAX_OBJ];
    complexRightFilter = new complex*[World::MAX_OBJ];

    for (int i = 0; i < World::MAX_OBJ; i++)
    {
        // TODO: Should we initialize filter values?
        overlapLeft[i] = new complex[bufferSize];
        overlapRight[i] = new complex[bufferSize];
        outputLeft[i] = new complex[2*bufferSize];
        outputRight[i] = new complex[2*bufferSize];
        complexLeftFilter[i] = new complex[2*bufferSize];
        complexRightFilter[i] = new complex[2*bufferSize];
        
        // TODO: Is there a more efficient way to know which objects
        // have changed position relative to the player?
        // Store some state each time an AudioObject is moved?
        // What if the player moves in the same direction?
        prevAzimuths[i] = INVALID_ANGLE;
        prevElevations[i] = INVALID_ANGLE;
    }    
}

Mixer3D::~Mixer3D()
{
    delete [] inputAO;
    delete [] outputLeft;
    delete [] outputLeft;
    delete [] leftFilter;
    delete [] rightFilter;
    delete [] complexLeftFilter;
    delete [] complexRightFilter;
    delete [] overlapLeft;
    delete [] overlapRight;
    delete [] overlapInput;
    delete [] prevAzimuths;
    delete [] prevElevations;
    delete [] azimuths;
    delete [] elevations;
}

void Mixer3D::updateAngles() {
    for (int i = 0; i < myWorld->getNumObj(); i++) {
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
        azimuths[i] = player.computeAzimuth(iAudioObj);
        elevations[i] = player.computeElevation(iAudioObj);
        printf("azimuths[%d]: %d\n", i, azimuths[i]);
        printf("elevations[%d]: %d\n", i, elevations[i]);
    }
}

bool Mixer3D::isPowerOfTwo(int x) {
    return !(x == 0) && !(x & (x - 1));
}

int Mixer3D::loadHRTF(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilterIn, complex *&rightFilterIn)
{
    int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, leftFilter, rightFilter);

    for (int i = 0; i < size; i++)
    {
        leftFilterIn[i] = (double)(leftFilter[i]);
        rightFilterIn[i] = (double)(rightFilter[i]);
    }

    return size;
}
void Mixer3D::convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT) {

	if (input == NULL || filter == NULL) {
		throw invalid_argument("Input and Filter must be non-NULL.");
	}

	if (!isPowerOfTwo(nFFT) || nFFT < nSig || nFFT < nFil) {
        throw invalid_argument("NFFT must be a power of two, bigger than the signal length, and bigger than the filter length.");
	}

	// Perform FFT on both input and filter.
    // TODO: "Streamline" CFFT class?
	CFFT::Forward(input, fInput, (unsigned int)nFFT);
	CFFT::Forward(filter, fFilter, (unsigned int)nFFT);

	for (int i = 0; i < nFFT; i++) {
		output[i] = fInput[i] * fFilter[i];
    }
	CFFT::Inverse(output, (unsigned int)nFFT);
}

void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSIG, long nFIL, long nFFT)
{
    // TODO: Modify parameter name, input is a data member
    convolution(input, leftFilter, leftOutput, nSIG, nFIL, nFFT);
    convolution(input, rightFilter, rightOutput, nSIG, nFIL, nFFT);
}

void Mixer3D::performMix(short *ioDataLeft, short *ioDataRight)
{
    //Zero the output arrays.
    for(int i = 0; i < bufferSize; i++)
    {
       ioDataLeft[i] = 0;
       ioDataRight[i] = 0;
    }
    
    //Iterate through all audio objects, obtain input data and calculate resulting audio data for each object.
    // TODO: Possible bug? Cuts short if one audioObj is inactive?
    for (int i = 0; i < myWorld->getNumObj() && myWorld->getAudioObj(i)->isActive(); i++) {
        // dynamically calculate the Azimuth and Elevation between every object and the player
        updateAngles();
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
   
        // loading in input data for the iteration accordingly
        if (!(iAudioObj->fillAudioData(inputAO, bufferSize))) {
            continue;
        }        
       
        // scale volume according to distance of object from player
        // TODO: Is this realistic?
        float iVolume = iAudioObj->getVolume();
        float iDistance = player.computeDistanceFrom(iAudioObj) ;
        float iAmplitudeFactor = iVolume / iDistance;
        for(int j = 0; j < bufferSize * 2; j++) {
            if ( j >= bufferSize ) {
                // zero pad
                inputAO[j] = 0;
            } else {
                inputAO[j] *= iAmplitudeFactor;
            }
        }
       
        if (azimuths[i] != prevAzimuths[i] ||
           elevations[i] != prevElevations[i]) {
            // object location relative to player has changed, so fetch a new filter
            filterLength = loadHRTF(&azimuths[i], &elevations[i], sampleRate, 1, complexLeftFilter[i], complexRightFilter[i]);
            
            // zero pad
            for (int j = filterLength; j < 2 * bufferSize; j++) {
                complexLeftFilter[i][j] = 0;
                complexRightFilter[i][j] = 0;
            }
            
            if (azimuths[i] < 0) {
                   azimuths[i] = -azimuths[i];
            }
            
            
            //Since the filter changed, we perform a convolution on the old input with the new filter and pull out its tail.
            stereoConvolution(overlapInput, complexLeftFilter[i], complexRightFilter[i], outputLeft[i], outputRight[i], bufferSize, filterLength, 2 * bufferSize);
        
            // update the overlap part for the next iteration
            for (int j = 0; j < bufferSize; j++) {
                    overlapLeft[i][j] = outputLeft[i][j + bufferSize];
                    overlapRight[i][j] = outputRight[i][j + bufferSize];
            }
        }
        
        //Perform the convolution of the current input and current filter.
        stereoConvolution(inputAO, complexLeftFilter[i], complexRightFilter[i], outputLeft[i], outputRight[i], bufferSize, filterLength, 2 * bufferSize);
        
        
        for (int j = 0; j < bufferSize; j++) {
            ioDataLeft[j] +=  (short((outputLeft[i][j].re())/2 + (overlapLeft[i][j].re())/2))/myWorld->getNumObj();
            ioDataRight[j] += (short((outputRight[i][j].re())/2 + (overlapRight[i][j].re())/2))/myWorld->getNumObj();
        }
        
  
        //Output the data to the output arrays in short integer format. In addition to pushing the output of the main
        //convolution, we also need to add the overlapped tail of the last output and divide by 2. Finally, we need
        //to divide by the number of audio objects to ensure no clipping.
        for (int j = 0; j < bufferSize; j++)
        {
            ioDataLeft[j]  += (short)( (outputLeft[i][j].re() + overlapLeft[i][j].re()) / 2*myWorld->getNumObj() );
            ioDataRight[j] += (short)( (outputRight[i][j].re() + overlapRight[i][j].re()) / 2*myWorld->getNumObj() );    
        }
        
        //Updating the overlapInput for the next iteration for the correpsonding obejct
        for (int j = 0; j < bufferSize * 2; j++) {
            if (j >= bufferSize) {
                overlapInput[j] = 0;
            } else {
                overlapInput[j] = inputAO[j];
            }
        }

        // TODO: If the filter has been changed, didn't we already do this?
        //Updating the default overlap information for the next iteration if the filter won't be changed
        for (int j = 0 ; j < bufferSize; j++) {
            overlapLeft[i][j]=outputLeft[i][j + bufferSize];
            overlapRight[i][j]=outputRight[i][j + bufferSize];
        }
    
        //storing the Azimuth value in this iteration for the comparison for the next iteration so that
        //we can know that whether the filter needs to be changed in the next iteration.
        prevAzimuths[i]=azimuths[i];
    }
}
