#include "auto_connect.h"
#include <time.h>
#include <sys/time.h>

static uint64_t get_time_msec()
{
    static struct timeval _timestamp;
    gettimeofday(&_timestamp, NULL);
    return _timestamp.tv_sec * 1000 + _timestamp.tv_usec / 1000;
}
vector<string> AutoConnect::list_uart_ports() {
    vector<string> uart_ports;
    DIR* dir = opendir("/dev/");
    if (dir == nullptr) {
        cerr << "Unable to open /dev/ directory." << endl;
        return uart_ports;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string port_name = entry->d_name;
        if (port_name.find("ttyS") != string::npos || port_name.find("ttyUSB") != string::npos) {
            uart_ports.push_back("/dev/" + port_name);
        }
    }

    closedir(dir);
    return uart_ports;
}

// Cấu hình cổng UART với baudrate cho trước
int AutoConnect::configure_uart(const std::string& port, int baud) {
    int fd = -1;
    // Open serial port
    // O_RDWR - Read and write
    // O_NOCTTY - Ignore special chars like CTRL-C
    fd = open(port.c_str(), O_RDWR | O_NONBLOCK | O_CLOEXEC | O_NOCTTY);

    // Check for Errors
    if (fd == -1) {
        /* Could not open the port. */
        return (-1);
    }

    // Finalize
    else {
        fcntl(fd, F_SETFL, 0);
    }
        // Check for Errors
    if (fd == -1) {
        /* Could not open the port. */
        return (-1);
    }
    // Check file descriptor
    if (!isatty(fd)) {
        fprintf(stderr, "\nERROR: file descriptor %d is NOT a serial port\n", fd);
        return -1;
    }

    // Read file descritor configuration
    struct termios config;

    if (tcgetattr(fd, &config) < 0) {
        fprintf(stderr, "\nERROR: could not read configuration of fd %d\n", fd);
        return -1;
    }

    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                        INLCR | PARMRK | INPCK | ISTRIP | IXON);
    // Output flags - Turn off output processing
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
                        ONOCR | OFILL | OPOST);
    #ifdef OLCUC
    config.c_oflag &= ~OLCUC;
    #endif
    #ifdef ONOEOT
    config.c_oflag &= ~ONOEOT;
    #endif
    // No line processing:
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    // Turn off character processing
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input

    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;
    
    // Set up stop bits

    config.c_cflag &= ~CSTOPB; // Set 1 stop bit (default)
    
    // One input byte is enough to return from read()
    // Inter-character timer off
    config.c_cc[VMIN]  = 0;  // Cho phép đọc ngay cả khi không có byte
    config.c_cc[VTIME] = 0;  // Không chờ đợi timeout


    // Get the current options for the port
    ////struct termios options;
    ////tcgetattr(fd, &options);

    // Apply baudrate
    switch (baud) {
        case 1200:
            if (cfsetispeed(&config, B1200) < 0 || cfsetospeed(&config, B1200) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        case 1800:
            cfsetispeed(&config, B1800);
            cfsetospeed(&config, B1800);
            break;

        case 9600:
            cfsetispeed(&config, B9600);
            cfsetospeed(&config, B9600);
            break;

        case 19200:
            cfsetispeed(&config, B19200);
            cfsetospeed(&config, B19200);
            break;

        case 38400:
            if (cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        case 57600:
            if (cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        case 115200:
            if (cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        // These two non-standard (by the 70'ties ) rates are fully supported on
        // current Debian and Mac OS versions (tested since 2010).
        case 460800:
            if (cfsetispeed(&config, B460800) < 0 || cfsetospeed(&config, B460800) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        case 921600:
            if (cfsetispeed(&config, B921600) < 0 || cfsetospeed(&config, B921600) < 0) {
                fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
                return -1;
            }

            break;

        default:
            fprintf(stderr, "ERROR: Desired baud rate %d could not be set, aborting.\n", baud);
            return -1;
            break;
    }

    // Finally, apply the configuration
    if (tcsetattr(fd, TCSAFLUSH, &config) < 0) {
        fprintf(stderr, "ERROR: could not set configuration of fd %d\n", fd);
        return -1;
    }

    if (fd <= 0) {
        printf("Connection attempt to port %s with %d baud, 8N1 failed, exiting.\n",port.c_str(), baud);
        return -1;
    }

    int RTS_flag;
    int DTR_flag;
    RTS_flag = TIOCM_RTS;
    DTR_flag = TIOCM_DTR;
    ioctl(fd, TIOCMBIS, &DTR_flag); //Set DTR pin
    ioctl(fd, TIOCMBIC, &RTS_flag); //clear RTS pin
    //getchar();
    usleep(100000);
    ioctl(fd, TIOCMBIC, &DTR_flag); //Clear DTR pin

    return fd;
}

void AutoConnect::send_request_data(int uart_fd) {
    while (!stop_flag.load()) {
        uint8_t buf[50] = {0};
        uint16_t len =  _sensor.get_msg_encode(buf,ODR_Interface :: MIN_ADD);
        write(uart_fd, buf, len);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
static uint64_t t_last;
static int serial_parse_char(uint8_t buf, rs485_rsp * mes){
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
bool AutoConnect::receive_uart_data(int uart_fd) {

    uint8_t byte[256];
    rs485_rsp message     = { 0 };
    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

        if (elapsed_time >= 5) { 
            stop_flag.store(true);  
            std::cout << "Timeout: No uart received within 5 seconds." << std::endl;
            return 0;
        }

        char buf[256] = { 0 };
        int result = read(uart_fd, &buf, 256);
        if (result > 0) {
            for (int i = 0; i < result; ++i) {

                if (serial_parse_char(buf[i], &message)) {
                    stop_flag.store(true); 
                    if (crc_modbus(&message) != message.checksum) 
                    {
                        printf("ERROR: checksum error!\r\n");
                        return false;
                    }
                    return true;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


std::string AutoConnect::get_uart_list() {

    vector<string> uart_ports = list_uart_ports();
    std::string output;

    if (uart_ports.empty()) {
        cout << "No UART ports found." << endl;
        return "";
    }
    for (const std::string& port : uart_ports) {
        
        int uart_fd = configure_uart(port, 9600); // Sử dụng baudrate 57600 cho MAVLink

        if (uart_fd == -1) {
            continue;
        }
        std::cout << "Open port " << port << std::endl;
        std::thread send_thread(&AutoConnect::send_request_data, this, uart_fd);

        bool result = receive_uart_data(uart_fd);

        if (send_thread.joinable()) {
            send_thread.join();
        }
        
        stop_flag.store(false); 
        close(uart_fd);

        if (result)
        {
            printf("Find a PORT: %s\n",port.c_str());
            return port;
        }
        
    }
    printf("CANNOT Find a PORT\n");
    return "";
}