#include "serial_interface.h"
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#define TIMEOUT 5000

static uint64_t t_last;


static uint64_t get_time_usec();
static uint64_t get_time_msec();
static int serial_parse_char(uint8_t buf, rs485_rsp * mes);
static void *start_serial_interface_read_thread(void *args);
static void *start_gimbal_interface_write_thread(void *args);
static int combineBytesToInt(const uint8_t* bytes, size_t length);

Serial_Interface::Serial_Interface(Serial_Port *serial_port, ODR_Interface *sensor){
    _serial_port = serial_port;
    _sensor = sensor;
}

Serial_Interface::~Serial_Interface()
{
}

int Serial_Interface::write_message(const rs485_message &message)
{
    if (_serial_port == nullptr) {
        printf("ERROR: serial port not exist\n");
        throw 1;
    }

    return _serial_port->write_message(message);
}

void Serial_Interface::start()
{
    int result = 0;

    // --------------------------------------------------------------------------
    //   CHECK SERIAL PORT
    // --------------------------------------------------------------------------
    if (_serial_port != nullptr) {
        if (_serial_port->status != SERIAL_PORT_OPEN) { // SERIAL_PORT_OPEN
            printf("ERROR: serial port not open\n");
            throw 1;
        }

    } else {
        printf("ERROR: serial port not exist\n");
        throw 1;
    }
    // --------------------------------------------------------------------------
    //   READ THREAD
    // --------------------------------------------------------------------------
    printf("START READ THREAD \n");
    result = pthread_create(&read_tid, NULL, &start_serial_interface_read_thread, this);

    if (result) throw result;
    // --------------------------------------------------------------------------
    //   WRITE THREAD
    // --------------------------------------------------------------------------
    printf("START WRITE THREAD \n");
    result = pthread_create(&write_tid, NULL, &start_gimbal_interface_write_thread, this);

    if (result) throw result;
}

void Serial_Interface::stop()
{
    // --------------------------------------------------------------------------
    //   CLOSE THREADS
    // --------------------------------------------------------------------------
    printf("CLOSE THREADS\n");
    // signal exit
    time_to_exit = true;
    // wait for exit
    pthread_join(read_tid, NULL);
    pthread_join(write_tid, NULL);
    // now the read and write threads are closed
    // still need to close the _serial_port separately
}


// ------------------------------------------------------------------------------
//   Read Thread
// ------------------------------------------------------------------------------
void Serial_Interface::start_read_thread()
{
    if (reading_status != THREAD_NOT_INIT) {
        printf("Read thread already running\n");

    } else {
        read_thread();
    }
}

void Serial_Interface::start_write_thread(void)
{
    if (writing_status != THREAD_NOT_INIT) {
        printf("Write thread already running\n");

    } else {
        write_thread();
    }
}

// ------------------------------------------------------------------------------
//   Quit Handler
// ------------------------------------------------------------------------------
void Serial_Interface::handle_quit( int sig )
{
    try {
        stop();

    } catch (int error) {
        printf("Warning, could not stop serial interface\n");
    }
}

