import spidev, time

spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz = 1350000

def analo_read(channel):
	r = spi.xfer2([1, (8+channel) << 4, 0])
	adc_out = ((r[1]&3) << 8) + r[2]
	return adc_out

while True:
	reading = analo_read(0)
	reading8 = analo_read(7)
	voltage = (reading *3.3)/1024
	voltage8 = (reading8 *3.3)/1024
	print("Reading[0] = %d\tVoltage[0] = %f\tReading[7] = %d\tVoltage[7] = %f" % (reading,voltage, reading8, voltage8))
	f=open("test.txt", 'a')
	f.write(str(reading) + ' ' + str(reading8) + '\n')
	time.sleep(0.01)
	f.close()
