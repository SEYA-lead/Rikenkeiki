////////// transmit status ////////////
void trans_state(void)
{
  U_INT s10,s1;
  U_INT i,lui_temp;
//  U_LONG st;

  tx_lamp_on();

  as2[0] = state_bf[0];
  as2[1] = state_bf[1];
  as2_to_dg1();
  s10 = dg1;
  as2[0] = state_bf[2];
  as2[1] = state_bf[3];
  as2_to_dg1();
  s1  = dg1;

//  nv_stat.bit15 = (s10 & 0x80) ? 1:0;  // 1:mente
//  nv_stat.bit14 = (s10 & 0x40) ? 1:0;  // 1:test
//  nv_stat.bit13 = (s10 & 0x20) ? 1:0;  // 1:skip
//  nv_stat.bit12 = (s10 & 0x10) ? 1:0;  // 1:initial
//  nv_stat.bit11 = (s10 & 0x08) ? 1:0;  // 1:over
//  nv_stat.bit10 = (s10 & 0x04) ? 1:0;  // 1:3rd alarm
//  nv_stat.bit9 = (s10 & 0x02) ? 1:0;   // 1:2nd alarm
//  nv_stat.bit8 = (s10 & 0x01) ? 1:0;   // 1:1st alarm
//  nv_stat.bit7 = (s1 & 0x80) ? 1:0;    // 1:sensor tbl
//  nv_stat.bit6 = (s1 & 0x40) ? 1:0;    // 1:com tbl
//  nv_stat.bit5 = (s1 & 0x20) ? 1:0;    // 1:flow tbl
//  nv_stat.bit4 = (s1 & 0x10) ? 1:0;    // 1:special
//  nv_stat.bit3 = (s1 & 0x08) ? 1:0;    // 00:vol 01:lel
//  nv_stat.bit2 = (s1 & 0x04) ? 1:0;    // 10:ppm 11:ppb
//  nv_stat.bit1 = (s1 & 0x02) ? 1:0;    // 00:1/1 01:1/10
//  nv_stat.bit0 = (s1 & 0x01) ? 1:0;    // 10:1/100 11:1/1000

  lui_temp = 0x01;
  for(i=8;i < 16;i++){
    if(s10 & lui_temp){
      set_bit(&nv_stat,i);
    }
    else{
      clr_bit(&nv_stat,i);
    }
    lui_temp = lui_temp << 1;
  }

  lui_temp = 0x01;
  for(i=0;i < 8;i++){
    if(s1 & lui_temp){
      set_bit(&nv_stat,i);
    }
    else{
      clr_bit(&nv_stat,i);
    }
    lui_temp = lui_temp << 1;
  }

  nvoNstate = nv_stat;
}
////////// transmit data ////////////
void trans_data(void)
{
  U_INT d10,d1,s1,loop;
  U_LONG da;

//  tx_lamp_on();

  as2[0] = data_bf[0];
  as2[1] = data_bf[1];
  as2_to_dg1();
  d10 = dg1;
  as2[0] = data_bf[2];
  as2[1] = data_bf[3];
  as2_to_dg1();
  d1  = dg1;

  da = d10;
  da = da*256;
  da = da + d1;
  if(da >= 0x8000){      // minus -> zero
    da = 0;
  }
  fl_from_ulong(da,&float_buf);

  as2[0] = state_bf[2];
  as2[1] = state_bf[3];
  as2_to_dg1();
  s1  = dg1;
  loop = s1 & 0x03;
  for( ;loop > 0;--loop){
    fl_div(&float_buf,&fl_ten,&float_buf);
  }
  nvoConc = *(SNVT_ppm_f *)&float_buf;
}
////////// transmit data ////////////
//void alarm_data(U_INT pui_alm_data)
//{
//  U_INT d10,d1,s1,loop;
//  U_LONG da;
//
//  as2[0] = ui_alarm_bf[pui_alm_data][0];
//  as2[1] = ui_alarm_bf[pui_alm_data][1];
//  as2_to_dg1();
//  d10 = dg1;
//  as2[0] = ui_alarm_bf[pui_alm_data][2];
//  as2[1] = ui_alarm_bf[pui_alm_data][3];
//  as2_to_dg1();
//  d1  = dg1;
//
//  da = d10;
//  da = da*256;
//  da = da + d1;
//  fl_from_ulong(da,&float_buf);
//
//  as2[0] = state_bf[2];
//  as2[1] = state_bf[3];
//  as2_to_dg1();
//  s1  = dg1;
//  loop = s1 & 0x03;
//  for( ;loop > 0;--loop){
//    fl_div(&float_buf,&fl_ten,&float_buf);
//  }
//  if(pui_alm_data){
//    nvoAlarmThresh2 = *(SNVT_ppm_f *)&float_buf;
//  }
//  else{
//    nvoAlarmThresh1 = *(SNVT_ppm_f *)&float_buf;
//  }
//}
////////// transmit data ////////////
void trb_data(void)
{
  tx_lamp_on();

  if((ui_trb_bf[2][6] != 0x30)||(ui_trb_bf[2][7] != 0x30)||(ui_trb_bf[3][0] != 0x30)){
    nvoFaultS.value = 18;           // = 9.0 (E-9) from E-88 to E-99
    nvoFaultS.state = 1;
  }
  else if((ui_trb_bf[0][2] != 0x30)||(ui_trb_bf[0][3] != 0x30)||(ui_trb_bf[0][4] != 0x30)||(ui_trb_bf[0][5] != 0x30)||(ui_trb_bf[0][7] != 0x30)){
    nvoFaultS.value = 2;            // = 1.0 (E-1) from E-08 to E-23, from E-28 to E-31
    nvoFaultS.state = 1;
  }
  else if((ui_trb_bf[2][1] != 0x30)||(ui_trb_bf[2][2] != 0x30)||(ui_trb_bf[2][3] != 0x30)){
    nvoFaultS.value = 14;           // = 7.0 (E-7) from E-68 to E-79
    nvoFaultS.state = 1;
  }
  else if((ui_trb_bf[1][4] != 0x30)||(ui_trb_bf[1][5] != 0x30)||(ui_trb_bf[1][6] != 0x30)){
    nvoFaultS.value = 10;           // = 5.0 (E-5) from E-48 to E-59
    nvoFaultS.state = 1;
  }
  else{
    nvoFaultS.value = 0;            // = no error
    nvoFaultS.state = 0;
  }
}
//////////////////////////
void tx_lamp_on(void)
{
  io_out(TXD_LP,LO);            // txd lamp on
  tx_lamp_timer = 20;           // 200msec
}
