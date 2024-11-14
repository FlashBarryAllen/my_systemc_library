
#include "test.h"
#include <gtest/gtest.h>
#include <bitset>

int Add(int a, int b) { return a + b; }

TEST(AddTest, Basic) {
    std::cout << "test gtest" << std::endl;
    EXPECT_EQ(Add(1, 2), 3);
}

TEST(bittest, Basic) {
    std::cout << "hi gtest" << std::endl;
    std::bitset<10> mybitset;
    mybitset.set(1);
    int index = mybitset._Find_first();
    mybitset.reset(1);

    std::cout << mybitset << std::endl;
}

void TEST_dpa() {
    // Test case 1: Basic functionality
    std::cout << "Test case 1: Basic functionality" << std::endl;
    dpa_scheduler arbiter1(4, 4);
    arbiter1.request(0, 2);
    arbiter1.request(0, 3);
    arbiter1.request(1, 0);
    arbiter1.request(1, 2);
    arbiter1.request(2, 0);
    arbiter1.request(2, 1);
    arbiter1.request(2, 3);
    arbiter1.request(3, 1);
    arbiter1.request(3, 3);

    for (int i = 0; i < arbiter1.m_num_ports * 2; i++) {
        std::cout << arbiter1.m_ptr << std::endl;
        arbiter1.arbitration();
        for (int row = 0; row < arbiter1.m_num_ports; row++) {
            for (int col = 0; col < arbiter1.m_num_ports; col++) {
                std::cout << arbiter1.m_grants[row][col] << ", ";
            }

            std::cout << " " << std::endl;
        }

        auto result = arbiter1.sch_result;

        bool grant_0_2 = arbiter1.has_granted(0, 2);
        bool grant_0_3 = arbiter1.has_granted(0, 3);
        bool grant_1_0 = arbiter1.has_granted(1, 0);
        bool grant_1_2 = arbiter1.has_granted(1, 2);
        bool grant_2_0 = arbiter1.has_granted(2, 0);
        bool grant_2_1 = arbiter1.has_granted(2, 1);
        bool grant_2_3 = arbiter1.has_granted(2, 3);
        bool grant_3_1 = arbiter1.has_granted(3, 1);
        bool grant_3_3 = arbiter1.has_granted(3, 3);
    }

    if (arbiter1.has_granted(1, 2)) {
        std::cout << "Input 1 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 1 is not granted access." << std::endl;
    }

    if (arbiter1.has_granted(2, 0)) {
        std::cout << "Input 2 is granted access to output 0." << std::endl;
    } else {
        std::cout << "Input 2 is not granted access." << std::endl;
    }

    // Test case 2: Invalid port numbers
    std::cout << "\nTest case 2: Invalid port numbers" << std::endl;
    try {
        arbiter1.request(-1, 2);  // Invalid input port
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    try {
        arbiter1.has_granted(4, 0);  // Invalid output port
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Test case 3: No requests
    std::cout << "\nTest case 3: No requests" << std::endl;
    dpa_scheduler arbiter2(2, 2);
    arbiter2.arbitration();
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (arbiter2.has_granted(i, j)) {
                std::cout << "Unexpected grant: Input " << i << " to output "
                          << j << std::endl;
            }
        }
    }

    // Test case 4: Multiple requests for the same output port
    std::cout << "\nTest case 4: Multiple requests for the same output port"
              << std::endl;
    dpa_scheduler arbiter3(3, 3);
    arbiter3.request(0, 2);
    arbiter3.request(1, 2);
    arbiter3.request(2, 2);
    arbiter3.arbitration();
    if (arbiter3.has_granted(0, 2)) {
        std::cout << "Input 0 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 0 is not granted access." << std::endl;
    }
    if (arbiter3.has_granted(1, 2)) {
        std::cout << "Input 1 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 1 is not granted access." << std::endl;
    }
    if (arbiter3.has_granted(2, 2)) {
        std::cout << "Input 2 is granted access to output 2." << std::endl;
    } else {
        std::cout << "Input 2 is not granted access." << std::endl;
    }
}

