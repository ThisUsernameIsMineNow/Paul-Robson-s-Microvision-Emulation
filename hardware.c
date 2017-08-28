//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Hardware.C
//      Purpose:    Hardware Interface Layer (modified for SDL2)
//      Author:     Paul Robson
//      Date:       1st January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "hardware.h"
#include "hwinterface.h"
#include "debugsc11.h"
#include <SDL2/SDL.h>

#include "font.h"                                                                       // 5 x 7 font data.

#define SOUND                                                                           // Sound enabled in this SDL interface

#define WIDTH   (800)
#define HEIGHT  (600)

static BOOL isSoundOn = FALSE;                                                          // Sound status.
int  pitch = 440;                                                                       // Current sound pitch
static int cyclePos;
static SDL_Window *window;                                                              // Window
static SDL_Renderer* renderer;                                                          // Accelerated Renderer.

static int widthDisplay = WIDTH/2;
static int heightDisplay = HEIGHT/2;

#define KEYCOUNT    (36)                                                                // 0-9 A-Z

static BOOL isKeyPressed[KEYCOUNT];                                                     // 36 key check values.
static int rotaryControl = 0;                                                           // Rotary Control position (0-100)
static int lastRotaryTimeCheck = 0;                                                     // Time of last check
static SDL_Surface *overlay;                                                            // Overlay bitmap
static SDL_Texture *overlayTexture;                                                     // as a texture

static void audioCallback(void *_beeper, Uint8 *_stream, int _length);

//*******************************************************************************************************
//                          Initialise the Interface Layer
//*******************************************************************************************************

void IF_Initialise(int w,int h)
{
    char buffer[64];
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0)                                     // Initialise SDL
        exit(fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() ));
    atexit(IF_Terminate);                                                               // Call terminate on the way out.
    sprintf(buffer,"Microvision Emulator (TMS1100) : Build %d",BUILD);
    window = SDL_CreateWindow(buffer, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,0);
    renderer = SDL_CreateRenderer(window, -1, 0);    
    SDL_SetRelativeMouseMode(TRUE);                                                     // Grab mouse events.
    overlay = NULL;                                                                     // No overlay.

    if (w != 0 && h != 0) {                                                             // Set display size if nonzero
        widthDisplay = w;heightDisplay = h;
    }

    #ifdef SOUND
    SDL_AudioSpec desiredSpec;                                                          // Create an SDL Audio Specification.
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audioCallback;
    SDL_AudioSpec obtainedSpec;                                                         // Request the specification.
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);
    isSoundOn = FALSE;
    IF_SetSound(FALSE);                                                                 // Sound off.
    #endif
}

//*******************************************************************************************************
//                      Render the interface layer - in debug mode, or not
//*******************************************************************************************************

BOOL IF_Render(BOOL debugMode)
{
    int key;
    SDL_Event event;
    BOOL quit = FALSE;
    while(SDL_PollEvent(&event))                                                        // Empty the event queue.
    {
        if (event.type == SDL_MOUSEMOTION) {
            rotaryControl = rotaryControl + event.motion.xrel / 2;
            if (rotaryControl < 0) rotaryControl = 0;
            if (rotaryControl > 100) rotaryControl = 100;
        }

        if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)                       // Is it a key event
        {
            key = event.key.keysym.sym;                                                 // This is the SDL Key Code
            if (key == SDLK_ESCAPE)                                                     // Esc key ends program.
                                quit = TRUE;
            if (key >= SDLK_0 && key <= SDLK_9) {
                ASSERT(RANGE(key-SDLK_0,0,9));
                isKeyPressed[key-SDLK_0] = (event.type == SDL_KEYDOWN);
            }
            if (key >= SDLK_a && key <= SDLK_z) { 
                ASSERT(RANGE(key-SDLK_a+10,10,35));
                isKeyPressed[key-SDLK_a+10] = (event.type == SDL_KEYDOWN);
            }

        } // end switch
    } // end of message processing

    SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
    SDL_RenderClear(renderer);
    if (debugMode) DB11_Draw();
    IF_RenderScreen(debugMode);
    SDL_RenderPresent(renderer);
    return quit;
}

//*******************************************************************************************************
//                                Load overlay if there is one.
//*******************************************************************************************************

