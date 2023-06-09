/**
 * @file Amoled.c
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-04-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// #include <iostream>
// #include <cstring>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "lvgl.h"
#include "porting/lv_port_disp.h"
#include "lv_demos.h"



#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))




#define LCD_RST     GPIO_NUM_1
#define LCD_I2      GPIO_NUM_2
#define LCD_I3      GPIO_NUM_3
#define LCD_CLK     GPIO_NUM_4
#define LCD_I1      GPIO_NUM_5
#define LCD_IO0     GPIO_NUM_6
#define LCD_CS      GPIO_NUM_7



#define LCD_CS_EN()     gpio_set_level(LCD_CS, 0)
#define LCD_CS_DIS()    gpio_set_level(LCD_CS, 1)

#define SPI_CS0     gpio_set_level(LCD_CS, 0)
#define SPI_CS1     gpio_set_level(LCD_CS, 1)

#define SPI_SDA0    gpio_set_level(LCD_IO0, 0)
#define SPI_SDA1    gpio_set_level(LCD_IO0, 1)

#define SPI_SCL0    gpio_set_level(LCD_CLK, 0)
#define SPI_SCL1    gpio_set_level(LCD_CLK, 1)


#define SPI_IO00    gpio_set_level(LCD_IO0, 0)
#define SPI_IO01    gpio_set_level(LCD_IO0, 1)

#define SPI_IO10    gpio_set_level(LCD_I1, 0)
#define SPI_IO11    gpio_set_level(LCD_I1, 1)

#define SPI_IO20    gpio_set_level(LCD_I2, 0)
#define SPI_IO21    gpio_set_level(LCD_I2, 1)

#define SPI_IO30    gpio_set_level(LCD_I3, 0)
#define SPI_IO31    gpio_set_level(LCD_I3, 1)








//SPI写数据
//----------------------------------------------------------------------
void  SPI_1L_SendData(uint16_t dat)
{  
  unsigned char i;

   for(i=0; i<8; i++)			
   {   
      if( (dat&0x80)!=0 ) SPI_SDA1;
      else                SPI_SDA0;

		dat  <<= 1;

	  SPI_SCL0;//delay_us(2);
    SPI_SCL1;	
		 
   }
	 
}

void SPI_WriteComm(uint16_t regval)
{ 
SPI_1L_SendData(0x02);
SPI_1L_SendData(0x00);
SPI_1L_SendData(regval);
SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_ReadComm(uint16_t regval)
{    
SPI_1L_SendData(0x03);//
SPI_1L_SendData(0x00);
SPI_1L_SendData(regval);
SPI_1L_SendData(0x00);//PAM
}


void SPI_WriteComm_QSPI(uint16_t regval)
{ 
SPI_1L_SendData(0x32);//32  
SPI_1L_SendData(0x00);
SPI_1L_SendData(regval);
SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_WriteData(uint16_t val)
{   
 unsigned char n;	
   for(n=0; n<8; n++)			
   {  
		if(val&0x80) SPI_SDA1;/*SPI_SDA 为写进去的值*/
		else         SPI_SDA0;
		 
		val<<= 1;

		SPI_SCL0;//delay_us(2);
		SPI_SCL1;
   }
}



void QSPI_WriteData(uint16_t val)
{
//SPI_CS=0; 			
if(val&0x80) SPI_IO31;
else  			 SPI_IO30;

if(val&0x40) SPI_IO21;
else  			 SPI_IO20;

if(val&0x20) SPI_IO11;
else   			 SPI_IO10;

if(val&0x10) SPI_IO01;
else  			 SPI_IO00;

SPI_SCL0;//delay_us(2);
SPI_SCL1;//delay_us(2);


if(val&0x08) SPI_IO31;else SPI_IO30;

if(val&0x04) SPI_IO21;else SPI_IO20;

if(val&0x02) SPI_IO11;else SPI_IO10;

if(val&0x01) SPI_IO01;else SPI_IO00;

SPI_SCL0;//delay_us(2);
SPI_SCL1;//delay_us(1);
}

