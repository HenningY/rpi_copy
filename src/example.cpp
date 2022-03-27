#include <iostream> 
#include <time.h> 
#include <string.h>
#include "../lib/LED_strip.h"
#include "../lib/OFrgba_to_bgriref.h"
#include "../lib/pca.h"
#include "../lib/pcaDefinition.h"
#include <fstream>

#define fps 30
using namespace std;

// the number of leds of every strip
// maximum 16 numbers in the vector ( maximum 16 led strips )
// declaration


// LED Parts

#define Finger_LEDs 28
#define Lower_Energy_LEDs 33
#define Upper_Energy_LEDs 69
#define Arm_Inside_LEDs 20
enum LEDparts {
    Finger_LEDd = 0,
    Lower_Energy_LEDd = 1,
    Upper_Energy_LEDd = 2,
    Arm_Inside_LEDd = 3
};
int Finger_LED[Finger_LEDs];
int Lower_Energy_LED[Lower_Energy_LEDs];
int Upper_Energy_LED[Upper_Energy_LEDs];
int Arm_Inside_LED[Arm_Inside_LEDs];

vector<uint16_t> nLeds;
vector< vector<char> > LED; 
LED_Strip strips;
vector< vector<vector<char>> > json;
vector<unsigned long long> timeStamps;
bool export_json=false;
// OF Parts

#define OF_Nums 20
enum OFparts {
    Thumb = 0,
    Fore_Finger_1 = 1,
    Fore_Finger_2 = 2,
    Middle_Finger_1 = 3,
    Middle_Finger_2 = 4,
    Ring_Finger_1 = 5,
    Ring_Finger_2 = 6,
    Little_Finger_1 = 7,
    Little_Finger_2 = 8,
    F_Diamond = 9,
    B_Diamond = 10,
    Palm_F_Side = 11,
    Palm_B_Side = 12,
    Palm_Shield = 13,
    Palm = 14,
    Arm_F_Side = 15,
    Arm_F_Shield = 16,
    Arm_B_Shield = 17,
    Arm_B_Side = 18,
    Arm_Inside = 19
};
/*
vector<vector<char>> OF;
PCA pca;
*/
unsigned long long timeStamp;


/*** need to modify by yourself ***/


/**********************************/

void init() {
    nLeds = {28, 33, 69, 20};
    strips.initialize(nLeds);
    LED.resize(nLeds.size());
    for(int i = 0; i < nLeds.size(); ++i){
		LED[i].resize(nLeds[i] * 3, (char)0);
	}
    // OF.resize(OF_Nums);
    // for(int i = 0; i < OF_Nums; ++i) {
    //     OF[i].resize(6);
    // }
    timeStamp = 0;
}

void delay(int t) {
    clock_t begin = clock();
    timeStamp += t;
    if (!export_json)
        while ((1000 * (clock() - begin)) / CLOCKS_PER_SEC < t); // CLOCK_PER_SEC is defined in time.h
}

int RGB(int r, int g, int b) {
    return (r << 8) | (g << 16) | b;
}

void lightchannel(int* channel, int size, int r, int g, int b) {
    for (int i = 0; i < size; ++i) {
        channel[i]  = RGB(r, g, b);
    }
}
/*
void loadOneStrip(uint8_t *strip, int channel) {
    for (int i = 0; i < nLeds[channel]; ++i) {
        buf[channel][3*i] = strip[3*i];
        buf[channel][3*i+1] = strip[3*i+1];
        buf[channel][3*i+2] = strip[3*i+2];
    }
}
*/

