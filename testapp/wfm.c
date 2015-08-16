
#include <rad1olib/setup.h>
#include <r0ketlib/display.h>
#include <r0ketlib/print.h>
#include <r0ketlib/itoa.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/menu.h>
#include <r0ketlib/select.h>
#include <r0ketlib/idle.h>
#include <fatfs/ff.h>
#include <r0ketlib/fs_util.h>

#include <r0ketlib/fs_util.h>
#include <rad1olib/pins.h>

#include <portalib/portapack.h>
#include <common/hackrf_core.h>
#include <common/rf_path.h>
#include <common/sgpio.h>
#include <common/tuning.h>
#include <libopencm3/lpc43xx/dac.h>

#include <portalib/complex.h>

static int64_t freq = 89300000;
static int tune = 50000;

void status() {
	lcdSetCrsr(0,0);
    lcdPrintln("FM RADIO (@hilse)");
    lcdPrintln("-----------------");
    lcdPrintln("L/R: tune");
    lcdPrintln("Btn: +- 50/1 kHz");
    lcdPrintln("Up/Down: gain/vol");
    lcdPrintln("");
    lcdPrint("Vol: ");
	lcdPrint(IntToStr(get_volume(),3,F_LONG));
    lcdPrintln("");
	lcdPrint(IntToStr(freq/1000,6,F_LONG));
	lcdPrintln("kHz              ");
    lcdDisplay();
}

//# MENU WFM
void wfm_menu()
{
	lcdClear();
	lcdDisplay();
	getInputWaitRelease();

	// RF initialization from ppack.c:
	dac_init(false);
	cpu_clock_set(204); // WARP SPEED! :-)
	hackrf_clock_init();
	rf_path_pin_setup();
	/* Configure external clock in */
	scu_pinmux(SCU_PINMUX_GP_CLKIN, SCU_CLK_IN | SCU_CONF_FUNCTION1);
	sgpio_configure_pin_functions();
	ON(EN_VDD);
	ON(EN_1V8);
	OFF(MIC_AMP_DIS);
	ON(MIC_AMP_DIS);
	delayms(500); // doesn't work without
	cpu_clock_set(204); // WARP SPEED! :-)
	si5351_init();
    status();
	portapack_init_wfm();
    set_frequency(freq);

	while(1)
	{
		switch(getInput())
		{
			case BTN_LEFT:
				freq -= tune;
				ssp1_set_mode_max2837();
                set_frequency(freq);
                status();
				break;
			case BTN_RIGHT:
				freq += tune;
				ssp1_set_mode_max2837();
                set_frequency(freq);
                status();
				break;
            case BTN_UP:
                set_volume(+1);
                status();
                break;
            case BTN_DOWN:
                set_volume(-1);
                status();
                break;
			case BTN_ENTER:
                if(tune == 50000) { tune = 1000; } else { tune = 50000; }
                break;
		}
	}
}
