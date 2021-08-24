#include "Uart.h"
#include "CpuDelay.h"
#include "Bsp.h"
#include "BspConfig.h"
#include "Debug.h"
#include "AccessPoint.h"
#include "Lcd.h"
#include "MotorHandling.h"
#include "MecaFeature.h"
#include "Ota.h"
#include "Board.h"
#include "ModeAutomatic.h"
#include "ModeManual.h"
#include "ModeSystem.h"
#include "ModeCalibration.h"

static void Midi_initDataTask(void);
static void Midi_queueTask(void * pvParam);
static void Midi_note(const char * note, const unsigned char value, const unsigned char mode);

typedef void (*ptxMidi)(const char *, const unsigned char, const unsigned char);

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_C1  0x24
#define MIDI_NOTE_C2  0x30
#define MIDI_NOTE_D1  0x14

static struct {
    const char * note;
    const char value;
    ptxMidi midiCallback;
} midi_protocol[] = {
    {"D1", MIDI_NOTE_D1, Midi_note},
    {"C1", MIDI_NOTE_C1, Midi_note},
    {"C2", MIDI_NOTE_C2, Midi_note},
    /* We can add note */
    { }
};

int oldFreqForMidi;

/*
   Pitch  /  Freq /  MIDI     Pitch  /  Freq /  MIDI     Pitch  /  Freq /  MIDI
   --------------------------------------------------------------------------------
   C0       -       12        C4    261.626    60*MC     C8   4186.009   108*PH
   C#0       -       13       C#4    277.183    61       C#8   4434.922   109
   D0       -       14        D4    293.665    62        D8   4698.637   110
   D#0       -       15       D#4    311.127    63       D#8   4978.032   111
   E0       -       16        E4    329.628    64        E8   5274.042   112
   F0       -       17        F4    349.228    65        F8   5587.652   113
   F#0       -       18       F#4    369.994    66       F#8   5919.912   114
   G0       -       19        G4    391.995    67        G8   6271.928   115
   G#0       -       20       G#4    415.305    68       G#8   6644.876   116
   A0     27.500    21*PL     A4    440.000    69        A8   7040.000   117
   A#0     29.135    22       A#4    466.164    70       A#8   7458.620   118
   B0     30.868    23        B4    493.883    71        B8   7902.133   119

   C1     32.703    24        C5    523.251    72        C9   8372.019   120
   C#1     34.648    25       C#5    554.365    73       C#9   8869.845   121
   D1     36.708    26        D5    587.330    74        D9   9397.273   122
   D#1     38.891    27       D#5    622.254    75       D#9   9956.064   123
   E1     41.203    28        E5    659.255    76        E9  10548.083   124
   F1     43.654    29        F5    698.457    77        F9  11175.305   125
   F#1     46.249    30       F#5    739.989    78       F#9  11839.823   126
   G1     48.999    31        G5    783.991    79        G9  12543.855   127
   G#1     51.913    32       G#5    830.609    80       G#9  13289.752    -
   A1     55.000    33        A5    880.000    81        A9       -       -
   A#1     58.270    34       A#5    932.328    82       A#9       -       -
   B1     61.735    35        B5    987.767    83        B9       -       -

   C2     65.406    36        C6   1046.502    84
   C#2     69.296    37       C#6   1108.731    85
   D2     73.416    38        D6   1174.659    86
   D#2     77.782    39       D#6   1244.508    87
   E2     82.407    40        E6   1318.510    88
   F2     87.307    41        F6   1396.913    89
   F#2     92.499    42       F#6   1479.978    90
   G2     97.999    43        G6   1567.982    91
   G#2    103.826    44       G#6   1661.219    92
   A2    110.000    45        A6   1760.000    93
   A#2    116.541    46       A#6   1864.655    94
   B2    123.471    47        B6   1975.533    95

   C3    130.813    48        C7   2093.005    96
   C#3    138.591    49       C#7   2217.461    97
   D3    146.832    50        D7   2349.318    98
   D#3    155.564    51       D#7   2489.016    99
   E3    164.814    52        E7   2637.021   100
   F3    174.614    53        F7   2793.826   101
   F#3    184.997    54       F#7   2959.956   102
   G3    195.998    55        G7   3135.964   103
   G#3    207.652    56       G#7   3322.438   104
   A3    220.000    57        A7   3520.000   105
   A#3    233.082    58       A#7   3729.310   106
   B3    246.942    59        B7   3951.066   107
 * */
static OsQueueHandle_t tsQueueMidi = NULL;

