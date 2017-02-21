# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\poornima\Documents\GitHub\thingTouch\2F5S1SOC.cydsn\2F5S1SOC.cyprj
# Date: Fri, 17 Feb 2017 12:10:46 GMT
#set_units -time ns
create_clock -name {CapSense_SampleClk(FFB)} -period 10625 -waveform {0 5312.5} [list [get_pins {ClockBlock/ff_div_7}]]
create_clock -name {CapSense_SenseClk(FFB)} -period 10625 -waveform {0 5312.5} [list [get_pins {ClockBlock/ff_div_6}]]
create_clock -name {Clock_PWM1(FFB)} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/ff_div_11}] [get_pins {ClockBlock/ff_div_12}]]
create_clock -name {I2C_Slave_SCBCLK(FFB)} -period 625 -waveform {0 312.5} [list [get_pins {ClockBlock/ff_div_2}]]
create_clock -name {UART_SCBCLK(FFB)} -period 708.33333333333326 -waveform {0 354.166666666667} [list [get_pins {ClockBlock/ff_div_5}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFCLK} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {CapSense_SampleClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {CapSense_SenseClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {Clock_PWM1} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]
create_generated_clock -name {I2C_Slave_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 15 31} [list]
create_generated_clock -name {UART_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 17 35} -nominal_period 708.33333333333326 [list]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\poornima\Documents\GitHub\thingTouch\2F5S1SOC.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\poornima\Documents\GitHub\thingTouch\2F5S1SOC.cydsn\2F5S1SOC.cyprj
# Date: Fri, 17 Feb 2017 12:10:27 GMT