void Write_Disp_Data(uint16_t val) //16BIT
{ 
	QSPI_WriteData(val>>8);
	QSPI_WriteData(val);
}

void SPI_4wire_data_1wire_Addr(uint16_t First_Byte,uint16_t Addr)
{   
SPI_1L_SendData(First_Byte);//
SPI_1L_SendData(0x00);
SPI_1L_SendData(Addr);
SPI_1L_SendData(0x00);//PA
}

void SPI_4wire_data_4wire_Addr(uint16_t First_Byte,uint16_t Addr)
{
SPI_1L_SendData(First_Byte);//
QSPI_WriteData(0x00);
QSPI_WriteData(Addr);
QSPI_WriteData(0x00);//PA
}



void SPI_4W_DATA_4W_ADDR_START(void )
{
SPI_4wire_data_4wire_Addr(0x12,0x2c);
}
	
	
	
void SPI_4W_DATA_1W_ADDR_START(void )
{
SPI_4wire_data_1wire_Addr(0x32,0x2C);
}

void SPI_4W_DATA_1W_ADDR_END(void )
{	
SPI_CS0;	
SPI_4wire_data_1wire_Addr(0x32,0x00);
SPI_CS1;	
}




void DO0180FS01_Init() // www.dwo.net.cn
{
SPI_CS0;
	SPI_WriteComm(0x11); //Sleep Out
SPI_CS1;

delay(120);

SPI_CS0;
	SPI_WriteComm(0x44); 
	SPI_WriteData(0x01);
	SPI_WriteData(0x66);
SPI_CS1;

SPI_CS0;
	SPI_WriteComm(0x35); //TE ON
	SPI_WriteData(0x00);
SPI_CS1;

SPI_CS0;
	SPI_WriteComm(0x3A); 
	SPI_WriteData(0x55); //Interface Pixel Format	16bit/pixel
	//SPI_WriteData(0x66); //Interface Pixel Format	18bit/pixel
	//SPI_WriteData(0x77); //Interface Pixel Format	24bit/pixel
SPI_CS1;

SPI_CS0;
	SPI_WriteComm(0x53); 
	SPI_WriteData(0x20);
SPI_CS1;

delay(10);

SPI_CS0;
	SPI_WriteComm(0x51); //Write Display Brightness	MAX_VAL=0XFF
	SPI_WriteData(0x00);
SPI_CS1;

delay(10);

SPI_CS0;
	SPI_WriteComm(0x29); //Display on  
SPI_CS1;

delay(10);

SPI_CS0;
	SPI_WriteComm(0x51); //Write Display Brightness	MAX_VAL=0XFF
	SPI_WriteData(0xFF);
SPI_CS1;
}  //end of AMOLED_Init


///*---------------------------------------------------------------------------
//此函数用于设置AMOLED“窗口”---GRAM
//---------------------------------------------------------------------------*/
void AMOLED_Block_Write(uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend)
{

SPI_CS0;
	SPI_WriteComm(0x2a); //Set Column Start Address   
	SPI_WriteData(Xstart>>8);
	SPI_WriteData(Xstart&0xff);
	SPI_WriteData(Xend>>8);
	SPI_WriteData(Xend&0xff);
SPI_CS1;
	
SPI_CS0;
	SPI_WriteComm(0x2b); //Set Row Start Address    
	SPI_WriteData(Ystart>>8);
	SPI_WriteData(Ystart&0xff);
	SPI_WriteData(Yend>>8);
	SPI_WriteData(Yend&0xff);
SPI_CS1;

SPI_CS0;	
	SPI_WriteComm(0x2c);//Memory Write
SPI_CS1;	

}


#define COL 368
#define ROW 448

