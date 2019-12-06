#******************************************************************************************************************#
# Copyright (c) 2018-2019, High Performance Computing Architecture and System
# research laboratory at University of North Carolina at Charlotte (HPCAS@UNCC)
# and Lawrence Livermore National Security, LLC.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#*****************************************************************************************************************#

#!/bin/bash
for filename in $1/*; do
    #echo "sss"
    ./tester "$filename" 
done
