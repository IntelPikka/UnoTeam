#include <Arduino.h>

/*
 * ============================================================
 * Mike Perry - The Ocean
 *
 * Arduino UNO / ATmega328P
 *
 * 악보 기준:
 *   Tempo : 180 BPM
 *   Time  : 4/4
 *   Key   : D minor
 *
 * 출력:
 *   Passive Piezo -> D10 (OC1B)
 *
 * 볼륨:
 *   10k Potentiometer -> A0
 *
 * 주의:
 *   Arduino UNO + 피에조 1개는 화음을 동시에 연주할 수 없으므로
 *   오른손 멜로디를 단선율로 변환한다.
 *
 *   후반부 화음은 멜로디가 되는 높은 음을 선택한다.
 *   왼손 반주는 제외한다.
 * ============================================================
 */


/* ============================================================
 * 핀 설정
 * ============================================================ */

#define BUZZER_PIN 10
#define VOLUME_PIN A0


/* ============================================================
 * 음계 주파수
 * ============================================================ */

#define NOTE_C4   262
#define NOTE_CS4  277
#define NOTE_D4   294
#define NOTE_DS4  311
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370
#define NOTE_G4   392
#define NOTE_GS4  415
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_B4   494

#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_B5   988

#define NOTE_C6   1047
#define NOTE_CS6  1109
#define NOTE_D6   1175
#define NOTE_DS6  1245
#define NOTE_E6   1319
#define NOTE_F6   1397
#define NOTE_FS6  1480
#define NOTE_G6   1568
#define NOTE_A6   1760

#define NOTE_REST 0


/* ============================================================
 * 템포
 *
 * 180 BPM
 *
 * 1박 = 60000 / 180
 *     = 약 333.33 ms
 * ============================================================ */

#define BPM 180

const unsigned long BEAT_MS = 60000UL / BPM;


/* ============================================================
 * 음표 구조체
 *
 * duration:
 *
 * 1  = 온음표
 * 2  = 2분음표
 * 4  = 4분음표
 * 8  = 8분음표
 * 16 = 16분음표
 *
 * 점음표:
 *
 * 점4분음표 = 3/2 박
 * 점8분음표 = 3/4 박
 *
 * 아래에서는 ms 값을 직접 사용한다.
 * ============================================================ */

typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Note;


/* ============================================================
 * 음표 길이
 * ============================================================ */

#define SIXTEENTH  83
#define EIGHTH     167
#define DOTTED_EIGHTH 250
#define QUARTER    333
#define DOTTED_QUARTER 500
#define HALF        667
#define DOTTED_HALF 1000
#define WHOLE      1333


/* ============================================================
 * The Ocean
 *
 * 최종 악보의 오른손을 단선율로 변환한 데이터
 *
 * 1~16마디:
 *   인트로/주제
 *
 * 17~33마디:
 *   전개 및 아르페지오 구간
 *
 * 34~43마디:
 *   클라이맥스 및 종지
 *
 * 동시화음은 단일 피에조의 한계 때문에
 * 멜로디가 되는 상성부를 선택한다.
 * ============================================================ */

