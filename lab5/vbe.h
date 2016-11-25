#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>

/** @defgroup vbe vbe
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Functions */
/**@{
 *
 * VBE Functions from Chapter 4
 */

// VBE functions are called using the INT 10h interrupt vector, passing arguments in the 80X86 registers.
#define VBE_INTERRUPT			0x10

// All VBE functions are called with the AH register set to 4Fh to distinguish them from the standard VGA
#define VBE_CALL				0x4F

// 4.3 Function 00h - Return VBE Controller Information
#define GET_VBE_CONTROLLER_INFO 0x00

// 4.4 Function 01h - Return VBE Mode Information
#define GET_VBE_MODE_INFO		0x01

// 4.5 Function 02h - Set VBE Mode
#define VBE_SET_MODE			0x02

// 4.6 Function 03h - Return Current VBE Mode
#define GET_CURRENT_VBE_MODE	0x03

// 4.7 Function 04h - Save/Restore State
#define SAVE_RESTORE_STATE		0x04

// 4.8 Function 05h - Display Window Control
#define DISPLAY_WINDOW_CONTROL	0x05

// 4.9 Function 06h - Set/Get Logical Scan Line Length
#define SET_GET_SCAN_LENGTH		0x06

// 4.10 Function 07h - Set/Get Display Start
#define SET_GET_DISPLAY_START	0x07

// 4.11 Function 08h - Set/Get DAC Palette Format
#define SET_GET_PALETTE_FORMAT		0x08

// 4.12 Function 09h - Set/Get Palette Data
#define SET_GET_PALETTE_DATA		0x09

// 4.13 Function 0Ah - Return VBE Protected Mode Interface
#define GET_PTR_PROTECTED

/** @} end of VBE preprocessor directives */



/** @name VBE Mode Info Block */
/**@{
 *
 * Packed VBE Mode Info Block 
 */ 
 
typedef struct {
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes; 	/**< @brief mode attributes */
  uint8_t WinAAttributes; 		/**< @brief window A attributes */
  uint8_t WinBAttributes; 		/**< @brief window B attributes */
  uint16_t WinGranularity; 	/**< @brief window granularity */
  uint16_t WinSize;		/**< @brief window size */
  uint16_t WinASegment;		/**< @brief window A start segment */
  uint16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  uint16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  uint8_t XCharSize; 		/**< @brief character cell width in pixels */
  uint8_t YCharSize; 		/**< @brief character cell height in pixels */
  uint8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  uint8_t BitsPerPixel; 		/**< @brief bits per pixel */
  uint8_t NumberOfBanks;		/**< @brief number of banks */
  uint8_t MemoryModel;		/**< @brief memory model type */
  uint8_t BankSize;		/**< @brief bank size in KB */
  uint8_t NumberOfImagePages;	/**< @brief number of images */
  uint8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  uint8_t RedMaskSize;		/* size of direct color red mask in bits */
  uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
  uint8_t GreenMaskSize;		/* size of direct color green mask in bits */
  uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  uint8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  uint8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 /* remainder of ModeInfoBlock */
} __attribute__((packed)) vbe_mode_info_t;

/** @} end of vbe_mode_info_t*/

//Based on VESA pdf, chapter 4.3
typedef struct {
    char    VESASignature[4];       /* 'VESA' 4 byte signature */
    short   VESAVersion;            /* VBE version number */
    phys_bytes OEMStringPtr;      	/* Pointer to OEM string */
    long    Capabilities;           /* Capabilities of video card */
    phys_bytes VideoModePtr;     	/* Pointer to supported modes */
    short   TotalMemory;            /* Number of 64kb memory blocks */
    short	OemSoftwareRev;			/* VBE implementation Software revision */
	char	*OemVendorNamePtr;		/* Pointer to Vendor Name String */
	char	*OemProductNamePtr;		/* Pointer to Product Name String */
	char	*OemProductRevPtr;		/* Pointer to Product Revision sTring */
	char	reserved[222];			/* Reserved for VBE implementation scratch area */
    char    OemData[256];			/* Data Area for OEM Strings  */
} __attribute__((packed)) vbe_info_block;


/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 * 
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 * 
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Returns onformation on the VBE controller.
 *
 * Initializes unpacked vbe_info_block structure passed as an adress,
 * by calling VBE function 0x00.
 * Returns VBE Controller Information.
 *
 * @param vbe_info_p adress of vbe_info_block structure to be initalized
 * @return 0 on succes, non-zero otherwise
 *
 */
int vbe_get_controller_info (vbe_info_block *vbe_info_p);

/**
 * @brief Asserts whether the VBE byte response indicates an error, and prints accordingly
 *
 * @param Byte returned in the AH
 *
 * @return 0 on success, non-zero otherwise
 */
int vbe_assert_error(unsigned char e);

 /** @} end of vbe */

#endif /* __VBE_H */
