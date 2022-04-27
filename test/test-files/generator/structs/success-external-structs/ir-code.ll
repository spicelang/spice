; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s_Vec_int_bool = type { i32, i1 }

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %v = alloca %_s_Vec_int_bool, align 8
  %0 = alloca %_s_Vec_int_bool, align 8
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 11, i32* %1, align 4
  %4 = load i32, i32* %1, align 4
  %5 = getelementptr inbounds %_s_Vec_int_bool, %_s_Vec_int_bool* %0, i32 0, i32 0
  store i32 %4, i32* %5, align 4
  store i1 false, i1* %2, align 1
  %6 = load i1, i1* %2, align 1
  %7 = getelementptr inbounds %_s_Vec_int_bool, %_s_Vec_int_bool* %0, i32 0, i32 1
  store i1 %6, i1* %7, align 1
  %8 = load %_s_Vec_int_bool, %_s_Vec_int_bool* %0, align 4
  store %_s_Vec_int_bool %8, %_s_Vec_int_bool* %v, align 4
  %9 = load %_s_Vec_int_bool, %_s_Vec_int_bool* %v, align 4
  call void @_mp_Vec_print(%_s_Vec_int_bool* %v)
  store i1 true, i1* %3, align 1
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}

declare void @_mp_Vec_print(%_s_Vec_int_bool*)
