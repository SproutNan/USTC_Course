#CLK100MHZ
set_property -dict { PACKAGE_PIN E3    IOSTANDARD LVCMOS33 } [get_ports { clk }];
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports { clk }];
#uart
set_property -dict { PACKAGE_PIN C4 IOSTANDARD LVCMOS33 } [get_ports { rx }];
#hexplay
set_property -dict { PACKAGE_PIN A14   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[0] }]; 
set_property -dict { PACKAGE_PIN A13   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[1] }]; 
set_property -dict { PACKAGE_PIN A16   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[2] }]; 
set_property -dict { PACKAGE_PIN A15   IOSTANDARD LVCMOS33     } [get_ports { hexplay_data[3] }]; 
set_property -dict { PACKAGE_PIN B17   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[0] }]; 
set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[1] }]; 
set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS33     } [get_ports { hexplay_an[2] }]; 
 
#Led
set_property -dict { PACKAGE_PIN G18 IOSTANDARD LVCMOS33 } [get_ports { led[7] }]; 
set_property -dict { PACKAGE_PIN F18 IOSTANDARD LVCMOS33 } [get_ports { led[6] }]; 
set_property -dict { PACKAGE_PIN E17 IOSTANDARD LVCMOS33 } [get_ports { led[5] }]; 
set_property -dict { PACKAGE_PIN D17 IOSTANDARD LVCMOS33 } [get_ports { led[4] }]; 
set_property -dict { PACKAGE_PIN G17 IOSTANDARD LVCMOS33 } [get_ports { led[3] }]; 
set_property -dict { PACKAGE_PIN E18 IOSTANDARD LVCMOS33 } [get_ports { led[2] }]; 
set_property -dict { PACKAGE_PIN D18 IOSTANDARD LVCMOS33 } [get_ports { led[1] }]; 
set_property -dict { PACKAGE_PIN C17 IOSTANDARD LVCMOS33 } [get_ports { led[0] }];
##Switches

set_property -dict { PACKAGE_PIN D14   IOSTANDARD LVCMOS33 } [get_ports { sw[0] }]; #IO_L1P_T0_AD0P_15 Sch=jb[1]
set_property -dict { PACKAGE_PIN F16   IOSTANDARD LVCMOS33 } [get_ports { sw[1] }]; #IO_L14N_T2_SRCC_15 Sch=jb[2]
set_property -dict { PACKAGE_PIN G16   IOSTANDARD LVCMOS33 } [get_ports { sw[2] }]; #IO_L13N_T2_MRCC_15 Sch=jb[3]
set_property -dict { PACKAGE_PIN H14   IOSTANDARD LVCMOS33 } [get_ports { sw[3] }]; #IO_L15P_T2_DQS_15 Sch=jb[4]
set_property -dict { PACKAGE_PIN E16   IOSTANDARD LVCMOS33 } [get_ports { sw[4] }]; #IO_L11N_T1_SRCC_15 Sch=jb[7]
set_property -dict { PACKAGE_PIN F13   IOSTANDARD LVCMOS33 } [get_ports { sw[5] }]; #IO_L5P_T0_AD9P_15 Sch=jb[8]
set_property -dict { PACKAGE_PIN G13   IOSTANDARD LVCMOS33 } [get_ports { sw[6] }]; #IO_0_15 Sch=jb[9]
set_property -dict { PACKAGE_PIN H16   IOSTANDARD LVCMOS33 } [get_ports { sw[7] }]; #IO_L13P_T2_MRCC_15 Sch=jb[10]
