#!/usr/bin/env python

import os, sys

CHK_BD_POS=0x100
CHK_SIG_LEN=0x110


def load_config(filename):
    config_d = {}
    f = file(filename, 'r')
    cont = f.readlines()
    cfg_cont = cont
    f.close()
    if len(cont) < 4 or cont[2][:15] != '# SBoot version':
        print 'No a vaild config file'
        return None

    i = 0
    slen = len(cont)
    while i < slen:
        line = cont[i].strip()
        if line.startswith('#'):
            pass  # Do nothing
        elif line.startswith('CONFIG_'):
            parts = line.split('=', 1)
            if len(parts) != 2:
                print 'Error, In ' + filename + ':' + str(i) + ',' + line
                return None
            key = parts[0].strip()
            value = parts[1].strip()
            config_d[key] = value
        i = i + 1
    return config_d


def strip_quota(string):
    if len(string) >= 2:
        if string[0] == "\"" and string[-1] == "\"":
            return string[1:-1]
        if string[0] == "'" and string[-1] == "'":
            return string[1:-1]
    return string


def get_board_name(cfg_d):
    bd_key = 'CONFIG_MSTAR_BD_BOARDNAME'
    if bd_key in cfg_d.keys():
        bd_val = strip_quota(cfg_d[bd_key])    # Maybe for TV-Case
        return bd_val
    
    chip_key = 'CONFIG_MSTAR_CHIP_NAME'        # Maybe for STB-Case
    if chip_key not in cfg_d.keys():
        return None
    else:
        chip_val = strip_quota(cfg_d[chip_key])
        chip_val = '_' + chip_val.upper()
    
    for k in cfg_d.keys():
        if (k.startswith('CONFIG_MSTAR_') and (chip_val in k) and ('_BD_' in k)):
            bd_val = k[len('CONFIG_MSTAR_'):]
            return bd_val
    return None


def get_mboot_flash(cfg_d):
    if 'CONFIG_MBOOT_IN_MMC_FLASH' in cfg_d.keys():
        return "MMC_FLASH"
    if 'CONFIG_MBOOT_IN_NAND_FLASH' in cfg_d.keys():
        return "NAND_FLASH"
    if 'CONFIG_MBOOT_IN_SPI_FLASH' in cfg_d.keys():
        return "SPI_FLASH"
    return 'Unkown'


def get_kernel_bits(cfg_d):
    if 'CONFIG_ARMv8_32BIT_KERNEL' in cfg_d.keys():
        return "32BIT"
    if 'CONFIG_ARMv8_64BIT_KERNEL' in cfg_d.keys():
        return "64BIT"
    return 'Unkown'


def get_security_level(cfg_d):
    if 'CONFIG_TEE' in cfg_d.keys():
        if 'CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE' in cfg_d.keys():
            security_label = "OPTEE"
        else:
            security_label = "R2-TEE"
    else:
        security_label = "Non-TEE"
    
    if 'CONFIG_SECURITY_BOOT' in cfg_d.keys():
        security_label += " SECURITY_BOOT"
    return security_label


def get_dtb_info(cfg_d):
    if "CONFIG_LOAD_DTB" in cfg_d.keys():
        return "True"
    return "False"


def gen_board_info(config_fname):
    config_d = load_config(config_fname)
    if config_d == None :
        print "\nLoad config from \".config\" failed !\n"
        return None

    bd_name = get_board_name(config_d)
    if bd_name == None:
        print "\nGet bd_name from \".config\" failed !\n"
        return None    # Vital information !!
    bd_string = "Board=" + bd_name + " \n"

    mboot_in   = get_mboot_flash(config_d)
    mb_string  = "MBoot_IN=" + mboot_in + " \n"
    kl_bits    = get_kernel_bits(config_d)
    kl_string  = "Kernel=" + kl_bits + " \n"
    se_label   = get_security_level(config_d)
    se_string  = "Security=" + se_label + " \n"
    dtb_info   = get_dtb_info(config_d)
    dtb_string = "Load_DTB=" + dtb_info + " \n"

    # totoal messge
    chunk = bd_string + mb_string + kl_string + se_string + dtb_string
    print chunk,
    #open('zzztest.bin', "w").write(chunk)
    return (chunk + '\0')


def append_chunk_header(bd_chunk, chkhdr_fname):
    chkhdr = open(chkhdr_fname, "r").read()
    chksiz = os.path.getsize(chkhdr_fname)
    remain = chksiz - CHK_BD_POS
    
    dummy_cont = '\xff' * remain
    if dummy_cont not in chkhdr:
        dummy_cont = '\x00' * remain
        if dummy_cont not in chkhdr:
            print "\nCan Not Modify %s, Region(From 0x%x To the end) is Not Empty !\n" % (chkhdr_fname, CHK_BD_POS)
            return False
        dummy_val = '\x00'
    else:
        dummy_val = '\xff'
    
    if len(bd_chunk) >= (remain - CHK_SIG_LEN):
        print "\nBoard chunk(0x%x) is too large\n" % (len(bd_chunk))
        return False
    
    new_chkhdr = chkhdr[:CHK_BD_POS] + bd_chunk + (chksiz - CHK_BD_POS - len(bd_chunk)) * dummy_val
    open(chkhdr_fname, "w").write(new_chkhdr)
    return True


def main(argv):
    global CHK_BD_POS
    if (len(argv) < 3):
        print "\nUsage :  %s  config_path  chunk_header_path\n" % argv[0]
        sys.exit(-1)
    config_fname = argv[1]
    chkhdr_fname = argv[2]

    chunk = gen_board_info(config_fname)
    if chunk == None:
        print "\nCan not generate chunk header !\n"
        sys.exit(-2)
    
    if (append_chunk_header(chunk, chkhdr_fname)) is True:
        print "Append Board info to chunk header success !"


if __name__ == '__main__':
    # Use default locale : traditional C English
    os.environ['LC_ALL'] = 'C'
    main(sys.argv)

