#ifndef __RTC_H
#define __RTC_H

 /* Useful Macros for the RTC */

#define BIT(n) (0x01<<(n))
#define OK		0

/* RTC Macros */
#define RTC_IRQ					8	/**< @brief RTC IRQ line */
#define RTC_INITIAL_HOOK_ID		8	/**< @brief RTC Initial hook_id */


#define RTC_ADDR_REG			0x70
#define RTC_DATA_REG			0x71

/* Control/ Status Register A */
#define A_UIP		BIT(7)		/**< @brief Update in Progress Register */
#define A_DV2		BIT(6)		/**< @brief DV2-DV0 : Control the counting chain */
#define A_DV1		BIT(5)
#define A_DV0		BIT(4)
#define A_RS3		BIT(3)		/**< @brief RS3-RS0 : Rate selector */
#define A_RS2		BIT(2)
#define A_RS1		BIT(1)
#define A_RS0		BIT(O)

/* Control/ Status Register B */
#define B_SET		BIT(7)		/**< @brief Inhibit updates of time/date registers */
#define B_PIE		BIT(6)		/**< @brief Enable Periodic Interrupts */
#define B_AEI		BIT(5)		/**< @brief Enable Alarm Interrupts */
#define B_UEI		BIT(4)		/**< @brief Enable Update Interrupts */
#define B_SQWE		BIT(3)		/**< @brief Enable square-wave generation */
#define B_DM		BIT(2)		/**< @brief Set registers in Binary or BCD*/
#define B_24_12		BIT(1)		/**< @brief Set hours range from [0,23] or [0,12] */
#define B_DSE		BIT(O)		/**< @brief Enable Daylight Savings Time */

/* Control/ Status Register C */
#define	C_IRQF		BIT(7)		/**< @brief IRQ Line Active */
#define C_PF		BIT(6)		/**< @brief Periodic Interrupt Pending */
#define C_AF		BIT(5)		/**< @brief Alarm Interrupt Pending */
#define C_UE		BIT(4)		/**< @brief Update Interrupt Pending */

/* Control/ Status Register D */
#define	D_VRT		BIT(7)		/**< @brief Valid Time */

/* Date Registers - Adress Location */
#define AL_SECONDS		0X00		/**< @brief Seconds */
#define AL_SEC_ALARM	0X01		/**< @brief Seconds Alarm */
#define AL_MINUTES		0X02		/**< @brief Minutes */
#define AL_MIN_ALARM	0X03		/**< @brief Minutes Alarm */
#define AL_HOURS		0X04		/**< @brief Hours (12-hour or 24-hour mode) */
#define AL_HOURS_ALARAM	0X05		/**< @brief Hours Alarm */
#define AL_DAY_OF_WEEK	0x06		/**< @brief Day of the Week (Sunday = 1) */
#define AL_DAY			0x07		/**< @brief Day of the Month */
#define AL_MONTH		0x08		/**< @brief Month */
#define AL_YEAR			0x09		/**< @brief Year */
#define AL_REGA			0x0A		/**< @brief Control Register A */
#define AL_REGB			0x0B		/**< @brief Control Register B */
#define AL_REGC			0x0C		/**< @brief Control Register C */
#define AL_REGD			0x0D		/**< @brief Control Register  */



typedef struct {
	unsigned long minute;
	unsigned long hour;
	unsigned long day;
	unsigned long month;
	unsigned long year;
} Date_t;

/*
 * @brief Parses a hexadecimal representation of a value to its BCD value
 *
 * @brief Value to be parsed
 *
 * @return Value parsed
 */
unsigned parserBCD (int value);

 /**
  * @brief Subscribes and enables RTC interrupts
  *
  * @return Returns bit order in interrupt mask; negative value on failure
  */
 int rtc_subscribe_int(void);

 /**
  * @brief Unsubscribes RTC interrupts
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int rtc_unsubscribe_int(void);

 /**
  * @brief Reads Value from register reg
  *
  * @param Register that will be read
  *
  * @return Returns bit order in interrupt mask; negative value on failure
  */
 int rtc_read_register(int reg);

 /**
  * @brief Writes value to register reg
  *
  * @param Register to be written
  *
  * New value of register reg
  *
  * @return Return 0 upon success and non-zero otherwise
  */
 int rtc_write_register(unsigned long reg, unsigned long value);

 /*
  * @brief Checks if RTC is being updated
  *
  * @return Return 0 if RTC is being updated and non-zero otherwise
  */
 int rtc_updating(void);

 /*
  * @brief Gets Date from the RTC
  *
  * @return Return a pointer to a date containing a day, a month and a year. If year is 0, function failed.
  */
 Date_t * rtc_read_date(void);


#endif
