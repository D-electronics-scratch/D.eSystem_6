#ifndef XHCI_H
#define XHCI_H

#pragma once
#include <stdint.h>



struct xhci_controller {
    uint8_t  bus;
    uint8_t  slot;
    uint8_t  function;
    uint64_t mmio_base;

    volatile uint32_t *op_regs;

    uint32_t cap_length;
    uint32_t hcsparams1;
    uint32_t hcsparams2;
    uint32_t hcsparams3;
    uint32_t hccparams;

    uint64_t *dcbaa;
    uint64_t *cmd_ring;
    uint64_t *cmd_ring_enq;  
    uint64_t *event_ring;
    uint64_t *input_context;
    uint32_t  slot_id;

    //uint64_t *ep0_ring;       
    uint8_t   cmd_cycle;      
};

extern struct xhci_controller xhci;

void xhci_init(void);
void find_xhci(void);

void xhci_read_capabilities(void);
void xhci_setup_dcbaa(void);
void xhci_setup_command_ring(void);
void xhci_setup_event_ring(void);

void xhci_reset_ports(void);
void xhci_check_ports(void);
void xhci_enable_slot(void);

void xhci_setup_input_context(void);
void xhci_setup_ep0_ring(void);
void xhci_init_input_context(void);
void xhci_address_device(void);

void xhci_poll_event_ring(void);
void xhci_poll_event_ring_1(void);

void xhci_build_get_device_descriptor(void);
void xhci_ep0_enqueue_get_descriptor(void);
void xhci_start_ep0_transfer(void);
#endif
