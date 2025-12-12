
#ifndef __CONFIG_TEI0022_H__
#define __CONFIG_TEI0022_H__

#include <asm/arch/base_addr_ac5.h>

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE	0x40000000

/* Booting Linux */
#define CONFIG_LOADADDR		0x01000000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TEI0022__ */