//清屏函数   
//全屏显示单色
void DM_Clear(uint16_t color)
{  	
	unsigned int i,j;  
	
	// DM_block_write(0,COL-1,0,ROW-1);
    AMOLED_Block_Write(0,COL-1,0,ROW-1);
	
	SPI_CS0;
	SPI_4wire_data_1wire_Addr(0x32,0x2C);

	for(i=0;i<COL;i++) {
        for(j=0;j<ROW;j++) {
            Write_Disp_Data(color);
        }
    }
		
			
    SPI_CS1;
			
    SPI_4W_DATA_1W_ADDR_END();
}


void DM_display(uint16_t Xstart, uint16_t Xend, uint16_t Ystart, uint16_t Yend, uint16_t color)
{  	
	unsigned int i,j;  
	
	// DM_block_write(0,COL-1,0,ROW-1);
    AMOLED_Block_Write(Xstart,Xend,Ystart,Yend);
	
	SPI_CS0;
	SPI_4wire_data_1wire_Addr(0x32,0x2C);

	for(i=0;i<(Xend - Xstart);i++) {
        for(j=0;j<(Yend - Ystart);j++) {
            Write_Disp_Data(color);
        }
    }
		
			
    SPI_CS1;
			
    SPI_4W_DATA_1W_ADDR_END();
}



uint16_t rgb888_2_rgb565(uint32_t color)
{
    uint_fast16_t ret = 0;

    uint_fast8_t R = (color & 0x00FF0000) >> 19;
    uint_fast8_t G = (color & 0x0000FF00) >> 10;
    uint_fast8_t B = (color & 0x000000FF) >> 3;

    ret = (R << 11) | (G << 5) | (B << 0);
    return ret;
}




void send_cmd(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length    = 8;
    t.tx_buffer = &cmd;

    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE;
    }

    /* Transmit */
    ret = spi_device_polling_transmit(spi, &t);

    assert(ret == ESP_OK);
}


void send_data(spi_device_handle_t spi, const uint8_t *data, int len, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;

    if (len==0) return;
    memset(&t, 0, sizeof(t));

    t.length    = len * 8;
    t.tx_buffer = data;

    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE;
    }


    /* Transmit */
    ret = spi_device_polling_transmit(spi, &t);


    assert(ret == ESP_OK);
}


void send_cmd_qspi(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length    = 8;
    t.tx_buffer = &cmd;

    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE | SPI_TRANS_MODE_QIO;
    }
    else {
        t.flags = SPI_TRANS_MODE_QIO;
    }

    /* Transmit */
    ret = spi_device_polling_transmit(spi, &t);

    assert(ret == ESP_OK);
}


void send_data_qspi(spi_device_handle_t spi, const uint16_t *data, int len, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;

    // printf("%d\n", len);

    if (len==0) return;
    memset(&t, 0, sizeof(t));

    t.length    = len * 8;
    t.tx_buffer = data;

    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE | SPI_TRANS_MODE_QIO;
    }
    else {
        t.flags = SPI_TRANS_MODE_QIO;
    }


    /* Transmit */
    ret = spi_device_polling_transmit(spi, &t);


    assert(ret == ESP_OK);
}


void send_data_qspi_queue(spi_device_handle_t spi, const uint8_t *data, int len, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;

    if (len==0) return;
    memset(&t, 0, sizeof(t));

    t.length    = len * 8;
    t.tx_buffer = data;

    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE | SPI_TRANS_MODE_QIO;
    }
    else {
        t.flags = SPI_TRANS_MODE_QIO;
    }


    /* Transmit */
    // ret = spi_device_polling_transmit(spi, &t);
    spi_device_queue_trans(spi, &t, portMAX_DELAY);


    assert(ret == ESP_OK);
}