void IF_LoadOverlay(char *binFile) {
    char buffer[128];
    if (strlen(binFile) < 5) return;                                                    // Must be something.bin at least !
    strcpy(buffer,binFile);strcpy(buffer+strlen(buffer)-3,"bmp");                       // Create a .bmp file name
    overlay = SDL_LoadBMP(buffer);                                                      // Try to load it.
    if (overlay != NULL)
        overlayTexture = SDL_CreateTextureFromSurface(renderer,overlay);
}

//*******************************************************************************************************
//                                          Render Screen
//*******************************************************************************************************

void IF_RenderScreen(BOOL debugMode) 
{
    SDL_Rect rMain;
    rMain.w = WIDTH/4;rMain.h = rMain.w;
    rMain.x = WIDTH-rMain.w-8;rMain.y = 8;
    if (!debugMode) {
        rMain.w = widthDisplay;
        rMain.h = heightDisplay;
        rMain.x = WIDTH / 2 - rMain.w / 2;
        rMain.y = HEIGHT / 2 - rMain.h / 2;
    }
    SDL_SetRenderDrawColor(renderer,160,160,160,0);
    SDL_RenderFillRect(renderer,&rMain);
    SDL_SetRenderDrawColor(renderer,32,32,32,0);
    for (int r = 0;r < 16;r++) {
        SDL_Rect rPixel;
        rPixel.w = rMain.w/16;
        rPixel.h = rMain.h/16;
        rPixel.x = rMain.x + rMain.w/2-rPixel.w*8;
        rPixel.y = rMain.y + rPixel.h * r + rMain.h/2 - rPixel.h*8;
        int pattern = HWIReadLCDRow(r);
        for (int c = 0;c < 16;c++) {
            if (pattern & 0x8000) SDL_RenderFillRect(renderer,&rPixel);
            else {
                if (debugMode) 
                    SDL_RenderDrawRect(renderer,&rPixel);
            }
            rPixel.x += rPixel.w; 
            pattern = (pattern << 1) & 0xFFFF;
        }
    }

    if (!debugMode && overlay != NULL) {                                                    // Draw the overlay in non-debug mode
        SDL_Rect rcDisplay;
        rcDisplay.w = rMain.w * 141 / 100;
        rcDisplay.h = rMain.h * 143 / 100;
        rcDisplay.x = rMain.x + rMain.w / 2 - rcDisplay.w/2;
        rcDisplay.y = rMain.y + rMain.h / 2 - rcDisplay.h/2;
        SDL_RenderCopy(renderer,overlayTexture,NULL,&rcDisplay);
    }
    rMain.h = rMain.h / 16;
    rMain.y = rMain.y + rMain.h * 17;
    if (!debugMode) rMain.y = HEIGHT - rMain.h - 32;
    SDL_SetRenderDrawColor(renderer,192,192,192,0);
    SDL_RenderFillRect(renderer,&rMain);
    rMain.x = rMain.x + rMain.w * rotaryControl / 100;
    rMain.w = 6;rMain.x -= rMain.w/2;rMain.y += 1;rMain.h -= 2;
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderFillRect(renderer,&rMain);
}

//*******************************************************************************************************
//                      Write Character to screen square (x,y) - 32 x 24
//*******************************************************************************************************

void IF_Write(int x,int y,char ch,int colour)
{
    int xCSize = WIDTH / 32;                                                            // Work out character box size.
    int yCSize = HEIGHT / 23;
    SDL_Rect rc;
    rc.x = xCSize * x;rc.y = yCSize * y;                                                // Erase character background.
    rc.w = xCSize;rc.h = yCSize;
    SDL_SetRenderDrawColor(renderer,0,0,64,255);
    SDL_RenderFillRect(renderer,&rc);
    if (ch <= ' ' || ch > 127) return;                                                  // Don't render control and space.
    unsigned char *byteData = fontdata + (int)((ch - ' ') * 5);                         // point to the font data
    int xp,yp,pixel;
    rc.w = xCSize * 16 / 100;                                                           // Work out pixel sizes
    rc.h = yCSize * 12 / 100;
    SDL_SetRenderDrawColor(renderer,(colour & 1) ? 255:0,(colour & 2) ? 255:0,(colour & 4) ? 255:0,255);
    for (xp = 0;xp < 5;xp++)                                                            // Font data is stored vertically
    {
        rc.x = xp * rc.w + x * xCSize;                                                  // Horizontal value
        pixel = *byteData++;                                                            // Pixel data for vertical line.
        for (yp = 0;yp < 7;yp++)                                                        // Work through pixels.
        {
            if (pixel & (1 << yp))                                                      // Bit 0 is the top pixel, if set.
            {
                rc.y = yp * rc.h + y * yCSize;                                          // Vertical value
                SDL_RenderFillRect(renderer,&rc);                                       // Draw Cell.
            }
        }
    }
}

