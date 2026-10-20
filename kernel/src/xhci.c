#include "xhci.h"
#include "pci.h"
#include "video.h"
#include "main.h"
#include "memory.h"
#include "paging.h" 






struct xhci_controller xhci;
uint64_t *ep0_ring;
uint8_t *config_buf;

struct usb_setup_packet *setup;
uint8_t *desc_buf;




static const char hid_to_ascii[256] = {
    [0x04] = 'a', [0x05] = 'b', [0x06] = 'c', [0x07] = 'd',
    [0x08] = 'e', [0x09] = 'f', [0x0A] = 'g', [0x0B] = 'h',
    [0x0C] = 'i', [0x0D] = 'j', [0x0E] = 'k', [0x0F] = 'l',
    [0x10] = 'm', [0x11] = 'n', [0x12] = 'o', [0x13] = 'p',
    [0x14] = 'q', [0x15] = 'r', [0x16] = 's', [0x17] = 't',
    [0x18] = 'u', [0x19] = 'v', [0x1A] = 'w', [0x1B] = 'x',
    [0x1C] = 'y', [0x1D] = 'z',

    [0x1E] = '1', [0x1F] = '2', [0x20] = '3', [0x21] = '4',
    [0x22] = '5', [0x23] = '6', [0x24] = '7', [0x25] = '8',
    [0x26] = '9', [0x27] = '0',

    [0x2C] = ' ',  // Space
    [0x2D] = '-',  
    [0x2E] = '=',  
    [0x2F] = '[',  
    [0x30] = ']',  
    [0x33] = ';',
    [0x34] = '\'',
    [0x36] = ',',
    [0x37] = '.',
    [0x38] = '/',
};


void find_xhci(void) {
    for (int bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {

                uint16_t vendor = pci_read_word(bus, slot, func, 0x00);
                if (vendor == 0xFFFF) continue;

                uint8_t class    = pci_read_byte(bus, slot, func, 0x0B);
                uint8_t subclass = pci_read_byte(bus, slot, func, 0x0A);
                uint8_t prog_if  = pci_read_byte(bus, slot, func, 0x09);

              
                if (class == 0x0C && subclass == 0x03 && prog_if == 0x30) {
                    xhci.bus      = bus;
                    xhci.slot     = slot;
                    xhci.function = func;



                    uint32_t bar0 = pci_read_dword(bus, slot, func, 0x10);
                    uint32_t bar1 = pci_read_dword(bus, slot, func, 0x14);

                    uint64_t mmio = ((uint64_t)bar1 << 32) | (bar0 & ~0xF);
                    xhci.mmio_base = mmio;


                    map_page(xhci.mmio_base & ~0xFFF,
                    xhci.mmio_base & ~0xFFF,
                    PAGE_PRESENT | PAGE_RW | PAGE_NO_CACHE);

                    return;
                }
            }
        }
    }
}

void xhci_read_capabilities(void) {

    if (xhci.mmio_base == 0) {
        return; 
    }
    volatile uint32_t *mmio = (volatile uint32_t *)xhci.mmio_base;

    xhci.cap_length = mmio[0] & 0xFF;
    xhci.hcsparams1 = mmio[1];
    xhci.hcsparams2 = mmio[2];
    xhci.hcsparams3 = mmio[3];
    xhci.hccparams  = mmio[4];

    xhci.op_regs = (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length);

    xhci.op_regs[0x04 / 4] = 1;         
    xhci.op_regs[0x00 / 4] |= 1;         
    while (xhci.op_regs[0x08 / 4] & 1);  
}

void xhci_setup_dcbaa(void) {
    uint64_t dcbaa_addr = (uint64_t)alloc_page();
    xhci.dcbaa = (uint64_t *)dcbaa_addr;

    
    xhci.op_regs[0x30 / 4] = (uint32_t)(dcbaa_addr & 0xFFFFFFFF);
    xhci.op_regs[0x34 / 4] = (uint32_t)(dcbaa_addr >> 32);
}




void xhci_setup_command_ring(void) {
    uint64_t *ring = alloc_page();
    for (int i = 0; i < 128; i++)
        ring[i] = 0;

    xhci.cmd_ring = ring;  

    ring[127] = ((uint64_t)ring & ~0xF) | (6ULL << 10) | 3ULL;
    xhci.cmd_ring      = ring;
    xhci.cmd_ring_enq  = ring;   
    xhci.cmd_cycle     = 1;      

    volatile uint64_t *crcr = (volatile uint64_t *)(xhci.op_regs + (0x18 / 4));
    *crcr = ((uint64_t)ring & ~0xF) | xhci.cmd_cycle;


}




