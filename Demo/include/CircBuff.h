//
//  CircBuff.h
//  
//
//  Created by Philadelphia Game Lab on 6/30/14.
//
//

#ifndef _CircBuff_h
#define _CircBuff_h

#include <stddef.h>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>
//#include <sndfile.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

template <class T>
class CircBuff{
protected:
    /*Declaring variables for use by circular buffer functions
     _begIndex is buffer head, 3DMixer will read from here
     _endIndex is buffer tail, buffer will write from wav file here
     _capacity is circular buffer capacity
     */
    size_t _begIndex, _endIndex, _capacity;
    
    //_size is the current size of the buffer, amount of data written to buffer
    std::atomic<size_t>_size;
    
    //pointer to circular buffer, start writing data from here
    T *_data;
    
public:
       CircBuff(size_t capacity)
    :   _begIndex(0)
    ,   _endIndex(0)
    ,   _size(0)
    ,   _capacity(capacity)
    {
        _data = new T[capacity*sizeof(T)];
    }
    
    ~CircBuff()
    {
        delete[] _data;
    }
    
    //Define write audio data to circular buffer
    size_t write(T *dataPtr, size_t samples);
    size_t writeSizeRemaining();
    size_t readSizeRemaining();
    template <class V>
    size_t read(V *dataPtr, size_t samples);
};

template <class T>
size_t CircBuff<T>::write(T *dataPtr, size_t samples)
{
    size_t capacity = _capacity;
    size_t samples_to_write = std::min(samples, capacity-_size);
    
    _size += samples_to_write;
    //writing to buffer
    
    if (samples_to_write <= capacity - _endIndex) {
        for(int i = 0; i < samples_to_write; i++) {
            _data[_endIndex+i] = dataPtr[i];
        }
        _endIndex += samples_to_write;
        if (_endIndex == capacity) {
            _endIndex = 0;
        }
    } else {
        size_t size1 = capacity - _endIndex;
        for(int i = 0; i < size1; i++) {
            _data[_endIndex+i] = dataPtr[i];
        }
        size_t size2 = samples_to_write - size1;
        for(int i = 0; i < size2; i++) {
            _data[i] = dataPtr[i+size1];
        }
        _endIndex = size2;
    }
    
    return samples_to_write;
}

template <class T>
size_t CircBuff<T>::writeSizeRemaining()
{
    return (_capacity - _size);
}

template <class T>
size_t CircBuff<T>::readSizeRemaining()
{
    return _size;
}

template <class T> template <class V>
size_t CircBuff<T>::read(V *dataPtr, size_t samples)
{
    if (samples == 0){
        return 0;
    }
    size_t capacity = _capacity;
    size_t samples_to_read;
    if (samples >= _size){
        samples_to_read = _size;
    }else{
        samples_to_read = samples;
    }
    // Read in a single step
    if (samples_to_read <= capacity - _begIndex){
        for(int i = 0; i < samples_to_read; i++) {
            dataPtr[i] = _data[_begIndex+i];
        }
        
        _begIndex += samples_to_read;
        if (_begIndex == capacity){
            _begIndex = 0;
        }
    }else{
        size_t size_1 = capacity - _begIndex;
        for(int i = 0; i < size_1; i++) {
            dataPtr[i] = _data[_begIndex+i];
        }
        size_t size_2 = samples_to_read - size_1;
        
        for(int i = 0; i < size_2; i++) {
            dataPtr[i+size_1] = _data[i];
        }
        _begIndex = size_2;
    }
    _size -= samples_to_read;
    return samples_to_read;
}



#endif
