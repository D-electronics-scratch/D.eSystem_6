#ifndef USB_CORE_H
#define USB_CORE_H

#pragma once
#include <stdint.h>
#include "xhci.h"
void xhci_full_init(void);
void xhci_get_device_descriptor(void);
void xhci_get_configuration_descriptor(void);
void xhci_setup_keyboard_endpoint(void);

#endif