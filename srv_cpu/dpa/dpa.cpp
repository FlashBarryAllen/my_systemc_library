#include "dpa.h"

#include <algorithm>

class Arbiter {
public:
    // Constructor with parameters for input/output size and priority vector
    Arbiter()
    {

    }

    Arbiter(int size, const std::vector<bool>& priority_vec) :
        size_(size), priority_vec_(priority_vec) {}

    // Function to arbitrate requests based on request signal, north, west, and mask
    bool Grant(bool request, bool north, bool west, bool mask) {
        // Check if request is valid and priority allows access
        if (request && mask) {
        // No neighbor with higher priority (based on north, west, and mask) has a grant
        if ((!north || !HasHigherPriority(north, west, mask)) &&
            (!west || !HasHigherPriority(west, north, mask))) {
            return true;
        }
        }
        return false;
    }

private:
    // Function to check if a neighbor has higher priority based on its position and mask
    bool HasHigherPriority(bool neighbor, bool other_neighbor, bool mask) const {
        int neighbor_pos = GetPosition(neighbor, other_neighbor);
        int my_pos = GetPosition(mask, 0, false); // Assuming mask indicates my position
        return neighbor_pos < my_pos;
    }

    // Function to calculate the priority position based on signal combination
    int GetPosition(bool signal1, bool signal2, bool reverse = false) const {
        int pos = 0;
        if (signal1) {
        pos += 1;
        }
        if (signal2) {
        pos += 2;
        }
        return reverse ? size_ - 1 - pos : pos;
    }

    int size_;
    std::vector<bool> priority_vec_;
};

VoqCBar::VoqCBar(int num_ports, const std::vector<bool>& priority_vec) :
    num_ports_(num_ports), priority_vec_(priority_vec) {
    grants_.resize(num_ports_, std::vector<bool>(num_ports_, false));
    requests_.resize(num_ports_, false);
    priority_vec_.resize(2 * num_ports_ - 1, false);
}

void VoqCBar::Request(int input_port, int output_port) {
    if (IsValidPort(input_port) && IsValidPort(output_port)) {
        requests_[input_port] = true;
    }
}

bool VoqCBar::HasGranted(int input_port, int output_port) {
    if (IsValidPort(input_port) && IsValidPort(output_port)) {
        return grants_[output_port][input_port];
    }
    return false;
    }

    void VoqCBar::Arbitrate() {
    // Initialize north, west, and mask signals for each arbiter
    std::vector<std::vector<bool>> north(num_ports_, std::vector<bool>(num_ports_, false));
    std::vector<std::vector<bool>> west(num_ports_, std::vector<bool>(num_ports_, false));
    std::vector<std::vector<bool>> mask(num_ports_, std::vector<bool>(num_ports_, false));

    // Set north, west, and mask signals based on port positions
    for (int i = 0; i < num_ports_; ++i) {
        for (int j = 0; j < num_ports_; ++j) {
        north[i][j] = (i > 0) ? grants_[i - 1][j] : false;
        west[i][j] = (j > 0) ? grants_[i][j - 1] : false;
        mask[i][j] = priority_vec_[GetPriorityIndex(i, j)];
        }
    }

    // Create arbiters for each cell
    std::vector<std::vector<Arbiter>> arbiters(num_ports_, std::vector<Arbiter>(num_ports_));
    for (int i = 0; i < num_ports_; ++i) {
        for (int j = 0; j < num_ports_; ++j) {
        arbiters[i][j] = Arbiter(num_ports_, priority_vec_);
        }
    }

    // Perform arbitration for each cell
    for (int i = 0; i < num_ports_; ++i) {
        for (int j = 0; j < num_ports_; ++j) {
        bool request = requests_[i];
        bool grant = arbiters[i][j].Grant(request, north[i][j], west[i][j], mask[i][j]);
        grants_[i][j] = grant;
        }
    }
}

int VoqCBar::GetPriorityIndex(int i, int j) const {
    return i * num_ports_ + j;
}

bool VoqCBar::IsValidPort(int port) const {
    return port >= 0 && port < num_ports_;
}

void VoqCBar::rotatePriorityVector() {
    // Rotate the vector one position to the left
    std::rotate(priority_vec_.begin(), priority_vec_.begin() + 1, priority_vec_.end());
}