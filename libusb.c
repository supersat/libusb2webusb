#include "libusb.h"
#include <stdio.h>

#define __USE_GNU
#include <dlfcn.h>

#include "server.h"

void *real_libusb;

int LIBUSB_CALL libusb_init(libusb_context **ctx) {
    printf("libusb_init\n");
    real_libusb = dlopen("/usr/lib/x86_64-linux-gnu/libusb-1.0.so.0", RTLD_NOW | RTLD_LOCAL);
    if (!real_libusb) {
        perror("Can't open real libusb");
        return LIBUSB_ERROR_OTHER;
    }
    ws_init();
    return ((int LIBUSB_CALL (*)(libusb_context **))(dlsym(real_libusb, "libusb_init")))(ctx);
}

void LIBUSB_CALL libusb_exit(libusb_context *ctx) {
    printf("libusb_exit\n");
    ws_shutdown();
    ((void LIBUSB_CALL (*)(libusb_context *))(dlsym(real_libusb, "libusb_exit")))(ctx);
}

void LIBUSB_CALL libusb_set_debug(libusb_context *ctx, int level) {
    printf("libusb_set_debug\n");
    ((void LIBUSB_CALL (*)(libusb_context *, int))(dlsym(real_libusb, "libusb_set_debug")))(ctx, level);
}

ssize_t LIBUSB_CALL libusb_get_device_list(libusb_context *ctx,
	libusb_device ***list) {
    printf("libusb_get_device_list\n");
    return ((ssize_t LIBUSB_CALL (*)(libusb_context *, libusb_device ***))(dlsym(real_libusb, "libusb_get_device_list")))(ctx, list);
}

libusb_device * LIBUSB_CALL libusb_get_parent(libusb_device *dev) {
    printf("libusb_get_parent\n");
    return ((libusb_device * LIBUSB_CALL (*)(libusb_device *))(dlsym(real_libusb, "libusb_get_parent")))(dev);
}

void LIBUSB_CALL libusb_free_device_list(libusb_device **list,
	int unref_devices) {
    printf("libusb_free_device_list\n");
    ((void LIBUSB_CALL (*)(libusb_device **, int))(dlsym(real_libusb, "libusb_free_device_list")))(list, unref_devices);
}

libusb_device * LIBUSB_CALL libusb_ref_device(libusb_device *dev) {
    printf("libusb_ref_device\n");
    return ((libusb_device * LIBUSB_CALL (*)(libusb_device *))(dlsym(real_libusb, "libusb_ref_device")))(dev);
}

void LIBUSB_CALL libusb_unref_device(libusb_device *dev) {
    printf("libusb_unref_device\n");
    ((void LIBUSB_CALL (*)(libusb_device *))(dlsym(real_libusb, "libusb_unref_device")))(dev);
}

int LIBUSB_CALL libusb_open(libusb_device *dev, libusb_device_handle **dev_handle) {
    printf("libusb_open\n");
    return ((int LIBUSB_CALL (*)(libusb_device *, libusb_device_handle **))(dlsym(real_libusb, "libusb_open")))(dev, dev_handle);
}

void LIBUSB_CALL libusb_close(libusb_device_handle *dev_handle) {
    printf("libusb_close\n");
     ((void LIBUSB_CALL (*)(libusb_device_handle *))(dlsym(real_libusb, "libusb_close")))(dev_handle);
}

int LIBUSB_CALL libusb_set_configuration(libusb_device_handle *dev_handle,
	int configuration) {
    printf("libusb_set_configuration\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_set_configuration")))(dev_handle, configuration);
}

int LIBUSB_CALL libusb_get_configuration(libusb_device_handle *dev,
	int *config) {
    printf("libusb_get_configuration\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int *))(dlsym(real_libusb, "libusb_get_configuration")))(dev, config);
}

int LIBUSB_CALL libusb_claim_interface(libusb_device_handle *dev_handle,
	int interface_number) {
    printf("libusb_claim_interface\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_claim_interface")))(dev_handle, interface_number);
}