bool Serial_Interface::get_flag_exit(void){
    return time_to_exit;
}
// ------------------------------------------------------------------------------
//   Read Thread
// ------------------------------------------------------------------------------
void Serial_Interface::read_thread(void)
{
    rs485_rsp message     = { 0 };

    if (_serial_port == nullptr) {
        printf("ERROR: serial port not exist\n");
        throw 1;
    }

    while (!time_to_exit) {
        if (writing_status != THREAD_RUNNING) {
            reading_status = THREAD_RUNNING;
            // ----------------------------------------------------------------------
            //   READ MESSAGE
            // ----------------------------------------------------------------------
            char buf[256] = { 0 };
            char raw_data[256] = {0};
            int result = _serial_port->read_message(buf, 256);

            // --------------------------------------------------------------------------
            //   PARSE MESSAGE
            // --------------------------------------------------------------------------
            if (result > 0) {
                for (int i = 0; i < result; ++i) {
                    if (serial_parse_char((uint8_t)buf[i], &message)){
                        messages_handler(message);
                    }
                }
            }

            reading_status = THREAD_IDLING;
        }

        usleep(100);   // sleep 1000us
    } // end: while not received all
}
void Serial_Interface::messages_handler(const rs485_rsp &message)
{
    if (crc_modbus(&message) != message.checksum) 
    {
        DebugError("ERROR: checksum error!\r\n");
        writing_status == THREAD_IDLING;
        return;
    }
    pthread_mutex_lock(&_messages.mutex);
    int value = combineBytesToInt(message.data, message.len);
    _sensor->setValue(_sensor_add,value);
    _sensor_add++;
    pthread_mutex_unlock(&_messages.mutex);
    writing_status = THREAD_IDLING;
}
void Serial_Interface::write_thread(void)
{
    uint64_t tnow_ms = 0;
    uint64_t time_send_param_ms = 0;

    // Blocking wait for new data
    while (!time_to_exit) {
        tnow_ms = get_time_msec();
        if (tnow_ms - time_send_param_ms >= 5000)
        {
            DebugError("ERROR: Time out get data from %u",_sensor_add);
            writing_status = THREAD_IDLING;
        }
        
        if (tnow_ms - time_send_param_ms >= 2513 && writing_status == THREAD_IDLING) {
            writing_status = THREAD_RUNNING;
            time_send_param_ms = get_time_msec();
            pthread_mutex_lock(&_messages.mutex);
            if (_sensor_add >= ODR_Interface::MAX_ADD)
            {
                _sensor_add = ODR_Interface::MIN_ADD;
            }
            uint8_t data[50] = {0};
            uint16_t len =  _sensor->get_msg_encode(data,_sensor_add);
            _serial_port->write_message(data,len);
            pthread_mutex_unlock(&_messages.mutex);
        }
        
        usleep(100);
    }
}

// ------------------------------------------------------------------------------
//  Pthread Starter Helper Functions
// ------------------------------------------------------------------------------
void *start_serial_interface_read_thread(void *args)
{
    // takes an gimbal object argument
    Serial_Interface *serial_interface = (Serial_Interface *)args;
    // run the object's read thread
    serial_interface->start_read_thread();
    // done!
    return NULL;
    
}
void *start_gimbal_interface_write_thread(void *args)
{
    // takes an gimbal object argument
    Serial_Interface *serial_interface = (Serial_Interface *)args;
    // run the object's read thread
    serial_interface->start_write_thread();
    // done!
    return NULL;
}
// ----------------------------------------------------------------------------------
//   Time
// ------------------- ---------------------------------------------------------------
static uint64_t get_time_usec()
{
    static struct timeval _timestamp;
    gettimeofday(&_timestamp, NULL);
    return _timestamp.tv_sec * 1000000 + _timestamp.tv_usec;
}

static uint64_t get_time_msec()
{
    static struct timeval _timestamp;
    gettimeofday(&_timestamp, NULL);
    return _timestamp.tv_sec * 1000 + _timestamp.tv_usec / 1000;
}

int combineBytesToInt(const uint8_t* bytes, size_t length) {
    int combinedValue = 0;
    for (size_t i = 0; i < length; ++i) {
        combinedValue |= static_cast<int>(bytes[i]) << (8 * (length - 1 - i));
    }
    return combinedValue;
}

int serial_parse_char(uint8_t buf, rs485_rsp * mes){
    static int state = 0;
    static bool flag_full_rsp = false;
    uint64_t t_now = get_time_msec();

    if((t_now - t_last) > 100){
        state = 0;
    }
    switch (state)
    {
        case 0:
        {
            mes->slave_adr = buf;
            state = 1;
        }
            break;
        case 1:
        {
            mes->func_code = buf;
            state = 2;
        }
            break;
        case 2:
        {
            mes->effective = buf;
            mes->len = 0;
            state = 3;
        }
            break;
        case 3:
        {
            mes->data[mes->len++] = buf;
            if(mes->len == 2){
                state = 4;
            }
        }
            break;
        case 4:
        {
            mes -> checksum = 0;
            mes->checksum |= buf;
            state = 5;  
        }
            break;
        case 5:
        {
            mes->checksum |= ((uint16_t)buf << 8);
            state = 0;
            flag_full_rsp = true;
        }
        default:
            break;
    }

    t_last = t_now;

    if(flag_full_rsp){
        flag_full_rsp = 0;
        return true;
    }
    return false;
}