const Note theOcean[] =
{

    /* ========================================================
     * 1마디
     * ======================================================== */

    { NOTE_REST, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },


    /* ========================================================
     * 2마디
     * ======================================================== */

    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C5, QUARTER },
    { NOTE_REST, EIGHTH },


    /* ========================================================
     * 3마디
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 4마디
     * ======================================================== */

    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 5마디
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },


    /* ========================================================
     * 6마디
     * ======================================================== */

    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C5, EIGHTH },
    { NOTE_REST, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C5, EIGHTH },


    /* ========================================================
     * 7마디
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 8마디
     * ======================================================== */

    { NOTE_C5, QUARTER },
    { NOTE_REST, QUARTER },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },


    /* ========================================================
     * 9마디
     * ======================================================== */

    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_E5, EIGHTH },


    /* ========================================================
     * 10마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, QUARTER },


    /* ========================================================
     * 11마디
     * ======================================================== */

    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 12마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_E5, EIGHTH },


    /* ========================================================
     * 13마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, QUARTER },


    /* ========================================================
     * 14마디
     * ======================================================== */

    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_D5, EIGHTH },


    /* ========================================================
     * 15마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_C5, EIGHTH },


    /* ========================================================
     * 16마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_F5, QUARTER },
    { NOTE_REST, EIGHTH },


    /* ========================================================
     * 17마디
     * F 계열
     * ======================================================== */

    { NOTE_F5, QUARTER },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },


    /* ========================================================
     * 18마디
     * ======================================================== */

    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, QUARTER },


    /* ========================================================
     * 19마디
     * Dm
     * ======================================================== */

    { NOTE_D5, QUARTER },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },


    /* ========================================================
     * 20마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, QUARTER },


    /* ========================================================
     * 21마디
     * Bb
     * ======================================================== */

    { NOTE_AS4, QUARTER },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_AS4, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 22마디
     * ======================================================== */

    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_AS4, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, QUARTER },


    /* ========================================================
     * 23마디
     * C
     * ======================================================== */

    { NOTE_C5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_C5, EIGHTH },
    { NOTE_G5, EIGHTH },


    /* ========================================================
     * 24마디
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_C5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, QUARTER },


    /* ========================================================
     * 25마디
     * F
     * ======================================================== */

    { NOTE_C5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },


    /* ========================================================
     * 26마디
     * ======================================================== */

    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, QUARTER },


    /* ========================================================
     * 27마디
     * Dm
     * ======================================================== */

    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_D6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_D6, EIGHTH },


    /* ========================================================
     * 28마디
     * ======================================================== */

    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_D6, EIGHTH },
    { NOTE_A5, QUARTER },


    /* ========================================================
     * 29마디
     * Bb
     * ======================================================== */

    { NOTE_AS4, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_AS5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_AS5, EIGHTH },


    /* ========================================================
     * 30마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_AS4, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_AS5, EIGHTH },
    { NOTE_F5, QUARTER },


    /* ========================================================
     * 31마디
     * C
     * ======================================================== */

    { NOTE_C5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },


    /* ========================================================
     * 32마디
     * ======================================================== */

    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_C5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, QUARTER },


    /* ========================================================
     * 33마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, QUARTER },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, QUARTER },


    /* ========================================================
     * 34마디
     * 클라이맥스 시작
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },


    /* ========================================================
     * 35마디
     * Dm
     * ======================================================== */

    { NOTE_D6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_D6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },


    /* ========================================================
     * 36마디
     * ======================================================== */

    { NOTE_D6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_D6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, QUARTER },


    /* ========================================================
     * 37마디
     * Bb
     * ======================================================== */

    { NOTE_AS5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_AS5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },


    /* ========================================================
     * 38마디
     * ======================================================== */

    { NOTE_AS5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_AS5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, QUARTER },


    /* ========================================================
     * 39마디
     * C
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },


    /* ========================================================
     * 40마디
     * ======================================================== */

    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_G5, EIGHTH },
    { NOTE_E5, QUARTER },


    /* ========================================================
     * 41마디
     * 마지막 Dm/F 계열
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, QUARTER },


    /* ========================================================
     * 42마디
     * ======================================================== */

    { NOTE_F5, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_C6, EIGHTH },
    { NOTE_A5, EIGHTH },
    { NOTE_F5, EIGHTH },
    { NOTE_D5, EIGHTH },
    { NOTE_C5, HALF },


    /* ========================================================
     * 43마디
     * 종지
     * ======================================================== */

    { NOTE_REST, WHOLE }
};


/* ============================================================
 * 전체 음표 개수
 * ============================================================ */

const uint16_t NOTE_COUNT =
    sizeof(theOcean) / sizeof(theOcean[0]);


/* ============================================================
 * 현재 주파수 설정
 *
 * Timer1 Fast PWM Mode 15
 *
 * D10 = OC1B
 *
 * PWM 주파수:
 *
 * F_CPU / (1 + OCR1A)
 *
 * F_CPU = 16 MHz
 * ============================================================ */

