; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define private ptr @_Z13getTestStringidb(i32 %0, double %1, i1 %2) {
  %result = alloca ptr, align 8
  %_arg0 = alloca i32, align 4
  %_arg1 = alloca double, align 8
  %_arg2 = alloca i1, align 1
  %_arg3 = alloca double, align 8
  store i32 %0, ptr %_arg0, align 4
  store double %1, ptr %_arg1, align 8
  store i1 %2, ptr %_arg2, align 1
  store double 1.340000e+00, ptr %_arg3, align 8
  ret ptr @anon.string.0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call ptr @_Z13getTestStringidb(i32 1, double 3.400000e+00, i1 true)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
