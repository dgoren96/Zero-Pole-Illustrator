/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_QUITBUTTON                 2       /* control type: command, callback function: QuitCallback */
#define  PANEL_show_nominator             3       /* control type: string, callback function: (none) */
#define  PANEL_input_nominator            4       /* control type: string, callback function: (none) */
#define  PANEL_show_denominator           5       /* control type: string, callback function: (none) */
#define  PANEL_input_denominator          6       /* control type: string, callback function: (none) */
#define  PANEL_ADD_POLE                   7       /* control type: command, callback function: addpole */
#define  PANEL_ADD_ZERO                   8       /* control type: command, callback function: addzero */
#define  PANEL_TAB                        9       /* control type: tab, callback function: (none) */
#define  PANEL_gain                       10      /* control type: numeric, callback function: (none) */
#define  PANEL_real                       11      /* control type: LED, callback function: (none) */
#define  PANEL_minphase                   12      /* control type: LED, callback function: (none) */
#define  PANEL_fir                        13      /* control type: LED, callback function: (none) */
#define  PANEL_causal                     14      /* control type: LED, callback function: (none) */
#define  PANEL_stable                     15      /* control type: LED, callback function: (none) */
#define  PANEL_DELETE_POLE                16      /* control type: command, callback function: Delete_Pole */
#define  PANEL_DELETE_ZERO                17      /* control type: command, callback function: Delete_Zero */
#define  PANEL_list_pole                  18      /* control type: listBox, callback function: (none) */
#define  PANEL_list_zero                  19      /* control type: listBox, callback function: (none) */

     /* tab page panel controls */
#define  TAB1_CANVAS                      2       /* control type: canvas, callback function: (none) */
#define  TAB1_PICTURE                     3       /* control type: picture, callback function: (none) */

     /* tab page panel controls */
#define  TAB2_a                           2       /* control type: string, callback function: (none) */
#define  TAB2_b                           3       /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK addpole(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK addzero(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Delete_Pole(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Delete_Zero(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