void send_data_qspi_queue_finish(spi_device_handle_t spi, int queue_num)
{
    spi_transaction_t *rtrans;
    esp_err_t ret;
    //Wait for all 6 transactions to be done and get back the results.
    for (int x=0; x < queue_num; x++) {
        ret=spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY);
        assert(ret==ESP_OK);
        //We could inspect rtrans now if we received any info back. The LCD is treated as write-only, though.
    }
}




void display_send_cmd(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    // SPI_1L_SendData(0x02);
    // SPI_1L_SendData(0x00);
    // SPI_1L_SendData(regval);
    // SPI_1L_SendData(0x00);//delay_us(2);
    send_cmd(spi, 0x02, true);
    send_cmd(spi, 0x00, true);
    send_cmd(spi, cmd, true);
    send_cmd(spi, 0x00, keep_cs_active);
}


void display_write_block(spi_device_handle_t spi, uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend)
{
// SPI_CS0;
// 	SPI_WriteComm(0x2a); //Set Column Start Address   
// 	SPI_WriteData(Xstart>>8);
// 	SPI_WriteData(Xstart&0xff);
// 	SPI_WriteData(Xend>>8);
// 	SPI_WriteData(Xend&0xff);
// SPI_CS1;
	
// SPI_CS0;
// 	SPI_WriteComm(0x2b); //Set Row Start Address    
// 	SPI_WriteData(Ystart>>8);
// 	SPI_WriteData(Ystart&0xff);
// 	SPI_WriteData(Yend>>8);
// 	SPI_WriteData(Yend&0xff);
// SPI_CS1;

// SPI_CS0;	
// 	SPI_WriteComm(0x2c);//Memory Write
// SPI_CS1;



    /* Set Column Start Address */
    display_send_cmd(spi, 0x2A, true);
    send_cmd(spi, Xstart >> 8, true);
    send_cmd(spi, Xstart & 0xFF, true);
    send_cmd(spi, Xend >> 8, true);
    send_cmd(spi, Xend & 0xFF, false);

    /* Set Row Start Address */
    display_send_cmd(spi, 0x2B, true);
    send_cmd(spi, Ystart >> 8, true);
    send_cmd(spi, Ystart & 0xFF, true);
    send_cmd(spi, Yend >> 8, true);
    send_cmd(spi, Yend & 0xFF, false);

    /* Memory Write */
    display_send_cmd(spi, 0x2C, false);

}


// #define COL 368
// #define ROW 448
static uint16_t* _full_databuffer;



