/////////////////////////////////////////////////////////////////////////
//      File:   SLAV_77.H
//      This file contains compiler directives to define data values
//      for various I/O devices in the Multi-Function I/O demo module.
//      Date:   1999-8-1    by kizaki
////////////////////////////////////////////////////////////////////////

#define         ON              1
#define         OFF             0
#define         HI              1
#define         LO              0

#define         STX             2
#define         ETX             3
#define         EOT             4
#define         ACK             6
#define         NAK             21

#define         TR_NG           2     // 2 = continus 3times errror

#define    U_CHAR   unsigned char
#define    U_INT    unsigned int
#define    U_LONG   unsigned long


///////////////// Network Variable declarations //////////////////////////
network output  SNVT_state nvoNstate;   // state
network output  SNVT_ppm_f nvoConc;   // data
//network output  SNVT_ppm_f nvoAlarmThresh1;  // 1st alarm data
//network output  SNVT_ppm_f nvoAlarmThresh2;  // 2nd alarm data
network input  SNVT_str_asc nviComand;       // Command input
network output  SNVT_str_asc nvoComand;      // Command output
network output  SNVT_switch nvoAlarmS;       // alarm switch
network output  SNVT_switch nvoFaultS;       // fault switch
network output  SNVT_str_asc nvoGasName;     // gas name
//network output  SNVT_lev_disc nvoAlarmType;  // alarm-type data
network output  SNVT_state nvoHertBeat;      // active counter
network input  SNVT_lev_disc nviReset;       // self-holding reset
//network input config SNVT_time_sec nciMintimeState = 50;   // Min send time
//network input config SNVT_time_sec nciMaxtimeState = 3000; // Max send time
//network input config SNVT_time_sec nciMintimeData = 50;   // Min send time
//network input config SNVT_time_sec nciMaxtimeData = 3000; // Max send time
//network input config SNVT_time_sec nciMintimeState = 10;   // Min send time
//network input config SNVT_time_sec nciMaxtimeState = 300; // Max send time
//network input config SNVT_time_sec nciMintimeData = 10;   // Min send time
//network input config SNVT_time_sec nciMaxtimeData = 300; // Max send time
network input config SNVT_time_sec nciMinSendTM = 10;  // Min send time
network input config SNVT_time_sec nciMaxSendTM = 50;  // Max send time
network input config SNVT_time_sec nciMinSendTI = 10;  // Min send time
network input config SNVT_time_sec nciMaxSendTI = 50;  // Max send time
network input config SNVT_time_sec nciMaxSendHB = 50;  // Max send time
//network input config SNVT_elapsed_tm nciMinSendTM;   // Min send time
//network input config SNVT_elapsed_tm nciMaxSendTM;   // Max send time
//network input config SNVT_elapsed_tm nciMinSendTI;   // Min send time
//network input config SNVT_elapsed_tm nciMaxSendTI;   // Max send time

/////////////////////// Network resource tuning pragmas /////////////
#pragma one_domain
#pragma app_buf_out_priority_count  0
#pragma net_buf_out_priority_count  0
//#pragma net_buf_in_count  11
#pragma net_buf_in_size   66
#pragma net_buf_in_count  7
#pragma app_buf_in_count  3
#pragma enable_sd_nv_names
//#pragma num_alias_table_entries 6

/////////////////////// NEURON I/O Objects //////////////////////////
IO_0    output  bit     TXD_LP;        // TXD LAMP(LOW ACT)
IO_1    output  bit     RXD_LP;        // RXD LAMP(LOW ACT)
//IO_2    input   bit     IO2;           // NOT USED
//IO_3    input   bit     IO3;           // NOT USED
//IO_4    input   bit     IO4;           // NOT USED
//IO_5    input   bit     IO5;           // NOT USED
//IO_6    input   bit     IO6;           // NOT USED
//IO_7    input   bit     IO7;                   // NOT USED
IO_8    input   serial baud(4800) RXD; // RXD
//IO_9    input   bit     IO9;           // NOT USED
IO_10   output  serial baud(4800) TXD; // TXD

////////////////// NEURON Timer Objects //////////////////////////////////
mtimer  repeating       time_led;
mtimer  repeating       time_com;
mtimer  repeating       time_wdg;
stimer                  max_tx_timer_state;
stimer                  max_tx_timer_data;
stimer                  min_tx_timer_state;
stimer                  min_tx_timer_data;
stimer                  max_tx_hertbeat;
//////////////////////// Global Declarations /////////////////////////////
U_INT  adr_nt;         // RM77NT address = 0
U_INT  adr_77;         // GD77   address = DUMMY
U_INT  state_bf[4];
U_INT  state_bf_b[4];
U_INT  data_bf[4];
U_INT  data_bf_b[4];
//U_INT  ui_alarm_bf[2][4];
//U_INT  ui_alarm_bf_b[2][4];
U_INT  ui_trb_bf[4][8];
U_INT  ui_trb_bf_b[4][8];
U_INT  ui_gas_name[10];
U_INT  ui_gas_name_b[10];
U_INT  ui_tx_cnt;      // 5sec counter
U_INT  ui_tx_cmd;      // command select flg
U_INT  ui_four_cnt;    // for 1sec counter
U_INT  ui_1sec_cnt;    // 1sec counter 0-255
U_INT  tx_lamp_timer;  // tx lamp on timer
U_INT  rx_lamp_timer;  // rx lamp on timer
far U_INT  rx_buf[64];     // RECIVE DATA BUFFER
far U_INT  tx_buf[64]; // TRANS DATA BUFFER
U_INT  rx_num;         // RECIVE CHARS NUMBER
U_INT  dg1;            // digital 1 byte
U_INT  as2[2];         // ascii 2 byte
U_INT  quic_out_fl_state;    // 1:quic out  0:else
U_INT  quic_out_fl_data;    // 1:quic out  0:else
U_INT  ui_quic_out_hertbeat; // 1:quic out  0:else
U_INT  denst_ok_fl;    // 1:denst trans ok 0:ng
U_INT  tr_ng_fl;       // 1:trans NG 0:OK
U_INT  tr_ng_cnt;      // trans NG count
U_INT  min_state_fl;    // 1:min time ok 0:waiting
U_INT  min_data_fl;     // 1:min time ok 0:waiting
float_type float_buf;  // float type temporaly
SNVT_state nv_stat;    // state union
SNVT_state nv_sec;     // state union
//SNVT_state nv_sec_b;   // state union
//SNVT_elapsed_tm nv_min_send_state;  // Min send time
//SNVT_elapsed_tm nv_max_send_state;  // Max send time
//SNVT_elapsed_tm nv_min_send_data;  // Min send time
//SNVT_elapsed_tm nv_max_send_data;  // Max send time
SNVT_time_sec nv_min_send_state;  // Min send time
SNVT_time_sec nv_max_send_state;  // Max send time
SNVT_time_sec nv_min_send_data;  // Min send time
SNVT_time_sec nv_max_send_data;  // Max send time
SNVT_time_sec nv_send_hertbeat;  // Max send time
