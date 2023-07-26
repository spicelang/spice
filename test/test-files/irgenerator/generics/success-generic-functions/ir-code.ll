; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define private double @_Z15genericFunctionid(i32 %0, double %1) {
  %result = alloca double, align 8
  %arg1 = alloca i32, align 4
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  store i32 %0, ptr %arg1, align 4
  store double %1, ptr %arg2, align 8
  store i32 10, ptr %arg3, align 4
  %3 = load i32, ptr %arg1, align 4
  %4 = sitofp i32 %3 to double
  %5 = load double, ptr %arg2, align 8
  %6 = fadd double %4, %5
  %7 = load i32, ptr %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  ret double %9
}

define private double @_Z15genericFunctionld(i64 %0, double %1) {
  %result = alloca double, align 8
  %arg1 = alloca i64, align 8
  %arg2 = alloca double, align 8
  %arg3 = alloca i32, align 4
  store i64 %0, ptr %arg1, align 8
  store double %1, ptr %arg2, align 8
  store i32 10, ptr %arg3, align 4
  %3 = load i64, ptr %arg1, align 8
  %4 = sitofp i64 %3 to double
  %5 = load double, ptr %arg2, align 8
  %6 = fadd double %4, %5
  %7 = load i32, ptr %arg3, align 4
  %8 = sitofp i32 %7 to double
  %9 = fadd double %6, %8
  ret double %9
}

define private i64 @_Z15genericFunctionlsl(i64 %0, i16 %1, i64 %2) {
  %result = alloca i64, align 8
  %arg1 = alloca i64, align 8
  %arg2 = alloca i16, align 2
  %arg3 = alloca i64, align 8
  store i64 %0, ptr %arg1, align 8
  store i16 %1, ptr %arg2, align 2
  store i64 %2, ptr %arg3, align 8
  %4 = load i16, ptr %arg2, align 2
  %5 = sext i16 %4 to i64
  %6 = load i64, ptr %arg1, align 8
  %7 = add i64 %6, %5
  %8 = load i64, ptr %arg3, align 8
  %9 = add i64 %7, %8
  ret i64 %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call double @_Z15genericFunctionid(i32 1, double 2.400000e+00)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %1)
  %3 = call double @_Z15genericFunctionld(i64 12, double 2.000000e+00)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, double %3)
  %5 = call i64 @_Z15genericFunctionlsl(i64 12, i16 1, i64 11)
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