int LIBUSB_CALL libusb_release_interface(libusb_device_handle *dev_handle,
	int interface_number) {
    printf("libusb_release_interface\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_release_interface")))(dev_handle, interface_number);
}

int LIBUSB_CALL libusb_set_interface_alt_setting(libusb_device_handle *dev_handle,
	int interface_number, int alternate_setting) {
    printf("libusb_set_interface_alt_setting\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int, int))(dlsym(real_libusb, "libusb_release_interface")))(dev_handle, interface_number, alternate_setting);
}

int LIBUSB_CALL libusb_reset_device(libusb_device_handle *dev_handle) {
    printf("libusb_reset_device\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *))(dlsym(real_libusb, "libusb_reset_device")))(dev_handle);
}

int LIBUSB_CALL libusb_kernel_driver_active(libusb_device_handle *dev_handle,
	int interface_number) {
    printf("libusb_kernel_driver_active\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_kernel_driver_active")))(dev_handle, interface_number);
}

int LIBUSB_CALL libusb_detach_kernel_driver(libusb_device_handle *dev_handle,
	int interface_number) {
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_detach_kernel_driver")))(dev_handle, interface_number);
}

int LIBUSB_CALL libusb_attach_kernel_driver(libusb_device_handle *dev_handle,
	int interface_number) {
    printf("libusb_attach_kernel_driver\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))(dlsym(real_libusb, "libusb_attach_kernel_driver")))(dev_handle, interface_number);
}

int LIBUSB_CALL libusb_get_device_descriptor(libusb_device *dev,
	struct libusb_device_descriptor *desc) {
    printf("libusb_get_device_descriptor\n");
    return ((int LIBUSB_CALL (*)(libusb_device *, struct libusb_device_descriptor *))(dlsym(real_libusb, "libusb_get_device_descriptor")))(dev, desc);
}

int LIBUSB_CALL libusb_get_config_descriptor(libusb_device *dev,
	uint8_t config_index, struct libusb_config_descriptor **config) {
    printf("libusb_get_config_descriptor\n");
    return ((int LIBUSB_CALL (*)(libusb_device *, uint8_t, struct libusb_config_descriptor **))
        (dlsym(real_libusb, "libusb_get_config_descriptor")))(dev, config_index, config);
 }

 void LIBUSB_CALL libusb_free_config_descriptor(
	struct libusb_config_descriptor *config) {
    printf("libusb_free_config_descriptor\n");
    ((void LIBUSB_CALL (*)(struct libusb_config_descriptor *))(dlsym(real_libusb, "libusb_config_descriptor")))(config);
}

int LIBUSB_CALL libusb_get_string_descriptor_ascii(libusb_device_handle *dev_handle,
	uint8_t desc_index, unsigned char *data, int length) {
    printf("libusb_get_string_descriptor_ascii\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, uint8_t, unsigned char *, int))
        (dlsym(real_libusb, "libusb_get_string_descriptor_ascii")))(dev_handle, desc_index, data, length);
}

int LIBUSB_CALL libusb_control_transfer(libusb_device_handle *dev_handle,
	uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
	unsigned char *data, uint16_t wLength, unsigned int timeout) {
    printf("libusb_control_transfer\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, uint8_t, uint8_t, uint16_t, uint16_t, unsigned char *, uint16_t, unsigned int))
        (dlsym(real_libusb, "libusb_control_transfer")))(dev_handle, request_type, bRequest, wValue, wIndex, data, wLength, timeout);
}

int LIBUSB_CALL libusb_bulk_transfer(libusb_device_handle *dev_handle,
	unsigned char endpoint, unsigned char *data, int length,
	int *actual_length, unsigned int timeout) {
    printf("libusb_bulk_transfer\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, unsigned char, unsigned char *, int, int *, unsigned int))
        (dlsym(real_libusb, "libusb_bulk_transfer")))(dev_handle, endpoint, data, length, actual_length, timeout);
}

int LIBUSB_CALL libusb_interrupt_transfer(libusb_device_handle *dev_handle,
	unsigned char endpoint, unsigned char *data, int length,
	int *actual_length, unsigned int timeout) {
    printf("libusb_interrupt_transfer\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, unsigned char, unsigned char *, int, int *, unsigned int))
        (dlsym(real_libusb, "libusb_interrupt_transfer")))(dev_handle, endpoint, data, length, actual_length, timeout);
}

struct libusb_transfer * LIBUSB_CALL libusb_alloc_transfer(int iso_packets) {
    printf("libusb_alloc_transfer\n");
    return ((struct libusb_transfer * LIBUSB_CALL (*)(int))
        (dlsym(real_libusb, "libusb_alloc_transfer")))(iso_packets);
}

void LIBUSB_CALL libusb_free_transfer(struct libusb_transfer *transfer) {
    printf("libusb_free_transfer\n");
    ((void LIBUSB_CALL (*)(struct libusb_transfer *))(dlsym(real_libusb, "libusb_free_transfer")))(transfer);
}

int LIBUSB_CALL libusb_submit_transfer(struct libusb_transfer *transfer)
{
    printf("libusb_submit_transfer\n");
    ((void LIBUSB_CALL (*)(struct libusb_transfer *))(dlsym(real_libusb, "libusb_free_transfer")))(transfer);  
}

int LIBUSB_CALL libusb_clear_halt(libusb_device_handle *dev_handle,
	unsigned char endpoint)
{
    printf("libusb_clear_halt\n");
    return ((int LIBUSB_CALL (*)(libusb_device_handle *, int))
        (dlsym(real_libusb, "libusb_clear_halt")))(dev_handle, endpoint);
}

int LIBUSB_CALL libusb_get_max_iso_packet_size(libusb_device *dev,
	unsigned char endpoint)
{
    printf("libusb_get_max_iso_packet_size\n");
    return ((int LIBUSB_CALL (*)(libusb_device *, int))
        (dlsym(real_libusb, "libusb_get_max_iso_packet_size")))(dev, endpoint);
}

uint8_t LIBUSB_CALL libusb_get_bus_number(libusb_device *dev)
{
    printf("libusb_get_bus_number\n");
    return ((uint8_t LIBUSB_CALL (*)(libusb_device *))
        (dlsym(real_libusb, "libusb_get_max_iso_packet_size")))(dev);
}

uint8_t LIBUSB_CALL libusb_get_device_address(libusb_device *dev)
{
    printf("libusb_get_device_address\n");
    return ((uint8_t LIBUSB_CALL (*)(libusb_device *))
        (dlsym(real_libusb, "libusb_get_device_address")))(dev);
}

int LIBUSB_CALL libusb_get_device_speed(libusb_device *dev)
{
    printf("libusb_get_device_speed\n");
    return ((int LIBUSB_CALL (*)(libusb_device *))
        (dlsym(real_libusb, "libusb_get_device_speed")))(dev);
}

uint8_t LIBUSB_CALL libusb_get_port_number(libusb_device *dev)
{
    printf("libusb_get_port_number\n");
    return ((uint8_t LIBUSB_CALL (*)(libusb_device *))
        (dlsym(real_libusb, "libusb_get_port_number")))(dev);
}

int LIBUSB_CALL libusb_get_port_numbers(libusb_device *dev, uint8_t *port_numbers, int port_numbers_len)
{
    printf("libusb_get_port_numbers\n");
    return ((int LIBUSB_CALL (*)(libusb_device *, uint8_t *, int))
        (dlsym(real_libusb, "libusb_get_port_numbers")))(dev, port_numbers, port_numbers_len);
}

int LIBUSB_CALL libusb_handle_events(libusb_context *ctx)
{
    printf("libusb_handle_events\n");
    return ((int LIBUSB_CALL (*)(libusb_context *))
        (dlsym(real_libusb, "libusb_handle_events")))(ctx);
}
