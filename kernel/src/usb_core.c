#include "xhci.h"
#include "main.h"
#include "usb_core.h"

extern void paging_init(uint64_t *root);




void xhci_full_init(void) {
    //uint64_t *root = alloc_page();
    //paging_init(root);

    xhci_init();  
    find_xhci();
    //xhci_read_capabilities(); 
    xhci_reset_ports();  
    xhci_check_ports(); 
    xhci_setup_input_context();
    //xhci_enable_slot();
    //xhci_address_device();
    //xhci_enqueue_command(); 
    //xhci_setup_dcbaa();
   // xhci_setup_command_ring();
    //xhci_setup_event_ring();
    //xhci_reset_ports();
    //xhci_check_ports();
    //xhci_poll_event_ring_1();    

    
    //xhci_setup_ep0_ring();
    //xhci_init_input_context();
    //xhci_address_device();
    xhci_poll_event_ring_1();     

    //xhci_build_get_device_descriptor();
    //xhci_ep0_enqueue_get_descriptor();
    //xhci_start_ep0_transfer();
    //xhci_poll_event_ring();   
}  
