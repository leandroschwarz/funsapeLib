/*CODED by Qiyong Mu on 21/06/2014
kylongmu@msn.com
*/
#include "mbed.h"
#include "MPU9250.h"        //Include library
 
DigitalOut myled(LED1);
Serial pc(SERIAL_TX, SERIAL_RX);
SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);
mpu9250_spi imu(spi,SPI_CS);   //define the mpu9250 object
int main(){
    pc.baud(115200);
    if(imu.init(1,BITS_DLPF_CFG_188HZ)){  //INIT the mpu9250
        printf("\nCouldn't initialize MPU9250 via SPI!");
    }    
    printf("\nWHOAMI=0x%2x\n",imu.whoami()); //output the I2C address to know if SPI is working, it should be 104
    wait(1);    
    printf("Gyro_scale=%u\n",imu.set_gyro_scale(BITS_FS_2000DPS));    //Set full scale range for gyros
    wait(1);  
    printf("Acc_scale=%u\n",imu.set_acc_scale(BITS_FS_16G));          //Set full scale range for accs
    wait(1);
    printf("AK8963 WHIAM=0x%2x\n",imu.AK8963_whoami());
    wait(0.1);  
    imu.AK8963_calib_Magnetometer();
    while(1) {
        //myled = 1;
        wait(0.1);
        /*
        imu.read_temp();
        imu.read_acc();
        imu.read_rot();
        imu.AK8963_read_Magnetometer();
        */
        imu.read_all();
        printf("%10.3f,%10.3f,%10.3f,%10.3f,%10.3f,%10.3f,%10.3f,%10.3f,%10.3f,%10.3f\n", 
            imu.Temperature,
            imu.gyroscope_data[0],
            imu.gyroscope_data[1],
            imu.gyroscope_data[2],
            imu.accelerometer_data[0],
            imu.accelerometer_data[1],
            imu.accelerometer_data[2],
            imu.Magnetometer[0],
            imu.Magnetometer[1],
            imu.Magnetometer[2]
            );
        //myled = 0;
        //wait(0.5);
    }
}
 