void TEST_islip() {
    islip* myislip = new islip(4, 4);
    myislip->init_priority_ptr();

    while (1) {
        myislip->init();

        for (auto i = 0; i < 4; i++) {
            myislip->set_ql(i, 0);
            myislip->set_ql(i, 1);
            myislip->set_ql(i, 2);
            myislip->set_ql(i, 3);
        }

        myislip->islip_sch();

        auto ret = myislip->sch_result;

        for (auto i = 0; i < ret.size(); i++) {
            auto in = ret[i].first;
            auto out = ret[i].second;

            std::cout << "(" << in << ", " << out << ")" << std::endl;
        }

        std::cout << std::endl;
    }

    return;
}

void TEST_encap_pkt() {
    const string data = "12345678";
    uint8_t* dstMac = new uint8_t[6];
    dstMac[0] = 12;
    uint8_t* srcMac = new uint8_t[6];
    srcMac[0] = 45;
    uint32_t sourceIP = 1020;
    uint32_t destinationIP = 3040;
    uint16_t sourcePort = 5555;
    uint16_t destinationPort = 80;

    EncapsulatePacket(data, dstMac, srcMac, sourceIP, destinationIP, sourcePort,
                      destinationPort);

    return;
}

void TEST_sch() {
    basic_sch sch = basic_sch(3);

    sch.set_val(0, 100);
    sch.set_val(1, 200);
    sch.set_val(2, 300);

    for (int i = 0; i < 6; i++) {
        int sch_ret = sch.rr_sch();

        if (sch_ret == -1) {
            continue;
        }

        int val = sch.m_input[sch_ret].front();
        sch.m_input[sch_ret].pop_front();

        std::cout << sch_ret << ", " << val << std::endl;

        sch.set_val(0, 400);
        sch.set_val(1, 500);
        sch.set_val(2, 600);
    }
}

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

class TokenBucket {
   public:
    TokenBucket(double rate, double capacity)
        : rate(rate),
          capacity(capacity),
          tokens(capacity),
          last_time(std::chrono::steady_clock::now()) {}

    bool consume(double tokens) {
        std::lock_guard<std::mutex> lock(mutex);
        add_tokens();
        if (this->tokens >= tokens) {
            this->tokens -= tokens;
            return true;
        } else {
            return false;
        }
    }

   private:
    void add_tokens() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - last_time;
        double new_tokens = elapsed.count() * rate;
        tokens = std::min(capacity, tokens + new_tokens);
        last_time = now;
    }

    double rate;
    double capacity;
    double tokens;
    std::chrono::steady_clock::time_point last_time;
    std::mutex mutex;
};

TEST(tokenbucket, basic) {
    TokenBucket bucket(5, 10);  // 每秒生成5个令牌，桶的容量为10个令牌

    for (int i = 0; i < 20; i++) {
        if (bucket.consume(1)) {
            std::cout << "发送数据包" << std::endl;
        } else {
            std::cout << "令牌不足，等待..." << std::endl;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));  // 每0.1秒尝试发送一个数据包
    }
}

#include <iostream>
#include <queue>

class NetworkNode {
   public:
    NetworkNode(int capacity) : capacity(capacity), buffer_usage(0) {}

    void sendData(int data) {
        if (buffer_usage + data <= capacity) {
            buffer.push(data);
            buffer_usage += data;
        } else {
            std::cout << "Buffer full, sending PFC frame\n";
            sendPFCFrame();
        }
    }

    void receiveData() {
        if (!buffer.empty()) {
            int data = buffer.front();
            buffer.pop();
            buffer_usage -= data;
            std::cout << "Data received: " << data << "\n";
        }
    }

   private:
    int capacity;
    int buffer_usage;
    std::queue<int> buffer;

    void sendPFCFrame() {
        std::cout << "Pausing data transmission\n";
        // Implement PFC frame sending logic here
    }
};

TEST(PFC, basic) {
    NetworkNode node(100);

    node.sendData(50);
    node.sendData(60);  // This will trigger PFC frame
    node.receiveData();
    node.sendData(60);  // Now this will be accepted
}

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Queue {
   public:
    Queue(int capacity, int priority) {
        this->capacity = capacity;
        this->priority = priority;
    }

    void enqueue(int packet) {
        if (buffer.size() < capacity) {
            buffer.push(packet);
        } else {
            cout << "Queue " << priority << " is full, dropping packet" << endl;
        }
    }

    void dequeue() {
        if (!buffer.empty()) {
            buffer.pop();
        }
    }

    int getRemainingCapacity() const { return capacity - buffer.size(); }

   public:
    int capacity;
    int priority;
    queue<int> buffer;
};

