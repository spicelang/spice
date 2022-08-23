; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define internal double @_f__void__genericFunction__int_double(i32 %0, double %1) {
entry.l4:
  %arg1 = alloca i32, align 4
  store i32 %0, ptr %arg1, align 4
  %arg2 = alloca double, align 8
  store double %1, ptr %arg2, align 8
  %arg3 = alloca i32, align 4
  store i32 10, ptr %arg3, align 4
  %result = alloca double, align 8
  %2 = load i32, ptr %arg1, align 4
  %3 = load double, ptr %arg2, align 8
  %4 = sitofp i32 %2 to double
  %5 = fadd double %4, %3
  %6 = load i32, ptr %arg3, align 4
  %7 = sitofp i32 %6 to double
  %8 = fadd double %5, %7
  %9 = alloca double, align 8
  store double %8, ptr %9, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

define internal double @_f__void__genericFunction__long_double(i64 %0, double %1) {
entry.l4:
  %arg1 = alloca i64, align 8
  store i64 %0, ptr %arg1, align 8
  %arg2 = alloca double, align 8
  store double %1, ptr %arg2, align 8
  %arg3 = alloca i32, align 4
  store i32 10, ptr %arg3, align 4
  %result = alloca double, align 8
  %2 = load i64, ptr %arg1, align 8
  %3 = load double, ptr %arg2, align 8
  %4 = sitofp i64 %2 to double
  %5 = fadd double %4, %3
  %6 = load i32, ptr %arg3, align 4
  %7 = sitofp i32 %6 to double
  %8 = fadd double %5, %7
  %9 = alloca double, align 8
  store double %8, ptr %9, align 8
  %10 = load double, ptr %9, align 8
  ret double %10
}

define i32 @main() {
entry.l12:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca i32, align 4
  store i32 1, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = alloca double, align 8
  store double 2.400000e+00, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = call double @_f__void__genericFunction__int_double(i32 %1, double %3)
  %5 = alloca double, align 8
  store double %4, ptr %5, align 8
  %6 = load double, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @0, double %6)
  %8 = alloca i64, align 8
  store i64 12, ptr %8, align 8
  %9 = load i64, ptr %8, align 8
  %10 = alloca double, align 8
  store double 2.000000e+00, ptr %10, align 8
  %11 = load double, ptr %10, align 8
  %12 = call double @_f__void__genericFunction__long_double(i64 %9, double %11)
  %13 = alloca double, align 8
  store double %12, ptr %13, align 8
  %14 = load double, ptr %13, align 8
  %15 = call i32 (ptr, ...) @printf(ptr @1, double %14)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

declare i32 @printf(ptr, ...)