void setFrequency(uint16_t frequency)
{
    if (frequency == 0)
    {
        OCR1B = 0;
        return;
    }


    uint32_t top;

    top =
        (16000000UL / frequency) - 1UL;


    if (top > 65535UL)
    {
        top = 65535UL;
    }


    OCR1A = (uint16_t)top;


    /*
     * 볼륨을 다시 읽어서
     * 현재 음의 PWM 듀티를 설정한다.
     */

    uint16_t volume =
        analogRead(VOLUME_PIN);


    /*
     * 최대 50% 듀티
     */

    uint32_t duty =
        ((uint32_t)(OCR1A + 1UL) *
         volume) / 2046UL;


    if (duty > OCR1A)
    {
        duty = OCR1A;
    }


    OCR1B = (uint16_t)duty;
}


/* ============================================================
 * 볼륨 업데이트
 * ============================================================ */

void updateVolume()
{
    uint16_t volume =
        analogRead(VOLUME_PIN);


    /*
     * 0 ~ 1023
     *
     * 0% ~ 50% duty
     */

    uint32_t duty =
        ((uint32_t)(OCR1A + 1UL) *
         volume) / 2046UL;


    if (duty > OCR1A)
    {
        duty = OCR1A;
    }


    OCR1B = (uint16_t)duty;
}


/* ============================================================
 * 소리 끄기
 * ============================================================ */

void stopSound()
{
    OCR1B = 0;
}


/* ============================================================
 * Timer1 초기화
 *
 * Fast PWM Mode 15
 *
 * TOP = OCR1A
 *
 * OC1B = D10
 * ============================================================ */

void timer1Init()
{
    pinMode(BUZZER_PIN, OUTPUT);


    /*
     * Timer1 초기화
     */

    TCCR1A = 0;
    TCCR1B = 0;


    /*
     * Fast PWM Mode 15
     *
     * WGM13:0 = 1111
     */

    TCCR1A |=
        (1 << COM1B1) |
        (1 << WGM11) |
        (1 << WGM10);


    TCCR1B |=
        (1 << WGM13) |
        (1 << WGM12);


    /*
     * Prescaler = 1
     *
     * 16 MHz
     */

    TCCR1B |=
        (1 << CS10);


    OCR1A = 0;
    OCR1B = 0;
}


/* ============================================================
 * 한 음 연주
 * ============================================================ */

void playNote(
    uint16_t frequency,
    uint16_t duration
)
{
    /*
     * 쉼표
     */

    if (frequency == NOTE_REST)
    {
        stopSound();

        unsigned long start =
            millis();


        while (millis() - start < duration)
        {
            delay(1);
        }


        return;
    }


    /*
     * 음의 주파수 설정
     */

    setFrequency(frequency);


    /*
     * 음표가 재생되는 동안
     * 가변저항을 계속 확인한다.
     *
     * 따라서 연주 중에도 볼륨 조절 가능
     */

    unsigned long start =
        millis();


    while (millis() - start < duration)
    {
        updateVolume();

        delay(2);
    }


    /*
     * 다음 음과 분리
     */

    stopSound();

    delay(5);
}


/* ============================================================
 * 전체 곡 연주
 * ============================================================ */

void playTheOcean()
{
    for (uint16_t i = 0;
         i < NOTE_COUNT;
         i++)
    {
        playNote(
            theOcean[i].frequency,
            theOcean[i].duration
        );
    }
}


/* ============================================================
 * 초기화
 * ============================================================ */

void setup()
{
    /*
     * 가변저항
     */

    pinMode(
        VOLUME_PIN,
        INPUT
    );


    /*
     * Timer1
     */

    timer1Init();


    /*
     * 처음에는 무음
     */

    stopSound();
}


/* ============================================================
 * 메인
 * ============================================================ */

void loop()
{
    /*
     * The Ocean 연주
     */

    playTheOcean();


    /*
     * 곡이 끝난 후 2초 대기
     */

    stopSound();

    delay(2000);
}