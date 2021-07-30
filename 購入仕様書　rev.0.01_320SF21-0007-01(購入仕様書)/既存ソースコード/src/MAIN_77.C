// '99.8.1 --- make program for GD-77

///////// reset rutine //////////
when( reset ) {

    watchdog_update();
//   post_events();
    io_init();                  // I/O initial
    adr_nt = 0;                 // RM77NT address = 0
    adr_77 = 0;                 // GD77   address = DUMMY
    time_wdg = 500;             // TIMER 500msec
//    nv_min_send_state = nciMinSendTM;
//    nv_max_send_state = nciMaxSendTM;
//    nv_min_send_data = nciMinSendTM;
//    nv_max_send_data = nciMaxSendTM;
    nv_min_send_state = nciMinSendTI;
    nv_max_send_state = nciMaxSendTI;
    nv_min_send_data = nciMinSendTM;
    nv_max_send_data = nciMaxSendTM;
    nv_send_hertbeat = nciMaxSendHB;
    max_min_state();
    max_min_data();
    max_hertbeat();
    time_led = 10;              // TIMER 10msec
    time_com = 250;             // COMUNICATION INTERVAL 250msec
    watchdog_update();
  }
//////////// 10msec timer //////////////
when(timer_expires(time_led))
{
    if(tx_lamp_timer){
        --tx_lamp_timer;
    }
    else{                       // tx_lamp_timer == 0
        io_out(TXD_LP,HI);      // txd lamp off
    }
    if(rx_lamp_timer){
        --rx_lamp_timer;
    }
    else{                       // rx_lamp_timer == 0
        io_out(RXD_LP,HI);      // rxd lamp off
    }
}
//////////// 500msec timer //////////////
when(timer_expires(time_wdg))
{
    watchdog_update();
}
/////////// default 300sec timer //////////////
when(timer_expires(max_tx_timer_state))
{
  quic_out_fl_state = 1;
  if(!nv_max_send_state){
    quic_out_fl_state = 2;
  }
}
/////////// default 300sec timer //////////////
when(timer_expires(max_tx_timer_data))
{
  quic_out_fl_data = 1;
  if(!nv_max_send_data){
    quic_out_fl_data = 2;
  }
}
/////////// default 5sec timer //////////////
when(timer_expires(min_tx_timer_state))
{
  min_state_fl = 1;
}
/////////// default 5sec timer //////////////
when(timer_expires(min_tx_timer_data))
{
  min_data_fl = 1;
}
/////////// default 300sec timer //////////////
when(timer_expires(max_tx_hertbeat))
{
  ui_quic_out_hertbeat = 1;
}
///////// read denst data 250msec //////////

