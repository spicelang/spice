; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define internal double @_f__void__genericFunction__int_double(i32 %0, double %1) {
entry.l4:
  %arg1 = alloca i32, align 4
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  %result = alloca double, align 8
  %2 = alloca double, align 8
  store i32 %0, ptr %arg1, align 4
  store double %1, ptr %arg2, align 8
  store i32 10, ptr %arg3, align 4
  %3 = load i32, ptr %arg1, align 4
  %4 = load double, ptr %arg2, align 8
  %5 = sitofp i32 %3 to double
  %6 = fadd double %5, %4
  %7 = load i32, ptr %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  store double %9, ptr %2, align 8
  %10 = load double, ptr %2, align 8
  ret double %10
}

define internal double @_f__void__genericFunction__long_double(i64 %0, double %1) {
entry.l4:
  %arg1 = alloca i64, align 8
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  %result = alloca double, align 8
  %2 = alloca double, align 8
  store i64 %0, ptr %arg1, align 8
  store double %1, ptr %arg2, align 8
  store i32 10, ptr %arg3, align 4
  %3 = load i64, ptr %arg1, align 8
  %4 = load double, ptr %arg2, align 8
  %5 = sitofp i64 %3 to double
  %6 = fadd double %5, %4
  %7 = load i32, ptr %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  store double %9, ptr %2, align 8
  %10 = load double, ptr %2, align 8
  ret double %10
}

define i32 @main() {
entry.l12:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca double, align 8
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  store i32 0, ptr %result, align 4
  store i32 1, ptr %0, align 4
  %6 = load i32, ptr %0, align 4
  store double 2.400000e+00, ptr %1, align 8
  %7 = load double, ptr %1, align 8
  %8 = call double @_f__void__genericFunction__int_double(i32 %6, double %7)
  store double %8, ptr %2, align 8
  %9 = load double, ptr %2, align 8
  %10 = call i32 (ptr, ...) @printf(ptr @0, double %9)
  store i64 12, ptr %3, align 8
  %11 = load i64, ptr %3, align 8
  store double 2.000000e+00, ptr %4, align 8
  %12 = load double, ptr %4, align 8
  %13 = call double @_f__void__genericFunction__long_double(i64 %11, double %12)
  store double %13, ptr %5, align 8
  %14 = load double, ptr %5, align 8
  %15 = call i32 (ptr, ...) @printf(ptr @1, double %14)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

declare i32 @printf(ptr, ...)