class Switch {
   public:
    Switch(int numQueues) {
        for (int i = 0; i < numQueues; i++) {
            queues.push_back(Queue(10, i));
        }
    }

    void sendPacket(int packet, int priority) {
        queues[priority].enqueue(packet);

        // Check if any queue is congested
        for (const Queue& queue : queues) {
            if (queue.getRemainingCapacity() <= 0) {
                sendPFCFrame(queue.priority);
            }
        }
    }

   public:
    void sendPFCFrame(int priority) {
        cout << "Sending PFC frame to sender of priority " << priority << endl;
    }

    vector<Queue> queues;
};

TEST(PFC, Gemini) {
    Switch myswitch(4);

    // Simulate sending packets
    for (int i = 0; i < 100; i++) {
        int priority = rand() % 4;
        int packet = 1;

        myswitch.sendPacket(packet, priority);
    }
}

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>

class Packet {
   public:
    int size;
    int priority;
    Packet(int s, int p) : size(s), priority(p) {}
};

class WREDQueue {
   private:
    std::queue<Packet> queue;
    int maxSize;
    double minThreshold;
    double maxThreshold;
    double maxP;

   public:
    WREDQueue(int maxQueueSize, double minTh, double maxTh, double maxProb)
        : maxSize(maxQueueSize),
          minThreshold(minTh),
          maxThreshold(maxTh),
          maxP(maxProb) {
        std::srand(std::time(0));  // Initialize random number generator
    }

    bool enqueue(Packet packet) {
        if (queue.size() >= maxSize) {
            std::cout << "Queue full. Packet dropped.\n";
            return false;
        }

        double avgQueueSize =
            (double)queue.size();  // Simplified average queue size
        double dropProbability = calculateDropProbability(avgQueueSize);

        if (avgQueueSize >= minThreshold && avgQueueSize <= maxThreshold) {
            if ((std::rand() / (double)RAND_MAX) < dropProbability) {
                std::cout << "Packet dropped by WRED. Priority: "
                          << packet.priority << "\n";
                return false;
            }
        }

        queue.push(packet);
        std::cout << "Packet enqueued. Priority: " << packet.priority << "\n";
        return true;
    }

    Packet dequeue() {
        if (queue.empty()) {
            throw std::runtime_error("Queue is empty.");
        }
        Packet packet = queue.front();
        queue.pop();
        return packet;
    }

   private:
    double calculateDropProbability(double avgQueueSize) {
        if (avgQueueSize < minThreshold) {
            return 0.0;
        }
        if (avgQueueSize > maxThreshold) {
            return 1.0;
        }
        return maxP * (avgQueueSize - minThreshold) /
               (maxThreshold - minThreshold);
    }
};

TEST(WRED, basic) {
    WREDQueue wredQueue(10, 3.0, 7.0, 0.5);

    for (int i = 0; i < 15; ++i) {
        Packet p(i + 1, std::rand() % 3);
        wredQueue.enqueue(p);
    }

    try {
        while (true) {
            Packet p = wredQueue.dequeue();
            std::cout << "Dequeued packet with priority: " << p.priority
                      << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

#include <iomanip>
#include <iostream>
#include <vector>

class CRC32 {
   public:
    CRC32() {
        // 初始化CRC表
        generateCRCTable();
    }

    uint32_t compute(const std::vector<uint8_t>& data) {
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < data.size(); ++i) {
            uint8_t byte = data[i];
            uint32_t lookupIndex = (crc ^ byte) & 0xFF;
            crc = (crc >> 8) ^ crcTable[lookupIndex];
        }
        return ~crc;  // 取反
    }

   private:
    std::vector<uint32_t> crcTable;

    void generateCRCTable() {
        uint32_t polynomial = 0x04C11DB7;
        crcTable.resize(256);
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t crc = i;
            for (uint32_t j = 0; j < 8; ++j) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ polynomial;
                } else {
                    crc = crc >> 1;
                }
            }
            crcTable[i] = crc;
        }
    }
};

TEST(CRC32, basic) {
    CRC32 crc32;
    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o', ' ',
                                 'W', 'o', 'r', 'l', 'd'};

    uint32_t crcValue = crc32.compute(data);

    std::cout << "CRC-32: 0x" << std::hex << std::uppercase << crcValue
              << std::endl;
}

// void answer(unique_ptr p1) { *p1 = 20; }

TEST(unique_ptr, basic) { unique_ptr p1 = make_unique<int>(10); }