void showLED() {
    /*
    loadOneStrip(Finger_LED, 0);
    loadOneStrip(Lower_Energy_LED, 1);
    loadOneStrip(Upper_Energy_LED, 2);
    loadOneStrip(Arm_Inside_LED, 3);
    */
    static clock_t lastTime = clock();
    if (!export_json) {
        for (int i = 0; i < Finger_LEDs; ++i) {
            LED[LEDparts::Finger_LEDd][3*i + 0] = char(Finger_LED[i] >> 16 & 0xff);
            LED[LEDparts::Finger_LEDd][3*i + 1] = char(Finger_LED[i] >> 8 & 0xff);
            LED[LEDparts::Finger_LEDd][3*i + 2] = char(Finger_LED[i] & 0xff);
        }
        for (int i = 0; i < Lower_Energy_LEDs; ++i) {
            LED[LEDparts::Lower_Energy_LEDd][3*i + 0] = char(Lower_Energy_LED[i] >> 16 & 0xff);
            LED[LEDparts::Lower_Energy_LEDd][3*i + 1] = char(Lower_Energy_LED[i] >> 8 & 0xff);
            LED[LEDparts::Lower_Energy_LEDd][3*i + 2] = char(Lower_Energy_LED[i] & 0xff);
        }
        for (int i = 0; i < Upper_Energy_LEDs; ++i) {
            LED[LEDparts::Upper_Energy_LEDd][3*i + 0] = char(Upper_Energy_LED[i] >> 16 & 0xff);
            LED[LEDparts::Upper_Energy_LEDd][3*i + 1] = char(Upper_Energy_LED[i] >> 8 & 0xff);
            LED[LEDparts::Upper_Energy_LEDd][3*i + 2] = char(Upper_Energy_LED[i] & 0xff);
        }
        for (int i = 0; i < Arm_Inside_LEDs; ++i) {
            LED[LEDparts::Arm_Inside_LEDd][3*i + 0] = char(Arm_Inside_LED[i] >> 16 & 0xff);
            LED[LEDparts::Arm_Inside_LEDd][3*i + 1] = char(Arm_Inside_LED[i] >> 8 & 0xff);
            LED[LEDparts::Arm_Inside_LEDd][3*i + 2] = char(Arm_Inside_LED[i] & 0xff);
        }
        strips.sendToStrip(LED);
    }
    else {
        json.push_back(LED);
        timeStamps.push_back(timeStamp);
    }
    // Write json
}
/*
void showOF() {
    pca.WriteAll(OF);
}

void rgbToOF(uint8_t r, uint8_t g, uint8_t b) {

}
*/

