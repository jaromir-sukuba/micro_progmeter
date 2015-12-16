void disp_init (void);
void disp_char_print (unsigned char data);
void disp_set_col (unsigned char add);
void disp_set_page (unsigned char add);
void disp_set_xy (unsigned char x, unsigned char y);
void disp_puts(char *data);
void disp_clr (unsigned char data);

#define Write_Address 0x78/*slave addresses with write*/
#define Read_Address 0x79/*slave addresses with read*/

