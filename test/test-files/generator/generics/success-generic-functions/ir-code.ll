; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define internal double @_f__void__genericFunction__int_double(i32 %0, double %1) {
entry:
  %arg1 = alloca i32, align 4
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  %result = alloca double, align 8
  %2 = alloca double, align 8
  store i32 %0, i32* %arg1, align 4
  store double %1, double* %arg2, align 8
  store i32 10, i32* %arg3, align 4
  %3 = load i32, i32* %arg1, align 4
  %4 = load double, double* %arg2, align 8
  %5 = sitofp i32 %3 to double
  %6 = fadd double %5, %4
  %7 = load i32, i32* %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  store double %9, double* %2, align 8
  %10 = load double, double* %2, align 8
  ret double %10
}

define internal double @_f__void__genericFunction__long_double(i64 %0, double %1) {
entry:
  %arg1 = alloca i64, align 8
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  %result = alloca double, align 8
  %2 = alloca double, align 8
  store i64 %0, i64* %arg1, align 8
  store double %1, double* %arg2, align 8
  store i32 10, i32* %arg3, align 4
  %3 = load i64, i64* %arg1, align 8
  %4 = load double, double* %arg2, align 8
  %5 = sitofp i64 %3 to double
  %6 = fadd double %5, %4
  %7 = load i32, i32* %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  store double %9, double* %2, align 8
  %10 = load double, double* %2, align 8
  ret double %10
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca double, align 8
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %6 = load i32, i32* %0, align 4
  store double 2.400000e+00, double* %1, align 8
  %7 = load double, double* %1, align 8
  %8 = call double @_f__void__genericFunction__int_double(i32 %6, double %7)
  store double %8, double* %2, align 8
  %9 = load double, double* %2, align 8
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), double %9)
  store i64 12, i64* %3, align 8
  %11 = load i64, i64* %3, align 8
  store double 2.000000e+00, double* %4, align 8
  %12 = load double, double* %4, align 8
  %13 = call double @_f__void__genericFunction__long_double(i64 %11, double %12)
  store double %13, double* %5, align 8
  %14 = load double, double* %5, align 8
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), double %14)
  %16 = load i32, i32* %result, align 4
  ret i32 %16
}

declare i32 @printf(i8*, ...)
