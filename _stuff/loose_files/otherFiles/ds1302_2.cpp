#include "DS1302.h"


DS1302::DS1302()
{
	//
}


void DS1302::_DS1302_start(void)
{
	digitalWrite(DS1302_CE_PIN, LOW);
	pinMode(DS1302_CE_PIN, OUTPUT);
	digitalWrite(DS1302_SCLK_PIN, LOW);
	pinMode(DS1302_SCLK_PIN, OUTPUT);
	pinMode(DS1302_IO_PIN, OUTPUT);
	digitalWrite(DS1302_CE_PIN, HIGH);
	delayMicroseconds(4);
}


void DS1302::_DS1302_stop(void)
{
	digitalWrite(DS1302_CE_PIN, LOW);
	delayMicroseconds(4);
}


uint8_t _DS1302_toggleread(void)
{
	uint8_t i, data;
	data = 0;
	for(i = 0; i <= 7; i++) {
		digitalWrite(DS1302_SCLK_PIN, HIGH);
		delayMicroseconds(1);
		digitalWrite(DS1302_SCLK_PIN, LOW);
		delayMicroseconds(1);
		bitWrite(data, i, digitalRead(DS1302_IO_PIN));
	}

	return(data);
}


void DS1302::_DS1302_togglewrite(uint8_t data, uint8_t release)
{
	for(int i = 0; i <= 7; i++) {
		digitalWrite(DS1302_IO_PIN, bitRead(data, i));
		delayMicroseconds(1);
		digitalWrite(DS1302_SCLK_PIN, HIGH);
		delayMicroseconds(1);
		if(release && i == 7) {
			pinMode(DS1302_IO_PIN, INPUT);

#if ARDUINO < 10003
			// For Arduino 1.0.3, removing the pull-up is no longer needed.
			// Setting the pin as 'INPUT' will already remove the pull-up.
			digitalWrite(DS1302_IO, LOW);
#endif
		} else {
			digitalWrite(DS1302_SCLK_PIN, LOW);
			delayMicroseconds(1);
		}
	}
}


uint8_t DS1302::DS1302_read(int address)
{
	uint8_t data;
	bitSet(address, DS1302_READBIT);
	_DS1302_start();
	_DS1302_togglewrite(address, true);
	data = _DS1302_toggleread();
	_DS1302_stop();

	return(data);
}


void DS1302::DS1302_write(int address, uint8_t data)
{
	bitClear(address, DS1302_READBIT);
	_DS1302_start();
	_DS1302_togglewrite(address, false);
	_DS1302_togglewrite(data, false);
	_DS1302_stop();
}


void DS1302::DS1302_clock_burst_read(uint8_t *p)
{
	_DS1302_start();
	_DS1302_togglewrite(DS1302_CLOCK_BURST_READ, true);
	for(int i = 0; i < 8; i++) {
		*p++ = _DS1302_toggleread();
	}
	_DS1302_stop();
}


void DS1302::DS1302_clock_burst_write(uint8_t *p)
{
	_DS1302_start();
	_DS1302_togglewrite(DS1302_CLOCK_BURST_WRITE, false);
	for(int i = 0; i < 8; i++) {
		_DS1302_togglewrite(*p++, false);
	}
	_DS1302_stop();
}



void DS1302::init(int seconds, int minutes, int hours, int dayOfWeek, int dayOfMonth, int month, int year, bool format,
		bool morning)
{
	DS1302_write(DS1302_ENABLE, 0);
	DS1302_write(DS1302_TRICKLE, 0x00);
	memset((char *) &dateTime, 0, sizeof(dateTime));
	dateTime.Seconds    = bin2bcd_l(seconds);
	dateTime.Seconds10  = bin2bcd_h(seconds);
	dateTime.CH         = 0;
	dateTime.Minutes    = bin2bcd_l(minutes);
	dateTime.Minutes10  = bin2bcd_h(minutes);

	if(format == H24) {
		dateTime.h24.Hour   = bin2bcd_l(hours);
		dateTime.h24.Hour10 = bin2bcd_h(hours);
		dateTime.h24.hour_12_24 = 0;
	} else {
		dateTime.h12.Hour   = bin2bcd_l(hours);
		dateTime.h12.Hour10 = bin2bcd_h(hours);
		dateTime.h12.AM_PM  = morning;
		dateTime.h12.hour_12_24 = 1;
	}

	dateTime.Date       = bin2bcd_l(dayOfMonth);
	dateTime.Date10     = bin2bcd_h(dayOfMonth);
	dateTime.Month      = bin2bcd_l(month);
	dateTime.Month10    = bin2bcd_h(month);
	dateTime.Day        = dayOfWeek;
	dateTime.Year       = bin2bcd_l(year - 2000);
	dateTime.Year10     = bin2bcd_h(year - 2000);
	dateTime.WP = 0;
	DS1302_clock_burst_write((uint8_t *) &dateTime);
}


void DS1302::read(void)
{
	DS1302_clock_burst_read((uint8_t *) &dateTime);
}


void DS1302::print(void)
{
	char buffer[128];
	sprintf(buffer, "Time = %02d:%02d:%02d, ",
			bcd2bin(dateTime.h24.Hour10, dateTime.h24.Hour),
			bcd2bin(dateTime.Minutes10, dateTime.Minutes),
			bcd2bin(dateTime.Seconds10, dateTime.Seconds));
	Serial.print(buffer);

	sprintf(buffer, "Date(day of month) = %d, Month = %d, Day(day of week) = %d, Year = %d",
			bcd2bin(dateTime.Date10, dateTime.Date),
			bcd2bin(dateTime.Month10, dateTime.Month),
			dateTime.Day,
			2000 + bcd2bin(dateTime.Year10, dateTime.Year));
	Serial.println(buffer);
}
