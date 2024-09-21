#ifndef SERIAL_INTERFACE_H_
#define SERIAL_INTERFACE_H_

#include "serial_port.h"
#include "rs485.h"
#include "ODR_01.h"


#define DEBUG 1

#if (DEBUG == 1)
# define DebugMsg(fmt, args ...)           do {printf("\33[39m" fmt "\033[0m\n\r",  ## args); } while(0);
# define DebugInfo(fmt, args ...)          do {printf("\33[34m" fmt "\033[0m\n\r", ## args); } while(0);
# define DebugWarning(fmt, args ...)       do {printf("\33[33m" fmt "\033[0m\n\r", ## args); } while(0);
# define DebugError(fmt, args ...)         do {printf("\33[91m" fmt "\033[0m\n\r", ## args); } while(0);
# define DebugSuccess(fmt, args ...)       do {printf("\33[92m" fmt "\033[0m\n\r", ## args); } while(0);
#else
# define DebugMsg(fmt, args ...)
# define DebugInfo(fmt, args ...)
# define DebugWarning(fmt, args ...)
# define DebugError(fmt, args ...)
# define DebugSuccess(fmt, args ...)
#endif


class Serial_Interface
{
public:
    Serial_Interface() = delete;
    Serial_Interface(Serial_Port *serial_port, ODR_Interface *sensor);

    void start();
    void stop();

    void handle_quit(int sig);
    
    int write_message(const rs485_message &message);

    bool get_flag_exit(void);

    // bool get_connection(void);

    // bool present();

    void start_read_thread();
    void start_write_thread();
    ~Serial_Interface();
private:
    /**
     * @brief messages used in interface
     *
     */
    struct messages_t {
        pthread_mutex_t mutex;

        messages_t()
        {
            pthread_mutex_init(&mutex, NULL);
        }

        ~messages_t()
        {
            pthread_mutex_destroy(&mutex);
        }

    };

    enum serial_thread_status_t {
        THREAD_NOT_INIT = 0,
        THREAD_RUNNING,
        THREAD_PROCESS,
        THREAD_IDLING,        
    };

    Serial_Port *_serial_port;
    ODR_Interface *_sensor;
    volatile serial_thread_status_t reading_status = THREAD_NOT_INIT;
    volatile serial_thread_status_t writing_status = THREAD_NOT_INIT;

    void messages_handler(const rs485_rsp &message);

    void read_thread(void);
    void write_thread(void);

    uint16_t _sensor_add = ODR_Interface :: MIN_ADD;

    bool time_to_exit = false;
    bool has_detected = false;

    pthread_t read_tid  = 0;
    pthread_t write_tid  = 0;

    messages_t _messages;
    /* data */
};


#endif // SERIAL_INTERFACE_H_