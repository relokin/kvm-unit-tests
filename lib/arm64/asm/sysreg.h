/*
 * Ripped off from arch/arm64/include/asm/sysreg.h
 *
 * Copyright (C) 2016, Red Hat Inc, Andrew Jones <drjones@redhat.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.
 */
#ifndef _ASMARM64_SYSREG_H_
#define _ASMARM64_SYSREG_H_

#include <linux/const.h>

#define sys_reg(op0, op1, crn, crm, op2) \
	((((op0)&3)<<19)|((op1)<<16)|((crn)<<12)|((crm)<<8)|((op2)<<5))

#ifdef __ASSEMBLY__
	.irp	num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
	.equ	.L__reg_num_x\num, \num
	.endr
	.equ	.L__reg_num_xzr, 31

	.macro	mrs_s, rt, sreg
	.inst	0xd5200000|(\sreg)|(.L__reg_num_\rt)
	.endm

	.macro	msr_s, sreg, rt
	.inst	0xd5000000|(\sreg)|(.L__reg_num_\rt)
	.endm
#else
#include <libcflat.h>

#define read_sysreg(r) ({					\
	u64 __val;						\
	asm volatile("mrs %0, " xstr(r) : "=r" (__val));	\
	__val;							\
})

#define write_sysreg(v, r) do {					\
	u64 __val = (u64)v;					\
	asm volatile("msr " xstr(r) ", %x0" : : "rZ" (__val));	\
} while (0)

#define read_sysreg_s(r) ({					\
	u64 __val;						\
	asm volatile("mrs_s %0, " xstr(r) : "=r" (__val));	\
	__val;							\
})

#define write_sysreg_s(v, r) do {				\
	u64 __val = (u64)v;					\
	asm volatile("msr_s " xstr(r) ", %x0" : : "rZ" (__val));\
} while (0)

#define write_regn_el0(__reg, __n, __val) \
	write_sysreg((__val), __reg ## __n ## _el0)

#define read_regn_el0(__reg, __n) \
	read_sysreg(__reg ## __n ## _el0)

asm(
"	.irp	num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30\n"
"	.equ	.L__reg_num_x\\num, \\num\n"
"	.endr\n"
"	.equ	.L__reg_num_xzr, 31\n"
"\n"
"	.macro	mrs_s, rt, sreg\n"
"	.inst	0xd5200000|(\\sreg)|(.L__reg_num_\\rt)\n"
"	.endm\n"
"\n"
"	.macro	msr_s, sreg, rt\n"
"	.inst	0xd5000000|(\\sreg)|(.L__reg_num_\\rt)\n"
"	.endm\n"
);
#endif /* __ASSEMBLY__ */

#define ICC_PMR_EL1			sys_reg(3, 0, 4, 6, 0)
#define ICC_SGI1R_EL1			sys_reg(3, 0, 12, 11, 5)
#define ICC_IAR1_EL1			sys_reg(3, 0, 12, 12, 0)
#define ICC_EOIR1_EL1			sys_reg(3, 0, 12, 12, 1)
#define ICC_GRPEN1_EL1			sys_reg(3, 0, 12, 12, 7)

/* System Control Register (SCTLR_EL1) bits */
#define SCTLR_EL1_EE	(1 << 25)
#define SCTLR_EL1_WXN	(1 << 19)
#define SCTLR_EL1_I	(1 << 12)
#define SCTLR_EL1_SA0	(1 << 4)
#define SCTLR_EL1_SA	(1 << 3)
#define SCTLR_EL1_C	(1 << 2)
#define SCTLR_EL1_A	(1 << 1)
#define SCTLR_EL1_M	(1 << 0)

#define SCTLR_EL1_RES1	(_BITUL(7) | _BITUL(8) | _BITUL(11) | _BITUL(20) | \
			 _BITUL(22) | _BITUL(23) | _BITUL(28) | _BITUL(29))
#define INIT_SCTLR_EL1_MMU_OFF	\
			SCTLR_EL1_RES1

#define SCTLR_EL2_RES1 (_BITUL(4) | _BITUL(5) | _BITUL(11) | _BITUL(16) | 	\
			_BITUL(18) | _BITUL(22) | _BITUL(23) | _BITUL(28) |	\
			_BITUL(29))


#define HCR_RW_SHIFT		31
#define HCR_RW			(UL(1) << HCR_RW_SHIFT)
#define HCR_HOST_NVHE_FLAGS	HCR_RW

#define CPTR_EL2_RES1	(UL(0xff) | _BITUL(9) | _BITUL(12) | _BITUL(13))
#define CPTR_EL2_TZ	_BITUL(8)

#define ID_AA64PFR0_GIC_SHIFT		24
#define ID_AA64PFR0_SVE_SHIFT		32

#define ID_AA64DFR0_PMUVER_SHIFT	8
#define ID_AA64DFR0_PMSVER_SHIFT	32

#define ID_AA64MMFR1_LOR_SHIFT		16

#define ICC_SRE_EL2_SRE		(1 << 0)
#define ICC_SRE_EL2_ENABLE	(1 << 3)

#define SYS_ICC_SRE_EL2				sys_reg(3, 4, 12, 9, 5)
#define SYS_ICH_HCR_EL2				sys_reg(3, 4, 12, 11, 0)

#define SYS_PMBIDR_EL1				sys_reg(3, 0, 9, 10, 7)

#define SYS_PMSCR_EL2				sys_reg(3, 4, 9, 9, 0)

#define SYS_LORC_EL1				sys_reg(3, 0, 10, 4, 3)
#define SYS_PMBIDR_EL1_P_SHIFT		4

#define SYS_PMSCR_EL2_PA_SHIFT		4
#define SYS_PMSCR_EL2_PCT_SHIFT		6

#define MDCR_EL2_E2PB_MASK	(UL(0x3))
#define MDCR_EL2_E2PB_SHIFT	(UL(12))

#define MDCR_EL2_TPMS		(1 << 14)

#define SYS_ZCR_EL2				sys_reg(3, 4, 1, 2, 0)
#define ZCR_ELx_LEN_MASK		0x1ff

#endif /* _ASMARM64_SYSREG_H_ */
