#include <math.h>
#include "../include/Mixer3D.h"
#include "../include/World.h"
#include "../include/fft.h"
#include "../include/mit_hrtf_lib.h"

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w):myWorld(w), bufferSize(bufSize), sampleRate(smpRate), bitDepth(bitD)
{
    Player &player = myWorld->getPlayer();
    inputAO = new complex[2*bufferSize];
    overlapInput = new complex[2 * bufferSize];
    fInput = new complex[2 * bufferSize];
    fFilter = new complex[2 * bufferSize];
    
    azimuth = new int[World::MAX_OBJ];
    elevation = new int[World::MAX_OBJ];
    previousAzimuth = new int[World::MAX_OBJ];
    signFlag = false;
    filterFlag = false;

    outputLeft = new complex*[World::MAX_OBJ];
    outputRight = new complex*[World::MAX_OBJ];
    overlapLeft = new complex*[World::MAX_OBJ];
    overlapRight = new complex*[World::MAX_OBJ];

    lFil = new short[2 * bufferSize];
    rFil = new short[2 * bufferSize];
    clFil = new complex*[World::MAX_OBJ];
    crFil = new complex*[World::MAX_OBJ];

    for (int i = 0; i < World::MAX_OBJ; i++)
    {
        overlapLeft[i] = new complex[bufferSize];
        overlapRight[i] = new complex[bufferSize];
        outputLeft[i] = new complex[2*bufferSize];
        outputRight[i] = new complex[2*bufferSize];
        clFil[i] = new complex[2*bufferSize];
        crFil[i] = new complex[2*bufferSize];
        previousAzimuth[i] = 190;
    }    
}

~Mixer3D()
{
    delete [] inputAO;
    delete [] outputLeft;
    delete [] outputLeft;
    delete [] lFil;
    delete [] rFil;
    delete [] clFil;
    delete [] crFil;
    delete [] overlapLeft;
    delete [] overlapRight;
    delete [] overlapInput;
    delete [] previousAzimuth;
    delete [] azimuth;
    delete [] elevation;
}

int Mixer3D::loadHRTF(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{
    int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, lFil, rFil);

    for (int i = 0; i < size; i++)
    {
        leftFilter[i] = (double)(lFil[i]);
        rightFilter[i] = (double)(rFil[i]);
    }

    return size;
}

void Mixer3D::convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT)
{
    CFFT::Forward(input, fInput, (unsigned int)nFFT);
    CFFT::Forward(filter, fFilter, (unsigned int)nFFT);

    for (int i = 0; i < nFFT; i++)
        output[i] = fInput[i] * fFilter[i];

    CFFT::Inverse(output, (unsigned int)nFFT);
}

void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSIG, long nFIL, long nFFT)
{
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
    for(int j = 0; j < myWorld->getAudioObj(j)->isActive() && myWorld->getNumObj(); j++)
    {
        AudioObj &currentAO = myWorld->getAudioObj(j);

        //Dynamically calculating the azimuth between every object and the player
        azimuth[j] = player->getAzimuth(currentAO);
        signFlag = azimuth[j] < 0;
        filterFlag = azimuth[j] / 5 != previousAzimuth[j] / 5;
        
        //Loading in input data for the iteration accordingly. If it fails, skip the object.
        if(!(currentAO->fillAudioData(inputAO, bufferSize)))
            continue;
        
        //Applying the distance factor and decrease the volumne if the object is far away from the player.
        float amplitudeFactor = currentAO->getVolume() / player->getDistance(currentAO);
        for(int i = 0; i < bufferSize ; i++)
            inputAO[i] *= amplitudeFactor;
        
        //Zero-padding the input for the convolution.
        for (int i = bufferSize; i < 2 * bufferSize; i++)
            inputAO[i] = 0;
        
        //If the filter has changed, we need to get the new filter and overwrite the saved overlap from the last iteration.
        if(filterFlag)
        {
            nTaps = loadHRTF(&azimuth[j], &elevation[j], sampleRate, 1, clFil[j], crFil[j]);
            
            //Zero-padding the filter for the convolution.
            for(int i = nTaps; i < 2 * bufferSize; i++)
            {
                clFil[j][i] = 0;
                crFil[j][i] = 0;
            }
            
            if(signFlag)
                azimuth[j] = -azimuth[j];
            
            //Since the filter changed, we perform a convolution on the old input with the new filter and pull out its tail.
            stereoConvolution(overlapInput, clFil[j], crFil[j], outputLeft[j], outputRight[j], bufferSize, nTaps, 2 * bufferSize);
            for (int i = 0; i < bufferSize; i++)
            {
                overlapLeft[j][i] = outputLeft[j][i + bufferSize];
                overlapRight[j][i] = outputRight[j][i + bufferSize];
            }        
        }
        
        //Perform the convolution of the current input and current filter.
        stereoConvolution(inputAO, clFil[j], crFil[j], outputLeft[j], outputRight[j], bufferSize, nTaps, 2 * bufferSize);
        
        //Output the data to the output arrays in short integer format. In addition to pushing the output of the main
        //convolution, we also need to add the overlapped tail of the last output and divide by 2. Finally, we need
        //to divide by the number of audio objects to ensure no clipping.
        for (int i = 0; i < bufferSize; i++)
        {
            ioDataLeft[i]  += (short)( (outputLeft[j][i].re() + overlapLeft[j][i].re()) / 2*myWorld->getNumObj() );
            ioDataRight[i] += (short)( (outputRight[j][i].re() + overlapRight[j][i].re()) / 2*myWorld->getNumObj() );    
        }
        
        //Updating the overlapInput for the next iteration for the corresponding object.
        for (int i = 0; i < bufferSize; i++)
            overlapInput[i] = inputAO[i];

        for (int i = bufferSize; i < 2 * bufferSize; i++)
            overlapInput[i] = 0;
    
        //Updating the default overlap information for the next iteration if the filter won't be changed.
        for(int i = 0 ; i < bufferSize; i++)
        {
            overlapLeft[j][i] = outputLeft[j][i + bufferSize];
            overlapRight[j][i] = outputRight[j][i + bufferSize];
        }

        previousAzimuth[j] = azimuth[j];   
    }
}