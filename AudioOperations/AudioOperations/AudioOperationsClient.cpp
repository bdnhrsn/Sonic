// AudioOperations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AudioOperations.h"

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* in = fopen("EpicDubstepExplosion.wav","r");

	if (!in) {
		std::cout << "!";
	} else {

		AudioOperations *op = new AudioOperations(in);
		op->fadeIn();

		std::cin.ignore();
		return 0;
	}
}