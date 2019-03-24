from pyBusPirateLite.SPI import SPI

spi = SPI(portname='COM7')
spi.pins = SPI.PIN_POWER | SPI.PIN_CS
spi.config = SPI.CFG_PUSH_PULL
spi.speed = '1MHz'

# send two bytes and receive answer
spi.cs = True
data = spi.transfer([0x82, 0x83, 0x84, 0x85, 0x86])
spi.cs = False

# Make this a loop so we can transfer data back and forth
