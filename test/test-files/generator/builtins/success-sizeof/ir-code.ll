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
@anonymous.0 = constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
@10 = private unnamed_addr constant [18 x i8] c"Size of int*: %d\0A\00", align 1
@11 = private unnamed_addr constant [29 x i8] c"Size of struct instance: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca double, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  %8 = alloca i8, align 1
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %anonymous.0 = alloca [7 x i32], align 4
  %12 = alloca i32, align 4
  %intVariable = alloca i32, align 4
  %13 = alloca i32*, align 8
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store double 0x40335992641B328B, double* %0, align 8
  %16 = load double, double* %0, align 8
  %17 = fmul double %16, -1.000000e+00
  store double %17, double* %1, align 8
  %18 = load double, double* %1, align 8
  store i32 64, i32* %2, align 4
  %19 = load i32, i32* %2, align 4
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %19)
  store i32 32, i32* %3, align 4
  %21 = load i32, i32* %3, align 4
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %21)
  store i32 16, i32* %4, align 4
  %23 = load i32, i32* %4, align 4
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), i32 %23)
  store i32 64, i32* %5, align 4
  %25 = load i32, i32* %5, align 4
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @3, i32 0, i32 0), i32 %25)
  store i8 13, i8* %6, align 1
  %27 = load i8, i8* %6, align 1
  store i32 8, i32* %7, align 4
  %28 = load i32, i32* %7, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @4, i32 0, i32 0), i32 %28)
  store i8 65, i8* %8, align 1
  %30 = load i8, i8* %8, align 1
  store i32 8, i32* %9, align 4
  %31 = load i32, i32* %9, align 4
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @5, i32 0, i32 0), i32 %31)
  store i32 64, i32* %10, align 4
  %33 = load i32, i32* %10, align 4
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @6, i32 0, i32 0), i32 %33)
  store i32 1, i32* %11, align 4
  %35 = load i32, i32* %11, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @8, i32 0, i32 0), i32 %35)
  store [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], [7 x i32]* %anonymous.0, align 4
  %37 = load [7 x i32], [7 x i32]* %anonymous.0, align 4
  store i32 224, i32* %12, align 4
  %38 = load i32, i32* %12, align 4
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @9, i32 0, i32 0), i32 %38)
  store i32 123, i32* %intVariable, align 4
  store i32* %intVariable, i32** %13, align 8
  %40 = load i32*, i32** %13, align 8
  store i32 64, i32* %14, align 4
  %41 = load i32, i32* %14, align 4
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @10, i32 0, i32 0), i32 %41)
  store i32 128, i32* %15, align 4
  %43 = load i32, i32* %15, align 4
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @11, i32 0, i32 0), i32 %43)
  %45 = load i32, i32* %result, align 4
  ret i32 %45
}

declare i32 @printf(i8*, ...)
