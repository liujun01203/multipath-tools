#include <stdio.h>

#include "checkers.h"
#include "vector.h"
#include "defaults.h"
#include "structs.h"
#include "config.h"
#include "pgpolicies.h"
#include "prio.h"

/*
 * Tuning suggestions on these parameters should go to
 * dm-devel@redhat.com (subscribers-only, see README)
 *
 * You are welcome to claim maintainership over a controller
 * family. Please mail the currently enlisted maintainer and
 * the upstream package maintainer.
 *
 * WARNING:
 *
 * Devices with a proprietary handler must also be included in
 * the kernel side. Currently at drivers/scsi/scsi_dh.c
 */
static struct hwentry default_hw[] = {
	/*
	 * Compellent Technologies/DELL
	 */
	{
		.vendor        = "COMPELNT",
		.product       = "Compellent Vol",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * Apple
	 *
	 * Maintainer : Shyam Sundar
	 * Mail : g.shyamsundar@yahoo.co.in
	 */
	{
		.vendor        = "APPLE.*",
		.product       = "Xserve RAID",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * StorageWorks/HPE
	 */
	{
		.vendor        = "3PARdata",
		.product       = "VV",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "DEC",
		.product       = "HSG80",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 hp_sw",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.checker_name  = HP_SW,
		.prio_name     = PRIO_HP_SW,
		.prio_args     = NULL,
	},
	{
		.vendor        = "HP",
		.product       = "A6189A",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* MSA 1000/MSA1500 EVA 3000/5000 with old firmware */
		.vendor        = "(COMPAQ|HP)",
		.product       = "(MSA|HSV)1.0.*",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 hp_sw",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.checker_name  = HP_SW,
		.prio_name     = PRIO_HP_SW,
		.prio_args     = NULL,
	},
	{
		/* MSA 1000/1500 with new firmware */
		.vendor        = "(COMPAQ|HP)",
		.product       = "MSA VOLUME",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* EVA 3000/5000 with new firmware, EVA 4000/6000/8000 */
		.vendor        = "(COMPAQ|HP)",
		.product       = "HSV1[01]1|HSV2[01]0|HSV3[046]0|HSV4[05]0",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* HP MSA2000 family with old firmware */
		.vendor        = "HP",
		.product       = "MSA2[02]12fc|MSA2012i",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* HP MSA2000 family with new firmware */
		.vendor        = "HP",
		.product       = "MSA2012sa|MSA23(12|24)(fc|i|sa)|MSA2000s VOLUME",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* HP MSA 1040/2040 family */
		.vendor        = "HP",
		.product       = "MSA (1|2)040 SA(N|S)",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* HP SVSP */
		.vendor        = "HP",
		.product       = "HSVX700",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* HP Smart Array */
		.vendor        = "HP",
		.product       = "LOGICAL VOLUME.*",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* HP P2000 family */
		.vendor        = "HP",
		.product       = "P2000 G3 FC|P2000G3 FC/iSCSI|P2000 G3 SAS|P2000 G3 iSCSI",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * DDN
	 */
	{
		.vendor        = "DDN",
		.product       = "SAN DataDirector",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * EMC/DELL
	 *
	 * Maintainer : Edward Goggin, EMC
	 * Mail : egoggin@emc.com
	 */
	{
		.vendor        = "EMC",
		.product       = "SYMMETRIX",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 6,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* DGC CLARiiON CX/AX and EMC VNX */
		.vendor        = "^DGC",
		.product       = "^RAID|^DISK|^VRAID",
		.bl_product    = "LUNZ",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 emc",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.checker_name  = EMC_CLARIION,
		.prio_name     = PRIO_EMC,
		.prio_args     = NULL,
		.retain_hwhandler = RETAIN_HWHANDLER_ON,
		.detect_prio   = DETECT_PRIO_ON,
	},
	{
		.vendor        = "EMC",
		.product       = "Invista",
		.bl_product    = "LUNZ",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 5,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "XtremIO",
		.product       = "XtremApp",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = "queue-length 0",
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.fast_io_fail  = 5,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * Fujitsu
	 */
	{
		.vendor        = "FSC",
		.product       = "CentricStor",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "FUJITSU",
		.product       = "ETERNUS_DX(H|L|M|400|8000)",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 10,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * Hitachi
	 *
	 * Maintainer : Matthias Rudolph
	 * Mail : matthias.rudolph@hds.com
	 */
	{
		.vendor        = "(HITACHI|HP)",
		.product       = "OPEN-.*",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "HITACHI",
		.product       = "DF.*",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_HDS,
		.prio_args     = NULL,
	},
	/*
	 * IBM
	 *
	 * Maintainer : Hannes Reinecke, SuSE
	 * Mail : hare@suse.de
	 */
	{
		.vendor        = "IBM",
		.product       = "ProFibre 4000R",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM FAStT 1722-600 */
		.vendor        = "IBM",
		.product       = "^1722-600",
		.bl_product    = "Universal Xport",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS4100 */
		.vendor        = "IBM",
		.product       = "^1724",
		.bl_product    = "Universal Xport",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS3200 / DS3300 / DS3400 */
		.vendor        = "IBM",
		.product       = "^1726",
		.bl_product    = "Universal Xport",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS4400 / DS4500 / FAStT700 */
		.vendor        = "IBM",
		.product       = "^1742",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		.vendor        = "IBM",
		.product       = "^1745|^1746",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS4700 */
		.vendor        = "IBM",
		.product       = "^1814",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS4800 */
		.vendor        = "IBM",
		.product       = "^1815",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS5000 */
		.vendor        = "IBM",
		.product       = "^1818",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM Netfinity Fibre Channel RAID Controller Unit */
		.vendor        = "IBM",
		.product       = "^3526",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* IBM DS4200 / FAStT200 */
		.vendor        = "IBM",
		.product       = "^3542",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM ESS F20 aka Shark */
		.vendor        = "IBM",
		.product       = "^2105800",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM ESS F20 aka Shark */
		.vendor        = "IBM",
		.product       = "^2105F20",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM DS6000 */
		.vendor        = "IBM",
		.product       = "^1750500",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* IBM DS8000 */
		.vendor        = "IBM",
		.product       = "^2107900",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM SAN Volume Controller */
		.vendor        = "IBM",
		.product       = "^2145",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* IBM S/390 ECKD DASD */
		.vendor        = "IBM",
		.product       = "S/390 DASD ECKD",
		.bl_product    = "S/390.*",
		.uid_attribute = "ID_UID",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM S/390 FBA DASD */
		.vendor        = "IBM",
		.product       = "S/390 DASD FBA",
		.bl_product    = "S/390.*",
		.uid_attribute = "ID_UID",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM IPR */
		.vendor        = "IBM",
		.product       = "^IPR.*",
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* IBM RSSM */
		.vendor        = "IBM",
		.product       = "1820N00",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		/* IBM XIV Storage System */
		.vendor        = "IBM",
		.product       = "2810XIV",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = 15,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 15,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * IBM Power Virtual SCSI Devices
	 *
	 * Maintainer : Brian King, IBM
	 * Mail : brking@linux.vnet.ibm.com
	 */
	{
		/* AIX VDASD */
		.vendor        = "AIX",
		.product       = "VDASD",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* IBM 3303 NVDISK */
		.vendor        = "IBM",
		.product       = "3303      NVDISK",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = FAILOVER,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		/* AIX NVDISK */
		.vendor        = "AIX",
		.product       = "NVDISK",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * DELL
	 */
	{
		/* DELL MD3000 */
		.vendor        = "DELL",
		.product       = "MD3000",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* DELL MD32xx/MD36xx */
		.vendor        = "DELL",
		.product       = "MD32xx|MD36xx",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		/* DELL MD34xx/MD38xx */
		.vendor        = "DELL",
		.product       = "MD34xx|MD38xx",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	/*
	 * NETAPP ONTAP family
	 *
	 * Maintainer : Martin George
	 * Mail : marting@netapp.com
	 */
	{
		.vendor        = "NETAPP",
		.product       = "LUN.*",
		.features      = "3 queue_if_no_path pg_init_retries 50",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.flush_on_last_del = FLUSH_ENABLED,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 128,
		.dev_loss      = MAX_DEV_LOSS_TMO,
		.prio_name     = PRIO_ONTAP,
		.prio_args     = NULL,
		.retain_hwhandler = RETAIN_HWHANDLER_ON,
		.detect_prio   = DETECT_PRIO_ON,
	},
	{
		/* IBM NSeries */
		.vendor        = "IBM",
		.product       = "Nseries.*",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 128,
		.prio_name     = PRIO_ONTAP,
		.prio_args     = NULL,
	},
	/*
	 * NEXENTA
	 *
	 * Maintainer : Yacine Kheddache
	 * Mail : yacine@alyseo.com
	 */
	{
		.vendor        = "NEXENTA",
		.product       = "COMSTAR",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 30,
		.minio         = 128,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * Pillar Data/Oracle
	 *
	 * Maintainer : Srinivasan Ramani
	 * Mail : srinivas.ramani@oracle.com
	 */
	{
		.vendor        = "Pillar",
		.product       = "Axiom.*",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * SGI
	 */
	{
		.vendor        = "SGI",
		.product       = "TP9[13]00",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "SGI",
		.product       = "TP9[45]00",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		.vendor        = "SGI",
		.product       = "IS.*",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	/*
	 * NEC
	 */
	{
		/* M-Series */
		.vendor        = "NEC",
		.product       = "DISK ARRAY",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * STK/Oracle
	 */
	{
		.vendor        = "STK",
		.product       = "OPENstorage D280",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	/*
	 * SUN/Oracle
	 */
	{
		.vendor        = "SUN",
		.product       = "(StorEdge 3510|T4)",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "SUN",
		.product       = "STK6580_6780",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		.vendor        = "EUROLOGC",
		.product       = "FC2502",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	/*
	 * Pivot3
	 *
	 * Maintainer : Bart Brooks, Pivot3
	 * Mail : bartb@pivot3.com
	 */
	{
		.vendor        = "PIVOT3",
		.product       = "RAIGE VOLUME",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 100,
		.prio_name     = DEFAULT_PRIO,
		.prio_args     = NULL,
	},
	{
		.vendor        = "SUN",
		.product       = "CSM200_R",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	/* SUN/LSI 2510, 2540, 2530, 2540 */
	{
		.vendor        = "SUN",
		.product       = "LCSM100_[IEFS]",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	/* StorageTek 6180 */
	{
		.vendor        = "SUN",
		.product       = "SUN_6180",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.minio_rq      = DEFAULT_MINIO_RQ,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	/* LSI/Engenio/NetApp RDAC
	 *
	 * Maintainer : Sean Stewart
	 * Mail : sean.stewart@netapp.com
	 */
	{
		.vendor        = "(NETAPP|LSI|ENGENIO)",
		.product       = "INF-01-00",
		.bl_product    = "Universal Xport",
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 30,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
		.detect_prio   = DETECT_PRIO_ON,
		.retain_hwhandler = RETAIN_HWHANDLER_ON,
	},
	{
		.vendor        = "STK",
		.product       = "FLEXLINE 380",
		.bl_product    = "Universal Xport",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
		.prio_args     = NULL,
	},
	{
		.vendor	       = "Intel",
		.product       = "Multi-Flex",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		.vendor	       = "(LIO-ORG|SUSE)",
		.product       = "RBD",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		.vendor	       = "DataCore",
		.product       = "SANmelody",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		.vendor	       = "DataCore",
		.product       = "Virtual Disk",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		.vendor        = "PURE",
		.product       = "FlashArray",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = "queue-length 0",
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.fast_io_fail  = 10,
		.prio_name     = DEFAULT_PRIO,
		.no_path_retry = 0,
		.dev_loss      = 60,
		.prio_args     = NULL,
	},
	{
		.vendor        = "HUAWEI",
		.product       = "XSG1",
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
	},
	/*
	 * Violin Memory
	 */
	{
		.vendor        = "VIOLIN",
		.product       = "CONCERTO ARRAY",
		.selector      = "round-robin 0",
		.pgpolicy      = MULTIBUS,
		.prio_name     = PRIO_ALUA,
		.minio         = 100,
		.rr_weight     = RR_WEIGHT_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.features      = "1 queue_if_no_path",
		.no_path_retry = 300,
	},
	/*
	 * Infinidat
	 */
	{
		.vendor        = "NFINIDAT",
		.product       = "InfiniBox.*",
		.prio_name     = PRIO_ALUA,
		.pgpolicy      = GROUP_BY_PRIO,
		.selector      = "round-robin 0",
		.features      = DEFAULT_FEATURES,
		.pgfailback    = 30,
		.rr_weight     = RR_WEIGHT_PRIO,
		.no_path_retry = NO_PATH_RETRY_FAIL,
		.minio_rq      = 1,
		.flush_on_last_del = FLUSH_ENABLED,
		.fast_io_fail  = 5,
		.dev_loss      = 30,
	},
	/*
	 * Tegile Systems
	 */
	{
		.vendor        = "TEGILE",
		.product       = "ZEBI-(FC|ISCSI)|INTELLIFLASH",
		.hwhandler     = "1 alua",
		.selector      = "round-robin 0",
		.pgpolicy      = GROUP_BY_PRIO,
		.no_path_retry = 10,
		.dev_loss      = 50,
		.prio_name     = PRIO_ALUA,
		.pgfailback    = 30,
		.minio         = 128,
	},
	/*
	 * EOL
	 */
	{
		.vendor        = NULL,
		.product       = NULL,
		.features      = NULL,
		.hwhandler     = NULL,
		.selector      = NULL,
		.pgpolicy      = 0,
		.pgfailback    = 0,
		.rr_weight     = 0,
		.no_path_retry = 0,
		.minio         = 0,
		.minio_rq      = 0,
		.checker_name  = NULL,
		.prio_name     = NULL,
		.prio_args     = NULL,
	},
};

extern int
setup_default_hwtable (vector hw)
{
	int r = 0;
	struct hwentry * hwe = default_hw;

	while (hwe->vendor) {
		r += store_hwe(hw, hwe);
		hwe++;
	}
	return r;
}
