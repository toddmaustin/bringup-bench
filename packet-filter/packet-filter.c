#include "libmin.h"

// Define protocol value for TCP
#define TCP_PROTOCOL 6

// Define constants for the 192.168.0.0/16 IP range.
#define FIXED_IP_FIRST 192
#define FIXED_IP_SECOND 168

#define PACKET_COUNT 100  // Total number of packets to simulate

// Packet structure definition
typedef struct {
    unsigned int src_ip;
    unsigned int dest_ip;
    unsigned short src_port;
    unsigned short dest_port;
    unsigned char protocol;  // E.g., 6 for TCP, 17 for UDP, etc.
    char payload[64];
} Packet;

// Generate a random packet.
// With ~5% probability, the packet will have the TCP protocol 
// and a destination IP within the range 192.168.0.0 - 192.168.255.255.
Packet generate_packet() {
    Packet pkt;
    
    if (libmin_rand() % 20 == 0) {
        // Force matching criteria: destination IP in 192.168.x.x and TCP protocol.
        pkt.dest_ip = ((unsigned int)FIXED_IP_FIRST << 24) |
                      ((unsigned int)FIXED_IP_SECOND << 16) |
                      (((unsigned int)libmin_rand() % 256) << 8) |
                      ((unsigned int)libmin_rand() % 256);
        pkt.protocol = TCP_PROTOCOL;
    } else {
        // Generate a random destination IP and a random protocol.
        pkt.dest_ip = (unsigned int)libmin_rand();
        pkt.protocol = (unsigned char)(libmin_rand() % 256);
    }
    
    // Generate a random source IP.
    pkt.src_ip = (unsigned int)libmin_rand();
    
    // Generate random source and destination ports.
    pkt.src_port = (unsigned short)(libmin_rand() % 65536);
    pkt.dest_port = (unsigned short)(libmin_rand() % 65536);
    
    // Fill the payload with 63 random uppercase characters and null-terminate.
    for (int i = 0; i < 63; i++) {
        pkt.payload[i] = 'A' + (libmin_rand() % 26);
    }
    pkt.payload[63] = '\0';
    
    return pkt;
}

// Check if the packet matches the filter criteria:
// The packet should be using TCP and have a destination IP in the range 192.168.0.0/16.
int check_packet_filter(Packet pkt) {
    if (pkt.protocol != TCP_PROTOCOL) {
        return FALSE;
    }
    // Extract the first two octets of the destination IP.
    unsigned int first_octet = (pkt.dest_ip >> 24) & 0xFF;
    unsigned int second_octet = (pkt.dest_ip >> 16) & 0xFF;
    return (first_octet == FIXED_IP_FIRST && second_octet == FIXED_IP_SECOND);
}

// Helper function to print an IP address in dotted notation.
void print_ip(unsigned int ip) {
    libmin_printf("%u.%u.%u.%u", 
           (ip >> 24) & 0xFF, 
           (ip >> 16) & 0xFF, 
           (ip >> 8) & 0xFF, 
           ip & 0xFF);
}

// Print the details of a packet.
void print_packet(Packet pkt) {
    libmin_printf("Packet Details:\n");
    libmin_printf("  Source IP: ");
    print_ip(pkt.src_ip);
    libmin_printf("\n  Destination IP: ");
    print_ip(pkt.dest_ip);
    libmin_printf("\n  Source Port: %u\n", pkt.src_port);
    libmin_printf("  Destination Port: %u\n", pkt.dest_port);
    libmin_printf("  Protocol: %u\n", pkt.protocol);
    libmin_printf("  Payload: %s\n", pkt.payload);
    libmin_printf("------------------------------\n");
}

int main() {
    // Seed the random number generator.
    libmin_srand(42);
    
    int packetCounter = 0;
    
    // Simulate packet processing.
    while (packetCounter < PACKET_COUNT) {
        Packet pkt = generate_packet();
        packetCounter++;
        
        // Use the updated filter: check for TCP and IP range.
        if (check_packet_filter(pkt)) {
            libmin_printf("Matched Packet #%d:\n", packetCounter);
            print_packet(pkt);
        }
    }
    
    libmin_success();
    return 0;
}

