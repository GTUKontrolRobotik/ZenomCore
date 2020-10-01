#--------------------------------------------------------------
#
# Zenom Hard real-time simulation environment
# Copyright (C) 2013
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Zenom License, Version 1.0
#
#--------------------------------------------------------------

ZENOM_CORE_VER_MAJ   = 1
ZENOM_CORE_VER_MIN   = 0
ZENOM_CORE_VER_PAT   = 0
ZENOM_CORE_VERSION   = $${ZENOM_CORE_VER_MAJ}.$${ZENOM_CORE_VER_MIN}.$${ZENOM_CORE_VER_PAT}

#--------------------------------------------------------------
# Install paths
#--------------------------------------------------------------

ZENOM_CORE_INSTALL_PREFIX = /usr/local

ZENOM_CORE_INSTALL_HEADERS   = $${ZENOM_INSTALL_PREFIX}/include/zenomcore
ZENOM_CORE_INSTALL_LIBS      = $${ZENOM_CORE_INSTALL_PREFIX}/lib64
ZENOM_CORE_INSTALL_BINS      = $${ZENOM_CORE_INSTALL_PREFIX}/bin
