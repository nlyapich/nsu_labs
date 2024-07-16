#!/bin/bash

gcc main.c -o main

rm ?_*

ln main a_create_dir
ln main b_print_dir
ln main c_delete_dir
ln main d_create_file
ln main e_print_file
ln main f_delete_file
ln main g_create_simlink
ln main h_print_simlink
ln main i_print_file_simlink
ln main j_delete_simlink
ln main k_create_hard_link
ln main l_delete_hard_link
ln main m_print_file_info
ln main n_change_file_permissions
