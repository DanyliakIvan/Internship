#include "oled.h"



fontDef font_6x8 = {6,8,font6x8};


// Send a byte to the command register    
void oledWriteCommand(uint8_t byte) 
{
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, 0x00, 1, &byte, 1, HAL_MAX_DELAY);
}

// Send data
void oledWriteData(uint8_t* buffer, size_t buff_size) 
{
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY);
}

// Init
void oledInit(void) 
{
    oledSetDisplayOn(0); //display off

    oledWriteCommand(0x20); //Set Memory Addressing Mode
    oledWriteCommand(0x00); // 00b,Horizontal Addressing Mode; 
                            // 01b,Vertical Addressing Mode;
                            // 10b,Page Addressing Mode (RESET); 
                            // 11b,Invalid
    oledWriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
    oledWriteCommand(0xC8); //Set COM Output Scan Direction
    oledWriteCommand(0x00); //---set low column address
    oledWriteCommand(0x10); //---set high column address
    oledWriteCommand(0x40); //--set start line address - CHECK

    oledSetContrast(0xFF);

    oledWriteCommand(0xA1); //--set segment re-map 0 to 127 - CHECK
    oledWriteCommand(0xA6); //--set normal color
    oledWriteCommand(0xFF);
    oledWriteCommand(0x3F); 

    oledWriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    oledWriteCommand(0xD3); //-set display offset - CHECK
    oledWriteCommand(0x00); //-not offset
    oledWriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
    oledWriteCommand(0xF0); //--set divide ratio
    oledWriteCommand(0xD9); //--set pre-charge period
    oledWriteCommand(0x22); //
    oledWriteCommand(0xDA); //--set com pins hardware configuration - CHECK
    oledWriteCommand(0x12);
    oledWriteCommand(0xDB); //--set vcomh

    oledWriteCommand(0x20); //0x20,0.77xVcc
    oledWriteCommand(0x8D); //--set DC-DC enable
    oledWriteCommand(0x14); //

    oledSetDisplayOn(1);    //--turn on SSD1306 panel

    // Clear screen
    oledFill(black);

    // Flush buffer to screen
    oledUpdateScreen();

    // Set default values for screen object
    OLED.currentX = 0;
    OLED.currentY = 0;
    OLED.initialized = 1;
    
}

void oledFill(OLED_COLOR color) 
{
    for (uint32_t i = 0; i < sizeof(oledBuffer); i++) 
    {
        oledBuffer[i] = (color == black) ? 0x00 : 0xFF;
    }
}

/* Write the screenbuffer with changed to the screen */
void oledUpdateScreen(void) 
{
    for (uint8_t i = 0; i < 64/8; i++) 
        {
        oledWriteCommand(0xB0 + i); // Set the current RAM page address.
        oledWriteCommand(0x00 + 0);
        oledWriteCommand(0x10 + 0);
        oledWriteData(&oledBuffer[OLED_WIDTH*i],OLED_WIDTH);
    }
}


/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * color => Pixel color
 */
void oledDrawPixel(uint8_t x, uint8_t y, OLED_COLOR color)
{
    if (y >= OLED_WIDTH || x >= OLED_WIDTH) 
    {
        // Don't write outside the buffer
        return;
    }

    // Draw in the right color
    if (color == white) 
    {
        oledBuffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
    } 
    else 
    { 
        oledBuffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}



/*
 * Draw 1 char to the screen buffer
 * ch       => char om weg te schrijven
 * Font     => Font waarmee we gaan schrijven
 * color    => Black or White
 */
char oledWriteChar(char ch, OLED_COLOR color)
{
    fontDef font = font_6x8;
    uint32_t i, b, j;
    
    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;
    
    // Check remaining space on current line
    if (OLED_HEIGHT < (OLED.currentY + font.fontHeight))
    {
        // Not enough space on current line
        return 0;
    }
    else if (OLED_WIDTH < (OLED.currentX + font.fontWidth) && OLED_HEIGHT >= (OLED.currentY + font.fontHeight))
    {
        OLED.currentX = 0;
        OLED.currentY += font.fontHeight;
    }
    
    // Use the font to write
    for(i = 0; i < font.fontHeight; i++) 
    {
        b = font.data[(ch - 32) * font.fontHeight + i];
        for(j = 0; j < font.fontWidth; j++) 
        {
            if((b << j) & 0x8000)  
            {
                oledDrawPixel(OLED.currentX + j, (OLED.currentY + i), (OLED_COLOR) color);
            } 
            else 
            {
                oledDrawPixel(OLED.currentX + j, (OLED.currentY + i), (OLED_COLOR)!color);
            }
        }
    }
    
    // The current space is now taken
    OLED.currentX += font.fontWidth;
    
    // Return written char for validation
    return ch;
}

/* Write full string to screenbuffer */
char oledWriteString(char* str, OLED_COLOR color)
{
    
    while (*str) 
    {
        if (oledWriteChar(*str, color) != *str)        
        {
            // Char could not be written
            return *str;
        }
        str++;
    }
    
    // Everything ok
    return *str;
}

/* Position the cursor */
void oledSetCursor(uint8_t x, uint8_t y)
{
    OLED.currentX = x;
    OLED.currentY = y;
}

void oledSetContrast(const uint8_t value) 
{
    const uint8_t kSetContrastControlRegister = 0x81;
    oledWriteCommand(kSetContrastControlRegister);
    oledWriteCommand(value);
}

void oledSetDisplayOn(const uint8_t on) 
{
    uint8_t value;
    if (on)
    {
        value = 0xAF;   // Display on
        OLED.displayOn = 1;
    } 
    else
    {
        value = 0xAE;   // Display off
        OLED.displayOn = 0;
    }
    oledWriteCommand(value);
}
