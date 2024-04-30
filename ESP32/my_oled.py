from machine import Pin, SoftI2C
import ssd1306
import gfx
from time import sleep

i2c = SoftI2C(scl=Pin(22), sda=Pin(21))

oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)
graphics = gfx.GFX(oled_width, oled_height, oled.pixel)

def print_data(msg):
    # convert byte array to string
    my_string = msg.decode('utf-8')
    # split string by spaces
    my_strings = my_string.split(" ")
    # convert string to float
    my_values = [float(item) for item in my_strings]

    # clear screen
    oled.fill(0)

    # iterate through values
    for ii,item in enumerate(my_values):
        # print string on new line
        oled.text(str(item), 0, 10*ii)

    # show screen
    oled.show()
    

def plot_data(msg):
    # convert byte array to string
    my_string = msg.decode('utf-8')
    # split string by spaces
    my_strings = my_string.split(" ")
    # convert string to float
    my_values = [float(item) for item in my_strings]

    # clear screen
    oled.fill(0)
    
    # calculate graph size
    graph_size = oled_width / (len(my_values) - 1)
    
    # find maximum and minimum values
    maxy = max(my_values)
    miny = min(my_values)
    
    # iterate through values
    for ii in range(len(my_values) - 1):
        x0 = int(ii * graph_size)
        x1 = int((ii + 1) * graph_size)
        y0 = int((my_values[ii] - miny) / (maxy - miny) * (oled_height - 1))
        y1 = int((my_values[ii + 1] - miny) / (maxy - miny) * (oled_height - 1))
        graphics.line(x0, oled_height - 1 - y0, x1, oled_height - 1 - y1, 1)


    # Display max and min values at the top and bottom of the screen
    oled.text("Max: {:.2f}".format(maxy), 0, 0)
    oled.text("Min: {:.2f}".format(miny), 0, oled_height - 8)
    
    # show screen
    oled.show()
