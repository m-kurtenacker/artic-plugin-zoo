# use Debug or Release
: ${BUILD_TYPE:=Debug}

#: ${CMAKE_MAKE:=""}
#: ${MAKE:="make -j4"}

# use this for ninja instead of make
: ${CMAKE_MAKE:="-G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DAnyDSL_REMOTE_URL=https://github.com/m-kurtenacker -DLLVM_INSTALL_ROOT=/usr/lib/llvm-16 -DAnyDSL_PKG_RV_URL=https://github.com/m-kurtenacker/rv"}
: ${MAKE:="../patch-and-make.sh"}

# set this to true if you don't have a github account
: ${HTTPS:=true}

# set this to true if you want to download and build the required version of CMake
: ${CMAKE:=false}
: ${BRANCH_CMAKE:=v3.27.7}

# set this to false if you don't want to build with LLVM
# setting to false is meant to speed up debugging and not recommended for end users
: ${LLVM:=true}
: ${LLVM_PREBUILD:=false}
: ${LLVM_AUTOBUILD:=false}
: ${LLVM_TARGETS:="AArch64;AMDGPU;ARM;NVPTX;X86"}
: ${LLVM_VERSION:=16.0.4}
: ${RV_TAG:=origin/unhinged_cleanup}
: ${RV_MODULE_BUILD:=ON}

# Set this to use an external llvm installation.
#: ${LLVM_EXTERN:=""}

# use this to debug thorin hash table performance
: ${THORIN_PROFILE:=false}

# set this to false if you don't build with LLVM
: ${RUNTIME_JIT:=true}

# set this to false to hide runtime debug output
: ${RUNTIME_DEBUG_OUTPUT:=true}

# set the default branches for each repository
: ${BRANCH_ARTIC:=plugins}
: ${BRANCH_IMPALA:=development}
: ${BRANCH_THORIN:=plugins}
: ${BRANCH_RUNTIME:=master}

: ${BRANCH_STINCILLA:=main}
: ${CLONE_STINCILLA:=false}
: ${BUILD_STINCILLA:=false}

: ${BRANCH_RODENT:=artic}
: ${CLONE_RODENT:=false}
: ${BUILD_RODENT:=false}
