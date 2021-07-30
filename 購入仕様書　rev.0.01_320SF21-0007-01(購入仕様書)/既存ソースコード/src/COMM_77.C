void command_op(void)
{
//  U_INT  i,rec_n,wait_cnt;
  U_INT  i,rec_n;

  for(i = 0 ; i < 31 ; ++i){
//    if(nviComand.ascii[i] == EOT){
    if(nviComand.ascii[i] == 0x00){
      rec_n = i;
      i = 31;
    }
  }

  tx_set();                           //  set STX ADR_NT ADR_77
  for(i = 0 ; i < rec_n ; ++i){
    tx_buf[5+i] = nviComand.ascii[i];
  }
  if(sum_io_in(5+i)){
//    if(sum_calc(rx_buf,rx_num-3) == dg1){ // if SUM OK
      if(rx_buf[rx_num-5] != NAK){  // if NAK
        tx_lamp_on();
        for(i = 0 ; i < rx_num - 9; ++i){
           nvoComand.ascii[i] = rx_buf[i+5];
        }
        for(i = rx_num - 9 ; i < 31; ++i){
           nvoComand.ascii[i] = 0x00;
        }
      }
//    }
  }
}

