# LLVM target architecture switches
option(SPICE_TARGET_AARCH64 "Enable AArch64 target" ON)
option(SPICE_TARGET_AMDGPU "Enable AMDGPU target" OFF)
option(SPICE_TARGET_ARM "Enable ARM target" OFF)
option(SPICE_TARGET_MIPS "Enable MIPS target" OFF)
option(SPICE_TARGET_NVPTX "Enable NVPTX target" OFF)
option(SPICE_TARGET_POWERPC "Enable PowerPC target" OFF)
option(SPICE_TARGET_RISCV "Enable RISC-V target" OFF)
option(SPICE_TARGET_WEBASSEMBLY "Enable WebAssembly target" ON)
option(SPICE_TARGET_X86 "Enable X86 target" ON)

# Switch to enable all target architectures
option(SPICE_ALL_TARGETS "Enable all targets" OFF)

# Takes the target name and a list of components to append (e.g. asmparser, codegen)
macro(add_target_if_enabled target_name)
    string(TOUPPER "${target_name}" target_upper)
    string(TOLOWER "${target_name}" target_lower)
    if (SPICE_TARGET_${target_upper} OR SPICE_ALL_TARGETS)
        list(APPEND LLVM_TARGET_ARCHITECTURES "${target_name}")
        foreach (COMP ${ARGN})
            list(APPEND LLVM_COMPONENTS "${target_lower}${COMP}")
        endforeach ()
    endif ()
endmacro()

# Define components per target explicitly
set(LLVM_COMPONENTS "")
set(LLVM_TARGET_ARCHITECTURES "")

add_target_if_enabled(AArch64 asmparser codegen)
add_target_if_enabled(AMDGPU asmparser codegen)
add_target_if_enabled(ARM asmparser codegen)
add_target_if_enabled(Mips asmparser codegen)
add_target_if_enabled(NVPTX codegen)
add_target_if_enabled(PowerPC asmparser codegen)
add_target_if_enabled(RISCV asmparser codegen)
add_target_if_enabled(WebAssembly asmparser codegen)
add_target_if_enabled(X86 asmparser codegen)

# Add always-present LLVM libs
list(APPEND LLVM_COMPONENTS mcjit target nativecodegen passes)

# Map components to library names
llvm_map_components_to_libnames(LLVM_LIBS ${LLVM_COMPONENTS})

# Print status messages
string(JOIN "," LLVM_TARGET_ARCHITECTURES_JOINED ${LLVM_TARGET_ARCHITECTURES})
message(STATUS "Spice: Enabled targets: ${LLVM_TARGET_ARCHITECTURES_JOINED}")