void display_push_color(spi_device_handle_t spi, uint16_t Xstart, uint16_t Xend, uint16_t Ystart, uint16_t Yend, uint16_t color)
{  	
	// unsigned int i,j;  
	
	// // DM_block_write(0,COL-1,0,ROW-1);
    // AMOLED_Block_Write(Xstart,Xend,Ystart,Yend);
	
	// SPI_CS0;
	// SPI_4wire_data_1wire_Addr(0x32,0x2C);

	// for(i=0;i<(Xend - Xstart);i++) {
    //     for(j=0;j<(Yend - Ystart);j++) {
    //         Write_Disp_Data(color);
    //     }
    // }
		
    // SPI_CS1;
			
    // SPI_4W_DATA_1W_ADDR_END();




    // display_write_block(spi, Xstart, Xend, Ystart, Yend);
    // display_write_block(spi, 0,COL-1,0,ROW-1);



    // /* Begin qspi */
    // send_cmd(spi, 0x32, true);
    // send_cmd(spi, 0x00, true);
    // send_cmd(spi, 0x2C, true);
    // send_cmd(spi, 0x00, true);

    // int shit = 0;

    // for (int i = 0; i < (Xend - Xstart); i++) {
    //     for (int j = 0; j < (Yend - Ystart); j++) {
    //         // Write_Disp_Data(color);
    //         // QSPI_WriteData(val>>8);
	//         // QSPI_WriteData(val);


    //         // send_cmd_qspi(spi, color >> 8, true);
    //         // // send_cmd_qspi(spi, color & 0xFF, true);

    //         // if ((i == ((Xend - Xstart) - 1)) && (j == ((Yend - Ystart) - 1))) {
    //         //     // send_cmd_qspi(spi, color & 0xFF, false);
    //         //     send_cmd_qspi(spi, color, false);
    //         // }
    //         // else {
    //         //     // send_cmd_qspi(spi, color & 0xFF, true);
    //         //     send_cmd_qspi(spi, color, true);
    //         // }

    //         uint8_t data_buffer = color >> 8;
    //         send_data_qspi(spi, &data_buffer, 1, true);
    //         data_buffer = color & 0xFF;
    //         send_data_qspi(spi, &data_buffer, 1, true);

    //         shit++;
    //         shit++;

    //     }
    // }

    // printf("%d\n", shit);

    // /* Stop qspi */
    // send_cmd(spi, 0x32, true);
    // send_cmd(spi, 0x00, true);
    // send_cmd(spi, 0x00, true);
    // send_cmd(spi, 0x00, false);


    // delay(100);








    // display_write_block(spi, 0, 200, 0, 200);
    // display_write_block(spi, Xstart + 100, Xend + 100, Ystart, Yend);

    /* To eliminate gaps, don't know why */
    // Yend += 2;
    display_write_block(spi, Xstart, Xend, Ystart, Yend);


    /* Begin qspi */
    uint8_t begin_cmd[] = {0x32, 0x00, 0x2C, 0x00};
    send_data(spi, begin_cmd, 4, true);


    int data_size = (Xend - Xstart) * (Yend - Ystart);
    // printf("%d\n", data_size * 2);

    /* Store color in big endian */
    for (int i = 0; i < data_size; i++) {
        _full_databuffer[i] = (color >> 8) | (color << 8);

        // _full_databuffer[i] = color >> 8;
        // _full_databuffer[i + 1] = color;
    }

    /* Can only send 16000 * 2 each time, don't know why */
    int div_num      = data_size / 16000;
    int div_last_num = data_size % 16000;
    // printf("%d %d\n", div_num, div_last_num);

    for (int i = 0; i < div_num; i++) {
        send_data_qspi(spi, _full_databuffer, 16000 * 2, true);
    }
    if (div_last_num != 0) {
        send_data_qspi(spi, _full_databuffer, div_last_num * 2, true);
    }

    /* Stop qspi */
    begin_cmd[2] = 0x00;
    send_data(spi, begin_cmd, 4, false);



}


void display_push_buffer(spi_device_handle_t spi, uint16_t Xstart, uint16_t Xend, uint16_t Ystart, uint16_t Yend, uint16_t* buffer)
{
    display_write_block(spi, Xstart, Xend, Ystart, Yend);


    /* Begin qspi */
    uint8_t begin_cmd[] = {0x32, 0x00, 0x2C, 0x00};
    send_data(spi, begin_cmd, 4, true);


    int data_size = (Xend - Xstart) * (Yend - Ystart);
    // printf("%d\n", data_size * 2);

    /* Swap color in big endian */
    // for (int i = 0; i < data_size; i++) {
    //     // _full_databuffer[i] = (color >> 8) | (color << 8);
    //     buffer[i] = (buffer[i] >> 8) | (buffer[i] << 8);

    //     // _full_databuffer[i] = color >> 8;
    //     // _full_databuffer[i + 1] = color;
    // }

    /* Can only send 16000 * 2 each time, don't know why */
    int div_num      = data_size / 16000;
    int div_last_num = data_size % 16000;
    // printf("%d %d\n", div_num, div_last_num);

    for (int i = 0; i < div_num; i++) {
        send_data_qspi(spi, &buffer[16000 * i], 16000 * 2, true);
        // send_data_qspi_queue(spi, &buffer[16000 * i], 16000 * 2, true);
    }
    if (div_last_num != 0) {
        send_data_qspi(spi, &buffer[16000 * div_num], div_last_num * 2, true);
        // send_data_qspi_queue(spi, &buffer[16000 * div_num], div_last_num * 2, true);
    }

    /* Stop qspi */
    begin_cmd[2] = 0x00;
    send_data(spi, begin_cmd, 4, false);


    // send_data_qspi_queue_finish(spi, 11);
}






