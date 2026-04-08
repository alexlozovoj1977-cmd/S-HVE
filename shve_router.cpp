/*
 * Scalable Harmonic Vector Equilibrium (S-HVE)
 * Discrete Attractor Gradient Routing Algorithm
 * * Author: Olexandr Lozovyi, Lviv, Ukraine
 * Published: Technical Disclosure Commons (2026)
 * DOI: 10.5281/zenodo.19470168
 * License: CC BY 4.0
 * * Description: 
 * This file demonstrates O(1) integer-based spatial routing 
 * on a 12-vector cuboctahedral matrix (Hexaxial Voxel Architecture),
 * completely bypassing floating-point arithmetic (IEEE 754).
 */

#include <iostream>
#include <stdint.h>

// 12 directional vectors of the Hexaxial Voxel Architecture (HVA)
const int8_t HVA_VECTORS[12][3] = {
    { 1,  1,  0}, { 1, -1,  0}, {-1,  1,  0}, {-1, -1,  0},
    { 1,  0,  1}, { 1,  0, -1}, {-1,  0,  1}, {-1,  0, -1},
    { 0,  1,  1}, { 0,  1, -1}, { 0, -1,  1}, { 0, -1, -1}
};

typedef struct { 
    int32_t x; 
    int32_t y; 
    int32_t z; 
} Node3D;

// Returns the optimal vector index (0-11) for quantum phase discharge
uint8_t route_phase_quantum(Node3D curr, Node3D attr) {
    uint8_t best_vector = 0;
    uint32_t min_dist_sq = UINT32_MAX;

    for (uint8_t i = 0; i < 12; i++) {
        int32_t dx = attr.x - (curr.x + HVA_VECTORS[i][0]);
        int32_t dy = attr.y - (curr.y + HVA_VECTORS[i][1]);
        int32_t dz = attr.z - (curr.z + HVA_VECTORS[i][2]);
        
        // Pure integer topological distance (no FPU square roots needed)
        uint32_t dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

        if (dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            best_vector = i;
        }
    }
    return best_vector;
}

// Demonstration of the routing in action
int main() {
    std::cout << "=== S-HVE Discrete Attractor Routing Demo ===" << std::endl;

    // Define a source and a target in the 3D integer space
    Node3D source_node = {0, 0, 0};
    Node3D target_node = {5, -3, 4}; // Example stimulus location
    Node3D current_node = source_node;

    std::cout << "Source: (0, 0, 0) | Target: (5, -3, 4)\n" << std::endl;

    int hops = 0;
    
    // Route until the target is reached
    while (current_node.x != target_node.x || 
           current_node.y != target_node.y || 
           current_node.z != target_node.z) 
    {
        // 1. Calculate next optimal hop O(1)
        uint8_t best_v = route_phase_quantum(current_node, target_node);
        
        // 2. Move to the next voxel
        current_node.x += HVA_VECTORS[best_v][0];
        current_node.y += HVA_VECTORS[best_v][1];
        current_node.z += HVA_VECTORS[best_v][2];
        
        hops++;
        
        std::cout << "Hop " << hops << ": "
                  << "Used Vector [" << (int)HVA_VECTORS[best_v][0] << ", " 
                                     << (int)HVA_VECTORS[best_v][1] << ", " 
                                     << (int)HVA_VECTORS[best_v][2] << "] "
                  << "-> New Position: (" << current_node.x << ", " 
                                          << current_node.y << ", " 
                                          << current_node.z << ")" << std::endl;
    }

    std::cout << "\nSUCCESS: Target reached in " << hops << " deterministic hops." << std::endl;
    std::cout << "Phase drift: 0 LSB (Mathematically Guaranteed)." << std::endl;

    return 0;
}