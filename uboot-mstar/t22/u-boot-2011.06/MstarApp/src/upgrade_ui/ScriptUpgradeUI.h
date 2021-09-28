#ifndef __SCRIPT_UPGRADE_UI_H__
#define __SCRIPT_UPGRADE_UI_H__

int do_bootupgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootupgrade_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootupgrade_progress(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootupgrade_clean(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootupgrade_ui(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
