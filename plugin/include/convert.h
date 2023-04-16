#ifndef CONVERT_H
#define CONVERT_H

#if defined(_WIN32) || defined(_WIN64)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#endif

extern "C" DLLEXPORT float convert(char *someText, double optValue, char *outputBuffer, int optBuffer1Size, char *pOptBuffer2, int optBuffer2Size, char **zData);

#endif