//void xhci_setup_event_ring(void) {
 //   uint64_t *ring = alloc_page();
//    xhci.event_ring = ring;
//
//
  //  volatile uint32_t *ir = xhci.op_regs + (0x20 / 4);
//
 //   ir[0x08 / 4] = (uint32_t)((uint64_t)ring);
//}

void xhci_init(void) {
    draw_text(framebuffer, 20, 20, "XHCI INIT START", 0xFFFFFF);

    find_xhci();
    if (xhci.mmio_base == 0) {
        draw_text(framebuffer, 20, 40, "NO XHCI FOUND", 0xFF0000);
        return;
    }

    draw_text(framebuffer, 20, 40, "XHCI FOUND", 0x00FF00);

    xhci_read_capabilities();
    draw_text(framebuffer, 20, 60, "CAPS READ", 0xFFFFFF);


}

  



void xhci_reset_ports(void) {
    uint32_t num_ports = xhci.hcsparams1 & 0xFF;
    volatile uint32_t *ports = xhci.op_regs + (0x400 / 4);

    for (uint32_t i = 0; i < num_ports; i++) {
        volatile uint32_t *portsc = &ports[i * 4];
        *portsc |= (1 << 4);
        while (*portsc & (1 << 4));
    }
}

void xhci_check_ports(void) {
    uint32_t num_ports = xhci.hcsparams1 & 0xFF;
    volatile uint32_t *ports = xhci.op_regs + (0x400 / 4);

    for (uint32_t i = 0; i < num_ports; i++) {
        volatile uint32_t *portsc = &ports[i * 4];
        uint32_t v = *portsc;

        uint32_t connected = v & 1;
        uint32_t enabled   = (v >> 1) & 1;
        uint32_t speed     = (v >> 21) & 0xF;

        draw_text(framebuffer, 430, 100, "|Port checked|", 0xFFFFFF);

        if (connected)
            draw_text(framebuffer, 430, 110, "|Connected|", 0xFFFFFF);
        else
            draw_text(framebuffer, 430, 110, "|Not connected|", 0xFFFFFF);

        if (enabled)
            draw_text(framebuffer, 430, 120, "|Enabled|", 0xFFFFFF);
        else
            draw_text(framebuffer, 430, 120, "|Not enabled|", 0xFFFFFF);

        if (connected) {
            draw_text(framebuffer, 430, 130, "|Device connected|", 0xFFFFFF);
            draw_text(framebuffer, 430, 140, "|Enable Slot|", 0xFFFFFF);
            xhci_enable_slot();
        } else {
            draw_text(framebuffer, 430, 130, "|No device!!|", 0xFFFFFF);
        }
    }
}

void xhci_enqueue_command(uint32_t field0, uint32_t field1, uint32_t field2, uint32_t field3) {
    uint64_t *trb = xhci.cmd_ring_enq;

    trb[0] = ((uint64_t)field1 << 32) | field0;
    trb[1] = ((uint64_t)field3 << 32) | field2;

    
    trb[0] |= xhci.cmd_cycle;

    
    xhci.cmd_ring_enq++;
    if (xhci.cmd_ring_enq == xhci.cmd_ring + 127) {
        
        xhci.cmd_ring_enq = xhci.cmd_ring;
        xhci.cmd_cycle ^= 1;
    }
}

void xhci_enable_slot(void) {
    
    uint32_t field0 = 0;
    uint32_t field1 = 0;
    uint32_t field2 = (9U << 10); 
    uint32_t field3 = 0;

    xhci_enqueue_command(field0, field1, field2, field3);

    volatile uint32_t *doorbell =
        (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length + 0x20);
    doorbell[0] = 0; 
}

void xhci_poll_event_ring_1(void) {
    uint64_t *ring = xhci.event_ring;
    uint64_t trb = ring[0];

    if (trb == 0) {
        draw_text(framebuffer, 430, 160, "|No event|", 0xFFFFFF);
        return;
    }

    uint32_t trb_type = (trb >> 10) & 0x3F;
    draw_text(framebuffer, 430, 170, "|Event TRB type detected|", 0xFFFFFF);

    if (trb_type == 33) {
        uint32_t slot_id = (trb >> 24) & 0xFF;
        xhci.slot_id = slot_id;
        draw_text(framebuffer, 430, 180, "|Command Completion Event! Slot ID set|", 0xFFFFFF);
    } else {
        draw_text(framebuffer, 430, 180, "|Unknown event type|", 0xFFFFFF);
    }
}

