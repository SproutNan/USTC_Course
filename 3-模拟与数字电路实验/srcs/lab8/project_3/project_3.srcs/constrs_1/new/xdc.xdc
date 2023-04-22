## Clock signal
set_property -dict { PACKAGE_PIN E3    IOSTANDARD LVCMOS33 } [get_ports { CLK100MHZ }]; #IO_L12P_T1_MRCC_35 Sch=clk100mhz
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports { CLK100MHZ }];

##Switches

set_property -dict { PACKAGE_PIN D14   IOSTANDARD LVCMOS33 } [get_ports { dir }]; #IO_L1P_T0_AD0P_15 Sch=jb[1]
set_property -dict { PACKAGE_PIN F16   IOSTANDARD LVCMOS33 } [get_ports { rst }]; #IO_L14N_T2_SRCC_15 Sch=jb[2]

##7-Segment Display

set_property -dict { PACKAGE_PIN A14   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[0] }]; #IO_L9N_T1_DQS_AD3N_15 Sch=xa_n[1]
set_property -dict { PACKAGE_PIN A13   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[1] }]; #IO_L9P_T1_DQS_AD3P_15 Sch=xa_p[1]
set_property -dict { PACKAGE_PIN A16   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[2] }]; #IO_L8N_T1_AD10N_15 Sch=xa_n[2]
set_property -dict { PACKAGE_PIN A15   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[3] }]; #IO_L8P_T1_AD10P_15 Sch=xa_p[2]
set_property -dict { PACKAGE_PIN B17   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[0] }]; #IO_L7N_T1_AD2N_15 Sch=xa_n[3]
set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[1] }]; #IO_L7P_T1_AD2P_15 Sch=xa_p[3]
set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[2] }]; #IO_L10N_T1_AD11N_15 Sch=xa_n[4]


##Buttons

set_property -dict { PACKAGE_PIN B18   IOSTANDARD LVCMOS33     } [get_ports { BTN }]; #IO_L10P_T1_AD11P_15 Sch=xa_p[4]