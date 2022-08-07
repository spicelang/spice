; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Test: %f\0A\00", align 1

define internal double @_f__void__getArg() {
entry.l1:
  %result = alloca double, align 8
  %0 = alloca double, align 8
  store double 4.300000e+00, double* %0, align 8
  %1 = load double, double* %0, align 8
  ret double %1
}

define internal double @_f__void__test() {
entry.l5:
  %0 = call double @_f__void__getArg()
  %1 = alloca double, align 8
  %2 = alloca double, align 8
  %result = alloca double, align 8
  store double %0, double* %1, align 8
  %3 = load double, double* %1, align 8
  %4 = fadd double %3, 1.200000e+00
  store double %4, double* %2, align 8
  %5 = load double, double* %2, align 8
  ret double %5
}

define i32 @main() {
entry.l9:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  store i32 0, i32* %result, align 4
  %1 = call double @_f__void__test()
  store double %1, double* %0, align 8
  %2 = load double, double* %0, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), double %2)
  %4 = load i32, i32* %result, align 4
  ret i32 %4
}

declare i32 @printf(i8*, ...)
