// SPDX-License-Identifier: GPL-2.0+
/*
 *  Copyright (C) 2012 Altera Corporation <www.altera.com>
 */
#include <common.h>

#include <dm.h>
#include <net.h>
#include <netdev.h>
#include <eeprom.h>
#include <i2c_eeprom.h>
#include <i2c.h>
#include <asm/gpio.h>

#include <command.h>

/** TE_read_eeprom_mac() - read unique id from eeprom and set eth mac address
 * 
 * @eeprom_mac_offset: start address for unique id
 *
 * @return 0 on success
 *
 */
int TE_read_eeprom_mac(int eeprom_mac_offset)
{	
	unsigned char enetaddr[6];	
	struct udevice *dev_eeprom;
	//struct udevice *dev_eth;
	int ret;
	// check eeprom device
	ret = uclass_first_device_check(UCLASS_I2C_EEPROM, &dev_eeprom);
	if (ret)
		return ret;
	// read unique id from eeprom
	ret = i2c_eeprom_read(dev_eeprom, eeprom_mac_offset, enetaddr, 6);
	if (ret)
		return ret;
	// validate and set new mac address
	if (is_valid_ethaddr(enetaddr)) {
		eth_env_set_enetaddr("ethaddr", enetaddr);
		printf("Net:	MAC address: %pM\n\r", enetaddr);
	}

	return 0;
}

/** TE_set_hps_gpio() - set hps gpio 
 * 
 * @hps_gpio: GPIO number
 * @value: GPIO value (0 for low or 1 for high)
 *
 * @return 0 on success
 *
 */
static int TE_set_hps_gpio(u32 hps_gpio, u32 value)
{
	int ret;

	/* grab the pin before we tweak it */
	ret = gpio_request(hps_gpio, "cmd_gpio");
	if (ret && ret != -EBUSY) {
		printf("     GPIO: requesting pin %u failed\n", hps_gpio);
		return ret;
	} 
	/* set gpio output */
	gpio_direction_output(hps_gpio, value);
	return 0;
}

static void reset_periph(void)
{
	/* deassert the reset pins of USB_RST (GPIO42) and ETH_RST (GPIO43) */
	printf("	Set USB_RST pin to '0'\n");
	TE_set_hps_gpio(42, 0);
	printf("	Set ETH_RST pin to '0'\n"); 
	TE_set_hps_gpio(43, 0);
	/* assert the reset pins of USB_RST (GPIO42) and ETH_RST (GPIO43) */
	printf("	Set USB_RST pin to '1'\n");
	TE_set_hps_gpio(42, 1);
	printf("	Set ETH_RST pin to '1'\n");
	TE_set_hps_gpio(43, 1);
}

int misc_init_r(void)
{	
	/* reset additional periphery */
	reset_periph();
#ifdef CONFIG_I2C_EEPROM
	/* set eth mac address */
	TE_read_eeprom_mac(0xfa);
#endif

	return 0;
}

void spl_display_print(void)
{
	printf("\nWelcome to the Trenz Electronic TEI0022 Board!\n\n");
}