static spi_device_handle_t _spi;


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    // if(disp_flush_enabled) {
    //     /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    //     int32_t x;
    //     int32_t y;
    //     for(y = area->y1; y <= area->y2; y++) {
    //         for(x = area->x1; x <= area->x2; x++) {
    //             /*Put a pixel to the display. For example:*/
    //             /*put_px(x, y, *color_p)*/
    //             color_p++;
    //         }
    //     }
    // }

    // uint32_t w = ( area->x2 - area->x1 + 1 );
    // uint32_t h = ( area->y2 - area->y1 + 1 );
    // // _lcd->startWrite();
    // _lcd->setAddrWindow( area->x1, area->y1, w, h );
    // // _lcd->pushColors( ( uint16_t * )&color_p->full, w * h, true);
    // _lcd->startWrite();
    // _lcd->writePixels((uint16_t*)&color_p->full, w * h, true);
    // _lcd->endWrite(); 
    // // _lcd->endWrite();


    // display_push_color(_spi, area->x1, area->x2, area->y1, area->y2, color_p->full);
    // printf("%d %d %d %d 0x%X\n", area->x1, area->x2, area->y1, area->y2, color_p->full);
    display_push_buffer(_spi, area->x1, area->x2, area->y1, area->y2, color_p);



    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}



void lvgl_init()
{
    lv_init();



    /*-------------------------
     * Initialize your display
     * -----------------------*/

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    // /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    // /* Example for 2) */
    // static lv_disp_draw_buf_t draw_buf_dsc_2;
    // static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
    // static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/

    /* Try to get buffer from PSRAM */
    lv_color_t * buf_3_1 = (lv_color_t *)heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    lv_color_t * buf_3_2 = (lv_color_t *)heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    /* If failed */
    if ((buf_3_1 == NULL) || (buf_3_2 == NULL)) {
        printf("[LVGL] malloc buffer from PSRAM fialed\n");
        while (1)
            delay(1000);
    } else {
        printf("[LVGL] malloc buffer from PSRAM successful\n");
        // printf("[LVGL] free PSRAM: %d\r\n", ESP.getFreePsram());
    }

    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_3;

    /*Required for Example 3)*/
    // disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);




    // lv_demo_stress();
    lv_demo_benchmark();




    while (1) {
        lv_timer_handler();
        delay(2);
    }



}














#define USE_SPI     1


