#ifndef DPA_H
#define DPA_H

#include <vector>

class Arbiter; // Forward declaration

class VoqCBar {
public:
    // Constructor with parameters for number of ports and priority vector
    VoqCBar(int num_ports, const std::vector<bool>& priority_vec);

    // Function to request access from a specific input port to an output port
    void Request(int input_port, int output_port);

    // Function to check if a specific input port is granted access to an output port
    bool HasGranted(int input_port, int output_port);

    // Function to perform the arbitration for all ports
    void Arbitrate();

    void rotatePriorityVector();

private:
    int num_ports_;
    std::vector<bool> priority_vec_;
    std::vector<std::vector<bool>> grants_;
    std::vector<bool> requests_;

    // Helper functions (declared here for encapsulation)
    int GetPriorityIndex(int i, int j) const;
    bool IsValidPort(int port) const;
};

#endif // DPA_H