/********************************************
 U_INT(1BYTE) ->  U_INT(ASCII)(2BYTE)
********************************************/
void    dg1_to_as2(void)
{
        if(dg1 >= 16){
                as2[0] = (dg1 / 16);    /** jyoui **/
                as2[1] = (dg1 % 16);    /** amari*16 (kai) **/
        }
        else{
                as2[0] = 0;             /** jyoui **/
                as2[1] = dg1;           /** kai **/
        }
        if(as2[0] > 9)          // A B C D E F
                as2[0] = 0x40 + (as2[0] - 9);
        else
                as2[0] = 0x30 + as2[0];

        if(as2[1] > 9)          // A B C D E F
                as2[1] = 0x40 + (as2[1] - 9);
        else
                as2[1] = 0x30 + as2[1];
}

/********************************************
  U_INT(ASCII)(2BYTE) -> U_INT(1BYTE)
********************************************/
void    as2_to_dg1(void)
{
        U_INT tmp1,tmp2;

        if(as2[0] >= 0x41)      // A B C D E F
                tmp1 = 10 + (as2[0] - 0x41);
        else
                tmp1 = as2[0] - 0x30;
        if(as2[1] >= 0x41)      // A B C D E F
                tmp2 = 10 + (as2[1] - 0x41);
        else
                tmp2 = as2[1] - 0x30;
        dg1 = (tmp1*16) + tmp2;
}
/********************************************
  calculation  sum
********************************************/
U_INT  sum_calc(U_INT *buffer,U_INT num)
{
        int i;
        U_INT sum;

        sum = 0;
        for(i = 0;i < num ;i++){
                sum += *buffer;
                ++buffer;
        }
        sum = 0 - sum;
        return(sum);
}
/********************************************
  I/O INITIAL
********************************************/
void  io_init(void)
{
//    U_INT dummy,loop;
    U_INT loop;

//    watchdog_update();
//    dummy = io_in(IO2);                // DUMMY INPUT
//    dummy = io_in(IO3);
//    dummy = io_in(IO4);
//    dummy = io_in(IO5);
//    dummy = io_in(IO6);
//    dummy = io_in(IO7);
//    dummy = io_in(IO9);
//    io_out (TXD_LP, HI);        // TXD LAMP OFF
//    io_out (RXD_LP, HI);        // RXD LAMP OFF
    tx_buf[0] = '#';
    io_out(TXD,tx_buf,1);       // RS-232c PORT INITIAL DUMMY
    for( loop=24 ; loop > 0 ; --loop){
            watchdog_update();
      delay(5000);             // 125msec wait
    }
//    watchdog_update();
}
//////////// tx_buf start set //////////////
void tx_set(void)
{
  tx_buf[0] = STX;         // Start of text(0x02)

  dg1 = adr_nt;            // RM-77NT address
  dg1_to_as2();
  tx_buf[1] = as2[0];
  tx_buf[2] = as2[1];

  dg1 = adr_77;             // GD-77 address
  dg1_to_as2();
  tx_buf[3] = as2[0];
  tx_buf[4] = as2[1];
}