void xhci_setup_input_context(void) {
    xhci.input_context = alloc_page();
}

void xhci_init_input_context(void) {
    uint32_t *ctx = (uint32_t *)xhci.input_context;

    ctx[0] = 0b11;

    uint32_t *slot_ctx = ctx + 8;
    slot_ctx[0] = 0;
    slot_ctx[1] = xhci.slot_id << 16;

    uint64_t ep0_ring_addr = (uint64_t)ep0_ring;

    uint32_t *ep0_ctx = ctx + 16;
    ep0_ctx[0] = 0; // EP0 Flags etc.
    ep0_ctx[1] = 512; // Max Packet Size
    ep0_ctx[2] = (uint32_t)(ep0_ring_addr & 0xFFFFFFFF);
    ep0_ctx[3] = (uint32_t)(ep0_ring_addr >> 32);
}

void xhci_address_device(void) {
    uint64_t addr = (uint64_t)xhci.input_context;

    uint32_t field0 = (uint32_t)(addr & 0xFFFFFFFF);
    uint32_t field1 = (uint32_t)(addr >> 32);
    uint32_t field2 = (11U << 10) | (xhci.slot_id << 24) | 1U; 
    uint32_t field3 = 0;

    xhci_enqueue_command(field0, field1, field2, field3);

    volatile uint32_t *doorbell =
        (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length + 0x20);
    doorbell[0] = 0;
}


struct erst_entry {
    uint64_t base;
    uint32_t size;
    uint32_t reserved;
};

void xhci_setup_event_ring(void) {
    uint64_t *ring = alloc_page();
    xhci.event_ring = ring;

    struct erst_entry *erst = alloc_page();
    erst[0].base     = (uint64_t)ring & ~0xF;
    erst[0].size     = 128;
    erst[0].reserved = 0;

    volatile uint32_t *ir = xhci.op_regs + (0x20 / 4);

    ir[0x00 / 4] = 1; 
    ir[0x02 / 4] = (uint32_t)((uint64_t)erst);
    ir[0x03 / 4] = (uint32_t)((uint64_t)erst >> 32);

    ir[0x04 / 4] = (uint32_t)((uint64_t)ring);
    ir[0x05 / 4] = (uint32_t)((uint64_t)ring >> 32);

    ir[-2] |= 1; 
}

struct usb_setup_packet {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

struct usb_setup_packet *setup;
uint8_t *desc_buf;

void xhci_build_get_device_descriptor(void) {
    setup   = alloc_page();
    desc_buf = (uint8_t *)alloc_page(); // Platz für Descriptor

    setup->bmRequestType = 0x80;
    setup->bRequest      = 6;
    setup->wValue        = (1 << 8) | 0;
    setup->wIndex        = 0;
    setup->wLength       = 18;
}

void xhci_ep0_enqueue_get_descriptor(void) {
    uint64_t *trb = ep0_ring;

    
    uint64_t setup_addr = (uint64_t)setup;
    trb[0] = (setup_addr & 0xFFFFFFFF) | ((setup_addr >> 32) << 32);
    trb[1] = (8 << 0) | (2ULL << 10); 
    trb[1] |= (1ULL << 5);            

   
    uint64_t data_addr = (uint64_t)desc_buf;
    trb += 2;
    trb[0] = (data_addr & 0xFFFFFFFF) | ((data_addr >> 32) << 32);
    trb[1] = (18 << 0) | (3ULL << 10); 
    trb[1] |= (1ULL << 16);            
    trb[1] |= (1ULL << 5);             
    trb += 2;
    trb[0] = 0;
    trb[1] = (4ULL << 10);             
    trb[1] |= (1ULL << 5);   
    
    
}


void xhci_start_ep0_transfer(void) {
    volatile uint32_t *doorbell =
        (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length + 0x20);

    
    doorbell[xhci.slot_id] = 1; 
}





void xhci_poll_event_ring(void) {
    uint64_t *ring = xhci.event_ring;
    uint64_t trb = ring[0];

    if (trb == 0) {
        draw_text(framebuffer, 430, 160, "|No event|", 0xFFFFFF);
        return;
    }

    uint32_t trb_type = (trb >> 10) & 0x3F;

    if (trb_type == 32) { 
        draw_text(framebuffer, 430, 170, "|Transfer Event|", 0xFFFFFF);

        char buf[64];
     
        draw_text(framebuffer, 430, 180, buf, 0xFFFFFF);
    }
}




uint64_t *ep0_ring;

void xhci_setup_ep0_ring(void) {
    ep0_ring = alloc_page();
    for (int i = 0; i < 128; i++)
        ep0_ring[i] = 0;
}




void xhci_get_configuration_descriptor(void) {
    setup = alloc_page();
    config_buf = alloc_page(); 

    setup->bmRequestType = 0x80;
    setup->bRequest      = 6;
    setup->wValue        = (2 << 8);
    setup->wIndex        = 0;
    setup->wLength       = 512;

    uint64_t *trb = ep0_ring;

    
    trb[0] = (uint64_t)setup;
    trb[1] = (8) | (2ULL << 10) | (1ULL << 5);

    
    trb += 2;
    trb[0] = (uint64_t)config_buf;
    trb[1] = (512) | (3ULL << 10) | (1ULL << 16) | (1ULL << 5);

   
    trb += 2;
    trb[0] = 0;
    trb[1] = (4ULL << 10) | (1ULL << 5);

   
    volatile uint32_t *doorbell =
        (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length + 0x20);
    doorbell[xhci.slot_id] = 1;

    xhci_poll_event_ring();
}



int find_hid_keyboard_interface(uint8_t *config_buf, int length) {
    int i = 0;
    while (i < length) {
        uint8_t bLength        = config_buf[i];
        uint8_t bDescriptorType= config_buf[i+1];

        if (bDescriptorType == 4) { 
            uint8_t bInterfaceClass    = config_buf[i+5];
            uint8_t bInterfaceSubClass = config_buf[i+6];
            uint8_t bInterfaceProtocol = config_buf[i+7];

            if (bInterfaceClass == 3 &&
                bInterfaceSubClass == 1 &&
                bInterfaceProtocol == 1) {
                
                return i;
            }
        }

        i += bLength;
    }
    return -1;
}






struct keyboard_ep {
    uint8_t  addr;
    uint16_t max_packet_size;
    uint8_t  interval;
};

struct keyboard_ep find_keyboard_endpoint(uint8_t *config_buf, int start, int length) {
    struct keyboard_ep ep = {0};
    int i = start;

