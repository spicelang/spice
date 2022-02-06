; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Size of double: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Size of long: %d\0A\00", align 1
@4 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@5 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@6 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@7 = private unnamed_addr constant [13 x i8] c"Hello Spice!\00", align 1
@8 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@9 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1
@anonymous.0 = dso_local constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i16, align 2
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i8, align 1
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i8, align 1
  %13 = alloca i32, align 4
  %14 = alloca i8*, align 8
  %15 = alloca i32, align 4
  %16 = alloca i1, align 1
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  %19 = alloca i32, align 4
  %20 = alloca i32, align 4
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  %24 = alloca i32, align 4
  %25 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store double 0x40335992641B328B, double* %0, align 8
  %26 = load double, double* %0, align 8
  %27 = fmul double %26, -1.000000e+00
  store double %27, double* %0, align 8
  %28 = load double, double* %0, align 8
  store i32 64, i32* %1, align 4
  %29 = load i32, i32* %1, align 4
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %29)
  store i32 353, i32* %2, align 4
  %31 = load i32, i32* %2, align 4
  store i32 32, i32* %3, align 4
  %32 = load i32, i32* %3, align 4
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %32)
  store i16 35, i16* %4, align 2
  %34 = load i16, i16* %4, align 2
  store i32 16, i32* %5, align 4
  %35 = load i32, i32* %5, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), i32 %35)
  store i64 9223372036854775807, i64* %6, align 8
  %37 = load i64, i64* %6, align 8
  store i32 64, i32* %7, align 4
  %38 = load i32, i32* %7, align 4
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @3, i32 0, i32 0), i32 %38)
  store i32 13, i32* %8, align 4
  %40 = load i32, i32* %8, align 4
  %41 = trunc i32 %40 to i8
  store i8 %41, i8* %9, align 1
  %42 = load i8, i8* %9, align 1
  store i32 8, i32* %10, align 4
  %43 = load i32, i32* %10, align 4
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @4, i32 0, i32 0), i32 %43)
  store i32 65, i32* %11, align 4
  %45 = load i32, i32* %11, align 4
  %46 = trunc i32 %45 to i8
  store i8 %46, i8* %12, align 1
  %47 = load i8, i8* %12, align 1
  store i32 8, i32* %13, align 4
  %48 = load i32, i32* %13, align 4
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @5, i32 0, i32 0), i32 %48)
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @7, i32 0, i32 0), i8** %14, align 8
  %50 = load i8*, i8** %14, align 8
  store i32 0, i32* %15, align 4
  %51 = load i32, i32* %15, align 4
  %52 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @6, i32 0, i32 0), i32 %51)
  store i1 false, i1* %16, align 1
  %53 = load i1, i1* %16, align 1
  store i32 1, i32* %17, align 4
  %54 = load i32, i32* %17, align 4
  %55 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @8, i32 0, i32 0), i32 %54)
  store i32 1, i32* %18, align 4
  store i32 2, i32* %19, align 4
  store i32 3, i32* %20, align 4
  store i32 4, i32* %21, align 4
  store i32 5, i32* %22, align 4
  store i32 6, i32* %23, align 4
  store i32 7, i32* %24, align 4
  %56 = load [7 x i32], [7 x i32]* @anonymous.0, align 4
  store i32 7, i32* %25, align 4
  %57 = load i32, i32* %25, align 4
  %58 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @9, i32 0, i32 0), i32 %57)
  %59 = load i32, i32* %result, align 4
  ret i32 %59
}
