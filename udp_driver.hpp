#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace udp_driver {

    class signal_registry {
    public:
        using signal_callback = std::function<void(const std::vector<uint8_t>&)>;
        void subscribe(uint64_t id, signal_callback callback);
        void notify(uint64_t id, const std::vector<uint8_t>& data);

    private:
        std::unordered_map<uint64_t, signal_callback> registry_;
        std::mutex mutex_;
    };

    class udp_listener {
    public:
        udp_listener(const std::string& address, uint16_t port, signal_registry& registry);
        ~udp_listener();
        void start();
        void stop();

    private:
        signal_registry& registry_;
        SOCKET socket_fd_;
        bool stop_;
        std::thread listener_thread_;
        void initialize_socket(const std::string& address, uint16_t port);
        void listen_for_packets();
        void close_socket();
    };

    class udp_sender {
    public:
        udp_sender(const std::string& address, uint16_t port);
        ~udp_sender();
        void send(uint64_t id, const std::vector<uint8_t>& data);

    private:
        std::string address_;
        uint16_t port_;
        SOCKET socket_fd_;
        void initialize_socket();
        void close_socket();
    };

    template <typename T>
    std::vector<uint8_t> serialize(const T& value) {
        std::vector<uint8_t> buffer(sizeof(T));
        std::memcpy(buffer.data(), &value, sizeof(T));
        return buffer;
    }

    template <typename T>
    T deserialize(const std::vector<uint8_t>& buffer) {
        T value;
        std::memcpy(&value, buffer.data(), sizeof(T));
        return value;
    }
}
