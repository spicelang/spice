; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Struct__int_bool_string = type { i32, i1, i8* }

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
@12 = private unnamed_addr constant [15 x i8] c"This is a test\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  %6 = alloca i32, align 4
  %7 = alloca i8, align 1
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  %intVariable = alloca i32, align 4
  %19 = alloca i32*, align 8
  %20 = alloca i32, align 4
  %21 = alloca %_s__Struct__int_bool_string, align 8
  %22 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store double 0x40335992641B328B, double* %0, align 8
  %23 = load double, double* %0, align 8
  %24 = fmul double %23, -1.000000e+00
  store double %24, double* %0, align 8
  %25 = load double, double* %0, align 8
  store i32 64, i32* %1, align 4
  %26 = load i32, i32* %1, align 4
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %26)
  store i32 32, i32* %2, align 4
  %28 = load i32, i32* %2, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %28)
  store i32 16, i32* %3, align 4
  %30 = load i32, i32* %3, align 4
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), i32 %30)
  store i32 64, i32* %4, align 4
  %32 = load i32, i32* %4, align 4
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @3, i32 0, i32 0), i32 %32)
  store i8 13, i8* %5, align 1
  %34 = load i8, i8* %5, align 1
  store i32 8, i32* %6, align 4
  %35 = load i32, i32* %6, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @4, i32 0, i32 0), i32 %35)
  store i8 65, i8* %7, align 1
  %37 = load i8, i8* %7, align 1
  store i32 8, i32* %8, align 4
  %38 = load i32, i32* %8, align 4
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @5, i32 0, i32 0), i32 %38)
  store i32 64, i32* %9, align 4
  %40 = load i32, i32* %9, align 4
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @6, i32 0, i32 0), i32 %40)
  store i32 1, i32* %10, align 4
  %42 = load i32, i32* %10, align 4
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @8, i32 0, i32 0), i32 %42)
  store i32 1, i32* %11, align 4
  store i32 2, i32* %12, align 4
  store i32 3, i32* %13, align 4
  store i32 4, i32* %14, align 4
  store i32 5, i32* %15, align 4
  store i32 6, i32* %16, align 4
  store i32 7, i32* %17, align 4
  %44 = load [7 x i32], [7 x i32]* @anonymous.0, align 4
  store i32 224, i32* %18, align 4
  %45 = load i32, i32* %18, align 4
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @9, i32 0, i32 0), i32 %45)
  store i32 123, i32* %intVariable, align 4
  %47 = load i32, i32* %intVariable, align 4
  store i32* %intVariable, i32** %19, align 8
  store i32* %intVariable, i32** %19, align 8
  %48 = load i32*, i32** %19, align 8
  store i32 64, i32* %20, align 4
  %49 = load i32, i32* %20, align 4
  %50 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @10, i32 0, i32 0), i32 %49)
  %51 = getelementptr inbounds %_s__Struct__int_bool_string, %_s__Struct__int_bool_string* %21, i32 0, i32 0
  store i32 5, i32* %51, align 4
  %52 = getelementptr inbounds %_s__Struct__int_bool_string, %_s__Struct__int_bool_string* %21, i32 0, i32 1
  store i1 true, i1* %52, align 1
  %53 = getelementptr inbounds %_s__Struct__int_bool_string, %_s__Struct__int_bool_string* %21, i32 0, i32 2
  store i8* getelementptr inbounds ([15 x i8], [15 x i8]* @12, i32 0, i32 0), i8** %53, align 8
  %54 = load %_s__Struct__int_bool_string, %_s__Struct__int_bool_string* %21, align 8
  store i32 128, i32* %22, align 4
  %55 = load i32, i32* %22, align 4
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @11, i32 0, i32 0), i32 %55)
  %57 = load i32, i32* %result, align 4
  ret i32 %57
}

declare i32 @printf(i8*, ...)