void Midi_init()
{
    Uart_init(1, 31250, RX_MIDI_3V3, TX_MIDI_3V3);
    CpuDelay_ms(200);
    Midi_initDataTask();

    if(tsQueueMidi == NULL)
        tsQueueMidi = OsQueueCreate(10, 3);

    if(tsQueueMidi != NULL)
        OsTaskCreate(Midi_queueTask, "MIDI task", 4096, NULL, 4, NULL);
}

static void Midi_queueTask(void * pvParam)
{
    unsigned char eAsyncMsg[4];

    for(;;) {
        memset(eAsyncMsg, '\0', 4);
        OsQueueReceive(tsQueueMidi, &eAsyncMsg, OsPortTimingPeriod);

        //        for(int i = 0; i < 3; i++)
        //            printf("%x\n", eAsyncMsg[i]);

        for(int i = 0; midi_protocol[i].note; i++) {
            if(midi_protocol[i].value == eAsyncMsg[1])
                midi_protocol[i].midiCallback(midi_protocol[i].note, (unsigned char)eAsyncMsg[1], (unsigned char)eAsyncMsg[0]);
        }
    }
}

void Midi_executeTaskFromISR(unsigned char * eEvent)
{
    OsQueueSendFromISR(tsQueueMidi, eEvent, NULL);
}

static void Midi_TASK_DATA()
{
    unsigned char data[4];
    memset(data, '\0', 4);

    for(;;) {
        if(Uart_read(1, 3, data) != -1) {
            Midi_executeTaskFromISR(data);
        }
    }
}

static void Midi_initDataTask(void)
{
    OsTaskCreate(Midi_TASK_DATA, "midi_uart_task", 4096, NULL, 4, NULL);
}

void Midi_sendCmd(int len, const char * data)
{
    Uart_write(1, len, data);
}

void Midi_sendD1CmdOn()
{
    //start cubase
    static char tmp[3] = {0x90, 0x14, 0x7F};
    Midi_sendCmd(3, tmp); //MIDI note D1 ON
}

void Midi_sendD2CmdOn()
{
    //put cubase to 0
    static char tmp[3] = {0x90, 0x38, 0x7F};
    Midi_sendCmd(3, tmp); //MIDI note D2 ON
}

void Midi_sendD3CmdOn()
{
    //stop cubase
    static char tmp[3] = {0x90, 0x50, 0x7F};
    Midi_sendCmd(3, tmp); //MIDI note D3 ON
}

void Midi_sendD1CmdOff()
{
    static char tmp[3] = {0x80, 0x14, 0x7F}; //MIDI note D1 OFF
    Midi_sendCmd(3, tmp);
}

void Midi_sendD2CmdOff()
{
    static char tmp[3] = {0x80, 0x38, 0x7F}; //MIDI note D2 OFF
    Midi_sendCmd(3, tmp);
}

void Midi_sendD3CmdOff()
{
    static char tmp[3] = {0x80, 0x50, 0x7F}; //MIDI note D3 OFF
    Midi_sendCmd(3, tmp);
}

static void Midi_note(const char * note, const unsigned char value, const unsigned char mode)
{
    if(Lcd_getMode() == LCD_MENU_MODE_TESTS)
        Lcd_sendTxtCmd(strlen(note), "t6", (const char *)note);

    if((Lcd_getMode() == LCD_MENU_MODE_AUTOMATIC) && (ModeAutomatic_modeIsStarted() == 1)) {
        if((mode == MIDI_NOTE_ON) && (value == MIDI_NOTE_C1)) {
            Debug_info("Note %s = on \n", note);
            oldFreqForMidi = MotorHandling_getFreq();
            //TODO: Switch wrom 2 and 8mm
            MotorHandling_start(ModeCalibration_getMotorValC1ForMidi(), 0);
        }

        if((mode == MIDI_NOTE_OFF) && (value == MIDI_NOTE_C1)) {
            Debug_info("Note %s = off \n", note);

            if(oldFreqForMidi > 0) {
                if(oldFreqForMidi > 15) {
                    MotorHandling_start(oldFreqForMidi, 0);
                } else {
                    MotorHandling_hookInit(oldFreqForMidi, 0);
                }
            }
        }

        if((mode == MIDI_NOTE_ON) && (value == MIDI_NOTE_C2)) {
            Debug_info("Note %s = on \n", note);
            //TODO: Switch wrom 2 and 8mm
            MotorHandling_start(ModeCalibration_getMotorValC2ForMidi(), 0);
        }

        if((mode == MIDI_NOTE_OFF) && (value == MIDI_NOTE_C2)) {
            Debug_info("Note %s = off \n", note);
        }
    }
}
