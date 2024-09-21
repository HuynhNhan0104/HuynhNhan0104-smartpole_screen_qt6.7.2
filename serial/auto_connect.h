#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <sys/ioctl.h> 
#include "ODR_01.h"

using namespace std;

class AutoConnect
{
private:

    vector<string> list_uart_ports();
    int configure_uart(const std::string& port, int baud);
    std::mutex uart_mutex;  // Mutex để đồng bộ hóa giữa các luồng gửi và nhận
    std::atomic<bool> stop_flag{false};  // Biến cờ để dừng luồng gửi
    ODR_Interface _sensor{0x15};
public:

    AutoConnect() = default;
    ~AutoConnect() = default;

    std::string get_uart_list();

    // Hàm gửi heartbeat qua UART
    void send_request_data(int uart_fd);

    // Hàm nhận dữ liệu UART và kiểm tra MAVLink heartbeat
    bool receive_uart_data(int uart_fd);

    // Hàm chính để bắt đầu gửi và nhận qua UART
    void start_uart_communication();
};