    while (i < length) {
        uint8_t bLength        = config_buf[i];
        uint8_t bDescriptorType= config_buf[i+1];

        if (bDescriptorType == 5) { 
            uint8_t bmAttributes = config_buf[i+3];
            uint8_t bEndpointAddress = config_buf[i+2];
            uint16_t wMaxPacketSize = config_buf[i+4] | (config_buf[i+5] << 8);
            uint8_t bInterval = config_buf[i+6];

            if ((bmAttributes & 0x3) == 3) {
                ep.addr            = bEndpointAddress;
                ep.max_packet_size = wMaxPacketSize;
                ep.interval        = bInterval;
                break;
            }
        }

        i += bLength;
    }

    return ep;
}






void xhci_setup_keyboard_endpoint(struct keyboard_ep ep) {
    uint64_t *kbd_ring = alloc_page();
    uint32_t *ctx = (uint32_t *)xhci.input_context;

    uint32_t *ep1_ctx = ctx + 24;
    ep1_ctx[0] = (3 << 3); // Interrupt IN
    ep1_ctx[1] = ep.max_packet_size;
    ep1_ctx[2] = (uint32_t)((uint64_t)kbd_ring);
    ep1_ctx[3] = (uint32_t)((uint64_t)kbd_ring >> 32);
}

void handle_keyboard_report(uint8_t *report) {
    uint8_t modifier = report[0];
    uint8_t keycode  = report[2]; 

    char c = hid_to_ascii[keycode];
    
    
}

void xhci_configure_endpoints(void) {
    uint64_t addr = (uint64_t)xhci.input_context;

    uint32_t field0 = (uint32_t)(addr);
    uint32_t field1 = (uint32_t)(addr >> 32);
    uint32_t field2 = (12U << 10) | (xhci.slot_id << 24); 
    uint32_t field3 = 0;

    xhci_enqueue_command(field0, field1, field2, field3);

    volatile uint32_t *doorbell =
        (volatile uint32_t *)(xhci.mmio_base + xhci.cap_length + 0x20);

    doorbell[0] = 0; 
}






void xhci_keyboard_arm_ring(uint64_t *kbd_ring, struct keyboard_ep ep) {
    uint64_t *trb = kbd_ring;

    trb[0] = 0; 
    trb[1] = ep.max_packet_size;    
    trb[1] |= (1ULL << 5);         
    trb[1] |= (1ULL << 16);         
    trb[1] |= (1ULL << 10);          
}
