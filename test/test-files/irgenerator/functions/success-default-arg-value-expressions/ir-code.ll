; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Test: %f\0A\00", align 1

define private double @_Z6getArgv() {
  %result = alloca double, align 8
  ret double 4.300000e+00
}

define private double @_Z4testv() {
  %result = alloca double, align 8
  %arg = alloca double, align 8
  %1 = call double @_Z6getArgv()
  %2 = fadd double %1, 1.200000e+00
  store double %2, ptr %arg, align 8
  %3 = load double, ptr %arg, align 8
  ret double %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call double @_Z4testv()
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
