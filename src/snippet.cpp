// snippet to pass Stuct to function to set RGB colours

/*
old code
/* ************* end readBattery() **********************************************/

/* define RGB values for LED status colours
int BLUE_BT_CONNECTED[] = {0, 0, 255};
int GREEN_FULLY_CHARGED[] = {0, 255, 0};
int YELLOW_LOW_BATT[] = {255, 255, 0};
int RED_CRITICALLY_low_BAT[] = {255, 0, 0};

int *status_colour_set[] = {BLUE_BT_CONNECTED, GREEN_FULLY_CHARGED, YELLOW_LOW_BATT, RED_CRITICALLY_low_BAT};  // array of pointers
*/
/*****************************************************************************
    function name : setRGBcolour()
------------------------
Description : sets a defined colour on RGB LED by setting R, G and B values in an array

Input Value : R, G and B values for a specific colour output
                see https://www.w3schools.com/colors/colors_picker.asp
Return Value: - n/a -
********************************************************************************/
void setRGBcolour(int r, int g, int b) {
    analogWrite(RED_LED_PIN, r);
    analogWrite(GREEN_LED_PIN, g);
    analogWrite(BLUE_LED_PIN, b);
}
/* ************* end setRGBcolour() **********************************************/
*/

struct StatusColour {
// add members with default while rgb {255,255,255}
  int red{255};
  int green{255};
  int blue{255};
 };

StatusColour blueBTconnected {0, 0, 255};
StatusColour greenFullBattChg {0, 255, 0};
StatusColour magentaLowBattChg {255, 0, 255};
StatusColour redCriticalLowBatt {255, 0, 0};

void setRGBcolour(StatusColour & statusColour) {
    analogWrite(RED_LED_PIN, statusColour.red);
    analogWrite(GREEN_LED_PIN, statusColour.green);
    analogWrite(BLUE_LED_PIN, statusColour.blue);
}
