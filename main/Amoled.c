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




void app_main()
{

    gpio_set_direction(LCD_RST, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_RST, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_CS, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_I2, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_I2, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_I3, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_I3, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_CLK, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_CLK, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_I1, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_I1, GPIO_PULLUP_ONLY);

    gpio_set_direction(LCD_IO0, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LCD_IO0, GPIO_PULLUP_ONLY);




    gpio_set_level(LCD_RST, 0);
    delay(10);
    gpio_set_level(LCD_RST, 1);





    DO0180FS01_Init();


    // DM_Clear(0xFFFF);


    uint32_t color_list[] = {0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF};
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


    while (1)
    {   
        

        // #define COL 368
        // #define ROW 448
        for (int i = 0; i < 16; i++) {
            printf("COLOR: 0x%lX 0x%X\n", color_list2[i], rgb888_2_rgb565(color_list2[i]));
            DM_display(0, 367, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list2[i]));

        }
        delay(1000);

        for (int i = 0; i < 16; i++) {
            printf("COLOR: 0x%lX 0x%X\n", color_list3[i], rgb888_2_rgb565(color_list3[i]));
            DM_display(0, 367, i * 27, i * 28 + 28, rgb888_2_rgb565(color_list3[i]));

        }
        delay(1000);



        DM_Clear(0);
        uint32_t color = 0;
        for (int i = 0; i < 0xFF; i++) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            DM_display(0, 367, i * 1, i * 1 + 2, rgb888_2_rgb565(color));

        }
        for (int i = 255; i > 0; i--) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            DM_display(0, 367, i * 1 + 256, i * 1 + 2 + 256, rgb888_2_rgb565(color));

        }

        delay(1000);

        DM_Clear(0);
        for (int i = 0; i < 0xFF; i++) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            DM_display(i * 1, i * 1 + 2, 0, 800, rgb888_2_rgb565(color));

        }
        for (int i = 255; i > 0; i--) {
            color = (i << 16) | (i << 8) | i;
            printf("COLOR: 0x%lX 0x%X\n", color, rgb888_2_rgb565(color));
            DM_display(i * 1 + 256, i * 1 + 2 + 256, 0, 800, rgb888_2_rgb565(color));

        }

        delay(1000);


        // for (int i = 0; i < color_num; i++) {
        //     printf("COLOR: 0x%lX 0x%X\n", color_list[i], rgb888_2_rgb565(color_list[i]));
        //     DM_Clear(rgb888_2_rgb565(color_list[i]));
        //     delay(1000);
        // }

    }








    while (1)
    {
        printf("6\n");

        delay(5000);


    }
    
}





// static uint8_t _cmd[] = {0x02, 0x00, 0x00, 0x00};

// void lcd_write_cmd(spi_device_handle_t spi, uint8_t cmd)
// {
//     spi_device_acquire_bus(spi, portMAX_DELAY);
//     spi_transaction_t t;
//     memset(&t, 0, sizeof(t));

//     // static uint8_t _data[] = {0x02, 0x00, 0x00, 0x00};
//     _cmd[2] = cmd;

//     t.length = 32;
//     t.tx_buffer = _cmd;
    
//     ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &t));

//     spi_device_release_bus(spi);
// }


// static uint8_t _data = 0x00;

// void lcd_write_data(spi_device_handle_t spi, uint8_t data)
// {
//     spi_device_acquire_bus(spi, portMAX_DELAY);
//     spi_transaction_t t;
//     memset(&t, 0, sizeof(t));

//     _data = data;

//     t.length = 8;
//     t.tx_buffer = &_data;
    
//     ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &t));

//     spi_device_release_bus(spi);
// }



// void qspi_init()
// {
//     gpio_set_direction(LCD_RST, GPIO_MODE_OUTPUT);
//     gpio_set_pull_mode(LCD_RST, GPIO_PULLUP_ONLY);
//     gpio_set_direction(LCD_CS, GPIO_MODE_OUTPUT);
//     gpio_set_pull_mode(LCD_CS, GPIO_PULLUP_ONLY);


//     spi_device_handle_t spi;

//     spi_bus_config_t buscfg={
//         .data0_io_num       = LCD_IO0,
//         .data1_io_num       = LCD_I1,
//         .sclk_io_num        = LCD_CLK,
//         .data2_io_num       = LCD_I2,
//         .data3_io_num       = LCD_I3,
//         // .flags              = SPICOMMON_BUSFLAG_QUAD,
//         // .max_transfer_sz    = PARALLEL_LINES*320*2+8
//     };
//     // spi_bus_config_t buscfg;
//     // buscfg.data0_io_num       = LCD_IO0;
//     // buscfg.data1_io_num       = LCD_I1;
//     // buscfg.sclk_io_num        = LCD_CLK;
//     // buscfg.data2_io_num       = LCD_I2;
//     // buscfg.data3_io_num       = LCD_I3;
//     // buscfg.intr_flags         = ESP_INTR_FLAG_IRAM;

//     spi_device_interface_config_t devcfg={
//         .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
//         .mode=0,                                //SPI mode 0
//         // .spics_io_num       = LCD_CS,           //CS pin
//         .spics_io_num       = -1,           //CS pin
//         .queue_size=7,                          //We want to be able to queue 7 transactions at a time
//         // .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
//     };
//     // spi_device_interface_config_t devcfg;
//     // devcfg.clock_speed_hz=10*1000*1000;
//     // devcfg.mode=0;
//     // devcfg.spics_io_num       = -1;
//     // devcfg.queue_size=7;


//     ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
//     ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));







    
    
//     gpio_set_level(LCD_RST, 0);
//     delay(100);
//     gpio_set_level(LCD_RST, 1);
//     delay(100);



//     /* Sleep out */
//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x11);
//     LCD_CS_DIS();

//     delay(120);


//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x44);
//     lcd_write_data(spi, 0x01);
//     lcd_write_data(spi, 0x66);
//     LCD_CS_DIS();


//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x35);
//     lcd_write_data(spi, 0x00);
//     LCD_CS_DIS();


//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x3A);
//     lcd_write_data(spi, 0x55);
//     LCD_CS_DIS();


//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x53);
//     lcd_write_data(spi, 0x20);
//     LCD_CS_DIS();

//     delay(10);


//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x51);
//     lcd_write_data(spi, 0x00);
//     LCD_CS_DIS();

//     delay(10);

//     /* Display on */
//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x29);
//     LCD_CS_DIS();

//     delay(10);



//     LCD_CS_EN();
//     lcd_write_cmd(spi, 0x51);
//     lcd_write_data(spi, 0xFF);
//     LCD_CS_DIS();



// }



// void app_main()
// {
//     // qspi_init();


//     while (1)
//     {
//         printf("6\n");

//         delay(5000);


//     }
    
// }