void app_main()
{

    _full_databuffer = (uint16_t*)heap_caps_malloc(COL * ROW * sizeof(uint16_t), MALLOC_CAP_SPIRAM);

    // SOC_SPI_MAXIMUM_BUFFER_SIZE

    #if USE_SPI

    esp_err_t ret;
    spi_device_handle_t spi;

    spi_bus_config_t buscfg = {
        // .miso_io_num=PIN_NUM_MISO,
        // .mosi_io_num=PIN_NUM_MOSI,
        // .sclk_io_num=PIN_NUM_CLK,
        // .quadwp_io_num=-1,
        // .quadhd_io_num=-1,
        // .max_transfer_sz=PARALLEL_LINES*320*2+8



        .data0_io_num       = LCD_IO0,
        .data1_io_num       = LCD_I1,
        .data2_io_num       = LCD_I2,
        .data3_io_num       = LCD_I3,
        .sclk_io_num        = LCD_CLK,
        .flags              = SPICOMMON_BUSFLAG_QUAD,
        // .max_transfer_sz    = COL * ROW * sizeof(uint16_t),
        // .max_transfer_sz    = 329728 * 2,
        .max_transfer_sz    = 16000 * 2,

        // .data0_io_num   = LCD_IO0,
        // .data1_io_num   = LCD_I1,
        // .data2_io_num   = -1,
        // .data3_io_num   = -1,
        // .sclk_io_num    = LCD_CLK,

    };

    spi_device_interface_config_t devcfg = {
        // .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
        // .mode=0,                                //SPI mode 0
        // .spics_io_num=PIN_NUM_CS,               //CS pin
        // .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        // .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line

        // .clock_speed_hz = 1*1000,
        // .clock_speed_hz = 1*1000*1000,
        .clock_speed_hz = SPI_MASTER_FREQ_40M,
        // .clock_speed_hz = SPI_MASTER_FREQ_80M,
        .mode           = 1,
        .spics_io_num   = LCD_CS,
        .queue_size     = 12,
        .flags          = SPI_DEVICE_HALFDUPLEX,


    };




    //Initialize the SPI bus
    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);


    //Attach the LCD to the SPI bus
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    _spi = spi;





    /* Reset display */
    gpio_set_direction(LCD_RST, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_RST, GPIO_PULLUP_ONLY);
    gpio_set_level(LCD_RST, 0);
    delay(10);
    gpio_set_level(LCD_RST, 1);
    delay(10);




    spi_device_acquire_bus(spi, portMAX_DELAY);


    



    // gpio_reset_pin(LCD_CS);
    // gpio_set_direction(LCD_CS, GPIO_MODE_OUTPUT);
    // gpio_set_pull_mode(LCD_CS, GPIO_PULLUP_ONLY);



    /* Sleep out */
    // SPI_CS0;
    display_send_cmd(spi, 0x11, false);
    // SPI_CS1;
    delay(120);

    // SPI_CS0;
    display_send_cmd(spi, 0x44, true);
    send_cmd(spi, 0x01, true);
    send_cmd(spi, 0x66, false);
    // SPI_CS1;


    /* TE ON */
    // SPI_CS0;
    display_send_cmd(spi, 0x35, true);
    send_cmd(spi, 0x00, false);
    // SPI_CS1;


    /* Interface Pixel Format 16bit/pixel */
    // SPI_CS0;
    display_send_cmd(spi, 0x3A, true);
    send_cmd(spi, 0x55, false);
    // SPI_CS1;


    // SPI_CS0;
    display_send_cmd(spi, 0x53, true);
    send_cmd(spi, 0x20, false);
    // SPI_CS1;
    delay(10);
    

    /* Write Display Brightness	MAX_VAL=0XFF */
    // SPI_CS0;
    display_send_cmd(spi, 0x51, true);
    send_cmd(spi, 0x00, false);
    // SPI_CS1;
    delay(10);


    /* Display ON */
    // SPI_CS0;
    display_send_cmd(spi, 0x29, false);
    // SPI_CS1;
    delay(10);


    /* Write Display Brightness	MAX_VAL=0XFF */
    // SPI_CS0;
    display_send_cmd(spi, 0x51, true);
    send_cmd(spi, 0xFF, false);
    // SPI_CS1;


    
    // spi_device_release_bus(spi);

    // delay(100);


    // spi_bus_remove_device(spi);
    // spi_bus_free(SPI2_HOST);


    #endif 



    lvgl_init();


    uint32_t color_list[] = {0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0x000000};
    int color_num = sizeof(color_list) / sizeof(uint32_t);

    uint32_t color_list2[] = {
        0xfff799, 0xffee6f, 0xecd452, 0xb6a014,
        0xd5ebe1, 0xb1d5c8, 0x99bcac, 0x80a492,
        0x8b7042, 0x775039, 0x5f4321, 0x422517,
        0xf3a694, 0xee7959, 0xba5140, 0xc12c1f
    };
    int color2_num = sizeof(color_list) / sizeof(uint32_t);

    uint32_t color_list3[] = {
        0x88abda, 0x6f94cd, 0x5976ba, 0x2e59a7,
        0x98b6c2, 0x7f9faf, 0x66889e, 0x547689,
        0xefefef, 0xd8d1c5, 0xc6beb1, 0xac9f8a,
        0xc0d09d, 0x938f4c, 0x7f754c, 0x595333
    };





    // while (1) {
    //     for (int i = 0; i < color_num; i++) {
    //         printf("COLOR: 0x%lX 0x%X\n", color_list[i], rgb888_2_rgb565(color_list[i]));
    //         // DM_Clear(rgb888_2_rgb565(color_list[i]));
    //         // display_push_color(spi, 0, 100, 0, 440, rgb888_2_rgb565(color_list[i]));
    //         display_push_color(spi, 0, COL - 1, 0, ROW - 1, rgb888_2_rgb565(color_list[i]));
    //         delay(800);
    //     }

    //     // for (int i = 0; i < 16; i++) {
    //     //     printf("COLOR: 0x%lX 0x%X\n", color_list2[i], rgb888_2_rgb565(color_list2[i]));
    //     //     // DM_display(0, 367, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list2[i]));
    //     //     display_push_color(spi, 200, 300, 200, 300, rgb888_2_rgb565(color_list2[i]));
    //     //     delay(800);
    //     // }   
        
    // }



    while (1)
    {   
        // #define COL 368
        // #define ROW 448
        for (int i = 0; i < 16; i++) {
            printf("COLOR: 0x%lX 0x%X\n", color_list2[i], rgb888_2_rgb565(color_list2[i]));
            // DM_display(0, 367, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list2[i]));
            display_push_color(spi, 0, COL-1, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list2[i]));

        }
        delay(500);
        // while (1);

        for (int i = 0; i < 16; i++) {
            printf("COLOR: 0x%lX 0x%X\n", color_list3[i], rgb888_2_rgb565(color_list3[i]));
            // DM_display(0, 367, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list3[i]));
            display_push_color(spi, 0, COL-1, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list3[i]));

        }
        delay(500);



        // DM_Clear(0);
        display_push_color(spi, 0, COL-1, 0, ROW-1, 0);
        uint32_t color = 0;
        for (int i = 0; i < 0xFF; i++) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            // DM_display(0, 367, i * 1, i * 1 + 2, rgb888_2_rgb565(color));
            display_push_color(spi, 0, COL-1, i * 1, i * 1 + 2, rgb888_2_rgb565(color));

        }
        for (int i = 255; i > 0; i--) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            // DM_display(0, 367, i * 1 + 256, i * 1 + 2 + 256, rgb888_2_rgb565(color));
            display_push_color(spi, 0, COL-1, i * 1 + 256, i * 1 + 2 + 256, rgb888_2_rgb565(color));

        }

        delay(500);

        // DM_Clear(0);
        display_push_color(spi, 0, COL-1, 0, ROW-1, 0);
        for (int i = 0; i < 0xFF; i++) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            // DM_display(i * 1, i * 1 + 2, 0, 800, rgb888_2_rgb565(color));
            display_push_color(spi, i * 1, i * 1 + 2, 0, 800, rgb888_2_rgb565(color));

        }
        for (int i = 255; i > 0; i--) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            // DM_display(i * 1 + 256, i * 1 + 2 + 256, 0, 800, rgb888_2_rgb565(color));
            display_push_color(spi, i * 1 + 256, i * 1 + 2 + 256, 0, 800, rgb888_2_rgb565(color));

        }

        delay(500);


        for (int i = 0; i < color_num; i++) {
            printf("COLOR: 0x%lX 0x%X\n", color_list[i], rgb888_2_rgb565(color_list[i]));
            // DM_Clear(rgb888_2_rgb565(color_list[i]));
            display_push_color(spi, 0, COL-1, 0, ROW-1, rgb888_2_rgb565(color_list[i]));
            delay(500);
        }

    }





}