//*******************************************************************************************************
//                              Terminate the interface layer
//*******************************************************************************************************

void IF_Terminate(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//*******************************************************************************************************
//                              Check to see if a key is pressed
//*******************************************************************************************************

BOOL IF_KeyPressed(char ch)
{
    if (!isalnum(ch)) return FALSE;
    if (isdigit(ch)) return isKeyPressed[ch - '0'];
    return isKeyPressed[toupper(ch)-'A'+10];
}

//*******************************************************************************************************
//                                    Control sound.
//*******************************************************************************************************

void IF_SetSound(BOOL isOn)
{
    if (isSoundOn == isOn) return;                                                      // No status change.
    isSoundOn = isOn;                                                                   // Update status
    #ifdef SOUND
    SDL_PauseAudio(isOn == 0);                                                          // If sound built in, turn on/off.
    #endif
}

void IF_SetPitch(int frequency)
{
    pitch = frequency;
}

//*******************************************************************************************************
//                              Audio Callback Function
//*******************************************************************************************************

static void audioCallback(void *_beeper, Uint8 *_stream, int _length)
{
    Sint16 *stream = (Sint16*) _stream;                                                 // Pointer to audio data
    int length = _length / 2;                                                           // Length of audio data
    int i;
    for (i = 0;i < length;i++)                                                          // Fill buffer with data
    {
        stream[i] = (cyclePos > 22050 ? -32767:32767);                                  // Square Wave 
        cyclePos = (cyclePos + pitch) % 44100;                                  
    }
}

//*******************************************************************************************************
//                  Get Tick Timer - needs about a 20Hz minimum granularity.
//*******************************************************************************************************

int IF_GetTime(void)
{
    return SDL_GetTicks();
}

//*******************************************************************************************************
//                                 Process Rotary Control moves every frame
//*******************************************************************************************************

void IF_ProcessRotaryControl() {
    int currentTime = IF_GetTime();                                                     // Current ms time
    int elapsed = currentTime - lastRotaryTimeCheck;                                    // Time since last check.
    lastRotaryTimeCheck = currentTime;                                                  // Update last check.
    if (elapsed > 100) elapsed = 100;                                                   // If paused for some reason.

    elapsed = elapsed / 10;
    if (IF_KeyPressed('O')) rotaryControl -= elapsed;
    if (IF_KeyPressed('P')) rotaryControl += elapsed;

    if (rotaryControl > 100) rotaryControl = 100;                                       // Force into range
    if (rotaryControl < 0) rotaryControl = 0;
}

//*******************************************************************************************************
//                                          Synchronise system.
//*******************************************************************************************************

static int lastSync = 0;

void XHWISynchronise(int milliseconds) {
    int syncTime = lastSync + milliseconds;                                             // Synchronise to this time.
    while (IF_GetTime() < syncTime) {}                                                  // Wait for time out.
    lastSync = IF_GetTime();                                                            // Remember time of last sync.
}

//*******************************************************************************************************
//                                          Test status of a key
//*******************************************************************************************************

static char * _keys[] = { "1QAZ","2WSX","3EDC" };

BYTE8 XHWIReadKeypad(int col,int row) {
    return IF_KeyPressed(_keys[col][row]);
}

//*******************************************************************************************************
//                                          Set Sound Pitch, 0 = Off
//*******************************************************************************************************

void XHWISetPitch(int freq) {
    if (freq == 0) 
        IF_SetSound(FALSE);
    else {
        IF_SetPitch(freq);
        IF_SetSound(TRUE);
    }
}

//*******************************************************************************************************
//                              Get Rotary Control Value (-1 if not supported)
//*******************************************************************************************************

int XHWIGetRotaryControl(void) {
    return rotaryControl;
}