void output() {
    // Finger_LED = 0,
    // Lower_Energy_LED = 1,
    // Upper_Energy_LED = 2,
    // Arm_Inside_LEDd= 3
    ofstream outfile;
    outfile.open("./json/LED.json");
    outfile << "{" << endl;
    outfile << "\t\"Finger_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Finger_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Finger_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Finger_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;
    outfile << "\t\"Lower_Energy_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Lower_Energy_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Lower_Energy_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Lower_Energy_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;

    outfile << "\t\"Upper_Energy_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Upper_Energy_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Upper_Energy_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
                        cout << colorCode << endl;
            if (j == Upper_Energy_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;

    outfile << "\t\"Arm_Inside_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Arm_Inside_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Arm_Inside_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Arm_Inside_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]" << endl;
    outfile << "}" << endl;

}
//---------------------------------- LED Function ----------------------------------//
void energy_stable() {
    for (int i=18; i<=50; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
    }
    for (int i=0; i<=16; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
    }
    showLED();
    delay(5000);  //duration
    for (int i=18; i<=50; i++) {
        Upper_Energy_LED[i] = RGB(0,0,0);
    }
    for (int i=0; i<=16; i++) {
        Lower_Energy_LED[i] = RGB(0,0,0);
    }
    showLED();
}
void fight_first_show() {
    for (int i=39; i>=29; i--) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        Upper_Energy_LED[79-i] = RGB(0,255,128);
        if ((i-2)%3==0) {
            showLED();
            delay(50);
        }
    }
    Upper_Energy_LED[20] = RGB(0,255,128);
    //showLED();
    //delay(110);
    Upper_Energy_LED[19] = RGB(0,255,128);
    Upper_Energy_LED[21] = RGB(0,255,128);
    Upper_Energy_LED[28] = RGB(0,255,128);
    //showLED();
    //delay(110);
    Upper_Energy_LED[18] = RGB(0,255,128);
    Upper_Energy_LED[22] = RGB(0,255,128);
    Upper_Energy_LED[27] = RGB(0,255,128);
    showLED();
    delay(50);
    for (int i=23; i<=26; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        //showLED();
        //delay(110);
    }
    showLED();
    delay(50);
    for (int i=0; i<=2; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
    }
    showLED();
    delay(110);
    for (int i=3; i<=9; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
        Lower_Energy_LED[19-i] = RGB(0,255,128);
        if (i%3==0) {
            showLED();
            delay(50);
        }
    }
    for (int i=0; i<=6; i++) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[17-i] = RGB(255,255,0);
        Upper_Energy_LED[58-i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
        Lower_Energy_LED[21+i] = RGB(255,255,0);
        Finger_LED[i] = RGB(255,255,0);
        Finger_LED[7+i] = RGB(255,255,0);
        Finger_LED[14+i] = RGB(255,255,0);
        Finger_LED[21+i] = RGB(255,255,0);
        if(i>=3) {
            Lower_Energy_LED[14+i] = RGB(0,255,128);
            Lower_Energy_LED[35-i] = RGB(0,255,128);
        }
        Arm_Inside_LED[i] = RGB(0,255,128);
        Arm_Inside_LED[19-i] = RGB(0,255,128);
        if (i%3 == 0) {
            showLED();
            delay(50);
        }
    }
    for (int i=0; i<=2; i++) {
        Upper_Energy_LED[7+i] = RGB(255,255,0);
        Upper_Energy_LED[66+i] = RGB(255,255,0);
    }
    for (int i=7; i<=12; i++) {
        Arm_Inside_LED[i] = RGB(0,255,128);
    }
    Upper_Energy_LED[10] = RGB(255,255,0);
    Upper_Energy_LED[51] = RGB(255,255,0);
    Lower_Energy_LED[28] = RGB(255,255,0);
    showLED();

    delay(1000); //duration
}
void hand_shake() {
    for (int i=29; i<=50; i++) {
        Upper_Energy_LED[i] = RGB(0,0,0);
    }
    for (int i=20; i<=26; i++) {
        Upper_Energy_LED[i] = RGB(0,0,0);
    }
    for (int i=0; i<=2; i++) {
        Lower_Energy_LED[i] = RGB(0,0,0);
    }
    Lower_Energy_LED[9] = RGB(0,0,0);
    Lower_Energy_LED[10] = RGB(0,0,0);
    for (int i=7; i<=20; i++) {
        Finger_LED[i] = RGB(0,0,0);
    }
    for (int i=10; i<=17; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        Upper_Energy_LED[41+i] = RGB(0,255,128);
    }
    showLED();
    delay(80);
    for (int i=18; i<=19; i++) {
        Upper_Energy_LED[i] = RGB(0,0,0);
        Upper_Energy_LED[9+i] = RGB(0,0,0);
        Lower_Energy_LED[i-15] = RGB(0,0,0);
        Lower_Energy_LED[i-11] = RGB(0,0,0);
        Lower_Energy_LED[i-7] = RGB(0,0,0);
        Lower_Energy_LED[i-3] = RGB(0,0,0);
    }
    for (int i=10; i<=17; i++) {
        Upper_Energy_LED[i] = RGB(0,0,0);
        Upper_Energy_LED[41+i] = RGB(0,0,0);
    }
    for (int i=0; i<=6; i++) {
        Finger_LED[i] = RGB(0,0,0);
        Finger_LED[21+i] = RGB(0,0,0);
    }
    for (int i=0; i<=9; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        Upper_Energy_LED[59+i] = RGB(0,255,128);
    }
    showLED();
    delay(80);
    for (int i=18; i<=19; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        Upper_Energy_LED[9+i] = RGB(0,255,128);
        Lower_Energy_LED[i-15] = RGB(0,255,128);
        Lower_Energy_LED[i-11] = RGB(0,255,128);
        Lower_Energy_LED[i-7] = RGB(0,255,128);
        Lower_Energy_LED[i-3] = RGB(0,255,128);
    }
    for (int i=10; i<=17; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
        Upper_Energy_LED[41+i] = RGB(0,255,128);
    }
    for (int i=0; i<=6; i++) {
        Finger_LED[i] = RGB(255,255,0);
        Finger_LED[21+i] = RGB(255,255,0);
    }
    for (int i=0; i<=9; i++) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
    }
    showLED();
    delay(80);
    for (int i=29; i<=50; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
    }
    for (int i=20; i<=26; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
    }
    for (int i=0; i<=2; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
    }
    Lower_Energy_LED[9] = RGB(0,255,128);
    Lower_Energy_LED[10] = RGB(0,255,128);
    for (int i=7; i<=20; i++) {
        Finger_LED[i] = RGB(255,255,0);
    }
    for (int i=10; i<=17; i++) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[41+i] = RGB(255,255,0);
    }
    showLED();
}
void tsunami()
{
    ///1
    for(int i=39;i>=37;i--)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
        Upper_Energy_LED[79-i]=RGB(0,255,128);
    }
    showLED();
    delay(40);
    ///2
    for(int i=36;i>=34;i--)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
        Upper_Energy_LED[79-i]=RGB(0,255,128);
    }
    for(int i=10;i<=12;i++)
    {
        Arm_Inside_LED[i]=RGB(0,255,128);
        Arm_Inside_LED[19-i]=RGB(0,255,128);
    }
    showLED();
    delay(40);
    ///3
    for(int i=33;i>=31;i--)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
        Upper_Energy_LED[79-i]=RGB(0,255,128);
    }
    for(int i=13;i<=15;i++)
    {
        Arm_Inside_LED[i]=RGB(0,255,128);
        Arm_Inside_LED[19-i]=RGB(0,255,128);
    }
    showLED();
    delay(40);
    ///4
    for(int i=30;i>=29;i--)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
        Upper_Energy_LED[79-i]=RGB(0,255,128);
    }
    for(int i=16;i<=18;i++)
    {
        Arm_Inside_LED[i]=RGB(0,255,128);
        Arm_Inside_LED[19-i]=RGB(0,255,128);
    }
    for(int i=10;i>=7;i--)
    {
        Upper_Energy_LED[i]=RGB(255,255,0);
    }
    for(int i=66;i<=68;i++)
    {
        Upper_Energy_LED[i]=RGB(255,255,0);
    }
    Upper_Energy_LED[51]=RGB(255,255,0);
    showLED();
    delay(40);
    ///5
    Arm_Inside_LED[0]=RGB(0,255,128);
    Arm_Inside_LED[19]=RGB(0,255,128);
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[4+i]=RGB(255,255,0);
        Upper_Energy_LED[11+i]=RGB(255,255,0);
        Upper_Energy_LED[52+i]=RGB(255,255,0);
        Upper_Energy_LED[63+i]=RGB(255,255,0);
    }
    for(int i=19;i<=21;i++)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
    }
    Upper_Energy_LED[28]=RGB(0,255,128);
    showLED();
    delay(40);
    ///6
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[1+i]=RGB(255,255,0);
        Upper_Energy_LED[14+i]=RGB(255,255,0);
        Upper_Energy_LED[55+i]=RGB(255,255,0);
        Upper_Energy_LED[60+i]=RGB(255,255,0);
    }
    for(int i=22;i<=24;i++)
    {
        Upper_Energy_LED[i]=RGB(0,255,128);
    }
    Upper_Energy_LED[18]=RGB(0,255,128);
    Upper_Energy_LED[27]=RGB(0,255,128);
    showLED();
    delay(40);
    ///7
    Upper_Energy_LED[25]=RGB(0,255,128);
    Upper_Energy_LED[26]=RGB(0,255,128);
    Upper_Energy_LED[0]=RGB(255,255,0);
    Upper_Energy_LED[17]=RGB(255,255,0);
    Upper_Energy_LED[58]=RGB(255,255,0);
    Upper_Energy_LED[59]=RGB(255,255,0);
    Lower_Energy_LED[0]=RGB(0,255,128);
    showLED();
    delay(40);
    ///8
    for(int i=1;i<=3;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    } 
    for(int i=15;i<=19;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    }  
    for(int i=21;i<=23;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    } 
    for(int i=30;i<=32;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    }  
    showLED();
    delay(40);
    ///9
    Lower_Energy_LED[20]=RGB(0,255,128);    
    for(int i=12;i<=14;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    }  
    for(int i=4;i<=7;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    } 
    Lower_Energy_LED[24]=RGB(0,255,128);
    Lower_Energy_LED[25]=RGB(0,255,128);
    Lower_Energy_LED[29]=RGB(0,255,128);
    showLED();
    delay(40);
    ///10
    for(int i=8;i<=11;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    } 
    for(int i=26;i<=28;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    } 
    showLED();
    delay(40);
    ///11
    for(int i=0;i<=1;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
    delay(40);
    ///12
    for(int i=2;i<=3;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
    delay(40);
    ///13
    for(int i=4;i<=6;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
}
void yellow_fade_out() {
    for (int i=240; i>=0; i=i-10) {
        for (int j=0; j<=17; j++) {
            Upper_Energy_LED[j] = RGB(i,i,0);
            Upper_Energy_LED[51+j] = RGB(i,i,0);
        }
        for (int j=0;j<=27; j++) {
            Finger_LED[j] = RGB(i,i,0);
        }
        for (int j=21; j<=28; j++) {
            Lower_Energy_LED[j] = RGB(i,i,0);
        }
        showLED();
        delay(50);
    }
}
void ebb()
{
    ///1
    for(int i=39;i>=37;i--)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
        Upper_Energy_LED[79-i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
    ///2
    for(int i=36;i>=34;i--)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
        Upper_Energy_LED[79-i]=RGB(0,0,0);
    }
    for(int i=10;i<=12;i++)
    {
        Arm_Inside_LED[i]=RGB(0,0,0);
        Arm_Inside_LED[19-i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
    ///3
    for(int i=33;i>=31;i--)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
        Upper_Energy_LED[79-i]=RGB(0,0,0);
    }
    for(int i=13;i<=15;i++)
    {
        Arm_Inside_LED[i]=RGB(0,0,0);
        Arm_Inside_LED[19-i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
    ///4
    for(int i=30;i>=29;i--)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
        Upper_Energy_LED[79-i]=RGB(0,0,0);
    }
    for(int i=16;i<=18;i++)
    {
        Arm_Inside_LED[i]=RGB(0,0,0);
        Arm_Inside_LED[19-i]=RGB(0,0,0);
    }
    for(int i=10;i>=7;i--)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
    }
    for(int i=66;i<=68;i++)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
    }
    Upper_Energy_LED[51]=RGB(0,0,0);
    showLED();
    delay(50);
    ///5
    Arm_Inside_LED[0]=RGB(0,0,0);
    Arm_Inside_LED[19]=RGB(0,0,0);
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[4+i]=RGB(0,0,0);
        Upper_Energy_LED[11+i]=RGB(0,0,0);
        Upper_Energy_LED[52+i]=RGB(0,0,0);
        Upper_Energy_LED[63+i]=RGB(0,0,0);
    }
    for(int i=19;i<=21;i++)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
    }
    Upper_Energy_LED[28]=RGB(0,0,0);
    showLED();
    delay(50);
    ///6
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[1+i]=RGB(0,0,0);
        Upper_Energy_LED[14+i]=RGB(0,0,0);
        Upper_Energy_LED[55+i]=RGB(0,0,0);
        Upper_Energy_LED[60+i]=RGB(0,0,0);
    }
    for(int i=22;i<=24;i++)
    {
        Upper_Energy_LED[i]=RGB(0,0,0);
    }
    Upper_Energy_LED[18]=RGB(0,0,0);
    Upper_Energy_LED[27]=RGB(0,0,0);
    showLED();
    delay(50);
    ///7
    Upper_Energy_LED[25]=RGB(0,0,0);
    Upper_Energy_LED[26]=RGB(0,0,0);
    Upper_Energy_LED[0]=RGB(0,0,0);
    Upper_Energy_LED[17]=RGB(0,0,0);
    Upper_Energy_LED[58]=RGB(0,0,0);
    Upper_Energy_LED[59]=RGB(0,0,0);
    Lower_Energy_LED[0]=RGB(0,0,0);
    showLED();
    delay(50);
    ///8
    for(int i=1;i<=3;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    } 
    for(int i=15;i<=19;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    }  
    for(int i=21;i<=23;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    } 
    for(int i=30;i<=32;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    }  
    showLED();
    delay(50);
    ///9
    Lower_Energy_LED[20]=RGB(0,0,0);    
    for(int i=12;i<=14;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    }  
    for(int i=4;i<=7;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    } 
    Lower_Energy_LED[24]=RGB(0,0,0);
    Lower_Energy_LED[25]=RGB(0,0,0);
    Lower_Energy_LED[29]=RGB(0,0,0);
    showLED();
    delay(50);
    ///10
    for(int i=8;i<=11;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    } 
    for(int i=26;i<=28;i++)
    {
        Lower_Energy_LED[i]=RGB(0,0,0);
    } 
    showLED();
    delay(50);
    ///11
    for(int i=0;i<=1;i++)
    {
        Finger_LED[i]=RGB(0,0,0);
        Finger_LED[7+i]=RGB(0,0,0);
        Finger_LED[14+i]=RGB(0,0,0);
        Finger_LED[21+i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
    ///12
    for(int i=2;i<=3;i++)
    {
        Finger_LED[i]=RGB(0,0,0);
        Finger_LED[7+i]=RGB(0,0,0);
        Finger_LED[14+i]=RGB(0,0,0);
        Finger_LED[21+i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
    ///13
    for(int i=4;i<=6;i++)
    {
        Finger_LED[i]=RGB(0,0,0);
        Finger_LED[7+i]=RGB(0,0,0);
        Finger_LED[14+i]=RGB(0,0,0);
        Finger_LED[21+i]=RGB(0,0,0);
    }
    showLED();
    delay(50);
}
void show_all() {
    lightchannel(Finger_LED, Finger_LEDs, 255, 255, 0);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 255, 128);
    for (int i=0; i<=17; i++) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[51+i] = RGB(255,255,0);
    }
    for (int i=18; i<=50; i++) {
        Upper_Energy_LED[i] = RGB(0,255,128);
    }
    for (int i=0; i<=16; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
    }
    for (int i=17; i<=20; i++) {
        Lower_Energy_LED[i] = RGB(0,255,128);
        Lower_Energy_LED[12+i] = RGB(0,255,128);
    }
    for (int i=21; i<=28; i++) {
        Lower_Energy_LED[i] = RGB(255,255,0);
    }
    showLED();
}
void yellowTsunami()
{
    ///13
    for(int i=4;i<=6;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
    delay(50);
    ///12
    for(int i=2;i<=3;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
    delay(50);
    ///11
    for(int i=0;i<=1;i++)
    {
        Finger_LED[i]=RGB(255,255,0);
        Finger_LED[7+i]=RGB(255,255,0);
        Finger_LED[14+i]=RGB(255,255,0);
        Finger_LED[21+i]=RGB(255,255,0);
    }
    showLED();
    delay(50);
    ///9
    Lower_Energy_LED[29]=RGB(0,255,128);
    showLED();
    delay(50);
    ///8
    for(int i=30;i<=32;i++)
    {
        Lower_Energy_LED[i]=RGB(0,255,128);
    }  
    showLED();
    delay(50);
    ///7
    Upper_Energy_LED[0]=RGB(255,255,0);
    Upper_Energy_LED[17]=RGB(255,255,0);
    Upper_Energy_LED[58]=RGB(255,255,0);
    Upper_Energy_LED[59]=RGB(255,255,0);;
    showLED();
    delay(50);
    ///6
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[1+i]=RGB(255,255,0);
        Upper_Energy_LED[14+i]=RGB(255,255,0);
        Upper_Energy_LED[55+i]=RGB(255,255,0);
        Upper_Energy_LED[60+i]=RGB(255,255,0);
    }
    showLED();
    delay(50);
    ///5
    for(int i=0;i<=2;i++)
    {
        Upper_Energy_LED[4+i]=RGB(255,255,0);
        Upper_Energy_LED[11+i]=RGB(255,255,0);
        Upper_Energy_LED[52+i]=RGB(255,255,0);
        Upper_Energy_LED[63+i]=RGB(255,255,0);
    }
    showLED();
    delay(50);
    ///4
    for(int i=10;i>=7;i--)
    {
        Upper_Energy_LED[i]=RGB(255,255,0);
    }
    for(int i=66;i<=68;i++)
    {
        Upper_Energy_LED[i]=RGB(255,255,0);
    }
    Upper_Energy_LED[51]=RGB(255,255,0);
    showLED();
    delay(50);
}
void all_fade_in() {
    for (int j=0; j<=120; j=j+10) {
        for (int i=0; i<=27; i++) {
            Finger_LED[i] = RGB(2*j,2*j,0);
        }
        for (int i=0; i<=19; i++) {
            Arm_Inside_LED[i] = RGB(0,2*j,j);
        }
        for (int i=0; i<=17; i++) {
            Upper_Energy_LED[i] = RGB(2*j,2*j,0);
            Upper_Energy_LED[51+i] = RGB(2*j,2*j,0);
        }
        for (int i=18; i<=50; i++) {
            Upper_Energy_LED[i] = RGB(0,2*j,j);
        }
        for (int i=0; i<=16; i++) {
            Lower_Energy_LED[i] = RGB(0,2*j,j);
        }
        for (int i=17; i<=20; i++) {
            Lower_Energy_LED[i] = RGB(0,2*j,j);
            Lower_Energy_LED[12+i] = RGB(0,2*j,j);
        }
        for (int i=21; i<=28; i++) {
            Lower_Energy_LED[i] = RGB(2*j,2*j,0);
        }
        showLED();
        delay(40);
    } 
}
void festival_first_show_yellow() {
    for (int i=0; i<=4; i++) {
        Upper_Energy_LED[4-i] = RGB(255,255,0);
        Upper_Energy_LED[5+i] = RGB(255,255,0);
        Upper_Energy_LED[13+i] = RGB(255,255,0);
        Upper_Energy_LED[54+i] = RGB(255,255,0);
        Upper_Energy_LED[63-i] = RGB(255,255,0);
        Upper_Energy_LED[64+i] = RGB(255,255,0);
        if(i<=2) {
            Upper_Energy_LED[12-i] = RGB(255,255,0);
            Upper_Energy_LED[53-i] = RGB(255,255,0);
        }
        showLED();
        delay(60);
    }
    for (int i=0; i<=4; i++) {
        Upper_Energy_LED[4-i] = RGB(0,0,0);
        Upper_Energy_LED[5+i] = RGB(0,0,0);
        Upper_Energy_LED[13+i] = RGB(0,0,0);
        Upper_Energy_LED[54+i] = RGB(0,0,0);
        Upper_Energy_LED[63-i] = RGB(0,0,0);
        Upper_Energy_LED[64+i] = RGB(0,0,0);
        if(i<=2) {
            Upper_Energy_LED[12-i] = RGB(0,0,0);
            Upper_Energy_LED[53-i] = RGB(0,0,0);
        }
        showLED();
        delay(60);
    }
}
void festival_first_show_pink() {
    for (int i=0; i<=4; i++) {
        Upper_Energy_LED[4-i] = RGB(220,20,150);
        Upper_Energy_LED[5+i] = RGB(220,20,150);
        Upper_Energy_LED[13+i] = RGB(220,20,150);
        Upper_Energy_LED[54+i] = RGB(220,20,150);
        Upper_Energy_LED[63-i] = RGB(220,20,150);
        Upper_Energy_LED[64+i] = RGB(220,20,150);
        if(i<=2) {
            Upper_Energy_LED[12-i] = RGB(220,20,150);
            Upper_Energy_LED[53-i] = RGB(220,20,150);
        }
        showLED();
        delay(50);
    }
    for (int i=0; i<=4; i++) {
        Upper_Energy_LED[4-i] = RGB(0,0,0);
        Upper_Energy_LED[5+i] = RGB(0,0,0);
        Upper_Energy_LED[13+i] = RGB(0,0,0);
        Upper_Energy_LED[54+i] = RGB(0,0,0);
        Upper_Energy_LED[63-i] = RGB(0,0,0);
        Upper_Energy_LED[64+i] = RGB(0,0,0);
        if(i<=2) {
            Upper_Energy_LED[12-i] = RGB(0,0,0);
            Upper_Energy_LED[53-i] = RGB(0,0,0);
        }
        showLED();
        delay(50);
    }
}
void festival_raise_hand() {
    for (int i=0; i<=4; i++) {
        Upper_Energy_LED[4-i] = RGB(255,255,0);
        Upper_Energy_LED[5+i] = RGB(255,255,0);
        Upper_Energy_LED[13+i] = RGB(255,255,0);
        Upper_Energy_LED[54+i] = RGB(255,255,0);
        Upper_Energy_LED[63-i] = RGB(255,255,0);
        Upper_Energy_LED[64+i] = RGB(255,255,0);
        if(i<=2) {
            Upper_Energy_LED[12-i] = RGB(255,255,0);
            Upper_Energy_LED[53-i] = RGB(255,255,0);
        }
    }
    Finger_LED[0] = RGB(255,255,0);
    Finger_LED[7] = RGB(255,255,0);
    Finger_LED[14] = RGB(255,255,0);
    Finger_LED[21] = RGB(255,255,0);
    showLED();
    delay(50);
    for (int i=1; i<=6; i++) {
        Finger_LED[i] = RGB(255,255,0);
        Finger_LED[7+i] = RGB(255,255,0);
        Finger_LED[14+i] = RGB(255,255,0);
        Finger_LED[21+i] = RGB(255,255,0);
        if (i%2 == 0) {
            showLED();
            delay(40);
        }
    }
}
void festival_finger_show() {
    for (int i=0; i<=4; i++) {
        Finger_LED[i] = RGB(255,255,0);
        Finger_LED[7+i] = RGB(220,20,150);
        Finger_LED[14+i] = RGB(255,255,0);
        Finger_LED[21+i] = RGB(220,20,150);
    }
    for (int i=4; i<=6; i++) {
        Finger_LED[i] = RGB(220,20,150);
        Finger_LED[7+i] = RGB(255,255,0);
        Finger_LED[14+i] = RGB(220,20,150);
        Finger_LED[21+i] = RGB(255,255,0);
        Lower_Energy_LED[22+i] = RGB(255,255,0);
    }
    for (int i=21; i<=25; i++) {
        Lower_Energy_LED[i] = RGB(220,20,150);
    }
    showLED();
    delay(220);
    for (int i=4; i<=6; i++) {
        Finger_LED[i] = RGB(255,255,0);
        Finger_LED[7+i] = RGB(220,20,150);
        Finger_LED[14+i] = RGB(255,255,0);
        Finger_LED[21+i] = RGB(220,20,150);
        Lower_Energy_LED[22+i] = RGB(220,20,150);
    }
    for (int i=0; i<=4; i++) {
        Finger_LED[i] = RGB(220,20,150);
        Finger_LED[7+i] = RGB(255,255,0);
        Finger_LED[14+i] = RGB(220,20,150);
        Finger_LED[21+i] = RGB(255,255,0);
    }
    for (int i=21; i<=25; i++) {
        Lower_Energy_LED[i] = RGB(255,255,0);
    }
    showLED();
    delay(220);
}
void yellow_crash() {
    for (int i=9; i>=8; i--) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
    }
    showLED();
    delay(40);
    for (int i=7; i>=6; i--) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[17-i] = RGB(255,255,0);
        Upper_Energy_LED[58-i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
    }
    showLED();
    delay(40);
    for (int i=5; i>=4; i--) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[17-i] = RGB(255,255,0);
        Upper_Energy_LED[58-i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
        Upper_Energy_LED[4+i] = RGB(0,0,0);
        Upper_Energy_LED[63+i] = RGB(0,0,0);
    }
    showLED();
    delay(40);
    for (int i=3; i>=0; i--) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[17-i] = RGB(255,255,0);
        Upper_Energy_LED[58-i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
        Upper_Energy_LED[4+i] = RGB(0,0,0);
        Upper_Energy_LED[13-i] = RGB(0,0,0);
        Upper_Energy_LED[54-i] = RGB(0,0,0);
        Upper_Energy_LED[63+i] = RGB(0,0,0);
        if (i%2 == 0) {
            showLED();
            delay(55);
        }
    }
    for (int i=3; i>=0; i--) {
        Upper_Energy_LED[i] = RGB(0,0,0);
        Upper_Energy_LED[17-i] = RGB(0,0,0);
        Upper_Energy_LED[58-i] = RGB(0,0,0);
        Upper_Energy_LED[29+i] = RGB(0,0,0);
        if (i%2 == 0) {
            showLED();
            delay(55);
        }
    }
}
void yellow_show() {
    Upper_Energy_LED[8] = RGB(255,255,0);
    Upper_Energy_LED[9] = RGB(255,255,0);
    Upper_Energy_LED[67] = RGB(255,255,0);
    Upper_Energy_LED[68] = RGB(255,255,0);
    showLED();
    delay(50);
    for (int i=7; i>=0; i--) {
        Upper_Energy_LED[i] = RGB(255,255,0);
        Upper_Energy_LED[17-i] = RGB(255,255,0);
        Upper_Energy_LED[58-i] = RGB(255,255,0);
        Upper_Energy_LED[59+i] = RGB(255,255,0);
        if (i%2 == 0) {
            showLED();
            delay(50);
        }
    }
}

//------------------------------- LED Function ends here ---------------------------------//

int main(int argc, char* argv[]) {
    // PCA pca;
    init();
    cerr << nLeds.size() << endl;
    if (argc > 1)
        export_json = (strcmp(argv[1], "output") == 0);
    //functions<<
    //energy_stable();

//-------------- PERFECT -------------//
    int c;
    cin>>c;

    festival_first_show_yellow();
    festival_first_show_pink();
    festival_first_show_yellow();
    festival_first_show_pink();
    festival_raise_hand();
    delay(250);

    lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    showLED();
    delay(1800);

    festival_finger_show();
    festival_finger_show();
    festival_finger_show();
    festival_finger_show();
    delay(100);

    yellow_crash();
    yellow_crash();

    lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    showLED();
    delay(900);
    yellow_show();
    delay(400);
    lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    
    int a;
    cin>>a;

    fight_first_show();
    hand_shake();
    delay(200);
    hand_shake();
    delay(300);
    yellow_fade_out();
    delay(500);
    yellowTsunami();
    delay(3500);
    ebb();
    show_all();
    delay(200);
    yellow_fade_out();
    delay(500);
    yellowTsunami();
    delay(100);
    ebb();
    delay(100);
    all_fade_in();
    delay(100);
    ebb();

    int b;
    cin>>b;

    tsunami();
    delay(850);
//------------------ PERFECT ------------------//
    
//---------------- TEST ----------------//
    // int c;
    // cin>>c;

    // festival_first_show_yellow();
    // festival_first_show_pink();
    // festival_first_show_yellow();
    // festival_first_show_pink();
    // festival_raise_hand();
    // delay(250);

    // lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    // lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    // lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    // lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    // showLED();
    // delay(1800);

    // festival_finger_show();
    // festival_finger_show();
    // festival_finger_show();
    // festival_finger_show();
    // delay(100);

    // yellow_crash();
    // yellow_crash();

    // lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    // lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    // lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    // lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    // showLED();
    // delay(900);
    // yellow_show();
    // delay(400);
    // lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    // lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    // lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    // lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);



    //functions>>
    
    // int delays = 110;
    // int times = 20;
    // while (times --) {
    //     lightchannel(Finger_LED, Finger_LEDs, 128, 128, 128);
    //     lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 128, 128, 128);
    //     lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 128, 128, 128);
    //     lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 128, 128, 128);

    //     // Upper_Energy_LED[10] = RGB(0, 0,128);
    //     // Lower_Energy_LED[2] = RGB(0, 0,128);
    //     showLED();
        
    //     delay(delays);
    //     lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    //     lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    //     lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    //     lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);
    //     // Upper_Energy_LED[2] = RGB(128,128,128);
    //     // Lower_Energy_LED[1] = RGB(0,128,128);
    //     showLED();
    //     delay(delays);
    // }
    if (export_json) {
        output();
    }
}