when(timer_expires(time_com))
{
  U_INT i,lui_temp;

  ++ui_tx_cnt;
  if(tr_ng_fl == 0)     tr_denst();       // now trans ok
  else                  tr_error();       // now trans ng
  if(ui_tx_cnt >= 64){                    // 15sec
    ui_tx_cnt = 0;
  }

  ++ui_four_cnt;
  if(ui_four_cnt >= 4){
    ui_four_cnt = 0;
    ++ui_1sec_cnt;
//    nv_sec.bit7 = (ui_1sec_cnt & 0x80) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit6 = (ui_1sec_cnt & 0x40) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit5 = (ui_1sec_cnt & 0x20) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit4 = (ui_1sec_cnt & 0x10) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit3 = (ui_1sec_cnt & 0x08) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit2 = (ui_1sec_cnt & 0x04) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit1 = (ui_1sec_cnt & 0x02) ? 1:0;    // 1sec cnt 8bit timer bit 7
//    nv_sec.bit0 = (ui_1sec_cnt & 0x01) ? 1:0;    // 1sec cnt 8bit timer bit 7
    lui_temp = 0x01;
    for(i=0;i < 8;i++){
      if(ui_1sec_cnt & lui_temp){
        set_bit(&nv_sec,i);
      }
      else{
        clr_bit(&nv_sec,i);
      }
      lui_temp = lui_temp << 1;
    }
  }
}
////////////   when quic output (when change data) ////////////////
when(quic_out_fl_data)
{
  if(quic_out_fl_data != 2){
    trans_data();
    trans_state();
  }
  quic_out_fl_data = 0;
  max_min_data();
//  max_tx_timer_data = nv_max_send_data / 10;  // default = 300sec
//  min_tx_timer_data = nv_min_send_data / 10;  // default = 5sec
//  min_data_fl = 0;
}
////////////   when quic output (when change data) ////////////////
when(quic_out_fl_state)
{
  U_INT i;

  if((nv_stat.bit8)||(nv_stat.bit9)){
    if((nv_stat.bit8)&&(nv_stat.bit9)){
      nvoAlarmS.value = 4;                      // = 2.0
      nvoAlarmS.state = 1;
    }
    else if(nv_stat.bit8){                      // 1st only
      nvoAlarmS.value = 2;                      // = 1.0
      nvoAlarmS.state = 1;
    }
    else{                                       // 2nd only
      nvoAlarmS.value = 3;                      // = 1.5
      nvoAlarmS.state = 1;
    }
  }
  else{
    nvoAlarmS.value = 0;                        // = 0.0
    nvoAlarmS.state = 0;
  }
  if(quic_out_fl_state != 2){
    trb_data();

    for(i=0;i < 10;i++){
      nvoGasName.ascii[i] = ui_gas_name[i];
    }
    nvoGasName.ascii[10] = 0x20;
    // 11:ppb 10:ppm 01:%LEL 00:%
    if(nv_stat.bit3){
      if(nv_stat.bit2){
        nvoGasName.ascii[11] = 'p';
        nvoGasName.ascii[12] = 'p';
        nvoGasName.ascii[13] = 'b';
        nvoGasName.ascii[14] = ' ';
      }
      else{
        nvoGasName.ascii[11] = 'p';
        nvoGasName.ascii[12] = 'p';
        nvoGasName.ascii[13] = 'm';
        nvoGasName.ascii[14] = ' ';
      }
    }
    else{
      if(nv_stat.bit2){
        nvoGasName.ascii[11] = '%';
        nvoGasName.ascii[12] = 'L';
        nvoGasName.ascii[13] = 'E';
        nvoGasName.ascii[14] = 'L';
      }
      else{
        nvoGasName.ascii[11] = '%';
        nvoGasName.ascii[12] = ' ';
        nvoGasName.ascii[13] = ' ';
        nvoGasName.ascii[14] = ' ';
      }
    }
  }
  quic_out_fl_state = 0;
  max_min_state();
//  max_tx_timer_state = nv_max_send_state / 10;  // default = 300sec
//  min_tx_timer_state = nv_min_send_state / 10;  // default = 5sec
//  min_state_fl = 0;
}
////////////   when quic output (when change data) ////////////////
when(ui_quic_out_hertbeat)
{
  if(nv_send_hertbeat){
    tx_lamp_on();
    nvoHertBeat = nv_sec;
  }
  ui_quic_out_hertbeat = 0;
  max_hertbeat();
}
//////////// command check ////////////////
when (nv_update_occurs (nviComand))
{
    rx_lamp_on();
    command_op();
}
//////////// Min send time NV UPDATE OCCURS ///////////
//when (nv_update_occurs (nciMinSendTM))
when (nv_update_occurs (nciMinSendTI))
{
  rx_lamp_on();
//  nv_min_send_state = nciMinSendTM;
  nv_min_send_state = nciMinSendTI;
//  max_min_state();
}
//////////// Max send time NV UPDATE OCCURS ///////////
//when (nv_update_occurs (nciMaxSendTM))
when (nv_update_occurs (nciMaxSendTI))
{
  rx_lamp_on();
//  nv_max_send_state = nciMaxSendTM;
  nv_max_send_state = nciMaxSendTI;
//  max_min_state();
}
//////////// Min send time NV UPDATE OCCURS ///////////
//when (nv_update_occurs (nciMinSendTM))
when (nv_update_occurs (nciMinSendTM))
{
  rx_lamp_on();
//  nv_min_send_data = nciMinSendTM;
  nv_min_send_data = nciMinSendTM;
//  max_min_data();
}
//////////// Max send time NV UPDATE OCCURS ///////////
//when (nv_update_occurs (nciMaxSendTM))
when (nv_update_occurs (nciMaxSendTM))
{
  rx_lamp_on();
//  nv_max_send_data = nciMaxSendTM;
  nv_max_send_data = nciMaxSendTM;
//  max_min_data();
}
//////////// Max send time NV UPDATE OCCURS ///////////
when (nv_update_occurs (nciMaxSendHB))
{
  rx_lamp_on();
  nv_send_hertbeat = nciMaxSendHB;
//  max_hertbeat();
}
//////////// trans error ////////////////
when (nv_update_fails (nvoConc))
{
    if(tr_ng_cnt >= TR_NG)  tr_ng_fl = 1;
    else                    ++tr_ng_cnt;
}
//////////// trans succeeds ////////////////
when (nv_update_succeeds (nvoConc))
{
    tr_ng_fl = tr_ng_cnt = 0;
}
//////////// reset succeeds ////////////////
when (nv_update_occurs (nviReset))
{

  if(nviReset){

    rx_lamp_on();

    tx_set();                //  set STX ADR_NT ADR_77
    tx_buf[5] = 'S';         // COMMAND
    tx_buf[6] = 'B';
    tx_buf[7] = ',';
    tx_buf[8] = 'W';         // WRITE
    tx_buf[9] = ',';
    tx_buf[10] = '1';

    if(sum_io_in(11)){
      if(rx_buf[11] == ETX){
      }
    }
  }
}
//////////// output denst ////////////////
void tr_denst (void)
{
  U_INT  i;

  tx_set();                //  set STX ADR_NT ADR_77

  if(denst_ok_fl == 0){

    tx_buf[5] = 'V';         // COMMAND
    tx_buf[6] = 'N';
    tx_buf[7] = ',';
    tx_buf[8] = 'W';         // WRITE
    tx_buf[9] = ',';
    tx_buf[10] = '5';        // Ver.50803
    tx_buf[11] = '0';
    tx_buf[12] = '8';
    tx_buf[13] = '0';
    tx_buf[14] = '3';

    if(sum_io_in(15)){
      if(rx_buf[15] == ETX)
        denst_ok_fl = 1;
    }
  }

  else{
    if(ui_tx_cnt < 64){
      if(ui_tx_cnt % 2){
        tx_buf[5] = 'D';         // COMMAND
        tx_buf[6] = 'G';
        tx_buf[7] = ',';
        tx_buf[8] = 'R';         // READ
        tx_buf[9] = ',';

        if(sum_io_in(10)){
          if((rx_buf[rx_num-5]!=NAK) && (rx_buf[18]==ETX)){
            for(i=0;i < 4;i++){
              state_bf[i] = rx_buf[i+10];
              if(state_bf_b[i] !=  state_bf[i]){  // if state_bf <> state_bf_b
                if(min_data_fl == 1){
                  quic_out_fl_data = 1;
                  state_bf_b[i] = state_bf[i];
                }
                if(i==1){                         //  alarm states
                  if(min_state_fl == 1){
                    quic_out_fl_state = 1;
                  }
                }
              }
            }
            for(i=0;i < 4;i++){
              data_bf[i] = rx_buf[i+14];
              if(data_bf_b[i] !=  data_bf[i]){  // if data_bf <> data_bf_b
                if(min_data_fl == 1){
                  quic_out_fl_data = 1;
                  data_bf_b[i] = data_bf[i];
                }
              }
            }
          }
        }
      }
      else{
        tx_buf[5] = 'T';         // COMMAND
        tx_buf[6] = 'D';
        tx_buf[7] = ',';
        tx_buf[8] = 'R';         // READ
        tx_buf[9] = ',';
        tx_buf[10] = '0';
        tx_buf[11] = 0x30 | ui_tx_cmd;
        if(sum_io_in(12)){
          if((rx_buf[rx_num-5]!=NAK) && (rx_buf[21]==ETX)){
            for(i=0;i < 8;i++){
              ui_trb_bf[ui_tx_cmd][i] = rx_buf[i+13];
              if(ui_trb_bf_b[ui_tx_cmd][i] !=  ui_trb_bf[ui_tx_cmd][i]){  // if ui_trb_bf <> ui_trb_bf_b
                if(min_state_fl == 1){
                  quic_out_fl_state = 1;
                  ui_trb_bf_b[ui_tx_cmd][i] = ui_trb_bf[ui_tx_cmd][i];
                }
              }
            }
          }
        }
        ++ui_tx_cmd;
        if(ui_tx_cmd > 3){
          ui_tx_cmd = 0;
        }
      }
    }
    else{
//      if(ui_tx_cmd <= 1){
//        tx_buf[5] = 'A';         // COMMAND
//        tx_buf[6] = 0x30 | (ui_tx_cmd + 1);
//        tx_buf[7] = ',';
//        tx_buf[8] = 'R';         // READ
//        tx_buf[9] = ',';
//        if(sum_io_in(10)){
//          if((rx_buf[rx_num-5]!=NAK) && (rx_buf[14]==ETX)){
//            for(i=0;i < 4;i++){
//              ui_alarm_bf[ui_tx_cmd][i] = rx_buf[i+10];
//              if(ui_alarm_bf_b[ui_tx_cmd][i] !=  ui_alarm_bf[ui_tx_cmd][i]){  // if ui_alarm_bf <> ui_alarm_bf_b
//                if(min_state_fl == 1){
//                  quic_out_fl_state = 1;
//                  ui_alarm_bf_b[ui_tx_cmd][i] = ui_alarm_bf[ui_tx_cmd][i];
//                }
//              }
//            }
//          }
//        }
//        ++ui_tx_cmd;
//      }
//      else{
        tx_buf[5] = 'G';         // COMMAND
        tx_buf[6] = 'N';
        tx_buf[7] = ',';
        tx_buf[8] = 'R';         // READ
        tx_buf[9] = ',';
        if(sum_io_in(10)){
          if((rx_buf[rx_num-5]!=NAK) && (rx_buf[20]==ETX)){
            for(i=0;i < 10;i++){
              ui_gas_name[i] = rx_buf[i+10];
              if(ui_gas_name_b[i] !=  ui_gas_name[i]){  // if ui_gas_name <> ui_gas_name_b
                if(min_state_fl == 1){
                  quic_out_fl_state = 1;
                  ui_gas_name_b[i] = ui_gas_name[i];
                }
              }
            }
          }
        }
//        ui_tx_cmd = 0;
//      }
    }
  }
}
//////////// output error ////////////////
void tr_error (void)
{
  tx_set();                //  set STX ADR_NT ADR_77

  tx_buf[5] = 'E';         // COMMAND
  tx_buf[6] = 'R';
  tx_buf[7] = ',';
  tx_buf[8] = 'W';         // READ
  tx_buf[9] = ',';
  tx_buf[10] = '1';

  if(sum_io_in(11)){};
}
//////////////////////////////////
U_INT sum_io_in(U_INT num)
{
  U_INT  i;

  tx_buf[num] = ETX;         // End of text(0x03)

  dg1 = sum_calc(tx_buf,num+1);  // num+1 byte sum
  dg1_to_as2();
  tx_buf[num+1]= as2[0];
  tx_buf[num+2]= as2[1];

  tx_buf[num+3]= EOT;         // End of transmition

  io_out(TXD,tx_buf,num+4);   // num+4 byte serial out
  for(i = 0 ; i < 3 ; ++i){  // retry 3times
    rx_num = io_in(RXD,rx_buf,32);    // max 86 byte
    if(rx_num)  i = 3;                // retry 3times end
  }
  if(rx_buf[rx_num-1]==EOT){     // if last == EOT
    as2[0] = rx_buf[rx_num -3];  // SUM
    as2[1] = rx_buf[rx_num -2];
    as2_to_dg1();
    if(sum_calc(rx_buf,rx_num-3) == dg1){ // if SUM OK
      as2[0] = rx_buf[3];  // 77 ADDRESS
      as2[1] = rx_buf[4];
      as2_to_dg1();
      adr_77 = dg1;
      return(1);
    }
  }
  return(0);
}
////////////////////////////
void rx_lamp_on (void)
{
  io_out(RXD_LP,LO);      // rxd lamp on
  rx_lamp_timer = 20;    // 200msec
}
////////////////////////////
when(wink)
{
    U_INT loop;

    for(loop=20;loop>0;--loop){
       watchdog_update();
       io_out(RXD_LP,LO);      // rxd lamp on
       io_out(TXD_LP,LO);            // txd lamp on
       delay(5000);             // 125msec wait
//       time_loop();
       io_out(RXD_LP,HI);      // rxd lamp off
       io_out(TXD_LP,HI);            // txd lamp off
       delay(5000);             // 125msec wait
//       time_loop();
    }
}
//void time_loop (void)
//{
//    U_INT timer1,timer2;
//       for(timer1=50;timer1>0;--timer1){
//          for(timer2=200;timer2>0;--timer2){
//            watchdog_update();
//          }
//       }
//}
void max_min_state(void)
{
    if(nv_max_send_state){
      max_tx_timer_state = nv_max_send_state / 10;  // 300sec
    }
    else{
//      max_tx_timer_state = 5;                   // def.5sec
      max_tx_timer_state = 6553;                   // MAX.6553sec
    }
    min_tx_timer_state = nv_min_send_state / 10;  // 5sec
    if(nv_min_send_state){
      min_state_fl = 0;
    }
}
void max_min_data(void)
{
    if(nv_max_send_data){
      max_tx_timer_data = nv_max_send_data / 10;  // 300sec
    }
    else{
//      max_tx_timer_data = 5;                    // def.5sec
      max_tx_timer_data = 6553;                    // MAX.6553sec
    }
    min_tx_timer_data = nv_min_send_data / 10;  // 5sec
    if(nv_min_send_data){
      min_data_fl = 0;
    }
}
void max_hertbeat(void)
{
  if(nv_send_hertbeat){
    max_tx_hertbeat = nv_send_hertbeat / 10;    // 300sec
  }
  else{
    max_tx_hertbeat = 5;                        // def.5sec
  }
}
